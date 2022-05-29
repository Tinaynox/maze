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
#if (!defined(_MazeMathAngles_hpp_))
#define _MazeMathAngles_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {
        //////////////////////////////////////////
        inline void NormalizeAnglePI(F32& _angle)
        {
            while (_angle < -c_pi) _angle += c_twoPi;
            while (_angle > c_pi) _angle -= c_twoPi;
        }

        //////////////////////////////////////////
        inline F32 NormalizedAnglePI(F32 _angle)
        {
            while (_angle < -c_pi) _angle += c_twoPi;
            while (_angle > c_pi) _angle -= c_twoPi;
            return _angle;
        }

        //////////////////////////////////////////
        inline void NormalizeAngleTwoPI(F32& _angle)
        {
            while (_angle < 0) _angle += c_twoPi;
            while (_angle > c_twoPi) _angle -= c_twoPi;
        }

        //////////////////////////////////////////
        inline F32 NormalizedAngleTwoPI(F32 _angle)
        {
            while (_angle < 0) _angle += c_twoPi;
            while (_angle > c_twoPi) _angle -= c_twoPi;
            return _angle;
        }

        //////////////////////////////////////////
        inline F32 LerpAngle(F32 _angleFrom, F32 _angleTo, F32 _ratio)
        {
            NormalizeAnglePI(_angleFrom);
            NormalizeAnglePI(_angleTo);

            if (Abs(_angleTo - _angleFrom) > c_pi)
            {
                if (_angleTo > _angleFrom)
                    _angleFrom += c_twoPi;
                else
                    _angleTo += c_twoPi;
            }

            return Lerp(_angleFrom, _angleTo, _ratio);
        }

        //////////////////////////////////////////
        inline F32 AnglesDifference(F32 _angle0, F32 _angle1)
        {
            NormalizeAnglePI(_angle0);
            NormalizeAnglePI(_angle1);

            if (Abs(_angle1 - _angle0) > c_pi)
            {
                if (_angle1 > _angle0)
                    _angle0 += c_twoPi;
                else
                    _angle1 += c_twoPi;
            }

            return _angle1 - _angle0;
        }

        //////////////////////////////////////////
        inline F32 AbsAnglesDifference(F32 _angle0, F32 _angle1)
        {
            return Abs(AnglesDifference(_angle0, _angle1));
        }
        
    
    
    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMath_hpp_
//////////////////////////////////////////
