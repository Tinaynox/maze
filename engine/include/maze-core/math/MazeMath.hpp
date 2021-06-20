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
#if (!defined(_MazeMath_hpp_))
#define _MazeMath_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {

        //////////////////////////////////////////
        static MAZE_CONSTEXPR F32 const c_pi = 3.14159265358979323846f;
        static MAZE_CONSTEXPR F32 const c_twoPi = 2.0f * c_pi;
        static MAZE_CONSTEXPR F32 const c_halfPi = 0.5f * c_pi;

        //////////////////////////////////////////
        MAZE_CORE_API extern std::random_device g_randomDevice;
        MAZE_CORE_API extern std::mt19937 g_mt19937;


        //////////////////////////////////////////
        MAZE_CORE_API bool SetupMath();

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Min(TValue const& _v0, TValue const& _v1)
        {
            return (_v0 < _v1) ? (_v0) : (_v1);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Min(TValue const& _v0, TValue const& _v1, TValue const& _v2)
        {
            return (_v0 < _v1) ? Min(_v0, _v2) : Min(_v1, _v2);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Min(TValue const& _v0, TValue const& _v1, TValue const& _v2, TValue const& _v3)
        {
            return (_v0 < _v1) ? Min(_v0, _v2, _v3) : Min(_v1, _v2, _v3);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Min(TValue const& v0, TValue const& v1, TValue const& v2, TValue const& v3, TValue const& v4)
        {
            return (v0 < v1) ? Min(v0, v2, v3, v4) : Min(v1, v2, v3, v4);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Max(TValue const& _v0, TValue const& _v1)
        {
            return (_v0 > _v1) ? (_v0) : (_v1);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Max(TValue const& _v0, TValue const& _v1, TValue const& _v2)
        {
            return (_v0 > _v1) ? Max(_v0, _v2) : Max(_v1, _v2);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Max(TValue const& _v0, TValue const& _v1, TValue const& _v2, TValue const& _v3)
        {
            return (_v0 > _v1) ? Max(_v0, _v2, _v3) : Max(_v1, _v2, _v3);
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Max(TValue const& _v0, TValue const& _v1, TValue const& _v2, TValue const& _v3, TValue const& _v4)
        {
            return (_v0 > _v1) ? Max(_v0, _v2, _v3, _v4) : Max(_v1, _v2, _v3, _v4);
        }


        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Clamp(TValue const& _v, TValue const& _min, TValue _max)
        {
            return Max(_min, Min(_max, _v));
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Clamp01(TValue const& _v)
        {
            return Max((TValue)0.0, Min((TValue)1.0, _v));
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR bool IsClamped(TValue const& _v, TValue const& _min, TValue _max)
        {
            return _v >= _min && _v <= _max;
        }

        //////////////////////////////////////////
        template <typename TValue, typename TScalar = F32>
        inline MAZE_CONSTEXPR TValue Lerp(TValue const& _v0, TValue const& _v1, TScalar _s) 
        {
            return _v0 + static_cast<TValue>((_v1 - _v0) * _s);
        }

        //////////////////////////////////////////
        template <>
        inline MAZE_CONSTEXPR U8 Lerp(U8 const& _v0, U8 const& _v1, F32 _s) 
        {
            return static_cast<U8>(static_cast<S32>(_v0) + static_cast<S32>((static_cast<S32>(_v1) - static_cast<S32>(_v0)) * _s));
        }

        //////////////////////////////////////////
        template <>
        inline MAZE_CONSTEXPR U32 Lerp(U32 const& _v0, U32 const& _v1, F32 _s) 
        {
            return static_cast<U32>(static_cast<S32>(_v0) + static_cast<S32>((static_cast<S32>(_v1) - static_cast<S32>(_v0)) * _s));
        }
    
        //////////////////////////////////////////
        template <typename TValue, typename TScalar = F32>
        inline MAZE_CONSTEXPR TValue ClampLerp(TValue const& _v0, TValue const& _v1, TScalar _s)
        {
            return _v0 + static_cast<TValue>((_v1 - _v0) * Clamp<TScalar>(_s, static_cast<TScalar>(0), static_cast<TScalar>(1)));
        }

        //////////////////////////////////////////
        template <>
        inline MAZE_CONSTEXPR U8 ClampLerp<U8, F32>(U8 const& _v0, U8 const& _v1, F32 _s)
        {
            return static_cast<U8>(static_cast<S32>(_v0) + static_cast<S32>((static_cast<S32>(_v1) - static_cast<S32>(_v0)) * Clamp<F32>(_s, static_cast<F32>(0), static_cast<F32>(1))));
        }
    
        //////////////////////////////////////////
        template <>
        inline MAZE_CONSTEXPR U32 ClampLerp<U32, F32>(U32 const& _v0, U32 const& _v1, F32 _s)
        {
            return static_cast<U32>(static_cast<S32>(_v0) + static_cast<S32>((static_cast<S32>(_v1) - static_cast<S32>(_v0)) * Clamp<F32>(_s, static_cast<F32>(0), static_cast<F32>(1))));
        }

        //////////////////////////////////////////
        template <typename TValue = F32, typename TScalar = F32>
        inline MAZE_CONSTEXPR F32 LerpFma(TValue const& _v0, TValue const& _v1, TScalar _s)
        {
            return fma(_s, _v1, fma(-_s, _v0, _v0));
        }

        //////////////////////////////////////////
        template <typename TValue = F64, typename TScalar = F64>
        inline MAZE_CONSTEXPR F64 LerpFma(TValue const& _v0, TValue const& _v1, TScalar _s)
        {
            return fma(_s, _v1, fma(-_s, _v0, _v0));
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline TValue SmoothStep(TValue _v0, TValue _v1, TValue _value)
        {
            // Scale, bias and saturate x to 0..1 range
            _value = Clamp<TValue>((_value - _v0) / (_v1 - _v0), (TValue)0.0, (TValue)1.0);

            // Evaluate polynomial
            return _value * _value * (3 - 2 * _value);
        }


        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Sqr(TValue const& _value)
        {
            return _value * _value;
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Sqrt(TValue const& _value)
        {
            return static_cast<TValue>(sqrt(_value)); 
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue InvSqrt(TValue const& _value)
        {
            return static_cast<TValue>(1.0f / sqrt(_value));
        }

        //////////////////////////////////////////
        // UnitRandom - [0;1]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue UnitRandom()
        {
            return TValue(rand()) / TValue(RAND_MAX);
        }

        //////////////////////////////////////////
        // SymmetricRandom - [-1;1]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue SymmetricRandom()
        {
            return TValue(2.0) * TValue(rand() / RAND_MAX) - TValue(1.0);
        }

        //////////////////////////////////////////
        // Random - [0;value]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Random(TValue _value)
        {
            return static_cast<TValue>(_value * UnitRandom());
        }

        //////////////////////////////////////////
        // RangeRandom - [low;high]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue RangeRandom(TValue const& _low, TValue const& _high)
        {
            return static_cast<TValue>((_high - _low) * UnitRandom() + _low);
        }

        //////////////////////////////////////////
        // RandomMT - [0;value]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline TValue RandomMT(TValue _value)
        {
            std::uniform_int_distribution<TValue> dist(static_cast<TValue>(0), _value);
            return dist(g_mt19937);
        }

        //////////////////////////////////////////
        // RangeRandomMT - [from;to)
        //
        //////////////////////////////////////////
        inline S32 RangeRandomMT(S32 _from, S32 _to)
        {
            std::uniform_int_distribution<S32> dist(_from, _to - 1);
            return dist(g_mt19937);
        }

        //////////////////////////////////////////
        template <>
        inline S32 RangeRandom(S32 const& _low, S32 const& _high)
        {
            return RangeRandomMT(_low, _high);
        }

        //////////////////////////////////////////
        inline U32 CustomRand(U32 _seed)
        {
            return ((_seed * 1103515245 + 12345) % 4294967295);
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline bool IsNaN(TValue const& _value)
        {
            return _value != _value;
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline bool IsInfinity(TValue const& _value)
        {
            if (       std::numeric_limits<TValue>::infinity() == _value
                ||    -std::numeric_limits<TValue>::infinity() == _value)
                return true;

            return false;
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Abs(TValue const& _value)
        {
            #pragma warning(push)
            #pragma warning(disable : 4146)

            return _value >= 0 ? _value : -_value;

            #pragma warning(pop)
        }

        //////////////////////////////////////////
        template <>
        inline F32 Abs<F32>(F32 const& _value)
        {
            return fabs(_value);
        }

        //////////////////////////////////////////
        template <>
        inline F64 Abs<F64>(F64 const& _value)
        {
            return fabs(_value);
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Floor(TValue const& _value)
        {
            return static_cast<TValue>(std::floor(_value));
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Ceil(TValue const& _value)
        {
            return static_cast<TValue>(std::ceil(_value));
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Round(TValue const& _value)
        {
            return static_cast<TValue>(std::round(_value));
        }

        //////////////////////////////////////////
        template <class TValue>
        inline MAZE_CONSTEXPR bool Equal(
            TValue const& _a,
            TValue const& _b,
            TValue _tolerance = std::numeric_limits<F32>::epsilon())
        {
            return(Abs(_b - _a) <= _tolerance);
        }

        ////////////////////////////////////
        inline F32 Sin(F32 _value)
        {
            return std::sinf(_value);
        }

        ////////////////////////////////////
        inline F32 Cos(F32 _value)
        {
            return std::cosf(_value);
        }

        ////////////////////////////////////
        inline F32 SinApprox(F32 _value)
        {
            F32 t = _value * 0.15915f;
            t = t - (S32)t;
            if (t < 0.5f)
                return (-16.0f * t * t) + (8.0f * t);
            else
                return (16.0f * t * t) - (16.0f * t) - (8.0f * t) + 8.0f;
        }

        ////////////////////////////////////
        inline F32 CosApprox(F32 _value)
        {
            F32 t = (_value + c_halfPi) * 0.15915f;
            t = t - (S32)t;
            if (t < 0.5f)
                return (-16.0f * t * t) + (8.0f * t);
            else
                return (16.0f * t * t) - (16.0f * t) - (8.0f * t) + 8.0f;
        }


        ////////////////////////////////////
        inline F32 ACos(F32 _value)
        {
            if (_value > -1)
            {
                if (_value < 1)
                    return acosf(_value);
                else
                    return 0;
            }
            else
            {
                return c_pi;
            }
        }

        ////////////////////////////////////
        inline F32 ASin(F32 _value)
        {
            if (_value > -1)
            {
                if (_value < 1)
                    return asinf(_value);
                else
                    return c_halfPi;
            }
            else
                return -c_halfPi;
        }

        ////////////////////////////////////
        inline F32 ATan2(F32 _x, F32 _y)
        {
            return atan2f(_x, _y);
        }

        ////////////////////////////////////
        inline F32 Exp(F32 _value)
        {
            return expf(_value);
        }

        ////////////////////////////////////
        template <class TValue>
        inline TValue Sign(TValue const& _value)
        {
            if (_value > 0)
                return (TValue)1;

            if (_value < 0)
                return (TValue)-1;

            return (TValue)0;
        }

        //////////////////////////////////////////
        template <class TValue>
        inline TValue Fract(TValue _value)
        {
            TValue avalue = Abs<TValue>(_value);
            return std::isinf(_value) ? (TValue)0.0 : Sign(_value) * (avalue - Floor<TValue>(avalue));
        }

        //////////////////////////////////////////
        inline void NormalizeAngle(F32& _angle)
        {
            while (_angle < -c_pi) _angle += c_twoPi;
            while (_angle > c_pi) _angle -= c_twoPi;
        }

        //////////////////////////////////////////
        inline F32 NormalizedAngle(F32 _angle)
        {
            while (_angle < -c_pi) _angle += c_twoPi;
            while (_angle > c_pi) _angle -= c_twoPi;
            return _angle;
        }

        //////////////////////////////////////////
        inline F32 LerpAngle(F32 _angleFrom, F32 _angleTo, F32 _ratio)
        {
            NormalizeAngle(_angleFrom);
            NormalizeAngle(_angleTo);

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
            NormalizeAngle(_angle0);
            NormalizeAngle(_angle1);

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
    
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Size Align(Size _x, Size _a) { return ((_x - 1) | (_a - 1)) + 1; }
    
    
    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMath_hpp_
//////////////////////////////////////////
