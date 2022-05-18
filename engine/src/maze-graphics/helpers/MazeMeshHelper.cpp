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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MeshHelper
    {
        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateQuadSubMesh(
            Vec2DF const& _size,
            Vec2DF const& _positionShift,
            bool _frontFaceToForward,
            Vec4DF const& _uv,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            if (_frontFaceToForward)
            {
                Maze::Vec3DF positions[] = {
                    Maze::Vec3DF(-0.5f, +0.5f, +0.0f) * _size + _positionShift,    // Top right
                    Maze::Vec3DF(-0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom right
                    Maze::Vec3DF(+0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom left
                    Maze::Vec3DF(+0.5f, +0.5f, +0.0f) * _size + _positionShift     // Top left
                };
                mesh->setPositions(positions, 4);

                Maze::Vec3DF normals[] = {
                    // Front
                    Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                    Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                    Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                    Maze::Vec3DF(+0.0f, +0.0f, +1.0f)     // Top left
                };
                mesh->setNormals(normals, 4);

                Maze::Vec4DF colors[] = {
                    _color,        // Top right
                    _color,        // Bottom right
                    _color,        // Bottom left
                    _color         // Top left
                };
                mesh->setColors(colors, 4);

                Maze::Vec2DF uvs0[] = {
                    Maze::Vec2DF(_uv.z, _uv.w),        // Top right
                    Maze::Vec2DF(_uv.z, _uv.y),        // Bottom right
                    Maze::Vec2DF(_uv.x, _uv.y),        // Bottom left
                    Maze::Vec2DF(_uv.x, _uv.w)         // Top left
                };
                mesh->setTexCoords(0, uvs0, 4);

                Maze::U32 const indices[] = {
                    0, 1, 3,                           // First triangle
                    1, 2, 3                            // Second triangle
                };
                Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
                mesh->setIndices(indices, indicesCount);

                // Generate tangents and bitangents
                Vector<Vec3DF> tangents;
                Vector<Vec3DF> bitangents;
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
                Maze::Vec3DF positions[] = {
                    Maze::Vec3DF(+0.5f, +0.5f, +0.0f) * _size + _positionShift,    // Top right
                    Maze::Vec3DF(+0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom right
                    Maze::Vec3DF(-0.5f, -0.5f, +0.0f) * _size + _positionShift,    // Bottom left
                    Maze::Vec3DF(-0.5f, +0.5f, +0.0f) * _size + _positionShift     // Top left
                };
                mesh->setPositions(positions, 4);

                Maze::Vec3DF normals[] = {
                    // Front
                    Maze::Vec3DF(+0.0f, +0.0f, -1.0f),    // Top right
                    Maze::Vec3DF(+0.0f, +0.0f, -1.0f),    // Bottom right
                    Maze::Vec3DF(+0.0f, +0.0f, -1.0f),    // Bottom left
                    Maze::Vec3DF(+0.0f, +0.0f, -1.0f)     // Top left
                };
                mesh->setNormals(normals, 4);

                Maze::Vec4DF colors[] = {
                    _color,        // Top right
                    _color,        // Bottom right
                    _color,        // Bottom left
                    _color         // Top left
                };
                mesh->setColors(colors, 4);

                Maze::Vec2DF uvs0[] = {
                    Maze::Vec2DF(_uv.z, _uv.w),        // Top right
                    Maze::Vec2DF(_uv.z, _uv.y),        // Bottom right
                    Maze::Vec2DF(_uv.x, _uv.y),        // Bottom left
                    Maze::Vec2DF(_uv.x, _uv.w)         // Top left
                };
                mesh->setTexCoords(0, uvs0, 4);

                Maze::U32 indices[] = {
                    0, 1, 3,                           // First triangle
                    1, 2, 3                            // Second triangle
                };
                Size const indicesCount = sizeof(indices) / sizeof(indices[0]);
                mesh->setIndices(indices, indicesCount);

                // Generate tangents and bitangents
                Vector<Vec3DF> tangents;
                Vector<Vec3DF> bitangents;
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
            Vec2DF const& _size,
            Vec2DF const& _positionShift,
            bool _frontFaceToForward,
            Vec4DF const& _uv,
            Vec4DF const& _color)
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
            Vec2DF const& _size,
            Vec2DF const& _positionShift,
            SpriteSliceBorder const& _sliceBorder,
            Vec2DF const& _originalSize,
            Vec4DF const& _uv,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vec2DF centerSize = Vec2DF(
                Math::Max(_size.x - _sliceBorder.left - _sliceBorder.right, 0.0f),
                Math::Max(_size.y - _sliceBorder.bottom - _sliceBorder.top, 0.0f));

            Vec2DF halfSize = _size/2.0f;
            Vec2DF centerHalfSize = centerSize/2.0f;

            Vec2DF centerPositionLB = -centerHalfSize;
            Vec2DF lbPosition = -halfSize;
            Vec2DF centerPositionRT = +centerHalfSize;
            Vec2DF rtPosition = halfSize;

            Vec2DF ltPosition = Vec2DF(-halfSize.x, centerHalfSize.y);
            Vec2DF ltPositionRT = Vec2DF(-centerHalfSize.x, halfSize.y);

            Vec2DF rbPosition = Vec2DF(centerHalfSize.x, -halfSize.y);
            Vec2DF rbPositionRT = Vec2DF(halfSize.x, -centerHalfSize.y);

            Vec2DF deltaUV = _uv.zw() - _uv.xy();

            Vec2DF centerUVLB = _uv.xy() + deltaUV * Vec2DF(_sliceBorder.left, _sliceBorder.bottom) / _originalSize;
            Vec2DF ltUVRT = Vec2DF(centerUVLB.x, _uv.w);
            Vec2DF centerUVRT = _uv.zw() - deltaUV * Vec2DF(_sliceBorder.right, _sliceBorder.top) / _originalSize;

            Maze::Vec3DF positions[] = {
                // LB
                Maze::Vec3DF(centerPositionLB.x, centerPositionLB.y, 0.0f) + _positionShift,    // Top right
                Maze::Vec3DF(centerPositionLB.x, lbPosition.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3DF(lbPosition.x, lbPosition.y, 0.0f) + _positionShift,                // Bottom left
                Maze::Vec3DF(lbPosition.x, centerPositionLB.y, 0.0f) + _positionShift,          // Top left
                // LT
                Maze::Vec3DF(ltPositionRT.x, ltPositionRT.y, 0.0f) + _positionShift,            // Top right
                Maze::Vec3DF(ltPositionRT.x, ltPosition.y, 0.0f) + _positionShift,              // Bottom right
                Maze::Vec3DF(ltPosition.x, ltPosition.y, 0.0f) + _positionShift,                // Bottom left
                Maze::Vec3DF(ltPosition.x, ltPositionRT.y, 0.0f) + _positionShift,              // Top left
                // RT
                Maze::Vec3DF(rtPosition.x, rtPosition.y, 0.0f) + _positionShift,                // Top right
                Maze::Vec3DF(rtPosition.x, centerPositionRT.y, 0.0f) + _positionShift,          // Bottom right
                Maze::Vec3DF(centerPositionRT.x, centerPositionRT.y, 0.0f) + _positionShift,    // Bottom left
                Maze::Vec3DF(centerPositionRT.x, rtPosition.y, 0.0f) + _positionShift,          // Top left
                // RB
                Maze::Vec3DF(rbPositionRT.x, rbPositionRT.y, 0.0f) + _positionShift,            // Top right
                Maze::Vec3DF(rbPositionRT.x, rbPosition.y, 0.0f) + _positionShift,              // Bottom right
                Maze::Vec3DF(rbPosition.x, rbPosition.y, 0.0f) + _positionShift,                // Bottom left
                Maze::Vec3DF(rbPosition.x, rbPositionRT.y, 0.0f) + _positionShift,              // Top left
                // L
                Maze::Vec3DF(-centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,       // Top right
                Maze::Vec3DF(-centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,      // Bottom right
                Maze::Vec3DF(-halfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,            // Bottom left
                Maze::Vec3DF(-halfSize.x, centerHalfSize.y, 0.0f) + _positionShift,             // Top left
                // T
                Maze::Vec3DF(centerHalfSize.x, halfSize.y, 0.0f) + _positionShift,              // Top right
                Maze::Vec3DF(centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,        // Bottom right
                Maze::Vec3DF(-centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,       // Bottom left
                Maze::Vec3DF(-centerHalfSize.x, halfSize.y, 0.0f) + _positionShift,             // Top left
                // R
                Maze::Vec3DF(halfSize.x, centerHalfSize.y, 0.0f) + _positionShift,              // Top right
                Maze::Vec3DF(halfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,             // Bottom right
                Maze::Vec3DF(centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,       // Bottom left
                Maze::Vec3DF(centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,        // Top left
                // B
                Maze::Vec3DF(centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,       // Top right
                Maze::Vec3DF(centerHalfSize.x, -halfSize.y, 0.0f) + _positionShift,             // Bottom right
                Maze::Vec3DF(-centerHalfSize.x, -halfSize.y, 0.0f) + _positionShift,            // Bottom left
                Maze::Vec3DF(-centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,      // Top left
                // C
                Maze::Vec3DF(centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,        // Top right
                Maze::Vec3DF(centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,       // Bottom right
                Maze::Vec3DF(-centerHalfSize.x, -centerHalfSize.y, 0.0f) + _positionShift,      // Bottom left
                Maze::Vec3DF(-centerHalfSize.x, centerHalfSize.y, 0.0f) + _positionShift,       // Top left
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3DF normals[] = {
                // LB
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // LT
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // RT
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // RB
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // L
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // T
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // B
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
                // C
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom right
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Bottom left
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),    // Top left
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4DF colors[] = {
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

            Maze::Vec2DF uvs0[] = {
                // LB
                Maze::Vec2DF(centerUVLB.x, centerUVLB.y),           // Top right
                Maze::Vec2DF(centerUVLB.x, _uv.y),                  // Bottom right
                Maze::Vec2DF(_uv.x, _uv.y),                         // Bottom left
                Maze::Vec2DF(_uv.x, centerUVLB.y),                  // Top left
                // LT
                Maze::Vec2DF(ltUVRT.x, ltUVRT.y),                   // Top right
                Maze::Vec2DF(ltUVRT.x, centerUVRT.y),               // Bottom right
                Maze::Vec2DF(_uv.x, centerUVRT.y),                  // Bottom left
                Maze::Vec2DF(_uv.x, ltUVRT.y),                      // Top left
                // RT
                Maze::Vec2DF(_uv.z, _uv.w),                         // Top right
                Maze::Vec2DF(_uv.z, centerUVRT.y),                  // Bottom right
                Maze::Vec2DF(centerUVRT.x, centerUVRT.y),           // Bottom left
                Maze::Vec2DF(centerUVRT.x, _uv.w),                  // Top left
                // RB
                Maze::Vec2DF(_uv.z, centerUVLB.y),                  // Top right
                Maze::Vec2DF(_uv.z, _uv.y),                         // Bottom right
                Maze::Vec2DF(centerUVRT.x, _uv.y),                  // Bottom left
                Maze::Vec2DF(centerUVRT.x, centerUVLB.y),           // Top left
                // L
                Maze::Vec2DF(centerUVLB.x, centerUVRT.y),           // Top right
                Maze::Vec2DF(centerUVLB.x, centerUVLB.y),           // Bottom right
                Maze::Vec2DF(_uv.x, centerUVLB.y),                  // Bottom left
                Maze::Vec2DF(_uv.x, centerUVRT.y),                  // Top left
                // T
                Maze::Vec2DF(centerUVRT.x, _uv.w),                  // Top right
                Maze::Vec2DF(centerUVRT.x, centerUVRT.y),           // Bottom right
                Maze::Vec2DF(centerUVLB.x, centerUVRT.y),           // Bottom left
                Maze::Vec2DF(centerUVLB.x, _uv.w),                  // Top left
                // R
                Maze::Vec2DF(_uv.z, centerUVRT.y),                  // Top right
                Maze::Vec2DF(_uv.z, centerUVLB.y),                  // Bottom right
                Maze::Vec2DF(centerUVRT.x, centerUVLB.y),           // Bottom left
                Maze::Vec2DF(centerUVRT.x, centerUVRT.y),           // Top left
                // B
                Maze::Vec2DF(centerUVRT.x, centerUVLB.y),           // Top right
                Maze::Vec2DF(centerUVRT.x, _uv.y),                  // Bottom right
                Maze::Vec2DF(centerUVLB.x, _uv.y),                  // Bottom left
                Maze::Vec2DF(centerUVLB.x, centerUVLB.y),           // Top left
                // C
                Maze::Vec2DF(centerUVRT.x, centerUVRT.y),           // Top right
                Maze::Vec2DF(centerUVRT.x, centerUVLB.y),           // Bottom right
                Maze::Vec2DF(centerUVLB.x, centerUVLB.y),           // Bottom left
                Maze::Vec2DF(centerUVLB.x, centerUVRT.y),           // Top left
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
            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;
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
            Vec2DF const& _size,
            Vec2DF const& _positionShift,
            SpriteSliceBorder const& _sliceBorder,
            Vec2DF const& _originalSize,
            Vec4DF const& _uv,
            Vec4DF const& _color)
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
            Vec3DF const& _size,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);
            
            Maze::Vec3DF positions[] = {
                // Front
                Maze::Vec3DF(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                Maze::Vec3DF(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3DF(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
                Maze::Vec3DF(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                // Back
                Maze::Vec3DF(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3DF(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3DF(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                Maze::Vec3DF(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                // Right
                Maze::Vec3DF(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                Maze::Vec3DF(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
                Maze::Vec3DF(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                Maze::Vec3DF(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                // Left
                Maze::Vec3DF(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3DF(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3DF(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3DF(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                // Top
                Maze::Vec3DF(-0.5f, +0.5f, -0.5f) * _size,        // -X+Y-Z
                Maze::Vec3DF(-0.5f, +0.5f, +0.5f) * _size,        // -X+Y+Z
                Maze::Vec3DF(+0.5f, +0.5f, +0.5f) * _size,        // +X+Y+Z
                Maze::Vec3DF(+0.5f, +0.5f, -0.5f) * _size,        // +X+Y-Z
                // Bottom
                Maze::Vec3DF(-0.5f, -0.5f, +0.5f) * _size,        // -X-Y+Z
                Maze::Vec3DF(-0.5f, -0.5f, -0.5f) * _size,        // -X-Y-Z
                Maze::Vec3DF(+0.5f, -0.5f, -0.5f) * _size,        // +X-Y-Z
                Maze::Vec3DF(+0.5f, -0.5f, +0.5f) * _size,        // +X-Y+Z
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3DF normals[] = {
                // Front
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),        // -X+Y+Z
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),        // -X-Y+Z
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),        // +X-Y+Z
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),        // +X+Y+Z
                // Back
                Maze::Vec3DF(+0.0f, +0.0f, -1.0f),        // -X-Y-Z
                Maze::Vec3DF(+0.0f, +0.0f, -1.0f),        // -X+Y-Z
                Maze::Vec3DF(+0.0f, +0.0f, -1.0f),        // +X+Y-Z
                Maze::Vec3DF(+0.0f, +0.0f, -1.0f),        // +X-Y-Z
                // Right
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),        // +X+Y+Z
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),        // +X-Y+Z
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),        // +X-Y-Z
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),        // +X+Y-Z
                // Left
                Maze::Vec3DF(-1.0f, +0.0f, +0.0f),        // -X+Y-Z
                Maze::Vec3DF(-1.0f, +0.0f, +0.0f),        // -X-Y-Z
                Maze::Vec3DF(-1.0f, +0.0f, +0.0f),        // -X-Y+Z
                Maze::Vec3DF(-1.0f, +0.0f, +0.0f),        // -X+Y+Z
                // Top
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),        // -X+Y-Z
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),        // -X+Y+Z
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),        // +X+Y+Z
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),        // +X+Y-Z
                // Bottom
                Maze::Vec3DF(+0.0f, -1.0f, +0.0f),        // -X-Y+Z
                Maze::Vec3DF(+0.0f, -1.0f, +0.0f),        // -X-Y-Z
                Maze::Vec3DF(+0.0f, -1.0f, +0.0f),        // +X-Y-Z
                Maze::Vec3DF(+0.0f, -1.0f, +0.0f),        // +X-Y+Z
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4DF colors[] = {
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

            Maze::Vec2DF uvs0[] = {
                // Front
                Maze::Vec2DF(1.0f, 1.0f),    // +X+Y-Z
                Maze::Vec2DF(1.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2DF(0.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2DF(0.0f, 1.0f),    // -X+Y-Z
                // Back
                Maze::Vec2DF(1.0f, 1.0f),    // +X+Y-Z
                Maze::Vec2DF(1.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2DF(0.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2DF(0.0f, 1.0f),    // -X+Y-Z
                // Right
                Maze::Vec2DF(1.0f, 1.0f),    // +X+Y+Z
                Maze::Vec2DF(1.0f, 0.0f),    // +X-Y+Z
                Maze::Vec2DF(0.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2DF(0.0f, 1.0f),    // +X+Y-Z
                // Left
                Maze::Vec2DF(1.0f, 1.0f),    // -X+Y-Z
                Maze::Vec2DF(1.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2DF(0.0f, 0.0f),    // -X-Y+Z
                Maze::Vec2DF(0.0f, 1.0f),    // -X+Y+Z
                // Top
                Maze::Vec2DF(1.0f, 1.0f),    // -X+Y-Z
                Maze::Vec2DF(1.0f, 0.0f),    // -X+Y+Z
                Maze::Vec2DF(0.0f, 0.0f),    // +X+Y+Z
                Maze::Vec2DF(0.0f, 1.0f),    // +X+Y-Z
                // Bottom
                Maze::Vec2DF(1.0f, 1.0f),    // -X-Y+Z
                Maze::Vec2DF(1.0f, 0.0f),    // -X-Y-Z
                Maze::Vec2DF(0.0f, 0.0f),    // +X-Y-Z
                Maze::Vec2DF(0.0f, 1.0f),    // +X-Y+Z
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
            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;
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
            Vec3DF const& _size,
            Vec4DF const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateCubeSubMesh(_size, _color));
            return mesh;
        }

        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateSpherifiedCubeSubMesh(
            F32 _radius,
            U32 _quality,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            static const Vec3DF origins[6] =
            {
                Vec3DF(-1.0, -1.0, -1.0),
                Vec3DF(1.0, -1.0, -1.0),
                Vec3DF(1.0, -1.0, 1.0),
                Vec3DF(-1.0, -1.0, 1.0),
                Vec3DF(-1.0, 1.0, -1.0),
                Vec3DF(-1.0, -1.0, -1.0)
            };

            static const Vec3DF rights[6] =
            {
                Vec3DF(2.0, 0.0, 0.0),
                Vec3DF(0.0, 0.0, 2.0),
                Vec3DF(-2.0, 0.0, 0.0),
                Vec3DF(0.0, 0.0, -2.0),
                Vec3DF(2.0, 0.0, 0.0),
                Vec3DF(2.0, 0.0, 0.0)
            };

            static const Vec3DF ups[6] =
            {
                Vec3DF(0.0, 2.0, 0.0),
                Vec3DF(0.0, 2.0, 0.0),
                Vec3DF(0.0, 2.0, 0.0),
                Vec3DF(0.0, 2.0, 0.0),
                Vec3DF(0.0, 0.0, 2.0),
                Vec3DF(0.0, 0.0, 2.0)
            };

            Vector<U32> indices;
            Vector<Vec3DF> positions;
            Vector<Vec3DF> normals;
            Vector<Vec4DF> colors;
            Vector<Vec2DF> uv0;

            S32 divisions = ((S32)_quality + 1) * 2;

            F32 const step = 1.0f / F32(divisions);
            Vec3DF const step3(step, step, step);

            F32 sphereRotSin = Math::Sin(Math::c_halfPi * 0.5f);
            F32 sphereRotCos = Math::Cos(Math::c_halfPi * 0.5f);

            S32 const k = divisions + 1;

            F32 const epsilon = std::numeric_limits<F32>::epsilon();
            
            // Rim
            for (S32 face = 0; face < 4; ++face)
            {
                Vec3DF const origin = origins[face];
                Vec3DF const right = rights[face];
                Vec3DF const up = ups[face];

                for (S32 j = 0; j < divisions + 1; ++j)
                {
                    Vec3DF const j3((F32)j, (F32)j, (F32)j);
                    for (S32 i = 0; i < divisions + 1; ++i)
                    {
                        Vec3DF const i3((F32)i, (F32)i, (F32)i);
                        Vec3DF const p = origin + step3 * (i3 * right + j3 * up);
                        Vec3DF const p2 = p * p;
                        Vec3DF n
                        (
                            p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                            p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                            p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                        );

                        // Rotate Y
                        F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                        F32 y0 = n.y;
                        F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;

                        Vec3DF position(x0, y0, z0);

                        Vec3DF normal = position.normalizedCopy();

                        F32 angle = -Math::ATan2(normal.x, normal.z);
                        F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                        F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                        if (face == 0 && u <= epsilon)
                            u += 1.0f;

                        normals.push_back(normal);
                        position = normal * _radius;

                        positions.emplace_back(position);
                        uv0.push_back(Vec2DF(u, v));
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
                Vec3DF const origin = origins[face];
                Vec3DF const right = rights[face];
                Vec3DF const up = ups[face];

                U32 vertexIndex = (U32)positions.size();

                // Left side
                {
                    for (S32 j = 0; j < divisions + 1; ++j)
                    {
                        Vec3DF const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = 0; i < divisions + 1 - j; ++i)
                        {
                            Vec3DF const i3((F32)i, (F32)i, (F32)i);
                            Vec3DF const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3DF const p2 = p * p;
                            Vec3DF n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3DF position(x0, y0, z0);

                            Vec3DF normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            if (u <= epsilon)
                                u += 1.0f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2DF(u, v));
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
                        Vec3DF const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = divisions - j; i < divisions + 1; ++i)
                        {
                            Vec3DF const i3((F32)i, (F32)i, (F32)i);
                            Vec3DF const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3DF const p2 = p * p;
                            Vec3DF n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3DF position(x0, y0, z0);

                            Vec3DF normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2DF(u, v));
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
                Vec3DF const origin = origins[face];
                Vec3DF const right = rights[face];
                Vec3DF const up = ups[face];

                U32 vertexIndex = (U32)positions.size();

                // Left side
                {
                    for (S32 j = 0; j < divisions + 1; ++j)
                    {
                        Vec3DF const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = 0; i < divisions + 1 - j; ++i)
                        {
                            Vec3DF const i3((F32)i, (F32)i, (F32)i);
                            Vec3DF const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3DF const p2 = p * p;
                            Vec3DF n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3DF position(x0, y0, z0);

                            Vec3DF normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            if (u <= epsilon)
                                u += 1.0f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2DF(u, v));
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
                        Vec3DF const j3((F32)j, (F32)j, (F32)j);
                        for (S32 i = divisions - j; i < divisions + 1; ++i)
                        {
                            Vec3DF const i3((F32)i, (F32)i, (F32)i);
                            Vec3DF const p = origin + step3 * (i3 * right + j3 * up);
                            Vec3DF const p2 = p * p;
                            Vec3DF n
                            (
                                p.x * std::sqrt(1.0f - 0.5f * (p2.y + p2.z) + p2.y * p2.z / 3.0f),
                                p.y * std::sqrt(1.0f - 0.5f * (p2.z + p2.x) + p2.z * p2.x / 3.0f),
                                p.z * std::sqrt(1.0f - 0.5f * (p2.x + p2.y) + p2.x * p2.y / 3.0f)
                            );

                            // Rotate Y
                            F32 x0 = sphereRotCos * n.x + sphereRotSin * n.z;
                            F32 y0 = n.y;
                            F32 z0 = -sphereRotSin * n.x + sphereRotCos * n.z;


                            Vec3DF position(x0, y0, z0);

                            Vec3DF normal = position.normalizedCopy();

                            F32 angle = -Math::ATan2(normal.x, normal.z);
                            F32 u = (angle / (Math::c_twoPi)) + 0.5f;
                            F32 v = (Math::ASin(normal.y) / Math::c_pi) + 0.5f;

                            normals.push_back(normal);
                            position = normal * _radius;

                            positions.emplace_back(position);
                            uv0.push_back(Vec2DF(u, v));
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
            for (Vec4DF& color : colors)
                color = _color;

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;
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
            Vec4DF const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateSpherifiedCubeSubMesh(_radius, _quality, _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateConeSubMesh(
            F32 _radius,
            F32 _height,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<U32> indices;
            Vector<Vec3DF> positions;
            Vector<Vec3DF> normals;
            Vector<Vec4DF> colors;
            Vector<Vec2DF> uv0;

            Vec3DF topPoint = Vec3DF::c_unitY * _height;

            Vec3DF prevPos = Vec3DF(Math::Cos(0.0f), 0.0f, Math::Sin(0.0f)) * _radius;
            Vec3DF prevNormPerp = Vec3DF(-Math::Sin(0.0f), 0.0f, Math::Cos(0.0f));
            for (S32 i = 1; i <= 24; ++i)
            {
                F32 a = (i / 24.0f) * Math::c_twoPi;
                Vec3DF pos = Vec3DF(Math::Cos(a), 0.0f, Math::Sin(a)) * _radius;
                
                positions.push_back(prevPos);
                positions.push_back(pos);
                positions.push_back(Vec3DF::c_zero);

                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3DF::c_negativeUnitY);

                positions.push_back(prevPos);
                positions.push_back(pos);
                positions.push_back(topPoint);

                Vec3DF curNormPerp(-Math::Sin(a), 0.0f, Math::Cos(a));

                Vec3DF prevNormal = (topPoint - prevPos).crossProduct(prevNormPerp).normalizedCopy();
                Vec3DF curNormal = (topPoint - pos).crossProduct(curNormPerp).normalizedCopy();
                normals.push_back(prevNormal);
                normals.push_back(curNormal);
                normals.push_back(Vec3DF::c_unitY);

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
                uv0[i] = (Vec2DF(positions[i].x / _radius, positions[i].y / _radius) + 1.0f) * 0.5f;
            }

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;
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
            Vec4DF const& _color)
        {
            MeshPtr mesh = Mesh::Create();
            mesh->addSubMesh(CreateConeSubMesh(_radius, _height, _color));
            return mesh;
        }


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateCylinderSubMesh(
            F32 _radius,
            F32 _height,
            Vec4DF const& _color)
        {
            SubMeshPtr mesh = SubMesh::Create();
            mesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<U32> indices;
            Vector<Vec3DF> positions;
            Vector<Vec3DF> normals;
            Vector<Vec4DF> colors;
            Vector<Vec2DF> uv0;

            Vec3DF prevVec = Vec3DF(Math::Cos(0.0f), 0.0f, Math::Sin(0.0f));
            Vec3DF prevPos = prevVec * _radius;
            for (S32 i = 1; i <= 24; ++i)
            {
                F32 a = (i / 24.0f) * Math::c_twoPi;
                Vec3DF vec = Vec3DF(Math::Cos(a), 0.0f, Math::Sin(a));
                Vec3DF pos = vec * _radius;

                positions.push_back(prevPos - Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(pos - Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(Vec3DF::c_zero - Vec3DF::c_unitY * _height * 0.5f);
                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3DF::c_negativeUnitY);

                positions.push_back(prevPos + Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(pos + Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(Vec3DF::c_unitY * _height * 0.5f);
                for (S32 j = 0; j < 3; ++j)
                    normals.push_back(Vec3DF::c_unitY);

                positions.push_back(prevPos - Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(pos - Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(pos + Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(pos + Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(prevPos + Vec3DF::c_unitY * _height * 0.5f);
                positions.push_back(prevPos - Vec3DF::c_unitY * _height * 0.5f);

                Vec3DF prevNormal = prevVec;
                Vec3DF curNormal = vec;
                normals.push_back(prevNormal);
                normals.push_back(curNormal);
                normals.push_back(curNormal);
                normals.push_back(curNormal);
                normals.push_back(prevNormal);
                normals.push_back(prevNormal);

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
                uv0[i] = (Vec2DF(positions[i].x / _radius, positions[i].y / _radius) + 1.0f) * 0.5f;
            }

            mesh->setPositions(&positions[0], positions.size());
            mesh->setNormals(&normals[0], normals.size());
            mesh->setTexCoords(0, &uv0[0], uv0.size());
            mesh->setColors(&colors[0], colors.size());
            mesh->setIndices(&indices[0], indices.size());

            // Generate tangents and bitangents
            Vector<Vec3DF> tangents;
            Vector<Vec3DF> bitangents;
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
            Vec4DF const& _color)
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
            Vec4DF const& _color)
        {
            return CreateCylinderSubMesh(_radius); // #TODO
        }

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateTorusMesh(
            F32 _radius,
            F32 _csRadius,
            S32 _sides,
            S32 _csSides,
            Vec4DF const& _color)
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

            Maze::Vec3DF positions[] = {
                Maze::Vec3DF(+0.0f, +0.0f, +0.0f),
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f) * _length,
                Maze::Vec3DF(+0.0f, +0.0f, +0.0f),
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f)* _length,
                Maze::Vec3DF(+0.0f, +0.0f, +0.0f),
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f)* _length,
            };
            mesh->setPositions(positions, sizeof(positions) / sizeof(positions[0]));

            Maze::Vec3DF normals[] = {
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),
                Maze::Vec3DF(+1.0f, +0.0f, +0.0f),
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),
                Maze::Vec3DF(+0.0f, +1.0f, +0.0f),
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),
                Maze::Vec3DF(+0.0f, +0.0f, +1.0f),
            };
            mesh->setNormals(normals, sizeof(normals) / sizeof(normals[0]));

            Maze::Vec4DF colors[] = {
                Maze::Vec4DF(+1.0f, +0.0f, +0.0f, +1.0f),
                Maze::Vec4DF(+1.0f, +0.0f, +0.0f, +1.0f),
                Maze::Vec4DF(+0.0f, +1.0f, +0.0f, +1.0f),
                Maze::Vec4DF(+0.0f, +1.0f, +0.0f, +1.0f),
                Maze::Vec4DF(+0.0f, +0.0f, +1.0f, +1.0f),
                Maze::Vec4DF(+0.0f, +0.0f, +1.0f, +1.0f),
            };
            mesh->setColors(colors, sizeof(colors) / sizeof(colors[0]));

            Maze::Vec4DF uvs0[] = {
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
                Maze::Vec4DF(0.0f, 0.0f, 0.0f, 0.0f),
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
