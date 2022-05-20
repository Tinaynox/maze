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
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {
        //////////////////////////////////////////
        // x^2 + px + q = 0
        MAZE_CORE_API Vector<F32> SolveNormalQuadratic(F32 _p, F32 _q)
        {
            F32 d = _p * _p - _q;
            Vector<F32> s;

            if (Math::IsNearZero(d))
            {
                s.push_back(-_p);
                return s;
            }
            else
            if (d < 0.0f)
                return Vector<F32>();

            F32 sqrtD = Math::Sqrt(d);
            
            s.push_back(sqrtD - _p);
            s.push_back(-sqrtD - _p);
            return s;
        }

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 = 0
        MAZE_CORE_API Vector<F32> SolveQuadratic(F32 _c0, F32 _c1, F32 _c2)
        {
            F32 p = _c1 / (2.0f * _c2);
            F32 q = _c0 / _c2;
            return SolveNormalQuadratic(p, q);
        }

        //////////////////////////////////////////
        // x^3 + Ax^2 + Bx + C = 0
        MAZE_CORE_API Vector<F32> SolveNormalCubic(F32 _a, F32 _b, F32 _c)
        {
            // Substitute x = t - A/3 to eliminate quadric term: t^3 + pt + q = 0 
            F32 aSq = _a * _a;
            F32 aCb = aSq * _a;
            F32 p = (-aSq / 3.0f + _b);
            F32 q = (((2.0f * aCb / 27.0f) - (_a * _b) / 3.0f) + _c);

            F32 p2 = p / 3.0f;
            F32 q2 = q / 2.0f;

            // Use Cardano's formula
            F32 pCb = p2 * p2 * p2;
            F32 d = q2 * q2 + pCb;

            Vector<F32> s;

            if (Math::IsNearZero(d))
            {
                // One triple solution
                if (Math::IsNearZero(q))
                {
                    s.push_back(0.0f);
                }
                else
                // One single and one double solution
                {
                    F32 u = Math::Cbrt(-q2);
                    s.push_back(2.0f * u);
                    s.push_back(-u);
                }
            }
            else
            // Casus irreducibilis : three real solutions
            if (d < 0)
            {
                F32 phi = Math::ACos(-q2 / Math::Sqrt(-pCb)) / 3.0f;
                F32 t = 2.0f * Math::Sqrt(-p2);
                s.push_back(t * Math::Cos(phi));
                s.push_back(-t * Math::Cos(phi + Math::c_pi / 3.0f));
                s.push_back(-t * Math::Cos(phi - Math::c_pi / 3.0f));
            }
            else
            // One real solution
            {
                F32 sqrtD = Math::Sqrt(d);
                F32 u = Math::Cbrt(-q2 + sqrtD);
                F32 v = Math::Cbrt(-q2 - sqrtD);
                F32 t = u + v;
                s.push_back(t);
                s.push_back(-t * 0.5f);                
            }

            // Resubstitute
            F32 sub = _a / 3.0f;
            for (F32& i : s)
                i -= sub;

            return s;
        }

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 = 0
        MAZE_CORE_API Vector<F32> SolveCubic(F32 _c0, F32 _c1, F32 _c2, F32 _c3)
        {
            F32 a = _c2 / _c3;
            F32 b = _c1 / _c3;
            F32 c = _c0 / _c3;
            return SolveNormalCubic(a, b, c);
        }

        //////////////////////////////////////////
        // x^4 + Ax^3 + Bx^2 + Cx + D = 0
        MAZE_CORE_API Vector<F32> SolveNormalQuartic(F32 _a, F32 _b, F32 _c, F32 _d)
        {
            // Substitute x = y - A/4 to eliminate cubic term: x^4 + px^2 + qx + r = 0
            F32 aSq = _a * _a;
            F32 p = -3.0f / 8.0f * aSq + _b;
            F32 q = 1.0f / 8.0f * aSq * _a - 1.0f / 2.0f * _a * _b + _c;
            F32 r = -3.0f / 256.0f * aSq * aSq + 1.0f / 16.0f * aSq * _b - 1.0f / 4.0f * _a * _c + _d;
            Vector<F32> s;

            if (Math::IsNearZero(r))
            {
                // No absolute term: y(y^3 + py + q) = 0
                s = SolveCubic(q, p, 0.0f, 1.0f);
                s.push_back(0.0f);
            }
            else
            {
                // Solve the resolvent cubic
                s = SolveCubic(
                    1.0f / 2.0f * r * p - 1.0f / 8.0f * q * q,
                    -r,
                    -1.0f / 2.0f * p,
                    1.0f);

                // Take the one real solution
                F32 z = s[0];

                // To build two quadric equations
                F32 u = z * z - r;
                F32 v = 2.0f * z - p;

                if (Math::IsNearZero(u))
                    u = 0.0f;
                else
                if (u > 0.0f)
                    u = Math::Sqrt(u);
                else
                    return {0.0f};

                if (Math::IsNearZero(v))
                    v = 0.0f;
                else
                if (v > 0.0f)
                    v = Math::Sqrt(v);
                else
                    return { 0.0f };

                s = SolveQuadratic(
                    z - u,
                    q < 0.0f ? -v : v,
                    1.0f);

                Vector<F32> s2 = SolveQuadratic(
                    z + u,
                    q < 0.0f ? v : -v,
                    1.0f);
                s.insert(
                    s.end(),
                    std::make_move_iterator(s2.begin()),
                    std::make_move_iterator(s2.end()));
            }

            // Resubstitute
            F32 sub = _a / 4.0f;
            for (F32& i : s)
                i -= sub;

            return s;
        }

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 + c[4]*x^4 = 0
        MAZE_CORE_API Vector<F32> SolveQuartic(F32 _c0, F32 _c1, F32 _c2, F32 _c3, F32 _c4)
        {
            F32 a = _c3 / _c4;
            F32 b = _c2 / _c4;
            F32 c = _c1 / _c4;
            F32 d = _c0 / _c4;
            return SolveNormalQuartic(a, b, c, d);
        }
        
    } // namespace Math
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
