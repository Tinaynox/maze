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
#if (!defined(_MazeToggleButton2D_hpp_))
#define _MazeToggleButton2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class ToggleButton2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ToggleButton2D
        : public Button2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ToggleButton2D, Button2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ToggleButton2D);

    public:

        //////////////////////////////////////////
        virtual ~ToggleButton2D();

        //////////////////////////////////////////
        static ToggleButton2DPtr Create();

        //////////////////////////////////////////
        void setChecked(bool _value);

        //////////////////////////////////////////
        inline bool getChecked() const { return m_checked; }


        //////////////////////////////////////////
        void setCheckByClick(bool _value) { m_checkByClick = _value; }

        //////////////////////////////////////////
        inline bool getCheckByClick() const { return m_checkByClick; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ToggleButton2D*, bool> eventCheckedChanged;

    protected:

        //////////////////////////////////////////
        ToggleButton2D();

        //////////////////////////////////////////
        using Button2D::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyCursorReleaseIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        virtual void updateTransitionUI() MAZE_OVERRIDE;

    protected:

        bool m_checked;
        bool m_checkByClick;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeToggleButton2D_hpp_
//////////////////////////////////////////
