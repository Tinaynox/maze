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
        static MAZE_CONSTEXPR F32 const c_rad2deg = 180.0f / c_pi;
        static MAZE_CONSTEXPR F32 const c_deg2rad = c_pi / 180.0f;


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
        template <class TValue>
        inline MAZE_CONSTEXPR TValue Convert(
            TValue const& _v,
            TValue const& _srcFrom,
            TValue const& _srcTo,
            TValue const& _destFrom,
            TValue const& _destTo)
        {
            return ((_v - _srcFrom) / (_srcTo - _srcFrom)) * (_destTo - _destFrom) + _destFrom;
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
        template <typename TValue>
        inline TValue SmoothLerp(TValue _v0, TValue _v1, TValue _progress)
        {
            _progress = _progress * _progress * (3 - 2 * _progress);
            return Lerp<TValue>(_v0, _v1, _progress);
        }

        // 1|y     **
        //  |    *    *
        //  |   *      *
        //  |  *        *
        //  | *          *
        //  |*            *
        //  |*____________*_x
        //  0      0.5    1
        //
        // Inverse parabolic interpolation (x = [0;1])
        template <typename TValue>
        inline TValue LerpInvParabola(TValue _x)
        {
            TValue xt = (_x * TValue(2.0) - TValue(1.0));
            TValue y = -(xt * xt) + TValue(1.0);
            return y;
        }


        // 1|y     **
        //  |    *    *
        //  |   *      *
        //  |   *      *
        //  |   *      *
        //  |  *        *
        //  |*____________*_x
        //  0      0.5    1
        //
        // Smooth inverse parabolic interpolation (x = [0;1])
        template <typename TValue>
        inline TValue LerpInvParabolaSmooth(TValue _x)
        {
            if (_x <= TValue(0.5))
                return SmoothStep(TValue(0.0), TValue(1.0), _x);
            else
                return SmoothStep(TValue(1.0), TValue(0.0), _x);
        }

        // Smooth inverse parabolic interpolation (x = [0;1])
        template <typename TValue>
        inline TValue LerpInvParabolaSmoothShift(
            TValue _x,
            TValue _xShift = TValue(0.5),
            TValue _yBegin = TValue(0.0),
            TValue _yPeak = TValue(1.0),
            TValue _yEnd = TValue(0.0))
        {
            if (_x <= _xShift)
                return SmoothStep(_yBegin, _yPeak, _x);
            else
                return SmoothStep(_yPeak, _yEnd, _x);
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
        inline MAZE_CONSTEXPR TValue Cbrt(TValue const& _value)
        {
            return static_cast<TValue>(cbrt(_value));
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue InvSqrt(TValue const& _value)
        {
            return static_cast<TValue>(1.0f / sqrt(_value));
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
        inline bool IsFinite(TValue const& _value)
        {
            return !IsNaN<TValue>(_value) && !IsInfinity<TValue>(_value);
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
        inline bool IsNear(TValue const& _value, TValue const& _otherValue, TValue const& _epsilon)
        {
            return Abs<TValue>(_value - _otherValue) <= _epsilon;
        }

        //////////////////////////////////////////
        template <class TValue = F32>
        inline bool IsNearZero(TValue const& _value, TValue const& _epsilon)
        {
            return Abs<TValue>(_value) <= _epsilon;
        }

        //////////////////////////////////////////
        inline bool IsNearZero(F32 _value, F32 _epsilon = 1e-9f)
        {
            return IsNearZero<F32>(_value, _epsilon);
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

        //////////////////////////////////////////
        inline F32 Sin(F32 _value)
        {
            return std::sinf(_value);
        }

        //////////////////////////////////////////
        inline F32 Cos(F32 _value)
        {
            return std::cosf(_value);
        }

        //////////////////////////////////////////
        inline F32 SinApprox(F32 _value)
        {
            F32 t = _value * 0.15915f;
            t = t - (S32)t;
            if (t < 0.5f)
                return (-16.0f * t * t) + (8.0f * t);
            else
                return (16.0f * t * t) - (16.0f * t) - (8.0f * t) + 8.0f;
        }

        //////////////////////////////////////////
        inline F32 CosApprox(F32 _value)
        {
            F32 t = (_value + c_halfPi) * 0.15915f;
            t = t - (S32)t;
            if (t < 0.5f)
                return (-16.0f * t * t) + (8.0f * t);
            else
                return (16.0f * t * t) - (16.0f * t) - (8.0f * t) + 8.0f;
        }


        //////////////////////////////////////////
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

        //////////////////////////////////////////
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

        //////////////////////////////////////////
        inline F32 ATan2(F32 _x, F32 _y)
        {
            return atan2f(_x, _y);
        }

        //////////////////////////////////////////
        inline F32 Exp(F32 _value)
        {
            return expf(_value);
        }

        //////////////////////////////////////////
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
        inline MAZE_CONSTEXPR Size Align(Size _x, Size _a) { return ((_x - 1) | (_a - 1)) + 1; }
    
    
    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMath_hpp_
//////////////////////////////////////////
