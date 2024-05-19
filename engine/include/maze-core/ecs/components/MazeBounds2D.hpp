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
#if (!defined(_MazeBounds2D_hpp_))
#define _MazeBounds2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/math/MazeAABB2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class Bounds2D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Bounds2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Bounds2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Bounds2D);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            BoundsDirty = MAZE_BIT(0)
        };

    public:

        //////////////////////////////////////////
        virtual ~Bounds2D();

        //////////////////////////////////////////
        static Bounds2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        AABB2D const& getBounds();

        //////////////////////////////////////////
        inline void dirtyBounds() { m_flags |= Flags::BoundsDirty; }

        //////////////////////////////////////////
        inline bool isDirty() const { return m_flags & Flags::BoundsDirty; }


        //////////////////////////////////////////
        void processInit(Transform2D* _transform);

    public:

        //////////////////////////////////////////
        MultiDelegate<AABB2D const&> eventBoundsCalculated;

    protected:

        //////////////////////////////////////////
        Bounds2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        AABB2D const& calculateBounds();


        //////////////////////////////////////////
        inline void setBounds(AABB2D const& _bounds) { m_bounds = _bounds; }

        //////////////////////////////////////////
        inline AABB2D const& getBounds() const { return m_bounds; }

    protected:
        Transform2DPtr m_transform;

        AABB2D m_bounds;

        S32 m_flags;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeBounds2D_hpp_
//////////////////////////////////////////
