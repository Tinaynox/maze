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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeTransform2DGizmos.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosDrawer2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Transform2DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Transform2DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Transform2DGizmos);


    //////////////////////////////////////////
    Transform2DGizmos::Transform2DGizmos()
    {

    }

    //////////////////////////////////////////
    Transform2DGizmos::~Transform2DGizmos()
    {

    }

    //////////////////////////////////////////
    Transform2DGizmosPtr Transform2DGizmos::Create()
    {
        Transform2DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Transform2DGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Transform2DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Transform2DGizmos::drawGizmosSelected2D(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer2D* _drawer)
    {
        Transform2D* transform2D = _component->castRaw<Transform2D>();

        static ColorF128 const c_rectColor = ColorF128(0.35f, 0.85f, 1.0f, 1.0f);
        static ColorF128 const c_pivotColor = ColorF128(1.0f, 0.8f, 0.0f, 1.0f);
        static ColorF128 const c_anchorColor = ColorF128(0.4f, 1.0f, 0.4f, 1.0f);

        Vec2F corners[4];
        transform2D->calculateWorldCorners(corners);

        _drawer->setColor(c_rectColor);
        _drawer->drawWireRect(corners);

        // getWorldPosition() transforms local (0,0), which is the rect's
        // un-pivoted bottom-left corner (see calculateWorldCorners) -- the
        // actual pivot point in local rect space is pivot * size.
        Vec2F pivotWorldPosition = transform2D->getWorldTransform().transform(transform2D->getSize() * transform2D->getPivot());

        Transform2DPtr const& parent = transform2D->getParent();
        if (parent)
        {
            Vec2F anchorWorldPosition = parent->getWorldTransform().transform(parent->getSize() * transform2D->getAnchor());

            _drawer->setColor(c_anchorColor);
            _drawer->drawLine(anchorWorldPosition, pivotWorldPosition);
            _drawer->drawCross(anchorWorldPosition, 8.0f);
        }

        _drawer->setColor(c_pivotColor);
        _drawer->drawCross(pivotWorldPosition, 8.0f);
    }

} // namespace Maze
//////////////////////////////////////////
