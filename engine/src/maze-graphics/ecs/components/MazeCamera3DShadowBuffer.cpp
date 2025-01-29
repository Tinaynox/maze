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
#include "maze-graphics/ecs/components/MazeCamera3DShadowBuffer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Camera3DShadowBuffer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Camera3DShadowBuffer, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Camera3DShadowBuffer);

    //////////////////////////////////////////
    Camera3DShadowBuffer::Camera3DShadowBuffer()
    {
    }

    //////////////////////////////////////////
    Camera3DShadowBuffer::~Camera3DShadowBuffer()
    {

    }

    //////////////////////////////////////////
    Camera3DShadowBufferPtr Camera3DShadowBuffer::Create()
    {
        Camera3DShadowBufferPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Camera3DShadowBuffer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Camera3DShadowBuffer::init()
    {
        m_shadowBuffer = RenderBuffer::Create(
            {
                Vec2U(1, 1),
                PixelFormat::None,
                PixelFormat::DEPTH_U24
            });
        m_shadowBuffer->setName("ShadowBuffer");
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setWrapS(TextureWrap::ClampToBorder);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setWrapT(TextureWrap::ClampToBorder);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setBorderColor(ColorU32::c_white);

        return true;
    }

    //////////////////////////////////////////
    RenderBufferPtr const& Camera3DShadowBuffer::updateShadowBuffer(Camera3D const* _camera)
    {
        if (_camera && _camera->getRenderTarget())
        {
            Vec2U size = _camera->getRenderTarget()->getRenderTargetSize();
            U32 shadowBufferSize = Math::Min(size.x, size.y) * 2u;
            shadowBufferSize = Math::Clamp(shadowBufferSize, 1u, 2048u);
            m_shadowBuffer->setSize(Vec2U(shadowBufferSize, shadowBufferSize));
        }

        return m_shadowBuffer;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Camera3DShadowBufferAddedToSampleEvent,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        Camera3D* _camera3D,
        Camera3DShadowBuffer* _camera3DShadowBuffer)
    {
        _camera3D->setShadowBuffer(_camera3DShadowBuffer->updateShadowBuffer(_camera3D));
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Camera3DShadowBufferPrepareForRender,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(),
            MAZE_ECS_ORDER_BEFORE(MAZE_HS("RenderControllerSystem"))),
        PostUpdateEvent const& _event,
        Entity* _entity,
        Camera3D* _camera3D,
        Camera3DShadowBuffer* _camera3DShadowBuffer)
    {
        _camera3DShadowBuffer->updateShadowBuffer(_camera3D);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Camera3DShadowBufferRemovedFromSampleEvent,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        EntityRemovedFromSampleEvent const& _event,
        Entity* _entity,
        Camera3D* _camera3D,
        Camera3DShadowBuffer* _camera3DShadowBuffer)
    {
        _camera3D->setShadowBuffer(nullptr);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
