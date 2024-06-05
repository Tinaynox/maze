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
#if (!defined(_MazeCollider2D_hpp_))
#define _MazeCollider2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Collider2D);
    MAZE_USING_SHARED_PTR(PhysicsMaterial2D);


    //////////////////////////////////////////
    // Class Collider2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Collider2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Collider2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Collider2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Collider2D();

        //////////////////////////////////////////
        static Collider2DPtr Create();


        //////////////////////////////////////////
        inline PhysicsMaterial2DPtr const& getPhysicsMaterial() const { return m_physicsMaterialRef.getMaterial(); }

        //////////////////////////////////////////
        inline PhysicsMaterial2DAssetRef const& getPhysicsMaterialRef() const { return m_physicsMaterialRef; }

        //////////////////////////////////////////
        inline void setPhysicsMaterialRef(PhysicsMaterial2DAssetRef const& _physicsMaterial) { m_physicsMaterialRef.setMaterial(_physicsMaterial.getMaterial()); }

        //////////////////////////////////////////
        void setPhysicsMaterial(String const& _physicsMaterial);


        //////////////////////////////////////////
        inline void setIsSensor(bool _value) { m_isSensor = _value; }

        //////////////////////////////////////////
        inline bool getIsSensor() const { return m_isSensor; }

    protected:

        //////////////////////////////////////////
        Collider2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

    protected:
        bool m_isSensor = false;
        PhysicsMaterial2DAssetRef m_physicsMaterialRef;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCollider2D_hpp_
//////////////////////////////////////////
