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
#if (!defined(_MazeTrailRenderer3DHider_hpp_))
#define _MazeTrailRenderer3DHider_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TrailRenderer3DHider);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);


    //////////////////////////////////////////
    // Class TrailRenderer3DHider
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TrailRenderer3DHider
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TrailRenderer3DHider, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TrailRenderer3DHider);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~TrailRenderer3DHider();

        //////////////////////////////////////////
        static TrailRenderer3DHiderPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);

        
        //////////////////////////////////////////
        inline void setColor(ColorGradient const& _value) { m_color = _value; }

        //////////////////////////////////////////
        inline ColorGradient const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void setDuration(Vec2F const& _value) { m_duration = _value; }

        //////////////////////////////////////////
        inline Vec2F const& getDuration() const { return m_duration; }


        //////////////////////////////////////////
        void startHiding();

    protected:

        //////////////////////////////////////////
        TrailRenderer3DHider();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            

        //////////////////////////////////////////
        void updateColor();

    protected:
        ColorGradient m_color;
        Vec2F m_duration = Vec2F(1.0f, 2.0f);

        TrailRenderer3DPtr m_trailRenderer;

        F32 m_timer = 0.0f;
        F32 m_time = 0.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrailRenderer3DHider_hpp_
//////////////////////////////////////////
