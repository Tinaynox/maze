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
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
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
    // Class MeshRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MeshRenderer, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RenderMeshAssetRef, renderMesh, RenderMeshAssetRef(), getRenderMeshRef, setRenderMeshRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<MaterialAssetRef>, materials, Vector<MaterialAssetRef>(), getMaterialRefs, setMaterialRefs),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MeshRenderer);


    //////////////////////////////////////////
    MeshRenderer::MeshRenderer()
        : m_renderSystem(nullptr)
        , m_enabled(true)
    {
        
    }

    //////////////////////////////////////////
    MeshRenderer::~MeshRenderer()
    {
        
    }

    //////////////////////////////////////////
    MeshRendererPtr MeshRenderer::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        MeshRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshRenderer, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool MeshRenderer::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool MeshRenderer::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<MeshRenderer>()->m_renderSystem;

        if (!Component::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void MeshRenderer::setMaterial(MaterialPtr const& _material)
    {
        Vector<MaterialAssetRef> materials = { MaterialAssetRef(_material) };
        setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    void MeshRenderer::setMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getOrLoadMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());
        setMaterial(material);
    }

    //////////////////////////////////////////
    void MeshRenderer::setMaterials(Vector<String> const& _materialNames)
    {
        Vector<MaterialAssetRef> materials;
        for (String const& materialName : _materialNames)
            materials.push_back(m_renderSystem->getMaterialManager()->getOrLoadMaterial(materialName));
        setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    void MeshRenderer::addMaterial(MaterialPtr const& _material)
    {
        Vector<MaterialAssetRef> materials = getMaterialRefs();
        materials.push_back(MaterialAssetRef(_material));
        setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    void MeshRenderer::addMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getOrLoadMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());
        addMaterial(material);
    }

    //////////////////////////////////////////
    void MeshRenderer::setMesh(MeshPtr const& _mesh)
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

        if (!m_renderMeshRef.getRenderMesh())
            m_renderMeshRef.setRenderMesh(renderTarget->createRenderMeshFromPool((S32)_mesh->getSubMeshesCount()));

        m_renderMeshRef.getRenderMesh()->loadFromMesh(
            _mesh,
            renderTarget.get());
    }

    //////////////////////////////////////////
    void MeshRenderer::setRenderMesh(String const& _renderMeshName)
    {
        setRenderMesh(m_renderSystem->getRenderMeshManager()->getOrLoadRenderMesh(_renderMeshName));
    }

    //////////////////////////////////////////
    void MeshRenderer::clearMesh()
    {
        if (!getRenderMesh())
            return;

        getRenderMesh()->clear();
    }

    //////////////////////////////////////////
    void MeshRenderer::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
    }

    //////////////////////////////////////////
    void MeshRenderer::processEntityRemoved()
    {
        //m_renderMeshRef.setRenderMesh(nullptr);
    }
    


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MeshRendererSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render3DDefaultPassGatherRenderUnitsEvent& _event,
        Entity* _entity,
        MeshRenderer* _meshRenderer,
        Transform3D* _transform3D)
    {
        if (!_meshRenderer->getEnabled())
            return;

        if (_meshRenderer->getRenderMask() && _meshRenderer->getRenderMask()->getMask() & _event.getPassParams()->renderMask)
        {
            if (_meshRenderer->getRenderMesh())
            {
                Vector<MaterialAssetRef> const& materials = _meshRenderer->getMaterialRefs();
                Vector<VertexArrayObjectPtr> const& vaos = _meshRenderer->getRenderMesh()->getVertexArrayObjects();

                if (vaos.empty())
                    return;

                Size c = Math::Max(vaos.size(), materials.size());

                for (Size i = 0, in = c; i < in; ++i)
                {
                    VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                    MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                    MaterialPtr const* material = nullptr;
                    if (!materials.empty())
                        material = &materials[i % materials.size()].getMaterial();

                    if (!material || !*material)
                        material = &_meshRenderer->getRenderSystem()->getMaterialManager()->getErrorMaterial();

                    RenderPassPtr const& firstRenderPass = (*material)->getFirstRenderPass();
#if (MAZE_DEBUG)
                    if (!firstRenderPass || !firstRenderPass->getShader())
                    {
                        Debug::LogError("Mesh(EID: %u): Shader is null!", _entity->getId());
                        return;
                    }
#endif

                    _event.getRenderUnits()->emplace_back(
                        RenderUnit
                        {
                            firstRenderPass,
                            vao,
                            _transform3D->getWorldPosition(),
                            1,
                            &_transform3D->getWorldTransform()
                        });

                }
            }
        }
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MeshRendererEntityRemoved,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        MeshRenderer* _meshRenderer)
    {
        _meshRenderer->processEntityRemoved();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MeshRendererOnEcsWorldWillBeDestroyed,
        {},
        {},
        EcsWorldWillBeDestroyedEvent const& _event,
        Entity* _entity,
        MeshRenderer* _meshRenderer)
    {
        // Release render mesh before RenderMeshPool will be destroyed
        _meshRenderer->setRenderMesh(RenderMeshPtr());
    }
    
    
} // namespace Maze
//////////////////////////////////////////
