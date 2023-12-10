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
#include "maze-editor-tools/ecs/components/gizmos/MazeLight3DGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Light3DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Light3DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Light3DGizmos);


    //////////////////////////////////////////
    Light3DGizmos::Light3DGizmos()
    {

    }

    //////////////////////////////////////////
    Light3DGizmos::~Light3DGizmos()
    {

    }

    //////////////////////////////////////////
    Light3DGizmosPtr Light3DGizmos::Create()
    {
        Light3DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Light3DGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Light3DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Light3DGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        Light3D* light3D = _component->castRaw<Light3D>();

        Transform3DPtr const& transform3D = light3D->getTransform();

        Vec3F const& position = transform3D->getWorldPosition();

        _drawer->drawBillboard(
            position,
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::LightGizmo),
            light3D->getColor());
    }

    //////////////////////////////////////////
    void Light3DGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        Light3D* light3D = _component->castRaw<Light3D>();

        Transform3DPtr const& transform3D = light3D->getTransform();

        Vec3F const& position = transform3D->getWorldPosition();

        Mat4F mat = transform3D->getWorldTransform();
        mat[0][3] = 0.0f;
        mat[1][3] = 0.0f;
        mat[2][3] = 0.0f;

        Vec3F directionForward = mat.transformAffine(Vec3F::c_unitZ).normalizedCopy();

        _drawer->setColor(light3D->getColor());

        
        F32 const radius = 0.33f;
        
        MAZE_CONSTEXPR F32 const angleDelta = Math::DegreesToRadians(30.0f);
        Size pointsCount = (Size)Math::Ceil((Math::c_twoPi / angleDelta) - angleDelta * 0.5f);
        Vector<Vec3F> circlePoints;
        for (F32 i = 0; i < Math::c_twoPi - angleDelta * 0.5f; i += angleDelta)
        {
            F32 c = Math::Cos(i);
            F32 s = Math::Sin(i);

            Vec3F d = directionForward.crossProduct(Vec3F(c, s, 0.0f) * radius);
            circlePoints.emplace_back(position + d);
        }
        
        for (Size i = 0; i < pointsCount - 1; ++i)
        {
            Vec3F const& circlePoint0 = circlePoints[i];
            Vec3F const& circlePoint1 = circlePoints[i + 1];

            _drawer->drawLine(circlePoint0, circlePoint1);
            _drawer->drawLine(circlePoint0, circlePoint0 + directionForward);
        }

        _drawer->drawLine(circlePoints.back(), circlePoints.front());
        _drawer->drawLine(circlePoints.back(), circlePoints.back() + directionForward);
    }

} // namespace Maze
//////////////////////////////////////////
