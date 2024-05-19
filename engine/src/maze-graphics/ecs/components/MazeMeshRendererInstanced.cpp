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
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MeshRendererInstanced
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MeshRendererInstanced, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RenderMeshAssetRef, renderMesh, RenderMeshAssetRef(), getRenderMeshRef, setRenderMeshRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialAssetRef, material, MaterialAssetRef(), getMaterialRef, setMaterialRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Mat4F>, modelMatricies, Vector<Mat4F>(), getModelMatrices, setModelMatrices),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec4F>, colors, Vector<Vec4F>(), getColors, setColors),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec4F>, uv0, Vector<Vec4F>(), getUV0, setUV0),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec4F>, uv1, Vector<Vec4F>(), getUV1, setUV1));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MeshRendererInstanced);


    //////////////////////////////////////////
    MeshRendererInstanced::MeshRendererInstanced()
        : m_renderSystem(nullptr)
        , m_enabled(true)
    {
        
    }

    //////////////////////////////////////////
    MeshRendererInstanced::~MeshRendererInstanced()
    {
        
    }

    //////////////////////////////////////////
    MeshRendererInstancedPtr MeshRendererInstanced::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        MeshRendererInstancedPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshRendererInstanced, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool MeshRendererInstanced::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool MeshRendererInstanced::init(
        Component* _component,
        EcsWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<MeshRendererInstanced>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::setMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());
        setMaterial(material);
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::setMesh(MeshPtr const& _mesh)
    {
        MAZE_ERROR_RETURN_IF(!m_renderSystem, "Render system is null");

        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene()->getMetaClass()->isInheritedFrom<EcsRenderScene>(), "Entity Scene is not Render Scene");
#endif

        if (!_mesh)
        {
            clearMesh();
            return;
        }

        RenderTargetPtr const& renderTarget = getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>()->getRenderTarget();

        if (!getRenderMesh())
            setRenderMesh(renderTarget->createRenderMeshFromPool((S32)_mesh->getSubMeshesCount()));

        getRenderMesh()->loadFromMesh(
            _mesh,
            renderTarget.get());
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::setRenderMesh(String const& _renderMeshName)
    {
        setRenderMesh(m_renderSystem->getRenderMeshManager()->getRenderMesh(_renderMeshName));
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::clearMesh()
    {
        if (!getRenderMesh())
            return;

        getRenderMesh()->clear();
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::processEntityRemoved()
    {
        setRenderMesh(RenderMeshPtr());
    }

    //////////////////////////////////////////
    void MeshRendererInstanced::resize(S32 _count)
    {
        m_modelMatricies.resize(_count);
        m_colors.resize(_count);
        m_uv0.resize(_count);
        m_uv1.resize(_count);
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MeshRendererInstancedSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render3DDefaultPassGatherRenderUnitsEvent& _event,
        Entity* _entity,
        MeshRendererInstanced* _meshRenderer,
        Transform3D* _transform3D)
    {
        if (!_meshRenderer->getEnabled())
            return;

        if (_meshRenderer->getRenderMask() && _meshRenderer->getRenderMask()->getMask() & _event.getPassParams()->renderMask)
        {
            if (_meshRenderer->getRenderMesh())
            {
                Material const* material = _meshRenderer->getMaterial().get();
                if (!material)
                    material = _event.getRenderTarget()->getRenderSystem()->getMaterialManager()->getErrorMaterial().get();

#if (MAZE_DEBUG)
                if (!material->getFirstRenderPass()->getShader())
                {
                    Debug::LogError("Mesh Instanced(EID: %u): Shader is null!", _entity->getId());
                    return;
                }
#endif

                Vector<VertexArrayObjectPtr> const& vaos = _meshRenderer->getRenderMesh()->getVertexArrayObjects();

                if (vaos.empty())
                    return;

                S32 count = (S32)_meshRenderer->getModelMatrices().size();
                if (count > 0)
                {
                    for (Size i = 0, in = vaos.size(); i < in; ++i)
                    {
                        VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                        MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                        Vec4F const* uvStreams[MAZE_UV_CHANNELS_MAX];
                        memset(uvStreams, 0, sizeof(uvStreams));
                        uvStreams[0] = _meshRenderer->getUV0Data();
                        uvStreams[1] = _meshRenderer->getUV1Data();

                        _event.getRenderUnits()->emplace_back(
                            RenderUnit
                            (
                                material->getFirstRenderPass(),
                                vao,
                                _transform3D->getWorldPosition(),
                                count,
                                _meshRenderer->getModelMatricesData(),
                                _meshRenderer->getColorsData(),
                                uvStreams
                            ));
                    }

                }
            }
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
