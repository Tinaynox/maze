//////////////////////////////////////////
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeEngineHeader.hpp"
#include "utils/MazeOutlineController.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class OutlineController
    //
    //////////////////////////////////////////
    OutlineController::OutlineController()
    {
    }

    //////////////////////////////////////////
    OutlineController::~OutlineController()
    {
        if (m_maskRenderBuffer)
            m_maskRenderBuffer->eventRenderBufferEndDraw.unsubscribe(this);

        if (m_maskCamera)
            m_maskCamera->eventCameraWillRender.unsubscribe(this);

        if (m_screenSpaceUniform)
            m_screenSpaceUniform->setValue(0.0f);
    }

    //////////////////////////////////////////
    OutlineControllerPtr OutlineController::Create(
        EcsScene* _scene,
        Transform2DPtr const& _compositeParent,
        Vec2U32 const& _size,
        HashedCString _compositeMaterialName)
    {
        OutlineControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(OutlineController, object, init(_scene, _compositeParent, _size, _compositeMaterialName));
        return object;
    }

    //////////////////////////////////////////
    bool OutlineController::init(
        EcsScene* _scene,
        Transform2DPtr const& _compositeParent,
        Vec2U32 const& _size,
        HashedCString _compositeMaterialName)
    {
        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        m_scene = _scene;

        MaterialPtr const& compositeMaterial = renderSystem->getMaterialManager()->getOrLoadMaterial(_compositeMaterialName);
        MAZE_ERROR_RETURN_VALUE_IF(!compositeMaterial, false, "Undefined outline composite material: %s", _compositeMaterialName.str);

        m_maskRenderBuffer = RenderBuffer::Create(
            {
                _size,
                PixelFormat::RGBA_U8,
                PixelFormat::DEPTH_U24
            });
        MAZE_ERROR_RETURN_VALUE_IF(!m_maskRenderBuffer, false, "Failed to create outline mask render buffer");
        m_maskRenderBuffer->setName("OutlineMask");
        m_maskRenderBuffer->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
        m_maskRenderBuffer->getColorTexture2D()->setMagFilter(TextureFilter::Linear);
        m_maskRenderBuffer->getColorTexture2D()->setWrapS(TextureWrap::ClampToEdge);
        m_maskRenderBuffer->getColorTexture2D()->setWrapT(TextureWrap::ClampToEdge);
        m_maskRenderBuffer->eventRenderBufferEndDraw.subscribe(this, &OutlineController::notifyMaskRenderBufferEndDraw);

        ShaderManagerPtr const& shaderManager = renderSystem->getShaderManager();
        m_sceneDepthMapUniform = shaderManager->ensureGlobalShaderUniform(MAZE_HCS("u_global_outlineSceneDepthMap"));
        m_sceneDepthParamsUniform = shaderManager->ensureGlobalShaderUniform(MAZE_HCS("u_global_outlineSceneDepthParams"));
        updateSceneDepthUniforms();

        m_screenSpaceUniform = shaderManager->ensureGlobalShaderUniform(MAZE_HCS("u_global_outlineScreenSpace"));
        m_screenSpaceUniform->setValue(1.0f);

        m_maskEnabledUniform = shaderManager->ensureGlobalShaderUniform(MAZE_HCS("u_global_outlineMaskEnabled"));

        m_compositeSprite = SpriteHelper::CreateSprite(
            Sprite::Create(m_maskRenderBuffer->getColorTexture2D()),
            _compositeParent->getSize(),
            Vec2F32::c_zero,
            compositeMaterial,
            _compositeParent,
            _scene);
        m_compositeSprite->getTransform()->setZ(1001);
        m_compositeSprite->getEntityRaw()->ensureComponent<Name>("OutlineCompositeSprite");
        m_compositeSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_compositeSprite->setPixelPerfect(true);
        m_compositeSprite->getEntityRaw()->setActiveSelf(false);

        return true;
    }

    //////////////////////////////////////////
    void OutlineController::update(F32 _dt)
    {
        bool maskEnabled = isMaskEnabled();
        if (!maskEnabled)
            m_maskHasContent = false;

        bool compositeActive = m_maskHasContent && m_sourceCamera;
        if (m_compositeSprite->getEntityRaw()->getActiveSelf() != compositeActive)
            m_compositeSprite->getEntityRaw()->setActiveSelf(compositeActive);

        if (!m_sourceCamera)
            return;

        ensureMaskCamera();
        if (!m_maskCamera)
            return;

        if (m_maskCamera->getEntityRaw()->getActiveSelf() != maskEnabled)
            m_maskCamera->getEntityRaw()->setActiveSelf(maskEnabled);
    }

    //////////////////////////////////////////
    void OutlineController::notifyMaskCameraWillRender(Camera3D* _camera)
    {
        if (!m_sourceCamera)
            return;

        _camera->setProjectionMode(m_sourceCamera->getProjectionMode());
        _camera->setFOV(m_sourceCamera->getFOV());
        _camera->setOrthographicSize(m_sourceCamera->getOrthographicSize());
        _camera->setNearZ(m_sourceCamera->getNearZ());
        _camera->setFarZ(m_sourceCamera->getFarZ());
        _camera->setRenderMask(m_sourceCamera->getRenderMask());
    }

    //////////////////////////////////////////
    bool OutlineController::isMaskEnabled() const
    {
        ShaderUniformVariant const& value = m_maskEnabledUniform->getVariant();
        return value.getType() != ShaderUniformType::UniformF32 || value.getF32() > 0.5f;
    }

    //////////////////////////////////////////
    void OutlineController::notifyMaskRenderBufferEndDraw(RenderBuffer* _renderBuffer)
    {
        m_maskHasContent = _renderBuffer->getDrawCalls() > 0;
        _renderBuffer->clearDrawCalls();
    }

    //////////////////////////////////////////
    void OutlineController::setSourceCamera(Camera3DPtr const& _camera)
    {
        if (m_sourceCamera == _camera)
            return;

        m_sourceCamera = _camera;

        if (m_maskCamera && m_sourceCamera)
            m_maskCamera->getEntityRaw()->ensureComponent<Transform3D>()->setParent(m_sourceCamera->getEntityRaw()->ensureComponent<Transform3D>());

        update(0.0f);
    }

    //////////////////////////////////////////
    void OutlineController::ensureMaskCamera()
    {
        if (m_maskCamera && m_maskCamera->getEntityRaw() && !m_maskCamera->getEntityRaw()->getRemoving())
            return;

        m_maskCamera.reset();

        if (!m_sourceCamera || !m_sourceCamera->getEntityRaw() || m_sourceCamera->getEntityRaw()->getRemoving())
            return;

        EntityPtr maskCameraEntity = m_scene->createEntity("OutlineMaskCamera");
        Transform3DPtr maskCameraTransform = maskCameraEntity->ensureComponent<Transform3D>();
        m_maskCamera = maskCameraEntity->createComponent<Camera3D>();
        m_maskCamera->setRenderPassType(RenderPassType::Outline);
        m_maskCamera->setRenderTarget(m_maskRenderBuffer);
        m_maskCamera->setClearColor(ColorU32(0, 0, 0, 0));
        m_maskCamera->setClearColorFlag(true);
        m_maskCamera->setClearDepthFlag(true);
        m_maskCamera->setClearSkyBoxFlag(false);
        m_maskCamera->setViewport(Rect2F(0.0f, 0.0f, 1.0f, 1.0f));
        m_maskCamera->setSortOrder(m_sourceCamera->getSortOrder() + 200);
        m_maskCamera->eventCameraWillRender.subscribe(this, &OutlineController::notifyMaskCameraWillRender);

        maskCameraTransform->setParent(m_sourceCamera->getEntityRaw()->ensureComponent<Transform3D>());
    }

    //////////////////////////////////////////
    void OutlineController::setSceneDepthBuffer(RenderBufferPtr const& _renderBuffer)
    {
        if (m_sceneDepthBuffer == _renderBuffer)
            return;

        m_sceneDepthBuffer = _renderBuffer;
        updateSceneDepthUniforms();
    }

    //////////////////////////////////////////
    void OutlineController::setSize(Vec2U32 const& _size)
    {
        if (!m_maskRenderBuffer || m_maskRenderBuffer->getSize() == _size)
            return;

        m_maskRenderBuffer->setSize(_size);
        updateSceneDepthUniforms();
    }

    //////////////////////////////////////////
    void OutlineController::setOrder(S32 _order)
    {
        if (m_maskRenderBuffer)
            m_maskRenderBuffer->setOrder(_order);
    }

    //////////////////////////////////////////
    void OutlineController::updateSceneDepthUniforms()
    {
        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        Texture2DPtr depthTexture;
        if (m_sceneDepthBuffer &&
            m_sceneDepthBuffer->getDepthTexture() &&
            m_sceneDepthBuffer->getDepthTexture()->getType() == TextureType::TwoDimensional &&
            m_sceneDepthBuffer->getSize() == m_maskRenderBuffer->getSize())
            depthTexture = m_sceneDepthBuffer->getDepthTexture()->cast<Texture2D>();

        if (depthTexture)
        {
            m_sceneDepthMapUniform->setValue(depthTexture);
            m_sceneDepthParamsUniform->setValue(Vec4F(1.0f, 0.0f, 0.0f, 0.0f));
        }
        else
        {
            m_sceneDepthMapUniform->setValue(renderSystem->getTextureManager()->getWhiteTexture());
            m_sceneDepthParamsUniform->setValue(Vec4F::c_zero);
        }
    }


} // namespace Maze
//////////////////////////////////////////
