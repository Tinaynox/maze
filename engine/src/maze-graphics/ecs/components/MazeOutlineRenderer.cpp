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
#include "maze-graphics/ecs/components/MazeOutlineRenderer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class OutlineRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(OutlineRenderer, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialAssetRef, outlineMaterial, MaterialAssetRef(), getOutlineMaterialRef, setOutlineMaterialRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(OutlineRenderer);


    //////////////////////////////////////////
    OutlineRenderer::OutlineRenderer()
    {
    }

    //////////////////////////////////////////
    OutlineRenderer::~OutlineRenderer()
    {
    }

    //////////////////////////////////////////
    OutlineRendererPtr OutlineRenderer::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        OutlineRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(OutlineRenderer, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool OutlineRenderer::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool OutlineRenderer::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<OutlineRenderer>()->m_renderSystem;

        if (!Component::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void OutlineRenderer::processEntityAwakened()
    {
        m_meshRenderer = getEntityRaw()->getComponentRaw<MeshRenderer>();
    }

    //////////////////////////////////////////
    void OutlineRenderer::setOutlineMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getOrLoadMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());

        MAZE_ERROR_IF(material && material->getRenderPassesCount(RenderPassType::Default) < 2,
            "Outline material must have at least 2 Default render passes (mask + shell)!");

        setOutlineMaterial(material);
    }

    //////////////////////////////////////////
    void OutlineRenderer::drawDefaultPass(
        RenderQueuePtr const& _renderQueue,
        DefaultPassParams const& _params,
        RenderUnit const& _renderUnit)
    {
        Vector<VertexArrayObjectPtr> const& vaos = m_meshRenderer->getRenderMesh()->getVertexArrayObjects();
        VertexArrayObjectPtr const& vao = vaos[_renderUnit.index % vaos.size()];

        MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

        TMat const* tm = reinterpret_cast<TMat const*>(_renderUnit.userData);

        _renderQueue->addDrawVAOInstancedCommand(
            vao.get(),
            1,
            tm);
    }

    //////////////////////////////////////////
    void OutlineRenderer::drawShadowPass(
        RenderQueuePtr const& _renderQueue,
        ShadowPassParams const& _params,
        RenderUnit const& _renderUnit)
    {
        // Outline shells don't cast shadows
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(OutlineRendererDefaultPassGatherRenderUnits,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render3DDefaultPassGatherRenderUnitsEvent& _event,
        Entity* _entity,
        OutlineRenderer* _outlineRenderer,
        MeshRenderer* _meshRenderer,
        Transform3D* _transform3D)
    {
        if (!_outlineRenderer->getEnabled())
            return;

        if (!_meshRenderer->getRenderMask() || !(_meshRenderer->getRenderMask()->getMask() & _event.getPassParams()->renderMask))
            return;

        if (!_meshRenderer->getRenderMesh())
            return;

        MaterialPtr const& outlineMaterial = _outlineRenderer->getOutlineMaterial();
        if (!outlineMaterial || outlineMaterial->getRenderPassesCount(RenderPassType::Default) < 2)
            return;

        RenderPassPtr const& maskRenderPass = outlineMaterial->getRenderPass(RenderPassType::Default, 0);
        RenderPassPtr const& outlineRenderPass = outlineMaterial->getRenderPass(RenderPassType::Default, 1);

        if (!maskRenderPass || !outlineRenderPass)
            return;

        Vector<VertexArrayObjectPtr> const& vaos = _meshRenderer->getRenderMesh()->getVertexArrayObjects();
        if (vaos.empty())
            return;

        for (S32 i = 0, in = (S32)vaos.size(); i < in; ++i)
        {
            VertexArrayObjectPtr const& vao = vaos[i];

            _event.getRenderUnits()->emplace_back(
                maskRenderPass.get(),
                _transform3D->getWorldPosition(),
                _outlineRenderer,
                i,
                reinterpret_cast<U64>(&_transform3D->getWorldTransform()),
                static_cast<S32>(vao->getResourceId().getIndex()));

            _event.getRenderUnits()->emplace_back(
                outlineRenderPass.get(),
                _transform3D->getWorldPosition(),
                _outlineRenderer,
                i,
                reinterpret_cast<U64>(&_transform3D->getWorldTransform()),
                static_cast<S32>(vao->getResourceId().getIndex()));
        }
    }


} // namespace Maze
//////////////////////////////////////////
