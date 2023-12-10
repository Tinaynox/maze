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
#if (!defined(_MazePerlinNoise_hpp_))
#define _MazePerlinNoise_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeRay.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class PerlinNoise
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PerlinNoise
    {
    public:

        //////////////////////////////////////////
        static inline F32 Noise(F32 _x)
        {
            S32 x = S32(Math::Floor(_x)) & 0xFF;
            _x -= Math::Floor(_x);
            F32 u = Fade(_x);
            return Math::Lerp(Grad(c_perm[x], _x), Grad(c_perm[x + 1], _x - 1.0f), u) * 2.0f;
        }

        //////////////////////////////////////////
        static inline F32 Noise(F32 _x, F32 _y)
        {
            S32 x = S32(Math::Floor(_x)) & 0xFF;
            S32 y = S32(Math::Floor(_y)) & 0xFF;
            _x -= Math::Floor(_x);
            _y -= Math::Floor(_y);
            F32 u = Fade(_x);
            F32 v = Fade(_y);
            S32 a = (c_perm[x] + y) & 0xFF;
            S32 b = (c_perm[x + 1] + y) & 0xFF;
            return Math::Lerp(
                Math::Lerp(Grad(c_perm[a], _x, _y), Grad(c_perm[b], _x - 1.0f, _y), u),
                Math::Lerp(Grad(c_perm[a + 1], _x, _y - 1), Grad(c_perm[b + 1], _x - 1.0f, _y - 1.0f), u),
                v);
        }

        //////////////////////////////////////////
        static inline F32 Noise(Vec2F const& _coord)
        {
            return Noise(_coord.x, _coord.y);
        }

        //////////////////////////////////////////
        static inline F32 Noise(F32 _x, F32 _y, F32 _z)
        {
            S32 x = S32(Math::Floor(_x)) & 0xFF;
            S32 y = S32(Math::Floor(_y)) & 0xFF;
            S32 z = S32(Math::Floor(_z)) & 0xFF;
            _x -= Math::Floor(_x);
            _y -= Math::Floor(_y);
            _z -= Math::Floor(_z);
            F32 u = Fade(_x);
            F32 v = Fade(_y);
            F32 w = Fade(_z);
            S32 a = (c_perm[x] + y) & 0xFF;
            S32 b = (c_perm[x + 1] + y) & 0xFF;
            S32 aa = (c_perm[a] + z) & 0xFF;
            S32 ba = (c_perm[b] + z) & 0xFF;
            S32 ab = (c_perm[a + 1] + z) & 0xFF;
            S32 bb = (c_perm[b + 1] + z) & 0xFF;
            return Math::Lerp(Math::Lerp(Math::Lerp(Grad(c_perm[aa], _x, _y, _z), Grad(c_perm[ba], _x - 1.0f, _y, _z), u),
                                         Math::Lerp(Grad(c_perm[ab], _x, _y - 1.0f, _z), Grad(c_perm[bb], _x - 1.0f, _y - 1.0f, _z), u), v),
                              Math::Lerp(Math::Lerp(Grad(c_perm[aa + 1], _x, _y, _z - 1.0f), Grad(c_perm[ba + 1], _x - 1.0f, _y, _z - 1.0f), u),
                                         Math::Lerp(Grad(c_perm[ab + 1], _x, _y - 1.0f, _z - 1.0f), Grad(c_perm[bb + 1], _x - 1.0f, _y - 1.0f, _z - 1.0f), u), v),
                              w);
        }

        //////////////////////////////////////////
        static inline F32 Noise(Vec3F const& _coord)
        {
            return Noise(_coord.x, _coord.y, _coord.z);
        }


        //////////////////////////////////////////
        // Fractal Brownian Motion
        //
        //////////////////////////////////////////
        static inline F32 Fbm(F32 _x, S32 _octave)
        {
            F32 f = 0.0f;
            F32 w = 0.5f;
            for (S32 i = 0; i < _octave; i++)
            {
                f += w * Noise(_x);
                _x *= 2.0f;
                w *= 0.5f;
            }
            return f;
        }

        //////////////////////////////////////////
        static inline F32 Fbm(Vec2F _coord, S32 _octave)
        {
            F32 f = 0.0f;
            F32 w = 0.5f;
            for (S32 i = 0; i < _octave; i++)
            {
                f += w * Noise(_coord);
                _coord *= 2.0f;
                w *= 0.5f;
            }
            return f;
        }

        //////////////////////////////////////////
        static inline F32 Fbm(F32 _x, F32 _y, S32 _octave)
        {
            return Fbm(Vec2F(_x, _y), _octave);
        }

        //////////////////////////////////////////
        static inline F32 Fbm(Vec3F _coord, S32 _octave)
        {
            F32 f = 0.0f;
            F32 w = 0.5f;
            for (S32 i = 0; i < _octave; i++)
            {
                f += w * Noise(_coord);
                _coord *= 2.0f;
                w *= 0.5f;
            }
            return f;
        }

        //////////////////////////////////////////
        static inline F32 Fbm(F32 _x, F32 _y, F32 _z, S32 _octave)
        {
            return Fbm(Vec3F(_x, _y, _z), _octave);
        }

    private:

        //////////////////////////////////////////
        PerlinNoise() = delete;

        //////////////////////////////////////////
        static inline F32 Fade(F32 _t)
        {
            return _t * _t * _t * (_t * (_t * 6.0f - 15.0f) + 10.0f);
        }

        //////////////////////////////////////////
        static inline F32 Grad(S32 _hash, F32 _x)
        {
            return (_hash & 1) == 0 ? _x : -_x;
        }

        //////////////////////////////////////////
        static inline F32 Grad(S32 _hash, F32 _x, F32 _y)
        {
            return ((_hash & 1) == 0 ? _x : -_x) + ((_hash & 2) == 0 ? _y : -_y);
        }

        //////////////////////////////////////////
        static inline F32 Grad(S32 _hash, F32 _x, F32 _y, F32 _z)
        {
            S32 h = _hash & 15;
            F32 u = h < 8 ? _x : _y;
            F32 v = h < 4 ? _y : (h == 12 || h == 14 ? _x : _z);
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }
        
    private:
        static S32 const c_perm[];
        
    };



} // namespace Maze
//////////////////////////////////////////


#endif // _MazePerlinNoise_hpp_
//////////////////////////////////////////
