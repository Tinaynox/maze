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
#include "maze-editor-tools/preview-inspectors/MazeFilePreviewInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
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


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class FilePreviewInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(FilePreviewInspector, PreviewInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(FilePreviewInspector);

    //////////////////////////////////////////
    FilePreviewInspector::FilePreviewInspector()
    {
        
    }

    //////////////////////////////////////////
    FilePreviewInspector::~FilePreviewInspector()
    {

    }

    //////////////////////////////////////////
    FilePreviewInspectorPtr FilePreviewInspector::Create(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        FilePreviewInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(FilePreviewInspector, object, init(
            _parent2D,
            _scene));
        return object;
    }

    //////////////////////////////////////////
    bool FilePreviewInspector::init(
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
    void FilePreviewInspector::update(F32 _dt)
    {
        if (m_filesDirty)
        {
            buildFiles();
            m_filesDirty = false;
        }

        
    }

    //////////////////////////////////////////
    void FilePreviewInspector::setFiles(Set<AssetFilePtr> const& _files)
    {
        m_files = _files;

        m_filesDirty = true;
    }

    //////////////////////////////////////////
    void FilePreviewInspector::buildFiles()
    {
        SceneDebugPreviewPtr scene = m_scene.lock();

        scene->clear();

        if (!m_files.empty())
        {
            AssetFilePtr const& assetFile = *m_files.begin();
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
                "Accessed: %s\n"
                "Size: %u bytes",
                creationDateLocal.formatDateTime().c_str(),
                modifiedDateLocal.formatDateTime().c_str(),
                accessedDateLocal.formatDateTime().c_str(),
                fileStats.fileSize);

            AbstractTextRenderer2DPtr info = EditorToolsUIHelper::CreateText(
                text.c_str(),
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(scene->getCanvas()->getTransform()->getHeight(), scene->getCanvas()->getTransform()->getHeight() - 4.0f),
                Vec2F(2.0f, 0.0f),
                scene->getCanvas()->getTransform(),
                scene.get(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));
            info->setColor(ColorU32::c_black);
        }
        
        m_filesDirty = false;
    }

    //////////////////////////////////////////
    bool FilePreviewInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        setFiles(_assetFiles);
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
