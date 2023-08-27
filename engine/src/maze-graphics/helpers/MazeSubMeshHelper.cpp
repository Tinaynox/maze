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
#include "MazeGraphicsHeader.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SubMeshHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API void CalculateTriangleBasis(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            Vec3DF const& _point2,
            Vec2DF const& _uv0,
            Vec2DF const& _uv1,
            Vec2DF const& _uv2,
            Vec3DF& _tangentX,
            Vec3DF& _tangentY)
        {
            Vec3DF p = _point1 - _point0;
            Vec3DF q = _point2 - _point0;
            F32 s1 = _uv1.x - _uv0.x;
            F32 t1 = _uv1.y - _uv0.y;
            F32 s2 = _uv2.x - _uv0.x;
            F32 t2 = _uv2.y - _uv0.y;

            F32 pqMatrix[2][3];
            pqMatrix[0][0] = p[0];
            pqMatrix[0][1] = p[1];
            pqMatrix[0][2] = p[2];
            pqMatrix[1][0] = q[0];
            pqMatrix[1][1] = q[1];
            pqMatrix[1][2] = q[2];
            F32 temp = 1.0f / ( s1 * t2 - s2 * t1);
            F32 stMatrix[2][2];
            stMatrix[0][0] =  t2 * temp;
            stMatrix[0][1] = -t1 * temp;
            stMatrix[1][0] = -s2 * temp;
            stMatrix[1][1] =  s1 * temp;
            F32 tbMatrix[2][3];

            // stMatrix * pqMatrix
            tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
            tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
            tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
            tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
            tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
            tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];

            _tangentX.set(tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2]);
            _tangentY.set(tbMatrix[1][0], tbMatrix[1][1], tbMatrix[1][2]);

            _tangentX.normalize();
            _tangentY.normalize();
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool GenerateTangentsAndBitangents(
            U32 const* _indices,
            Size _indicesCount,
            Vec3DF const* _positions,
            Vec2DF const* _uvs,
            Vec3DF const* _normals,
            Size _verticesCount,
            Vector<Vec3DF>& _outTangents,
            Vector<Vec3DF>& _outBitangents)
        {
            MAZE_PROFILE_EVENT("GenerateTangentsAndBitangents");

            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;


            MAZE_ERROR_RETURN_VALUE_IF(_indicesCount % 3 != 0, false, "Indices is not a multiple of three!");
            Size trianglesCount = _indicesCount / 3;

            _outTangents.resize(_verticesCount);
            _outBitangents.resize(_verticesCount);

            for (Size triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
            {
                Size i0 = triangleIndex * 3;
                Size i1 = i0 + 1;
                Size i2 = i1 + 1;

                Size index0 = _indices[i0];
                Size index1 = _indices[i1];
                Size index2 = _indices[i2];

                Vec3DF const& p0 = _positions[index0];
                Vec3DF const& p1 = _positions[index1];
                Vec3DF const& p2 = _positions[index2];

                Vec2DF const& uv0 = _uvs[index0];
                Vec2DF const& uv1 = _uvs[index1];
                Vec2DF const& uv2 = _uvs[index2];

                Vec3DF tangent;
                Vec3DF bitangent;

                CalculateTriangleBasis(
                    p0, p1, p2,
                    uv0, uv1, uv2,
                    tangent, bitangent);

                tangents.emplace_back(tangent);
                bitangents.emplace_back(bitangent);
            }

            Vector<Vec3DF> rt;
            Vector<Vec3DF> rb;
            rt.reserve(_verticesCount);
            rb.reserve(_verticesCount);

            for (Size i = 0; i < _verticesCount; ++i)
            {
                rt.clear();
                rb.clear();

                for (Size triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
                {
                    Size i0 = triangleIndex * 3;
                    Size i1 = i0 + 1;
                    Size i2 = i1 + 1;

                    Size index0 = _indices[i0];
                    Size index1 = _indices[i1];
                    Size index2 = _indices[i2];

                    if (index0 == i || index1 == i || index2 == i)
                    {
                        rt.push_back(tangents[triangleIndex]);
                        rb.push_back(bitangents[triangleIndex]);
                    }
                }

                Vec3DF tangentRes(0.0f, 0.0f, 0.0f);
                Vec3DF bitangentRes(0.0f, 0.0f, 0.0f);
                for (Size j = 0, jn = rt.size(); j < jn; ++j)
                {
                    tangentRes += rt[j];
                    bitangentRes += rb[j];
                }
                tangentRes /= F32(rt.size());
                bitangentRes /= F32(rb.size());

                tangentRes = Math::Ortogonalize(_normals[i], tangentRes);
                bitangentRes = Math::Ortogonalize(_normals[i], bitangentRes);

                _outTangents[i] = tangentRes;
                _outBitangents[i] = bitangentRes;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void FlipX(
            RenderDrawTopology _drawTopology,
            Vector<U32>& _indices,
            Vector<Vec3DF>* _positions,
            Vector<Vec3DF>* _normals,
            Vector<Vec3DF>* _tangents)
        {
            switch (_drawTopology)
            {
                case RenderDrawTopology::Lines:
                {
                    for (S32 i = 0, in = (S32)_indices.size(); i < in; i += 2)
                        std::swap(_indices[i], _indices[i + 1]);

                    break;
                }
                case RenderDrawTopology::Triangles:
                {
                    for (S32 i = 0, in = (S32)_indices.size(); i < in; i += 3)
                        std::swap(_indices[i], _indices[i + 2]);

                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                }
            }

            auto flipX =
                [](Vector<Vec3DF>* _array)
                {
                    if (_array)
                    {
                        for (Vec3DF& value : *_array)
                            value.x = -value.x;
                    }
                };
            flipX(_positions);
            flipX(_normals);
            flipX(_tangents);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec2DF NormalizeUV(Vec2DF const& _uv)
        {
            Vec2DF uv = _uv;
            while (uv.x > 1.0f) uv.x -= 1.0f;
            while (uv.x < 0.0f) uv.x += 1.0f;
            while (uv.y > 1.0f) uv.y -= 1.0f;
            while (uv.y < 0.0f) uv.y += 1.0f;
            return uv;
        }

    } // namespace SubMeshHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
