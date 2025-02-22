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
#pragma once
#if (!defined(_MazeDirectoryPreviewInspector_hpp_))
#define _MazeDirectoryPreviewInspector_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/preview-inspectors/MazePreviewInspector.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DirectoryPreviewInspector);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Shader);
    

    //////////////////////////////////////////
    // Class DirectoryPreviewInspector
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API DirectoryPreviewInspector
        : public PreviewInspector
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DirectoryPreviewInspector, PreviewInspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(DirectoryPreviewInspector);

    public:

        //////////////////////////////////////////
        virtual ~DirectoryPreviewInspector();

        //////////////////////////////////////////
        static DirectoryPreviewInspectorPtr Create(
            Transform2DPtr const& _parent2D,
            SceneDebugPreviewPtr const& _scene);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool getCameraActive() const MAZE_OVERRIDE { return false; }

        //////////////////////////////////////////
        virtual bool getCanvasActive() const MAZE_OVERRIDE { return true; }


        //////////////////////////////////////////
        void setDirectories(Set<AssetFilePtr> const& _directories);

        //////////////////////////////////////////
        virtual bool setAssetFiles(Set<AssetFilePtr> const& _assetFiles) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        DirectoryPreviewInspector();

        //////////////////////////////////////////
        virtual bool init(
            Transform2DPtr const& _parent2D,
            SceneDebugPreviewPtr const& _scene) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void buildDirectories();

    protected:
        Set<AssetFilePtr> m_directories;

        bool m_directoriesDirty = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDirectoryPreviewInspector_hpp_
//////////////////////////////////////////
