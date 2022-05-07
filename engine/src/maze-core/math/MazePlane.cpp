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
#include "MazeCoreHeader.hpp"
#include "maze-core/math/MazePlane.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Plane
    //
    //////////////////////////////////////////
    bool Plane::raycast(Ray const& _ray, F32& _dist)
    {
        F32 denom = m_normal.dotProduct(_ray.getDirection());
        if (Math::Abs(denom) > 0.0001f)
        {
            F32 t = (m_point - _ray.getPoint()).dotProduct(m_normal) / denom;
            if (t >= 0.0f)
            {
                Vec3DF p = _ray.getPoint() + t * _ray.getDirection();
                _dist = (p - _ray.getPoint()).length();
                return true;
            }
        }
        return false;
    }
    

} // namespace Maze
//////////////////////////////////////////
