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
#include "MazeWaterHeader.hpp"
#include "maze-plugin-water/ecs/systems/MazeRenderWaterSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderWaterSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWaterSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(RenderWaterSystem);

    //////////////////////////////////////////
    RenderWaterSystem::RenderWaterSystem()
    {
    }

    //////////////////////////////////////////
    RenderWaterSystem::~RenderWaterSystem()
    {
        if (m_worldRaw)
            m_worldRaw->eventComponentSystemAdded.unsubscribe(this);

        if (m_renderControlSystem)
            m_renderControlSystem->getModule3D()->eventPrePass.unsubscribe(this);
    }

    //////////////////////////////////////////
    RenderWaterSystemPtr RenderWaterSystem::Create()
    {
        RenderWaterSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderWaterSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool RenderWaterSystem::init()
    {
        

        return true;
    }

    //////////////////////////////////////////
    void RenderWaterSystem::processSystemAdded()
    {
        m_waterRenderersSample = m_worldRaw->requestInclusiveSample<WaterRenderer3D>();
        m_worldRaw->eventComponentSystemAdded.subscribe(this, &RenderWaterSystem::notifyComponentSystemAdded);

        m_renderControlSystem = m_worldRaw->getSystem<RenderControlSystem>();
        if (m_renderControlSystem)
            m_renderControlSystem->getModule3D()->eventPrePass.subscribe(this, &RenderWaterSystem::notifyRenderPrePass);
    }

    //////////////////////////////////////////
    void RenderWaterSystem::processSystemRemoved()
    {
        if (m_worldRaw)
            m_worldRaw->eventComponentSystemAdded.unsubscribe(this);

        if (m_renderControlSystem)
            m_renderControlSystem->getModule3D()->eventPrePass.unsubscribe(this);
    }

    //////////////////////////////////////////
    void RenderWaterSystem::processUpdate(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void RenderWaterSystem::createBuffers(Vec2U32 const& _size)
    {
        Debug::Log("Creating RenderWaterSystem buffers: %ux%u", _size.x, _size.y);
        m_reflectionBuffer = RenderBuffer::Create(
            {
                {_size.x, _size.y},
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_reflectionBuffer->getColorTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
        m_reflectionBuffer->getColorTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);

        m_refractionBuffer = RenderBuffer::Create(
            {
                {_size.x, _size.y},
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_refractionBuffer->getColorTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
        m_refractionBuffer->getColorTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
        Debug::Log("RenderWaterSystem buffers created");
    }

    //////////////////////////////////////////
    void RenderWaterSystem::resizeBuffers(Vec2U32 const& _size)
    {
        createBuffers(_size);
    }

    //////////////////////////////////////////
    void RenderWaterSystem::notifyRenderPrePass(
        RenderTarget* _renderTarget,
        DefaultPassParams const& _params)
    {
        if (!m_reflectionBuffer || !m_refractionBuffer)
        {
            createBuffers(_renderTarget->getRenderTargetSize());
        }

        Vec3F32 cameraPosition = _params.cameraTransform.getAffineTranslation();

        m_waterRenderersSample->process(
            [&](Entity* _entity, WaterRenderer3D* _waterRenderer)
            {
                F32 waterY = _waterRenderer->getTransform()->getWorldPosition().y;

                DefaultPassParams params = _params;
                params.renderMask &= ~(S32)DefaultRenderMask::Water;
                params.viewport = Rect2DF(0.0f, 0.0f, 1.0f, 1.0f);

                // Refraction buffer (Under water)
                m_renderControlSystem->getModule3D()->drawDefaultPass(
                    m_refractionBuffer.get(),
                    params,
                    nullptr,
                    [&](RenderQueuePtr const& _renderQueue)
                    {
                        _renderQueue->addEnableClipPlaneCommand(
                            0,
                            { 0.0f, -1.0f, 0.0f, waterY + 0.2f });
                    },
                    [](RenderQueuePtr const& _renderQueue)
                    {
                        _renderQueue->addDisableClipPlaneCommand(0);
                    },
                    nullptr);

                F32 cameraTranslation = 2.0f * (cameraPosition.y - waterY);
                cameraPosition.y -= cameraTranslation;
            
                Vec3F32 cameraRotation = Quaternion::GetEuler(params.cameraTransform);
                Vec3F32 cameraScale = params.cameraTransform.getAffineScaleSignless();

                cameraRotation.x = -cameraRotation.x;
                                
                params.cameraTransform = Mat4F32::CreateTranslationMatrix(cameraPosition);
                params.cameraTransform = params.cameraTransform *
                    //Quaternion(cameraRotation).toRotationMatrix();
                    Mat4F32::CreateRotationMatrix(cameraRotation);
                params.cameraTransform = params.cameraTransform * Mat4F32::CreateScaleMatrix(cameraScale);
                
                // Reflection buffer (Above the water level)
                m_renderControlSystem->getModule3D()->drawDefaultPass(
                    m_reflectionBuffer.get(),
                    params,
                    nullptr,
                    [&](RenderQueuePtr const& _renderQueue)
                    {
                        _renderQueue->addEnableClipPlaneCommand(
                            0,
                            {0.0f, 1.0f, 0.0f, -waterY + 0.02f });
                    },
                    [](RenderQueuePtr const& _renderQueue)
                    {
                        _renderQueue->addDisableClipPlaneCommand(0);
                    },
                    nullptr);


                _waterRenderer->prepare(
                    m_reflectionBuffer.get(),
                    m_refractionBuffer.get());
            });
    }

    //////////////////////////////////////////
    void RenderWaterSystem::notifyComponentSystemAdded(ComponentSystemPtr const& _system)
    {
        if (_system->getClassUID() == ClassInfo<RenderControlSystem>::UID())
        {
            if (!m_renderControlSystem)
            {
                m_renderControlSystem = _system->cast<RenderControlSystem>();
                if (m_renderControlSystem)
                    m_renderControlSystem->getModule3D()->eventPrePass.subscribe(this, &RenderWaterSystem::notifyRenderPrePass);
            }
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
