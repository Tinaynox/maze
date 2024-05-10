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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/preview-inspectors/MazeRenderMeshPreviewInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class RenderMeshPreviewInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderMeshPreviewInspector, PreviewInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(RenderMeshPreviewInspector);

    //////////////////////////////////////////
    RenderMeshPreviewInspector::RenderMeshPreviewInspector()
    {
        
    }

    //////////////////////////////////////////
    RenderMeshPreviewInspector::~RenderMeshPreviewInspector()
    {

    }

    //////////////////////////////////////////
    RenderMeshPreviewInspectorPtr RenderMeshPreviewInspector::Create(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        RenderMeshPreviewInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderMeshPreviewInspector, object, init(
            _parent2D,
            _scene));
        return object;
    }

    //////////////////////////////////////////
    bool RenderMeshPreviewInspector::init(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        if (!PreviewInspector::init(
            _parent2D,
            _scene))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::update(F32 _dt)
    {
        if (m_renderMeshesDirty)
        {
            buildRenderMeshes();
            m_renderMeshesDirty = false;
        }        
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::processCursorPress(Vec2F const& _positionOS, CursorInputEvent const& _event)
    {
        if (_event.button == 1)
        {
            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::processCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _event)
    {
        Vec2F deltaPosition = _positionOS - m_cursorPositionLastFrame;

        if (_event.button == 1)
        {
            F32 yawAngle = m_scene->getYawAngle();
            F32 pitchAngle = m_scene->getPitchAngle();
            yawAngle += deltaPosition.x * 0.0075f;
            pitchAngle -= deltaPosition.y * 0.0075f;
            pitchAngle = Math::Clamp(pitchAngle, -Math::c_halfPi, Math::c_halfPi);
            m_scene->setYawAngle(yawAngle);
            m_scene->setPitchAngle(pitchAngle);

            updateCameraPosition();

            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::processCursorWheel(CursorWheelInputEvent const& _event)
    {
        m_cameraDistance -= _event.deltaWheel * 0.01f;
        m_cameraDistance = Math::Clamp(m_cameraDistance, 1.0f, 20.0f);
        updateCameraPosition();
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::setRenderMeshes(Set<RenderMeshPtr> const& _renderMeshes)
    {
        m_renderMeshes = _renderMeshes;

        m_renderMeshesDirty = true;
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::buildRenderMeshes()
    {
        m_scene->clear();

        if (!m_renderMeshes.empty())
        {
            // Main Light
            EntityPtr lightEntity = m_scene->createEntity();
            Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
            mainLight3D->setColor(ColorU32(255, 255, 255));
            mainLight3D->getTransform()->setParent(m_scene->getPreviewNodeTransform());
            mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
            mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
            lightEntity->ensureComponent<Name>("Light");

            // DebugGrid
            EntityPtr debugGridRendererEntity = m_scene->createEntity();
            debugGridRendererEntity->createComponent<DebugGridRenderer>(m_scene->getCamera());
            debugGridRendererEntity->getComponent<Transform3D>()->setParent(m_scene->getPreviewNodeTransform());

            // Axes
            EntityPtr axesMeshRendererEntity = m_scene->createEntity();
            axesMeshRendererEntity->createComponent<Transform3D>()->setParent(m_scene->getPreviewNodeTransform());
            MeshRendererPtr axesMeshRenderer = axesMeshRendererEntity->createComponent<MeshRenderer>();
            axesMeshRenderer->setRenderMesh(RenderMesh::Create(MeshHelper::CreateCoordinateAxes()));
            axesMeshRenderer->setMaterial(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::DebugAxis));

            // RenderMesh
            EntityPtr renderMesh = m_scene->createEntity();
            renderMesh->ensureComponent<Transform3D>()->setParent(m_scene->getPreviewNodeTransform());
            MeshRendererPtr meshRenderer = renderMesh->ensureComponent<MeshRenderer>();
            meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::MeshPreview));
            meshRenderer->setRenderMesh(*m_renderMeshes.begin());

            updateCameraPosition();
        }
        
        m_renderMeshesDirty = false;
    }

    //////////////////////////////////////////
    void RenderMeshPreviewInspector::updateCameraPosition()
    {
        F32 yawAngle = m_scene->getYawAngle();
        F32 pitchAngle = m_scene->getPitchAngle();
        Quaternion q(pitchAngle, yawAngle, 0.0f);

        Vec3F direction = q * Vec3F::c_unitZ;

        m_scene->getCamera()->getTransform()->setLocalPosition(-direction * m_cameraDistance);
    }

    //////////////////////////////////////////
    bool RenderMeshPreviewInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        Set<RenderMeshPtr> renderMeshes;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();

        for (AssetFilePtr const& assetFile : _assetFiles)
        {
            RenderMeshPtr const& renderMesh = renderMeshManager->getRenderMesh(assetFile);
            if (renderMesh)
                renderMeshes.insert(renderMesh);
        }

        setRenderMeshes(renderMeshes);

        return true;
    }

    //////////////////////////////////////////
    bool RenderMeshPreviewInspector::setObjects(Set<ObjectPtr> const& _objects)
    {
        Set<RenderMeshPtr> renderMeshes;
        for (ObjectPtr const& object : _objects)
            renderMeshes.insert(std::static_pointer_cast<RenderMesh>(object));

        setRenderMeshes(renderMeshes);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
