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
#if (!defined(_MazeExperimental_hpp_))
#define _MazeExperimental_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Experimental);
    MAZE_USING_SHARED_PTR(Bounds2D);


    //////////////////////////////////////////
    // Class Experimental
    //
    //////////////////////////////////////////
    class MAZE_UI_API Experimental
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Experimental, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Experimental);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Experimental();

        //////////////////////////////////////////
        static ExperimentalPtr Create();


        //////////////////////////////////////////
        int getTest() const { return m_test; }

        //////////////////////////////////////////
        void setTest(int _value) { m_test = _value; }


        //////////////////////////////////////////
        ColorGradient getColorGradient() const { return m_colorGradient; }

        //////////////////////////////////////////
        void setColorGradient(ColorGradient _value) { m_colorGradient = _value; }


        //////////////////////////////////////////
        AnimationCurve getAnimationCurve() const { return m_animationCurve; }

        //////////////////////////////////////////
        void setAnimationCurve(AnimationCurve _value) { m_animationCurve = _value; }


        //////////////////////////////////////////
        AnimationCurve getAnimationCurve2() const { return m_animationCurve2; }

        //////////////////////////////////////////
        void setAnimationCurve2(AnimationCurve _value) { m_animationCurve2 = _value; }


        //////////////////////////////////////////
        int getTest2() const { return m_test2; }

        //////////////////////////////////////////
        void setTest2(int _value) { m_test2 = _value; }

    protected:

        //////////////////////////////////////////
        Experimental();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


    protected:
        int m_test;
        ColorGradient m_colorGradient;
        AnimationCurve m_animationCurve;
        AnimationCurve m_animationCurve2;
        int m_test2;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeExperimental_hpp_
//////////////////////////////////////////
