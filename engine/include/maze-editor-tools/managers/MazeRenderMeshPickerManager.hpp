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
#if (!defined(_MazeRenderMeshPickerManager_hpp_))
#define _MazeRenderMeshPickerManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMeshPickerManager);
    

    //////////////////////////////////////////
    // Class RenderMeshPickerManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API RenderMeshPickerManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using RenderMeshPickerCallback = std::function<void(RenderMeshPtr const& _renderMesh)>;

    public:

        //////////////////////////////////////////
        virtual ~RenderMeshPickerManager();

        //////////////////////////////////////////
        static void Initialize(RenderMeshPickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline RenderMeshPickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline RenderMeshPickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openRenderMeshPicker(
            RenderMeshPickerCallback _callback,
            RenderMeshPtr const& _renderMesh = RenderMeshPtr());


        //////////////////////////////////////////
        void setRenderMesh(RenderMeshPtr const& _renderMesh);

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }


        //////////////////////////////////////////
        bool isRenderMeshPickerEditorWindowOpened();

        //////////////////////////////////////////
        bool isRenderMeshPickerEditorWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<RenderMeshPtr const&> eventRenderMeshChanged;

    protected:

        //////////////////////////////////////////
        RenderMeshPickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openRenderMeshPickerEditorWindow();

        //////////////////////////////////////////
        void closeRenderMeshPickerEditorWindow();

        //////////////////////////////////////////
        void openRenderMeshPickerEditorScene();

        //////////////////////////////////////////
        void closeRenderMeshPickerEditorScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static RenderMeshPickerManager* s_instance;

        RenderWindowPtr m_renderMeshPickerRenderWindow;

        RenderMeshPickerCallback m_callback;

        RenderMeshPtr m_renderMesh;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMeshPickerManager_hpp_
//////////////////////////////////////////
