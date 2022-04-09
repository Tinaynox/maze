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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/preview-inspectors/MazeMaterialsPreviewInspector.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MaterialsPreviewInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MaterialsPreviewInspector, PreviewInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(MaterialsPreviewInspector);

    //////////////////////////////////////////
    MaterialsPreviewInspector::MaterialsPreviewInspector()
    {
        
    }

    //////////////////////////////////////////
    MaterialsPreviewInspector::~MaterialsPreviewInspector()
    {

    }

    //////////////////////////////////////////
    MaterialsPreviewInspectorPtr MaterialsPreviewInspector::Create(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        MaterialsPreviewInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialsPreviewInspector, object, init(
            _parent2D,
            _scene));
        return object;
    }

    //////////////////////////////////////////
    bool MaterialsPreviewInspector::init(
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
    void MaterialsPreviewInspector::update(F32 _dt)
    {
        if (m_materialsDirty)
        {
            buildMaterials();
            m_materialsDirty = false;
        }

        
    }

    //////////////////////////////////////////
    void MaterialsPreviewInspector::processCursorPress(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        if (_event.button == 1)
        {
            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void MaterialsPreviewInspector::processCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        Vec2DF deltaPosition = _positionOS - m_cursorPositionLastFrame;

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
    void MaterialsPreviewInspector::setMaterials(Set<MaterialPtr> const& _materials)
    {
        m_materials = _materials;

        m_materialsDirty = true;
    }

    //////////////////////////////////////////
    void MaterialsPreviewInspector::buildMaterials()
    {
        m_scene->clear();

        if (!m_materials.empty())
        {
            // Main Light
            EntityPtr lightEntity = m_scene->createEntity();
            Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
            mainLight3D->setColor(ColorU32(255, 255, 255));
            mainLight3D->getTransform()->setParent(m_scene->getPreviewNodeTransform());
            mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
            mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
            lightEntity->ensureComponent<Name>("Light");

            // Sphere
            EntityPtr sphere = m_scene->createEntity();
            sphere->ensureComponent<Transform3D>()->setParent(m_scene->getPreviewNodeTransform());
            MeshRendererPtr sphereMeshRenderer = sphere->ensureComponent<MeshRenderer>();
            sphereMeshRenderer->setMaterial(*m_materials.begin());
            sphereMeshRenderer->setMesh(MeshHelper::CreateSpherifiedCubeMesh(3.0f));

            updateCameraPosition();
        }
        
        m_materialsDirty = false;
    }

    //////////////////////////////////////////
    void MaterialsPreviewInspector::updateCameraPosition()
    {
        F32 yawAngle = m_scene->getYawAngle();
        F32 pitchAngle = m_scene->getPitchAngle();
        Quaternion q(pitchAngle, yawAngle, 0.0f);

        Vec3DF direction = q * Vec3DF::c_unitZ;

        m_scene->getCamera()->getTransform()->setLocalPosition(-direction * 7.0f);
    }

    //////////////////////////////////////////
    bool MaterialsPreviewInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        Set<MaterialPtr> materials;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        for (AssetFilePtr const& assetFile : _assetFiles)
        {
            MaterialPtr const& material = materialManager->getMaterial(assetFile);
            if (material)
                materials.insert(material);
        }

        setMaterials(materials);

        return true;
    }

    //////////////////////////////////////////
    bool MaterialsPreviewInspector::setObjects(Set<ObjectPtr> const& _objects)
    {
        Set<MaterialPtr> materials;
        for (ObjectPtr const& object : _objects)
            materials.insert(std::static_pointer_cast<Material>(object));

        setMaterials(materials);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
