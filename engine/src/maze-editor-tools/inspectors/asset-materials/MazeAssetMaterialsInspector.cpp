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
#include "maze-editor-tools/inspectors/asset-materials/MazeAssetMaterialsInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
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
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DF.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DF.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DF.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DS.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DS.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DS.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DU.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DU.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DU.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class AssetMaterialsInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetMaterialsInspector, MaterialsInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(AssetMaterialsInspector);

    //////////////////////////////////////////
    AssetMaterialsInspector::AssetMaterialsInspector()
    {
        
    }

    //////////////////////////////////////////
    AssetMaterialsInspector::~AssetMaterialsInspector()
    {

    }

    //////////////////////////////////////////
    AssetMaterialsInspectorPtr AssetMaterialsInspector::Create(Transform2DPtr const& _parent)
    {
        AssetMaterialsInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetMaterialsInspector, object, init(_parent));
        return object;
    }

    //////////////////////////////////////////
    bool AssetMaterialsInspector::init(Transform2DPtr const& _parent)
    {
        if (!Inspector::init(_parent))
            return false;
        

        return true;
    }

    //////////////////////////////////////////
    void AssetMaterialsInspector::update(F32 _dt)
    {
        MaterialsInspector::update(_dt);
    }

    //////////////////////////////////////////
    void AssetMaterialsInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        m_assetFiles = _assetFiles;

        Set<MaterialPtr> materials;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        for (AssetFilePtr const& assetFile : m_assetFiles)
        {
            MaterialPtr const& material = materialManager->getMaterial(assetFile);
            if (material)
                materials.insert(material);
        }

        setMaterials(materials);
    }

} // namespace Maze
//////////////////////////////////////////
