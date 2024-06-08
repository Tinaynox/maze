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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DebugGridRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DebugGridRenderer, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(DebugGridRenderer);

    //////////////////////////////////////////
    DebugGridRenderer::DebugGridRenderer()
        : m_renderSystem(nullptr)
        , m_camera(nullptr)
        , m_prevMeshCameraPosition(Vec3F::c_zero)
    {
    }

    //////////////////////////////////////////
    DebugGridRenderer::~DebugGridRenderer()
    {

    }

    //////////////////////////////////////////
    DebugGridRendererPtr DebugGridRenderer::Create(Camera3DPtr const& _camera, RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        DebugGridRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(DebugGridRenderer, object, init(_camera, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool DebugGridRenderer::init(Camera3DPtr const& _camera, RenderSystem* _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_camera = _camera;

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void DebugGridRenderer::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("DebugGridRenderer::update");

        Vec3F const& cameraPosition = m_camera->getTransform()->getWorldPosition();

        if (cameraPosition.squaredDistance(m_prevMeshCameraPosition) >= 1.0f)
        {
            updateMesh();
        }
    }

    //////////////////////////////////////////
    void DebugGridRenderer::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();

        updateMesh();
    }

    //////////////////////////////////////////
    void DebugGridRenderer::updateMesh()
    {
        MeshPtr mesh = Mesh::Create();
        mesh->setRenderDrawTopology(RenderDrawTopology::Lines);

        Size iterations = 128;
        F32 const increment = 1.0f;
        Vec4F const primaryColor = ColorU32(192, 192, 192, 130).toVec4F32();
        Vec4F const secondaryColor = ColorU32(192, 192, 192, 50).toVec4F32();
        S32 const majorLineIncrement = 10;

        F32 length = iterations * increment;
        ++iterations;

        Vector<Vec3F> positions;
        Vector<Vec3F> normals;
        Vector<Vec4F> colors;
        Vector<U16> indices;

        positions.resize(iterations * 4);
        normals.resize(iterations * 4);
        colors.resize(iterations * 4);
        indices.resize(iterations * 4);

        Vec3F pivot(
            m_camera->getTransform()->getWorldPosition().x,
            0.0f,
            m_camera->getTransform()->getWorldPosition().z);

        Vec3F tangent = Vec3F::c_unitX;
        Vec3F bitangent = Vec3F::c_unitZ;

        static std::function<F32(Vec3F const& _val, Vec3F const& _mask)> const valueFromMaskFunc =
            [](Vec3F const& _val, Vec3F const& _mask) -> F32
            {
                if (Math::Abs(_mask.x) > 0.0001f)
                    return _val.x;
                else
                    if (Math::Abs(_mask.y) > 0.0001f)
                        return _val.y;
                    else
                        return _val.z;
            };
    

        Vec3F start = pivot - tangent * (length * 0.5f) - bitangent * (length * 0.5f);

        static std::function<Vec3F(Vec3F const& _val, Vec3F const& _mask, F32 _snapValue)> const snapValueByMaskFunc =
            [&](Vec3F const& _val, Vec3F const& _mask, F32 _snapValue) -> Vec3F
        {
            F32 x = _val.x;
            F32 y = _val.y;
            F32 z = _val.z;

            F32 const c_epsilon = 0.0001f;
            return Vec3F(
                Math::Abs(_mask.x) < c_epsilon ? x : _snapValue * Math::Round(x / _snapValue),
                Math::Abs(_mask.y) < c_epsilon ? y : _snapValue * Math::Round(y / _snapValue),
                Math::Abs(_mask.z) < c_epsilon ? z : _snapValue * Math::Round(z / _snapValue));
        };

        start = snapValueByMaskFunc(start, bitangent + tangent, increment);

        S32 startTangentValue = (S32)valueFromMaskFunc(start, tangent);
        S32 startBitangentValue = (S32)valueFromMaskFunc(start, bitangent);

        S32 highlightOffsetTangent = (S32)((startTangentValue % S32(increment * majorLineIncrement)) / increment);
        S32 highlightOffsetBitangent = (S32)((startBitangentValue % S32(increment * majorLineIncrement)) / increment);
        

        U16 vertex = 0;
        U32 index = 0;

        for (Size i = 0; i < iterations; ++i)
        {
            Vec3F a = start + tangent * (F32)i * increment;
            Vec3F b = start + bitangent * (F32)i * increment;

            positions[vertex + 0] = a;
            positions[vertex + 1] = a + bitangent * length;

            positions[vertex + 2] = b;
            positions[vertex + 3] = b + tangent * length;

            indices[index++] = vertex;
            indices[index++] = vertex + 1;
            indices[index++] = vertex + 2;
            indices[index++] = vertex + 3;

            Vec4F color = (i + highlightOffsetTangent) % majorLineIncrement == 0 ? primaryColor : secondaryColor;

            colors[vertex + 0] = color;
            colors[vertex + 1] = color;

            color = (i + highlightOffsetBitangent) % majorLineIncrement == 0 ? primaryColor : secondaryColor;

            colors[vertex + 2] = color;
            colors[vertex + 3] = color;

            vertex += 4;
        }

        Vec3F normal = bitangent.crossProduct(tangent);
        for (Size i = 0; i < iterations * 4; ++i)
            normals[i] = normal;

        mesh->setPositions(&positions[0], positions.size());
        mesh->setNormals(&normals[0], normals.size());
        mesh->setColors(&colors[0], colors.size());
        mesh->setIndices(&indices[0], indices.size());
    
        m_meshRenderer->setMaterial(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::DebugGrid));

        m_meshRenderer->setMesh(mesh);

        m_prevMeshCameraPosition = m_camera->getTransform()->getWorldPosition();
    }

    
} // namespace Maze
//////////////////////////////////////////
