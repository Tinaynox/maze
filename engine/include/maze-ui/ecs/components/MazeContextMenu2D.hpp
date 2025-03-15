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
#if (!defined(_MazeContextMenu2D_hpp_))
#define _MazeContextMenu2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ContextMenu2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    // Class ContextMenu2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ContextMenu2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ContextMenu2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ContextMenu2D);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        using CallbackFunction = std::function<void(MenuListTree2DPtr const&)>;

    public:

        //////////////////////////////////////////
        virtual ~ContextMenu2D();

        //////////////////////////////////////////
        static ContextMenu2DPtr Create();



        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }


        //////////////////////////////////////////
        inline CallbackFunction getCallbackFunction() const { return m_callback; }

        //////////////////////////////////////////
        inline void setCallbackFunction(CallbackFunction _callback) { m_callback = _callback; }


        //////////////////////////////////////////
        inline void setCursorButtonIndex(S32 _value) { m_cursorButtonIndex = _value; }

        //////////////////////////////////////////
        inline S32 getCursorButtonIndex() const { return m_cursorButtonIndex; }

    protected:

        //////////////////////////////////////////
        ContextMenu2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void notifyCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent& _inputEvent);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;        
        CallbackFunction m_callback;

        S32 m_cursorButtonIndex;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextMenu2D_hpp_
//////////////////////////////////////////
