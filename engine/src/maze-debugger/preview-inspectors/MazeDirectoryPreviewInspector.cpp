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
#include "maze-debugger/preview-inspectors/MazeDirectoryPreviewInspector.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
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
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
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


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class DirectoryPreviewInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DirectoryPreviewInspector, PreviewInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(DirectoryPreviewInspector);

    //////////////////////////////////////////
    DirectoryPreviewInspector::DirectoryPreviewInspector()
    {
        
    }

    //////////////////////////////////////////
    DirectoryPreviewInspector::~DirectoryPreviewInspector()
    {

    }

    //////////////////////////////////////////
    DirectoryPreviewInspectorPtr DirectoryPreviewInspector::Create(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        DirectoryPreviewInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(DirectoryPreviewInspector, object, init(
            _parent2D,
            _scene));
        return object;
    }

    //////////////////////////////////////////
    bool DirectoryPreviewInspector::init(
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
    void DirectoryPreviewInspector::update(F32 _dt)
    {
        if (m_directoriesDirty)
        {
            buildDirectories();
            m_directoriesDirty = false;
        }

        
    }

    //////////////////////////////////////////
    void DirectoryPreviewInspector::setDirectories(Set<AssetFilePtr> const& _directories)
    {
        m_directories = _directories;

        m_directoriesDirty = true;
    }

    //////////////////////////////////////////
    void DirectoryPreviewInspector::buildDirectories()
    {
        m_scene->clear();

        if (!m_directories.empty())
        {
            AssetFilePtr const& assetFile = *m_directories.begin();
            FileStats fileStats = assetFile->getFileStats();
            UnixTime creationTimeLocal = DateTimeHelper::ConvertUTCToLocal(fileStats.creationTimeUTC);
            UnixTime modifiedTimeLocal = DateTimeHelper::ConvertUTCToLocal(fileStats.modifiedTimeUTC);
            UnixTime accessedTimeLocal = DateTimeHelper::ConvertUTCToLocal(fileStats.accessedTimeUTC);

            DateTime creationDateLocal = DateTimeHelper::ConvertUnixTimeToDateTime(creationTimeLocal);
            DateTime modifiedDateLocal = DateTimeHelper::ConvertUnixTimeToDateTime(modifiedTimeLocal);
            DateTime accessedDateLocal = DateTimeHelper::ConvertUnixTimeToDateTime(accessedTimeLocal);

            String text;
            StringHelper::FormatString(
                text,
                "Created: %s\n"
                "Modified: %s\n"
                "Accessed: %s",
                creationDateLocal.formatDateTime().c_str(),
                modifiedDateLocal.formatDateTime().c_str(),
                accessedDateLocal.formatDateTime().c_str());

            SystemTextRenderer2DPtr info = SpriteHelper::CreateSystemText(
                text.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2DF(m_scene->getCanvas()->getTransform()->getHeight(), m_scene->getCanvas()->getTransform()->getHeight() - 4.0f),
                Vec2DF(2.0f, 0.0f),
                m_scene->getCanvas()->getTransform(),
                m_scene.get(),
                Vec2DF(0.0f, 0.5f),
                Vec2DF(0.0f, 0.5f));
            info->setColor(ColorU32::c_black);
        }
        
        m_directoriesDirty = false;
    }

    //////////////////////////////////////////
    bool DirectoryPreviewInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        setDirectories(_assetFiles);
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
