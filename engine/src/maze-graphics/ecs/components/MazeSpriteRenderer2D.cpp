//////////////////////////////////////////
//
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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(SpriteRenderMode);


    //////////////////////////////////////////
    // Class SpriteRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpriteRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpriteAssetRef, sprite, SpriteAssetRef(), getSpriteRef, setSpriteRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialAssetRef, material, MaterialAssetRef(), getMaterialRef, setMaterialRefCopy),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32::c_white, getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpriteRenderMode, renderMode, SpriteRenderMode::Simple, getRenderMode, setRenderMode));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpriteRenderer2D);


    //////////////////////////////////////////
    SpriteRenderer2D::SpriteRenderer2D()
    {
    }

    //////////////////////////////////////////
    SpriteRenderer2D::~SpriteRenderer2D()
    {
        setSprite(SpritePtr());
    }

    //////////////////////////////////////////
    SpriteRenderer2DPtr SpriteRenderer2D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SpriteRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpriteRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SpriteRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;
        return true;
    }

    //////////////////////////////////////////
    bool SpriteRenderer2D::init(
        Component* _component,
        EcsWorld* _world,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _world, _copyData))
            return false;

        enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
        enableFlag(SpriteRenderer2D::Flags::ColorDirty);
        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
        enableFlag(SpriteRenderer2D::Flags::UV0Dirty);

        return true;
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setSpriteRef(SpriteAssetRef const& _sprite)
    {
        if (m_spriteRef.getSprite() == _sprite.getSprite())
            return;

        if (m_spriteRef.getSprite())
        {
            m_spriteRef.getSprite()->eventDataChanged.unsubscribe(this);
        }

        m_spriteRef.setSprite(_sprite.getSprite());

        if (m_spriteRef.getSprite())
        {
            m_spriteRef.getSprite()->eventDataChanged.subscribe(this, &SpriteRenderer2D::notifySpriteDataChanged);
        }

        enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
        enableFlag(SpriteRenderer2D::Flags::UV0Dirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setSprite(HashedString const& _spriteName)
    {
        Maze::RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        Maze::SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        Maze::SpritePtr const& sprite = spriteManager->getOrLoadSprite(_spriteName.asHashedCString());
        setSprite(sprite);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialRef(MaterialAssetRef const& _material)
    {
        if (m_materialRef.getMaterial() == _material.getMaterial())
            return;

        m_materialRef.setMaterial(_material.getMaterial());

        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterial(CString _materialName)
    {
        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_materialName);
        setMaterial(material);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialRefCopy(MaterialAssetRef const& _material)
    {
        setMaterial(_material.getMaterial() ? _material.getMaterial()->createCopy() : MaterialPtr());
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialCopy(MaterialPtr const& _material)
    {
        setMaterial(_material ? _material->createCopy() : MaterialPtr());
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialCopy(CString _materialName)
    {
        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_materialName);
        setMaterialCopy(material);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setColor(ColorU32 _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        enableFlag(SpriteRenderer2D::Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setRenderMode(SpriteRenderMode _renderMode)
    {
        if (m_renderMode == _renderMode)
            return;

        m_renderMode = _renderMode;

        enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
        enableFlag(SpriteRenderer2D::Flags::UV0Dirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setPixelPerfect(bool _value)
    {
        if (m_pixelPerfect == _value)
            return;

        m_pixelPerfect = _value;

        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
    }
    
    //////////////////////////////////////////
    void SpriteRenderer2D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        m_meshRenderer->setMaterial(getMaterial());

        if (getMaterial())
        {
            Texture2DPtr texture;

            if (getSprite())
            {
                texture = getSprite()->getTexture();

                if (!texture)
                    texture = getMaterial()->getRenderSystem()->getTextureManager()->ensureBuiltinTexture2D(BuiltinTexture2DType::Error);
            }
            else
            {
                texture = getMaterial()->getRenderSystem()->getTextureManager()->ensureBuiltinTexture2D(BuiltinTexture2DType::White);
            }

            if (texture)
            {
                getMaterial()->setUniform(MAZE_HCS("u_baseMap"), texture);
                getMaterial()->setUniform(MAZE_HCS("u_baseMapTexelSize"), 1.0f / (Vec2F)texture->getSize());
            }
        }

        disableFlag(SpriteRenderer2D::Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMeshData()
    {
        S32 totalQuadsCount = 0;
        m_localMatrices.clear();
        m_localColors.clear();
        m_localUV0s.clear();

        auto addQuad =
            [&, this](TMat const& _tm, Vec4F const& _color, Vec4F const& _uv0)
            {
                m_localMatrices.push_back(_tm);
                m_localColors.push_back(_color);
                m_localUV0s.push_back(_uv0);
                ++totalQuadsCount;
            };

        switch (m_renderMode)
        {
            case SpriteRenderMode::Simple:
            {
                Vec4F uv;
                if (getSprite())
                    uv = Vec4F(
                        getSprite()->getTextureCoordLB(),
                        getSprite()->getTextureCoordRT());
                else
                    uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f);

                addQuad(
                    TMat::CreateScale(m_transform->getSize()),
                    Vec4F::c_one,
                    uv);
                break;
            }
            case SpriteRenderMode::Sliced:
            {
                if (getSprite())
                {
                    Vec2F const& size = m_transform->getSize();
                    SpriteSliceBorder const& sliceBorder = getSprite()->getSliceBorder();
                    Vec2F const& nativeSize = getSprite()->getNativeSize();
                    Vec2F const& texCoordLB = getSprite()->getTextureCoordLB();
                    Vec2F const& texCoordRT = getSprite()->getTextureCoordRT();

                    Vec2F centerSize = Vec2F(
                        Math::Max(size.x - sliceBorder.left - sliceBorder.right, 0.0f),
                        Math::Max(size.y - sliceBorder.bottom - sliceBorder.top, 0.0f));

                    Vec2F centerPositionLB = Vec2F(sliceBorder.left, sliceBorder.bottom);
                    Vec2F centerPositionRT = centerPositionLB + centerSize;

                    Vec2F deltaUV = texCoordRT - texCoordLB;

                    Vec2F centerUVLB = texCoordLB + deltaUV * Vec2F(sliceBorder.left, sliceBorder.bottom) / nativeSize;
                    Vec2F ltUVRT = Vec2F(centerUVLB.x, texCoordRT.y);
                    Vec2F centerUVRT = texCoordRT - deltaUV * Vec2F(sliceBorder.right, sliceBorder.top) / nativeSize;


                    if (sliceBorder.left > 0.0f)
                    {
                        // LB 
                        if (sliceBorder.bottom > 0.0f)
                        {
                            addQuad(
                                TMat::CreateScale(sliceBorder.left, sliceBorder.bottom),
                                Vec4F::c_one,
                                Vec4F(texCoordLB, centerUVLB));
                        }

                        // Left
                        addQuad(
                            TMat::CreateTranslation(0.0f, centerPositionLB.y).transform(TMat::CreateScale(sliceBorder.left, centerSize.y)),
                            Vec4F::c_one,
                            Vec4F(Vec2F(texCoordLB.x, centerUVLB.y), Vec2F(centerUVLB.x, centerUVRT.y)));

                        // LT 
                        if (sliceBorder.top > 0.0f)
                        {
                            addQuad(
                                TMat::CreateTranslation(0.0f, centerPositionRT.y).transform(TMat::CreateScale(sliceBorder.left, sliceBorder.top)),
                                Vec4F::c_one,
                                Vec4F(Vec2F(texCoordLB.x, centerUVRT.y), Vec2F(centerUVLB.x, texCoordRT.y)));
                        }
                    }

                    // Bottom
                    if (sliceBorder.bottom > 0.0f)
                    {
                        addQuad(
                            TMat::CreateTranslation(centerPositionLB.x, 0.0f).transform(TMat::CreateScale(centerSize.x, sliceBorder.bottom)),
                            Vec4F::c_one,
                            Vec4F(Vec2F(centerUVLB.x, texCoordLB.y), Vec2F(centerUVRT.x, centerUVLB.y)));
                    }

                    // Center
                    addQuad(
                        TMat::CreateTranslation(centerPositionLB).transform(TMat::CreateScale(centerSize)),
                        Vec4F::c_one,
                        Vec4F(centerUVLB, centerUVRT));

                    // Top
                    if (sliceBorder.top > 0.0f)
                    {
                        addQuad(
                            TMat::CreateTranslation(centerPositionLB.x, centerPositionRT.y).transform(TMat::CreateScale(centerSize.x, sliceBorder.top)),
                            Vec4F::c_one,
                            Vec4F(Vec2F(centerUVLB.x, centerUVRT.y), Vec2F(centerUVRT.x, texCoordRT.y)));
                    }

                    if (sliceBorder.right > 0.0f)
                    {
                        // RB 
                        if (sliceBorder.bottom > 0.0f)
                        {
                            addQuad(
                                TMat::CreateTranslation(centerPositionRT.x, 0.0f).transform(TMat::CreateScale(sliceBorder.right, sliceBorder.bottom)),
                                Vec4F::c_one,
                                Vec4F(Vec2F(centerUVRT.x, texCoordLB.y), Vec2F(texCoordRT.x, centerUVLB.y)));
                        }

                        // Right
                        addQuad(
                            TMat::CreateTranslation(centerPositionRT.x, centerPositionLB.y).transform(TMat::CreateScale(sliceBorder.right, centerSize.y)),
                            Vec4F::c_one,
                            Vec4F(Vec2F(centerUVRT.x, centerUVLB.y), Vec2F(texCoordRT.x, centerUVRT.y)));

                        // RT 
                        if (sliceBorder.top > 0.0f)
                        {
                            addQuad(
                                TMat::CreateTranslation(centerPositionRT.x, centerPositionRT.y).transform(TMat::CreateScale(sliceBorder.right, sliceBorder.top)),
                                Vec4F::c_one,
                                Vec4F(Vec2F(centerUVRT.x, centerUVRT.y), texCoordRT));
                        }
                    }
                }
                else
                {
                    addQuad(
                        TMat::CreateScale(m_transform->getSize()),
                        Vec4F::c_one,
                        Vec4F(0.0f, 0.0f, 1.0f, 1.0f));
                }

                break;
            }
            case SpriteRenderMode::Tiled:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
            default:
            {
                break;
            }
        }

        m_meshRenderer->resize(totalQuadsCount);

        disableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMeshRendererColors()
    {
        if (!m_meshRenderer)
            return;

        Vec4F vertexColor = m_color.toVec4F32();
        if (m_canvasRenderer)
            vertexColor.w *= m_canvasRenderer->getAlpha();

        Size colorsCount = m_meshRenderer->getColors().size();
        for (Size i = 0; i < colorsCount; ++i)
            m_meshRenderer->setColor(i, m_localColors[i] * vertexColor);

        disableFlag(SpriteRenderer2D::Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMeshRendererUV0s()
    {
        if (!m_meshRenderer)
            return;

        Size elementsCount = m_meshRenderer->getUV0().size();
        // #TODO: optimize with memcpy
        for (Size i = 0; i < elementsCount; ++i)
            m_meshRenderer->setUV0(i, m_localUV0s[i]);

        disableFlag(SpriteRenderer2D::Flags::UV0Dirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMeshRendererModelMatrices()
    {
        if (!m_meshRenderer)
            return;

        Size transformCount = m_meshRenderer->getModelMatrices().size();
        MAZE_DEBUG_ERROR_IF(transformCount != m_localMatrices.size(), "Invalid elements count!");
        if (transformCount == 0)
            return;

        Vec2F pixelPerfectShift = Vec2F::c_zero;
        if (m_pixelPerfect)
        {
            TMat initTm = m_transform->getWorldTransform().transform(m_localMatrices[0]);
            Vec2F translation = initTm.getTranslation2D();
            pixelPerfectShift = Math::Round(translation) - translation;
        }

        for (Size i = 0; i < transformCount; ++i)
        {
            TMat tm = m_transform->getWorldTransform().transform(m_localMatrices[i]);
            if (m_pixelPerfect)
                tm.setTranslation(tm.getTranslation2D() + pixelPerfectShift);
            m_meshRenderer->setModelMatrix(i, tm);
        }

        disableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRendererInstanced>();
        m_canvasRenderer = getEntityRaw()->ensureComponent<CanvasRenderer>();

        
        m_meshRenderer->setRenderMesh(
            RenderMeshManager::GetCurrentInstancePtr()->ensureBuiltinRenderMesh(
                BuiltinRenderMeshType::QuadNullPivot));

        enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
        enableFlag(SpriteRenderer2D::Flags::ColorDirty);
        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
        enableFlag(SpriteRenderer2D::Flags::UV0Dirty);
    }
    
    //////////////////////////////////////////
    void SpriteRenderer2D::notifySpriteDataChanged(Sprite* _sprite)
    {
        enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);
        enableFlag(SpriteRenderer2D::Flags::MaterialDirty);
        enableFlag(SpriteRenderer2D::Flags::UV0Dirty);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::prepareForRender()
    {
        if (getFlag(SpriteRenderer2D::Flags::MeshDataDirty))
            updateMeshData();

        if (getFlag(SpriteRenderer2D::Flags::ModelMatricesDirty))
            updateMeshRendererModelMatrices();

        if (getFlag(SpriteRenderer2D::Flags::ColorDirty))
            updateMeshRendererColors();

        if (getFlag(SpriteRenderer2D::Flags::MaterialDirty))
            updateMaterial();

        if (getFlag(SpriteRenderer2D::Flags::UV0Dirty))
            updateMeshRendererUV0s();
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SpriteRenderer2DSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render2DPostUpdateEvent& _event,
        Entity* _entity,
        SpriteRenderer2D* _spriteRenderer)
    {
        if (_spriteRenderer->getTransform()->isSizeChanged())
            _spriteRenderer->enableFlag(SpriteRenderer2D::Flags::MeshDataDirty);
        
        if (_spriteRenderer->getTransform()->isWorldTransformChanged())
            _spriteRenderer->enableFlag(SpriteRenderer2D::Flags::ModelMatricesDirty);

        if (_spriteRenderer->getCanvasRenderer()->isAlphaDirty())
            _spriteRenderer->enableFlag(SpriteRenderer2D::Flags::ColorDirty);
        
        _spriteRenderer->prepareForRender();
    }
    
} // namespace Maze
//////////////////////////////////////////
