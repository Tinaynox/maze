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
#if (!defined(_MazePreviewInspector_hpp_))
#define _MazePreviewInspector_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-editor-tools/scenes/SceneDebugPreview.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PreviewInspector);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class PreviewInspector
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PreviewInspector
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(PreviewInspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PreviewInspector);

    public:

        //////////////////////////////////////////
        virtual ~PreviewInspector();

        //////////////////////////////////////////
        virtual bool getCameraActive() const { return false; }

        //////////////////////////////////////////
        virtual bool getCanvasActive() const { return false; }

        //////////////////////////////////////////
        virtual void processCursorPress(Vec2F const& _positionOS, CursorInputEvent& _event) {};

        //////////////////////////////////////////
        virtual void processCursorDrag(Vec2F const& _positionOS, CursorInputEvent& _event) {};

        //////////////////////////////////////////
        virtual void processCursorWheel(CursorWheelInputEvent const& _event) {};
        

        //////////////////////////////////////////
        virtual bool setAssetFiles(Set<AssetFilePtr> const& _assetFiles) { return false; }

        //////////////////////////////////////////
        virtual bool setObjects(Set<ObjectPtr> const& _objects) { return false; }

    protected:

        //////////////////////////////////////////
        PreviewInspector();

        //////////////////////////////////////////
        virtual bool init(
            Transform2DPtr const& _parent2D,
            SceneDebugPreviewPtr const& _scene);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:
        Transform2DPtr m_parent2D;
        SceneDebugPreviewWPtr m_scene;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePreviewInspector_hpp_
//////////////////////////////////////////
