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
// Note: This file defines EcsHelper::GetFirstTrunkComponent / FindComponentRawRecursive,
// declared (but not defined) in MazeEcsHelper.hpp. It requires the complete definitions
// of Transform2D and Transform3D, so include it explicitly only after
// "maze-core/ecs/components/MazeTransform2D.hpp" and
// "maze-core/ecs/components/MazeTransform3D.hpp" are already fully visible.
//////////////////////////////////////////
#pragma once
#if (!defined(_MazeEcsHelper_inl_))
#define _MazeEcsHelper_inl_


//////////////////////////////////////////
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EcsHelper
    {
        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* GetFirstTrunkComponent(Entity* _entity)
        {
            if (Transform2D* transform2D = _entity->getComponentRaw<Transform2D>())
                return transform2D->getFirstTrunkComponent<TComponent>();
            else
            if (Transform3D* transform3D = _entity->getComponentRaw<Transform3D>())
                return transform3D->getFirstTrunkComponent<TComponent>();

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* FindComponentRawRecursive(Entity* _entity)
        {
            if (TComponent* component = _entity->getComponentRaw<TComponent>())
                return component;

            if (Transform2D* transform2D = _entity->getComponentRaw<Transform2D>())
                return transform2D->findComponentRawRecursive<TComponent>();

            if (Transform3D* transform3D = _entity->getComponentRaw<Transform3D>())
                return transform3D->findComponentRawRecursive<TComponent>();

            return nullptr;
        }

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsHelper_inl_
//////////////////////////////////////////
