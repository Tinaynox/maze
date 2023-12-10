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
#include "game/gizmos/SpaceObjectGizmos.hpp"
#include "game/SpaceObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SpaceObjectGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpaceObjectGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpaceObjectGizmos);


    //////////////////////////////////////////
    SpaceObjectGizmos::SpaceObjectGizmos()
    {

    }

    //////////////////////////////////////////
    SpaceObjectGizmos::~SpaceObjectGizmos()
    {

    }

    //////////////////////////////////////////
    SpaceObjectGizmosPtr SpaceObjectGizmos::Create()
    {
        SpaceObjectGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpaceObjectGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SpaceObjectGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SpaceObjectGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {

    }

    //////////////////////////////////////////
    void SpaceObjectGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        SpaceObject* spaceObject = _component->castRaw<SpaceObject>();

        _drawer->setColor(ColorF128::c_magenta);
        _drawer->drawWireCircle(
            spaceObject->getTransform()->getLocalPosition() + spaceObject->getWeaponPointDirected(),
            Vec3F32::c_unitZ,
            0.1f);
    }

} // namespace Maze
//////////////////////////////////////////
