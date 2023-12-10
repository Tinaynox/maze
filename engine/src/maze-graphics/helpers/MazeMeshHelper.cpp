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
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MeshHelper
    {
        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateQuadSubMesh(
            Vec2F const& _size,
            Vec2F const& _positionShift,
            bool _frontFaceToForward,
            Vec4F const& _uv,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            if (_frontFaceToForward)
            {
                Maze::Vec3F positions[] = {
                    Maze::Vec3F(-0.5f, +0.5f, +0.0f) * _size + _positionShift,    // Top right
                    Maze::Vec3F(-0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom right
                    Maze::Vec3F(+0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom left
                    Maze::Vec3F(+0.5f, +0.5f, +0.0f) * _size + _positionShift     // Top left
                };
                mesh->setPositions(positions, 4);

                Maze::Vec3F normals[] = {
                    // Front
                    Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                    Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                    Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                    Maze::Vec3F(+0.0f, +0.0f, +1.0f)     // Top left
                };
                mesh->setNormals(normals, 4);

                Maze::Vec4F colors[] = {
                    _color,        // Top right
                    _color,        // Bottom right
                    _color,        // Bottom left
                    _color         // Top left
                };
                mesh->setColors(colors, 4);

                Maze::Vec2F uvs0[] = {
                    Maze::Vec2F(_uv.z, _uv.w),        // Top right
                    Maze::Vec2F(_uv.z, _uv.y),        // Bottom right
                    Maze::Vec2F(_uv.x, _uv.y),        // Bottom left
                    Maze::Vec2F(_uv.x, _uv.w)         // Top left
                };
                mesh->setTexCoords(0, uvs0, 4);

                Maze::U32 const indices[] = {
                    0, 1, 3,                           // First triangle
                    1, 2, 3                            // Second triangle
                };
                Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
                mesh->setIndices(indices, indicesCount);

                // Generate tangents and bitangents
                Vector<Vec3F> tangents;
                Vector<Vec3F> bitangents;
                if (SubMeshHelper::GenerateTangentsAndBitangents(
                    &indices[0],
                    indicesCount,
                    &positions[0],
                    &uvs0[0],
                    &normals[0],
                    4,
                    tangents,
                    bitangents))
                {
                    mesh->setTangents(&tangents[0], tangents.size());
                    mesh->setBitangents(&bitangents[0], bitangents.size());
                }
            }
            else
            {
                Maze::Vec3F positions[] = {
                    Maze::Vec3F(+0.5f, +0.5f, +0.0f) * _size + _positionShift,    // Top right
                    Maze::Vec3F(+0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom right
                    Maze::Vec3F(-0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom left
                    Maze::Vec3F(-0.5f, +0.5f, +0.0f) * _size + _positionShift     // Top left
                };
                mesh->setPositions(positions, 4);

                Maze::Vec3F normals[] = {
                    // Front
                    Maze::Vec3F(+0.0f, +0.0f, -1.0f),    // Top right
                    Maze::Vec3F(+0.0f, +0.0f, -1.0f),    // Bottom right
                    Maze::Vec3F(+0.0f, +0.0f, -1.0f),    // Bottom left
                    Maze::Vec3F(+0.0f, +0.0f, -1.0f)     // Top left
                };
                mesh->setNormals(normals, 4);

                Maze::Vec4F colors[] = {
                    _color,        // Top right
                    _color,        // Bottom right
                    _color,        // Bottom left
                    _color         // Top left
                };
                mesh->setColors(colors, 4);

                Maze::Vec2F uvs0[] = {
                    Maze::Vec2F(_uv.z, _uv.w),        // Top right
                    Maze::Vec2F(_uv.z, _uv.y),        // Bottom right
                    Maze::Vec2F(_uv.x, _uv.y),        // Bottom left
                    Maze::Vec2F(_uv.x, _uv.w)         // Top left
                };
                mesh->setTexCoords(0, uvs0, 4);

                Maze::U32 indices[] = {
                    0, 1, 3,                           // First triangle
                    1, 2, 3                            // Second triangle
                };
                Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
                mesh->setIndices(indices, indicesCount);

                // Generate tangents and bitangents
                Vector<Vec3F> tangents;
                Vector<Vec3F> bitangents;
                if (SubMeshHelper::GenerateTangentsAndBitangents(
                    &indices[0],
                    indicesCount,
                    &positions[0],
                    &uvs0[0],
                    &normals[0],
                    4,
                    tangents,
                    bitangents))
                {
                    mesh->setTangents(&tangents[0], tangents.size());
                    mesh->setBitangents(&bitangents[0], bitangents.size());
                }
            }


            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateQuadMesh(
            Vec2F const& _size,
            Vec2F const& _positionShift,
            bool _frontFaceToForward,
            Vec4F const& _uv,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(
                CreateQuadSubMesh(
                    _size,
                    _positionShift,
                    _frontFaceToForward,
                    _uv,
                    _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateSlicedPanelSubMesh(
            Vec2F const& _size,
            Vec2F const& _positionShift,
            SpriteSliceBorder const& _sliceBorder,
            Vec2F const& _originalSize,
            Vec4F const& _uv,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vec2F centerSize = Vec2F(
                Math::Max(_size.x - _sliceBorder.left - _sliceBorder.right, 0.0f),
                Math::Max(_size.y - _sliceBorder.bottom - _sliceBorder.top, 0.0f));

            Vec2F halfSize = _size/2.0f;
            Vec2F centerHalfSize = centerSize/2.0f;

            //Vec2F centerPositionLB = -centerHalfSize;
            Vec2F centerPositionLB = Vec2F(_sliceBorder.left, _sliceBorder.bottom) - halfSize;
            Vec2F centerPositionRT = centerPositionLB + centerSize;

            Vec2F lbPosition = -halfSize;
            Vec2F rtPosition = halfSize;

            Vec2F deltaUV = _uv.zw() - _uv.xy();

            Vec2F centerUVLB = _uv.xy() + deltaUV * Vec2F(_sliceBorder.left, _sliceBorder.bottom) / _originalSize;
            Vec2F ltUVRT = Vec2F(centerUVLB.x, _uv.w);
            Vec2F centerUVRT = _uv.zw() - deltaUV * Vec2F(_sliceBorder.right, _sliceBorder.top) / _originalSize;

            Maze::Vec3F positions[] = {
                // LB
                Maze::Vec3F(centerPositionLB.x, centerPositionLB.y, 0.0f) + _positionShift,    // Top right
                Maze::Vec3F(centerPositionLB.x, lbPosition.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3F(lbPosition.x, lbPosition.y, 0.0f) + _positionShift,                // Bottom left
                Maze::Vec3F(lbPosition.x, centerPositionLB.y, 0.0f) + _positionShift,          // Top left
                // LT
                Maze::Vec3F(centerPositionLB.x, rtPosition.y, 0.0f) + _positionShift,          // Top right
                Maze::Vec3F(centerPositionLB.x, centerPositionRT.y, 0.0f) + _positionShift,    // Bottom right
                Maze::Vec3F(lbPosition.x, centerPositionRT.y, 0.0f) + _positionShift,          // Bottom left
                Maze::Vec3F(lbPosition.x, rtPosition.y, 0.0f) + _positionShift,                // Top left
                // RT
                Maze::Vec3F(rtPosition.x, rtPosition.y, 0.0f) + _positionShift,                // Top right
                Maze::Vec3F(rtPosition.x, centerPositionRT.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3F(centerPositionRT.x, centerPositionRT.y, 0.0f) + _positionShift,    // Bottom left
                Maze::Vec3F(centerPositionRT.x, rtPosition.y, 0.0f) + _positionShift,          // Top left
                // RB
                Maze::Vec3F(rtPosition.x, centerPositionLB.y, 0.0f) + _positionShift,          // Top right
                Maze::Vec3F(rtPosition.x, lbPosition.y, 0.0f) + _positionShift,                // Bottom right
                Maze::Vec3F(centerPositionRT.x, lbPosition.y, 0.0f) + _positionShift,          // Bottom left
                Maze::Vec3F(centerPositionRT.x, centerPositionLB.y, 0.0f) + _positionShift,    // Top left
                // L
                Maze::Vec3F(centerPositionLB.x, centerPositionRT.y, 0.0f) + _positionShift,    // Top right
                Maze::Vec3F(centerPositionLB.x, centerPositionLB.y, 0.0f) + _positionShift,    // Bottom right
                Maze::Vec3F(lbPosition.x, centerPositionLB.y, 0.0f) + _positionShift,          // Bottom left
                Maze::Vec3F(lbPosition.x, centerPositionRT.y, 0.0f) + _positionShift,          // Top left
                // T
                Maze::Vec3F(centerPositionRT.x, rtPosition.y, 0.0f) + _positionShift,          // Top right
                Maze::Vec3F(centerPositionRT.x, centerPositionRT.y, 0.0f) + _positionShift,    // Bottom right
                Maze::Vec3F(centerPositionLB.x, centerPositionRT.y, 0.0f) + _positionShift,    // Bottom left
                Maze::Vec3F(centerPositionLB.x, rtPosition.y, 0.0f) + _positionShift,          // Top left
                // R
                Maze::Vec3F(rtPosition.x, centerPositionRT.y, 0.0f) + _positionShift,          // Top right
                Maze::Vec3F(rtPosition.x, centerPositionLB.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3F(centerPositionRT.x, centerPositionLB.y, 0.0f) + _positionShift,    // Bottom left
                Maze::Vec3F(centerPositionRT.x, centerPositionRT.y, 0.0f) + _positionShift,    // Top left
                // B
                Maze::Vec3F(centerPositionRT.x, centerPositionLB.y, 0.0f) + _positionShift,    // Top right
                Maze::Vec3F(centerPositionRT.x, lbPosition.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3F(centerPositionLB.x, lbPosition.y, 0.0f) + _positionShift,          // Bottom left
                Maze::Vec3F(centerPositionLB.x, centerPositionLB.y, 0.0f) + _positionShift,    // Top left
                // C
                Maze::Vec3F(centerPositionRT.x, centerPositionRT.y, 0.0f) + _positionShift,    // Top right
                Maze::Vec3F(centerPositionRT.x, centerPositionLB.y, 0.0f) + _positionShift,    // Bottom right
                Maze::Vec3F(centerPositionLB.x, centerPositionLB.y, 0.0f) + _positionShift,    // Bottom left
                Maze::Vec3F(centerPositionLB.x, centerPositionRT.y, 0.0f) + _positionShift,    // Top left
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3F normals[] = {
                // LB
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // LT
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // RT
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // RB
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // L
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // T
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // B
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
                // C
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),    // Top left
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4F colors[] = {
                // LB
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // LT
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // RT
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // RB
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // L
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // T
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // R
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // B
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
                // C
                _color,    // Top right
                _color,    // Bottom right
                _color,    // Bottom left
                _color,    // Top left
            };
            mesh->setColors(colors, sizeof(colors) / sizeof(colors[0]));

            Maze::Vec2F uvs0[] = {
                // LB
                Maze::Vec2F(centerUVLB.x, centerUVLB.y),           // Top right
                Maze::Vec2F(centerUVLB.x, _uv.y),                  // Bottom right
                Maze::Vec2F(_uv.x, _uv.y),                         // Bottom left
                Maze::Vec2F(_uv.x, centerUVLB.y),                  // Top left
                // LT
                Maze::Vec2F(ltUVRT.x, ltUVRT.y),                   // Top right
                Maze::Vec2F(ltUVRT.x, centerUVRT.y),               // Bottom right
                Maze::Vec2F(_uv.x, centerUVRT.y),                  // Bottom left
                Maze::Vec2F(_uv.x, ltUVRT.y),                      // Top left
                // RT
                Maze::Vec2F(_uv.z, _uv.w),                         // Top right
                Maze::Vec2F(_uv.z, centerUVRT.y),                  // Bottom right
                Maze::Vec2F(centerUVRT.x, centerUVRT.y),           // Bottom left
                Maze::Vec2F(centerUVRT.x, _uv.w),                  // Top left
                // RB
                Maze::Vec2F(_uv.z, centerUVLB.y),                  // Top right
                Maze::Vec2F(_uv.z, _uv.y),                         // Bottom right
                Maze::Vec2F(centerUVRT.x, _uv.y),                  // Bottom left
                Maze::Vec2F(centerUVRT.x, centerUVLB.y),           // Top left
                // L
                Maze::Vec2F(centerUVLB.x, centerUVRT.y),           // Top right
                Maze::Vec2F(centerUVLB.x, centerUVLB.y),           // Bottom right
                Maze::Vec2F(_uv.x, centerUVLB.y),                  // Bottom left
                Maze::Vec2F(_uv.x, centerUVRT.y),                  // Top left
                // T
                Maze::Vec2F(centerUVRT.x, _uv.w),                  // Top right
                Maze::Vec2F(centerUVRT.x, centerUVRT.y),           // Bottom right
                Maze::Vec2F(centerUVLB.x, centerUVRT.y),           // Bottom left
                Maze::Vec2F(centerUVLB.x, _uv.w),                  // Top left
                // R
                Maze::Vec2F(_uv.z, centerUVRT.y),                  // Top right
                Maze::Vec2F(_uv.z, centerUVLB.y),                  // Bottom right
                Maze::Vec2F(centerUVRT.x, centerUVLB.y),           // Bottom left
                Maze::Vec2F(centerUVRT.x, centerUVRT.y),           // Top left
                // B
                Maze::Vec2F(centerUVRT.x, centerUVLB.y),           // Top right
                Maze::Vec2F(centerUVRT.x, _uv.y),                  // Bottom right
                Maze::Vec2F(centerUVLB.x, _uv.y),                  // Bottom left
                Maze::Vec2F(centerUVLB.x, centerUVLB.y),           // Top left
                // C
                Maze::Vec2F(centerUVRT.x, centerUVRT.y),           // Top right
                Maze::Vec2F(centerUVRT.x, centerUVLB.y),           // Bottom right
                Maze::Vec2F(centerUVLB.x, centerUVLB.y),           // Bottom left
                Maze::Vec2F(centerUVLB.x, centerUVRT.y),           // Top left
            };
            mesh->setTexCoords(0, uvs0, sizeof(uvs0) / sizeof(uvs0[0]));

            Maze::U32 indices[] = {
                // LB
                0 + 0, 1 + 0, 3 + 0,                                // First triangle
                1 + 0, 2 + 0, 3 + 0,                                // Second triangle
                // LT
                0 + 4, 1 + 4, 3 + 4,                                // First triangle
                1 + 4, 2 + 4, 3 + 4,                                // Second triangle
                // RT
                0 + 8, 1 + 8, 3 + 8,                                // First triangle
                1 + 8, 2 + 8, 3 + 8,                                // Second triangle
                // RB
                0 + 12, 1 + 12, 3 + 12,                             // First triangle
                1 + 12, 2 + 12, 3 + 12,                             // Second triangle
                // L
                0 + 16, 1 + 16, 3 + 16,                             // First triangle
                1 + 16, 2 + 16, 3 + 16,                             // Second triangle
                // T
                0 + 20, 1 + 20, 3 + 20,                             // First triangle
                1 + 20, 2 + 20, 3 + 20,                             // Second triangle
                // R
                0 + 24, 1 + 24, 3 + 24,                             // First triangle
                1 + 24, 2 + 24, 3 + 24,                             // Second triangle
                // B
                0 + 28, 1 + 28, 3 + 28,                             // First triangle
                1 + 28, 2 + 28, 3 + 28,                             // Second triangle
                // C
                0 + 32, 1 + 32, 3 + 32,                             // First triangle
                1 + 32, 2 + 32, 3 + 32,                             // Second triangle
            };
            Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
            mesh->setIndices(indices, indicesCount);

            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indicesCount,
                &positions[0],
                &uvs0[0],
                &normals[0],
                sizeof(positions) / sizeof(positions[0]),
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }

            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateSlicedPanelMesh(
            Vec2F const& _size,
            Vec2F const& _positionShift,
            SpriteSliceBorder const& _sliceBorder,
            Vec2F const& _originalSize,
            Vec4F const& _uv,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(
                CreateSlicedPanelSubMesh(
                    _size,
                    _positionShift,
                    _sliceBorder,
                    _originalSize,
                    _uv,
                    _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateCubeSubMesh(
            Vec3F const& _size,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);
            
            Maze::Vec3F positions[] = {
                // Front
                Maze::Vec3F(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                Maze::Vec3F(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3F(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
                Maze::Vec3F(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                // Back
                Maze::Vec3F(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3F(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3F(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                Maze::Vec3F(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                // Right
                Maze::Vec3F(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                Maze::Vec3F(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
                Maze::Vec3F(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                Maze::Vec3F(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                // Left
                Maze::Vec3F(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3F(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3F(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3F(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                // Top
                Maze::Vec3F(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3F(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                Maze::Vec3F(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                Maze::Vec3F(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                // Bottom
                Maze::Vec3F(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3F(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3F(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                Maze::Vec3F(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3F normals[] = {
                // Front
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),        // -X+Y+Z
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),        // -X-Y+Z
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),        // +X-Y+Z
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),        // +X+Y+Z
                // Back
                Maze::Vec3F(+0.0f, +0.0f, -1.0f),        // -X-Y-Z
                Maze::Vec3F(+0.0f, +0.0f, -1.0f),        // -X+Y-Z
                Maze::Vec3F(+0.0f, +0.0f, -1.0f),        // +X+Y-Z
                Maze::Vec3F(+0.0f, +0.0f, -1.0f),        // +X-Y-Z
                // Right
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),        // +X+Y+Z
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),        // +X-Y+Z
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),        // +X-Y-Z
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),        // +X+Y-Z
                // Left
                Maze::Vec3F(-1.0f, +0.0f, +0.0f),        // -X+Y-Z
                Maze::Vec3F(-1.0f, +0.0f, +0.0f),        // -X-Y-Z
                Maze::Vec3F(-1.0f, +0.0f, +0.0f),        // -X-Y+Z
                Maze::Vec3F(-1.0f, +0.0f, +0.0f),        // -X+Y+Z
                // Top
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),        // -X+Y-Z
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),        // -X+Y+Z
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),        // +X+Y+Z
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),        // +X+Y-Z
                // Bottom
                Maze::Vec3F(+0.0f, -1.0f, +0.0f),        // -X-Y+Z
                Maze::Vec3F(+0.0f, -1.0f, +0.0f),        // -X-Y-Z
                Maze::Vec3F(+0.0f, -1.0f, +0.0f),        // +X-Y-Z
                Maze::Vec3F(+0.0f, -1.0f, +0.0f),        // +X-Y+Z
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4F colors[] = {
                // Front
                _color,    // -X+Y+Z
                _color,    // -X-Y+Z
                _color,    // +X-Y+Z
                _color,    // +X+Y+Z
                // Back
                _color,    // +X+Y-Z
                _color,    // +X-Y-Z
                _color,    // -X-Y-Z
                _color,    // -X+Y-Z
                // Right
                _color,    // +X+Y+Z
                _color,    // +X-Y+Z
                _color,    // +X-Y-Z
                _color,    // +X+Y-Z
                // Left
                _color,    // -X+Y-Z
                _color,    // -X-Y-Z
                _color,    // -X-Y+Z
                _color,    // -X+Y+Z
                // Top
                _color,    // -X+Y-Z
                _color,    // -X+Y+Z
                _color,    // +X+Y+Z
                _color,    // +X+Y-Z
                // Bottom
                _color,    // -X-Y+Z
                _color,    // -X-Y-Z
                _color,    // +X-Y-Z
                _color,    // +X-Y+Z
            };
            mesh->setColors(colors, sizeof(colors) / sizeof(colors[0]));

            Maze::Vec2F uvs0[] = {
                // Front
                Maze::Vec2F(1.0f, 1.0f),    // +X+Y-Z
                Maze::Vec2F(1.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2F(0.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2F(0.0f, 1.0f),    // -X+Y-Z
                // Back
                Maze::Vec2F(1.0f, 1.0f),    // +X+Y-Z
                Maze::Vec2F(1.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2F(0.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2F(0.0f, 1.0f),    // -X+Y-Z
                // Right
                Maze::Vec2F(1.0f, 1.0f),    // +X+Y+Z
                Maze::Vec2F(1.0f, 0.0f),    // +X-Y+Z
                Maze::Vec2F(0.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2F(0.0f, 1.0f),    // +X+Y-Z
                // Left
                Maze::Vec2F(1.0f, 1.0f),    // -X+Y-Z
                Maze::Vec2F(1.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2F(0.0f, 0.0f),    // -X-Y+Z
                Maze::Vec2F(0.0f, 1.0f),    // -X+Y+Z
                // Top
                Maze::Vec2F(1.0f, 1.0f),    // -X+Y-Z
                Maze::Vec2F(1.0f, 0.0f),    // -X+Y+Z
                Maze::Vec2F(0.0f, 0.0f),    // +X+Y+Z
                Maze::Vec2F(0.0f, 1.0f),    // +X+Y-Z
                // Bottom
                Maze::Vec2F(1.0f, 1.0f),    // -X-Y+Z
                Maze::Vec2F(1.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2F(0.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2F(0.0f, 1.0f),    // +X-Y+Z
            };
            mesh->setTexCoords(0, uvs0, sizeof(uvs0) / sizeof(uvs0[0]));

            Maze::U32 indices[] = {
                // Front
                0 + 0, 1 + 0, 3 + 0,
                1 + 0, 2 + 0, 3 + 0,
                // Back
                0 + 4, 1 + 4, 3 + 4,
                1 + 4, 2 + 4, 3 + 4,
                // Right
                0 + 8, 1 + 8, 3 + 8,
                1 + 8, 2 + 8, 3 + 8,
                // Left
                0 + 12, 1 + 12, 3 + 12,
                1 + 12, 2 + 12, 3 + 12,
                // Top
                0 + 16, 1 + 16, 3 + 16,
                1 + 16, 2 + 16, 3 + 16,
                // Bottom
                0 + 20, 1 + 20, 3 + 20,
                1 + 20, 2 + 20, 3 + 20,
            };
            Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
            mesh->setIndices(indices, indicesCount);
            
            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indicesCount,
                &positions[0],
                &uvs0[0],
                &normals[0],
                24,
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }

            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCubeMesh(
            Vec3F const& _size,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateCubeSubMesh(_size, _color));
            return mesh;
        }

        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateSpherifiedCubeSubMesh(
            F32 _radius,
            U32 _quality,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            static const Vec3F origins[6] =
            {
                Vec3F(-1.0, -1.0, -1.0),
                Vec3F(1.0, -1.0, -1.0),
                Vec3F(1.0, -1.0, 1.0),
                Vec3F(-1.0, -1.0, 1.0),
                Vec3F(-1.0, 1.0, -1.0),
                Vec3F(-1.0, -1.0, -1.0)
            };

            static const Vec3F rights[6] =
            {
                Vec3F(2.0, 0.0, 0.0),
                Vec3F(0.0, 0.0, 2.0),
                Vec3F(-2.0, 0.0, 0.0),
                Vec3F(0.0, 0.0, -2.0),
                Vec3F(2.0, 0.0, 0.0),
                Vec3F(2.0, 0.0, 0.0)
            };

            static const Vec3F ups[6] =
            {
                Vec3F(0.0, 2.0, 0.0),
                Vec3F(0.0, 2.0, 0.0),
                Vec3F(0.0, 2.0, 0.0),
                Vec3F(0.0, 2.0, 0.0),
                Vec3F(0.0, 0.0, 2.0),
                Vec3F(0.0, 0.0, 2.0)
            };

            Vector<U32> indices;
            Vector<Vec3F> positions;
            Vector<Vec3F> normals;
            Vector<Vec4F> colors;
            Vector<Vec2F> uv0;

            S32 divisions = ((S32)_quality + 1) * 2;

            F32 const step = 1.0f / F32(divisions);
            Vec3F const step3(step, step, step);

            F32 sphereRotSin = Math::Sin(Math::c_halfPi * 0.5f);
            F32 sphereRotCos = Math::Cos(Math::c_halfPi * 0.5f);

            S32 const k = divisions + 1;

            F32 const epsilon = std::numeric_limits<F32>::epsilon();
            
            // Rim
            for (S32 face = 0; face < 4; ++face)
            {
                Vec3F const origin = origins[face];
                Vec3F const right = rights[face];
                Vec3F const up = ups[face];

                for (S32 j = 0; j < divisions + 1; ++j)
                {
                    Vec3F const j3((F32)j, (F32)j, (F32)j);
                    for (S32 i = 0; i < divisions + 1; ++i)
                    {
                        Vec3F const i3((F32)i, (F32)i, (F32)i);
                        Vec3F const p = origin + step3 * (i3 * right + j3 * up);
                        Vec3F const p2 = p * p;
                        Vec3F n
                        (
                            p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                            p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                            p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                        );

                        // Rotate Y
                        F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                        F32 y0 = n.y;
                        F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;

                        Vec3F position(x0, y0, z0);

                        Vec3F normal = position.normalizedCopy();

                        F32 angle = -Math::ATan2(normal.x, normal.z);
                        F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                        F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                        if (face == 0 && u <= epsilon)
                            u += 1.0f;

                        normals.push_back(normal);
                        position = normal * _radius;

                        positions.emplace_back(position);
                        uv0.push_back(Vec2F(u, v));
                    }
                }

                // Indices
                for (S32 j = 0; j < divisions; ++j)
                {
                    bool const bottom = j < (divisions / 2);
                    for (S32 i = 0; i < divisions; ++i)
                    {
                        bool const left = i < (divisions / 2);

                        S32 const a = (face * k + j) * k + i;
                        S32 const b = (face * k + j) * k + i + 1;
                        S32 const c = (face * k + j + 1) * k + i;
                        S32 const d = (face * k + j + 1) * k + i + 1;

                        if (bottom ^ left)
                        {
                            S32 a0 = a;
                            S32 b0 = c;
                            S32 c0 = d;
                            S32 d0 = b;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)b0);
                            indices.emplace_back((U32)d0);
                            indices.emplace_back((U32)b0);
                            indices.emplace_back((U32)c0);
                            indices.emplace_back((U32)d0);
                        }
                        else
                        {
                            S32 a0 = a;
                            S32 b0 = c;
                            S32 c0 = d;
                            S32 d0 = b;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)b0);
                            indices.emplace_back((U32)c0);
                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)c0);
                            indices.emplace_back((U32)d0);
                        }
                    }
                }
            }
            
    
            // Top
            for (U32 face = 4; face < 5; ++face)
            {
                Vec3F const origin = origins[face];
                Vec3F const right = rights[face];
                Vec3F const up = ups[face];

                U32 vertexIndex = (U32)positions.size();

                // Left side
                {
                    for (S32 j = 0; j < divisions + 1; ++j)
                    {
                        Vec3F const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = 0; i < divisions + 1 - j; ++i)
                        {
                            Vec3F const i3((F32)i, (F32)i, (F32)i);
                            Vec3F const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3F const p2 = p * p;
                            Vec3F n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3F position(x0, y0, z0);

                            Vec3F normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            if (u <= epsilon)
                                u += 1.0f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2F(u, v));
                        }
                    }

                    // Indices
                    S32 v0 = 0;
                    for (S32 j = 0; j < divisions; ++j)
                    {
                        for (S32 i = 0; i < divisions - j; ++i)
                        {
                            S32 a0 = vertexIndex + v0 + i;
                            S32 b0 = a0 + 1;
                            S32 c0 = vertexIndex + v0 + divisions + 1 - j + i;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)c0);
                            indices.emplace_back((U32)b0);
                            

                            if (i > 0)
                            {
                                S32 a0 = vertexIndex + v0 + i;
                                S32 b0 = vertexIndex + v0 + divisions + 1 - j + i;
                                S32 c0 = b0 - 1;

                                indices.emplace_back((U32)a0);
                                indices.emplace_back((U32)c0);
                                indices.emplace_back((U32)b0);
                            }
                        }

                        v0 += divisions + 1 - j;
                    }
                }

                vertexIndex = (U32)positions.size();

                // Right side
                {
                    for (S32 j = divisions; j >= 0; --j)
                    {
                        Vec3F const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = divisions - j; i < divisions + 1; ++i)
                        {
                            Vec3F const i3((F32)i, (F32)i, (F32)i);
                            Vec3F const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3F const p2 = p * p;
                            Vec3F n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3F position(x0, y0, z0);

                            Vec3F normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2F(u, v));
                        }
                    }

                    // Indices
                    S32 v0 = 0;
                    for (S32 j = 0; j < divisions; ++j)
                    {
                        for (S32 i = 0; i < divisions - j; ++i)
                        {
                            S32 a0 = vertexIndex + v0 + i;
                            S32 b0 = vertexIndex + v0 + divisions + 1 - j + i;
                            S32 c0 = a0 + 1;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)c0);
                            indices.emplace_back((U32)b0);

                            if (i > 0)
                            {
                                S32 a0 = vertexIndex + v0 + i;
                                S32 b0 = vertexIndex + v0 + divisions - j + i;
                                S32 c0 = vertexIndex + v0 + divisions + 1 - j + i;

                                indices.emplace_back((U32)a0);
                                indices.emplace_back((U32)c0);
                                indices.emplace_back((U32)b0);
                            }
                            
                        }

                        v0 += divisions + 1 - j;
                    }
                }
            }
            
            // Bottom
            for (U32 face = 5; face < 6; ++face)
            {
                Vec3F const origin = origins[face];
                Vec3F const right = rights[face];
                Vec3F const up = ups[face];

                U32 vertexIndex = (U32)positions.size();

                // Left side
                {
                    for (S32 j = 0; j < divisions + 1; ++j)
                    {
                        Vec3F const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = 0; i < divisions + 1 - j; ++i)
                        {
                            Vec3F const i3((F32)i, (F32)i, (F32)i);
                            Vec3F const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3F const p2 = p * p;
                            Vec3F n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3F position(x0, y0, z0);

                            Vec3F normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            if (u <= epsilon)
                                u += 1.0f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2F(u, v));
                        }
                    }

                    
                    // Indices
                    S32 v0 = 0;
                    for (S32 j = 0; j < divisions; ++j)
                    {
                        for (S32 i = 0; i < divisions - j; ++i)
                        {
                            S32 a0 = vertexIndex + v0 + i;
                            S32 b0 = a0 + 1;
                            S32 c0 = vertexIndex + v0 + divisions + 1 - j + i;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)b0);
                            indices.emplace_back((U32)c0);

                            if (i > 0)
                            {
                                S32 a0 = vertexIndex + v0 + i;
                                S32 b0 = vertexIndex + v0 + divisions + 1 - j + i;
                                S32 c0 = b0 - 1;

                                indices.emplace_back((U32)a0);
                                indices.emplace_back((U32)b0);
                                indices.emplace_back((U32)c0);
                            }
                        }

                        v0 += divisions + 1 - j;
                    }
                    
                }

                vertexIndex = (U32)positions.size();

                // Right side
                {
                    for (S32 j = divisions; j >= 0; --j)
                    {
                        Vec3F const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = divisions - j; i < divisions + 1; ++i)
                        {
                            Vec3F const i3((F32)i, (F32)i, (F32)i);
                            Vec3F const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3F const p2 = p * p;
                            Vec3F n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3F position(x0, y0, z0);

                            Vec3F normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2F(u, v));
                        }
                    }

                    // Indices
                    S32 v0 = 0;
                    for (S32 j = 0; j < divisions; ++j)
                    {
                        for (S32 i = 0; i < divisions - j; ++i)
                        {
                            S32 a0 = vertexIndex + v0 + i;
                            S32 b0 = vertexIndex + v0 + divisions + 1 - j + i;
                            S32 c0 = a0 + 1;

                            indices.emplace_back((U32)a0);
                            indices.emplace_back((U32)b0);
                            indices.emplace_back((U32)c0);

                            if (i > 0)
                            {
                                S32 a0 = vertexIndex + v0 + i;
                                S32 b0 = vertexIndex + v0 + divisions - j + i;
                                S32 c0 = vertexIndex + v0 + divisions + 1 - j + i;

                                indices.emplace_back((U32)a0);
                                indices.emplace_back((U32)b0);
                                indices.emplace_back((U32)c0);
                            }

                        }

                        v0 += divisions + 1 - j;
                    }
                }
            }

            colors.resize(positions.size());
            for (Vec4F& color : colors)
                color = _color;

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indices.size(),
                &positions[0],
                &uv0[0],
                &normals[0],
                positions.size(),
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }

            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateSpherifiedCubeMesh(
            F32 _radius,
            U32 _quality,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateSpherifiedCubeSubMesh(_radius, _quality, _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateConeSubMesh(
            F32 _radius,
            F32 _height,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<U32> indices;
            Vector<Vec3F> positions;
            Vector<Vec3F> normals;
            Vector<Vec4F> colors;
            Vector<Vec2F> uv0;

            Vec3F topPoint = Vec3F::c_unitY * _height;

            Vec3F prevPos = Vec3F(Math::Cos(0.0f), 0.0f, Math::Sin(0.0f)) * _radius;
            Vec3F prevNormPerp = Vec3F(-Math::Sin(0.0f), 0.0f, Math::Cos(0.0f));
            for (S32 i = 1; i <= 24; ++i)
            {
                F32 a = (i / 24.0f) * Math::c_twoPi;
                Vec3F pos = Vec3F(Math::Cos(a), 0.0f, Math::Sin(a)) * _radius;
                
                positions.push_back(prevPos);
                positions.push_back(pos);
                positions.push_back(Vec3F::c_zero);

                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3F::c_negativeUnitY);

                positions.push_back(topPoint);
                positions.push_back(pos);
                positions.push_back(prevPos);

                Vec3F curNormPerp(-Math::Sin(a), 0.0f, Math::Cos(a));

                Vec3F prevNormal = (topPoint - prevPos).crossProduct(prevNormPerp).normalizedCopy();
                Vec3F curNormal = (topPoint - pos).crossProduct(curNormPerp).normalizedCopy();
                normals.push_back(Vec3F::c_unitY);
                normals.push_back(curNormal);
                normals.push_back(prevNormal);

                prevPos = pos;
                prevNormPerp = curNormPerp;
            }

            indices.resize(positions.size());
            colors.resize(positions.size());
            uv0.resize(positions.size());
            for (Size i = 0; i < positions.size(); i++)
            {
                indices[i] = (U32)i;
                colors[i] = _color;
                uv0[i] = (Vec2F(positions[i].x / _radius, positions[i].y / _radius) + 1.0f) * 0.5f;
            }

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indices.size(),
                &positions[0],
                &uv0[0],
                &normals[0],
                positions.size(),
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }


            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateConeMesh(
            F32 _radius,
            F32 _height,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateConeSubMesh(_radius, _height, _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateCylinderSubMesh(
            F32 _radius,
            F32 _height,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<U32> indices;
            Vector<Vec3F> positions;
            Vector<Vec3F> normals;
            Vector<Vec4F> colors;
            Vector<Vec2F> uv0;

            Vec3F prevVec = Vec3F(Math::Cos(0.0f), 0.0f, Math::Sin(0.0f));
            Vec3F prevPos = prevVec * _radius;
            for (S32 i = 1; i <= 24; ++i)
            {
                F32 a = (i / 24.0f) * Math::c_twoPi;
                Vec3F vec = Vec3F(Math::Cos(a), 0.0f, Math::Sin(a));
                Vec3F pos = vec * _radius;

                positions.push_back(prevPos - Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(pos - Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(Vec3F::c_zero - Vec3F::c_unitY * _height * 0.5f);
                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3F::c_negativeUnitY);

                positions.push_back(Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(pos + Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(prevPos + Vec3F::c_unitY * _height * 0.5f);
                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3F::c_unitY);

                positions.push_back(pos + Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(pos - Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(prevPos - Vec3F::c_unitY * _height * 0.5f);

                positions.push_back(prevPos - Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(prevPos + Vec3F::c_unitY * _height * 0.5f);
                positions.push_back(pos + Vec3F::c_unitY * _height * 0.5f);

                Vec3F prevNormal = prevVec;
                Vec3F curNormal = vec;
                normals.push_back(curNormal);
                normals.push_back(curNormal);
                normals.push_back(prevNormal);

                normals.push_back(prevNormal);
                normals.push_back(prevNormal);
                normals.push_back(curNormal);

                prevVec = vec;
                prevPos = pos;
            }

            indices.resize(positions.size());
            colors.resize(positions.size());
            uv0.resize(positions.size());
            for (Size i = 0; i < positions.size(); i++)
            {
                indices[i] = (U32)i;
                colors[i] = _color;
                uv0[i] = (Vec2F(positions[i].x / _radius, positions[i].y / _radius) + 1.0f) * 0.5f;
            }

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indices.size(),
                &positions[0],
                &uv0[0],
                &normals[0],
                positions.size(),
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }


            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCylinderMesh(
            F32 _radius,
            F32 _height,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateCylinderSubMesh(_radius, _height, _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateTorusSubMesh(
            F32 _radius,
            F32 _csRadius,
            S32 _sides,
            S32 _csSides,
            Vec4F const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<U32> indices;
            Vector<Vec3F> positions;
            Vector<Vec3F> normals;
            Vector<Vec4F> colors;
            Vector<Vec2F> uv0;

            S32 verticesCount = (_sides + 1) * (_csSides + 1);
            S32 indicesCount = _sides * _csSides * 6;

            indices.resize(indicesCount);
            positions.resize(verticesCount);
            normals.resize(verticesCount);
            colors.resize(verticesCount);
            uv0.resize(verticesCount);

            S32 angleincs = 360 / _sides;
            S32 csAngleincs = 360 / _csSides;

            S32 vertexIndex = 0;
            for (S32 j = 0; j <= 360; j += csAngleincs)
            {
                F32 angleJ = Math::DegreesToRadians(F32(j));
                F32 currentradius = _radius + (_csRadius * Math::Cos(angleJ));
                F32 zval = _csRadius * Math::Sin(angleJ);

                for (S32 i = 0; i <= 360; i += angleincs)
                {
                    F32 angleI = Math::DegreesToRadians(F32(i));

                    positions[vertexIndex].x = currentradius * Math::Cos(angleI);
                    positions[vertexIndex].z = currentradius * Math::Sin(angleI);
                    positions[vertexIndex].y = zval;

                    F32 u = i / 360.0f;
                    F32 v = 2.0f * j / 360.0f - 1.0f;
                    if (v < 0.0f)
                        v = -v;

                    colors[vertexIndex] = _color;
                    uv0[vertexIndex] = Vec2F(u, v);

                    vertexIndex++;
                }
            }

            vertexIndex = 0;
            for (S32 j = 0; j <= 360; j += csAngleincs)
            {
                for (S32 i = 0; i <= 360; i += angleincs)
                {
                    F32 angleI = Math::DegreesToRadians(F32(i));
                    F32 xc = _radius * Math::Cos(angleI);
                    F32 yc = _radius * Math::Sin(angleI);

                    Vec3F const& pos = positions[vertexIndex];
                    Vec3F normal(
                        pos.x - xc,
                        pos.y,
                        pos.z - yc);
                    normal.normalize();
                    normals[vertexIndex] = normal;

                    vertexIndex++;
                }
            }

            S32 nextrow = _sides + 1;
            S32 index = 0;
            for (S32 i = 0; i < _csSides; i++)
            {
                for (S32 j = 0; j < _sides; j++)
                {
                    S32 a = i * nextrow + j;
                    S32 b = (i + 1) * nextrow + j;
                    S32 c = i * nextrow + j + 1;
                    S32 d = (i + 1) * nextrow + j + 1;

                    indices[index + 0] = a;
                    indices[index + 1] = b;
                    indices[index + 2] = c;

                    indices[index + 3] = c;
                    indices[index + 4] = b;
                    indices[index + 5] = d;
                    index += 6;
                }
            }

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3F> tangents;
            Vector<Vec3F> bitangents;
            if (SubMeshHelper::GenerateTangentsAndBitangents(
                &indices[0],
                indices.size(),
                &positions[0],
                &uv0[0],
                &normals[0],
                positions.size(),
                tangents,
                bitangents))
            {
                mesh->setTangents(&tangents[0], tangents.size());
                mesh->setBitangents(&bitangents[0], bitangents.size());
            }

            return mesh;
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateTorusMesh(
            F32 _radius,
            F32 _csRadius,
            S32 _sides,
            S32 _csSides,
            Vec4F const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateTorusSubMesh(_radius, _csRadius, _sides, _csSides, _color));
            return mesh;
        }



        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCoordinateAxes(F32 _length)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Lines);

            Maze::Vec3F positions[] = {
                Maze::Vec3F(+0.0f, +0.0f, +0.0f),
                Maze::Vec3F(+1.0f, +0.0f, +0.0f) * _length,
                Maze::Vec3F(+0.0f, +0.0f, +0.0f),
                Maze::Vec3F(+0.0f, +1.0f, +0.0f)* _length,
                Maze::Vec3F(+0.0f, +0.0f, +0.0f),
                Maze::Vec3F(+0.0f, +0.0f, +1.0f)* _length,
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3F normals[] = {
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),
                Maze::Vec3F(+1.0f, +0.0f, +0.0f),
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),
                Maze::Vec3F(+0.0f, +1.0f, +0.0f),
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),
                Maze::Vec3F(+0.0f, +0.0f, +1.0f),
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4F colors[] = {
                Maze::Vec4F(+1.0f, +0.0f, +0.0f, +1.0f),
                Maze::Vec4F(+1.0f, +0.0f, +0.0f, +1.0f),
                Maze::Vec4F(+0.0f, +1.0f, +0.0f, +1.0f),
                Maze::Vec4F(+0.0f, +1.0f, +0.0f, +1.0f),
                Maze::Vec4F(+0.0f, +0.0f, +1.0f, +1.0f),
                Maze::Vec4F(+0.0f, +0.0f, +1.0f, +1.0f),
            };
            mesh->setColors(colors, sizeof(colors) / sizeof(colors[0]));

            Maze::Vec4F uvs0[] = {
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4F(0.0f, 0.0f, 0.0f, 0.0f),
            };
            mesh->setTexCoords(0, uvs0, sizeof(uvs0) / sizeof(uvs0[0]));


            Maze::U32 indices[] = {
                0, 1,
                2, 3,
                4, 5,
            };
            mesh->setIndices(indices, sizeof(indices) / sizeof(indices[0]));

            return mesh;
        }

    } // namespace MeshHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
