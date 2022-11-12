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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeRandom.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-particles/MazeParticleSystem3DZone.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticleSystem3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ParticleSystem3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DMainModule, mainModule, ParticleSystem3DMainModule(), getMainModule, setMainModule),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DShapeModule, shapeModule, ParticleSystem3DShapeModule(), getShapeModule, setShapeModule),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DRendererModule, rendererModule, ParticleSystem3DRendererModule(), getRendererModule, setRendererModule)
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ParticleSystem3D);


    //////////////////////////////////////////
    ParticleSystem3D::ParticleSystem3D()
        : m_renderSystem(nullptr)
        , m_time(0.0f)
        , m_timeEmission(0.0f)
    {
        
    }

    //////////////////////////////////////////
    ParticleSystem3D::~ParticleSystem3D()
    {
        
    }

    //////////////////////////////////////////
    ParticleSystem3DPtr ParticleSystem3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        ParticleSystem3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticleSystem3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool ParticleSystem3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool ParticleSystem3D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<ParticleSystem3D>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ParticleSystem3D::update(F32 _dt)
    {
        if (m_state != ParticleSystemState::Playing)
            return;

        S32 aliveCount = 0;

        if (ParticleSystemSimulationSpace::World == m_mainModule.getTransformPolicy())
        {
            aliveCount = updateEmitter(
                _dt,
                m_transform->getWorldPosition());
        }
        else
        {
            aliveCount = updateEmitter(
                _dt,
                Vec3DF::c_zero);
        }
    }

    //////////////////////////////////////////
    S32 ParticleSystem3D::updateEmitter(F32 _dt, const Vec3DF& _position)
    {
        bool iterationFinished;
        updateTime(_dt, iterationFinished);

        S32 aliveCount = m_particles.getAliveCount();

        m_mainModule.updateLifetime(m_particles, 0, aliveCount, _dt);
        m_rendererModule.updateLifetime(m_particles, 0, aliveCount, _dt);

        // Update alive count and bounds
        m_particles.update();

        aliveCount = m_particles.getAliveCount();

        {

            S32 maxCount = m_rendererModule.getParticlesMaxCount();
            S32 maxCountToEmit = maxCount > aliveCount ? maxCount - aliveCount : 0;

            if (maxCountToEmit && m_mainModule.getEmission().enabled && m_state == ParticleSystemState::Playing)
            {
                S32 particleSeed = rand() % c_particleSystemParametersCount;

                F32 iterationProgress = m_time / m_mainModule.getCurrentDuration();

                auto postEmitUpdate = [&](S32 _emissionCount)
                {
                    m_mainModule.updateLifetime(m_particles, aliveCount, aliveCount + _emissionCount, _dt);
                    m_rendererModule.updateLifetime(m_particles, aliveCount, aliveCount + _emissionCount, _dt);

                    aliveCount += _emissionCount;
                    maxCountToEmit -= _emissionCount;
                };

                auto emit = [&](S32 _emissionCount, Vec3DF _spawnPosition)
                {
                    m_particles.addAliveCount(_emissionCount);

                    emitParticles(aliveCount, aliveCount + _emissionCount, _spawnPosition, iterationProgress);
                    postEmitUpdate(_emissionCount);
                };

                S32 emissionCount = calculateEmissionCount(particleSeed, iterationProgress, maxCountToEmit, iterationFinished);
                if (emissionCount > 0)
                    emit(emissionCount, _position);

                // Emission per distance
                F32 emissionPerLength;
                m_mainModule.getEmission().emissionPerDistance.sample(particleSeed, iterationProgress, emissionPerLength);
                if (emissionPerLength > 0.0f)
                {
                    Vec3DF posWS = m_transform->getWorldPosition();
                    if (m_emissionPerDistancePreviousPositionWSDirty)
                    {
                        m_emissionPerDistancePreviousPositionWSDirty = false;
                        m_emissionPerDistancePreviousPositionWS = posWS;
                    }
                    else
                    {
                        Vec3DF toPosWS = posWS - m_emissionPerDistancePreviousPositionWS;
                        F32 distanceOffsetSq = toPosWS.squaredLength();
                        if (distanceOffsetSq > 0.0f)
                        {
                            F32 distanceOffset = Math::Sqrt(distanceOffsetSq);
                            Vec3DF toPosWSDir = toPosWS / distanceOffset;
                            if (distanceOffset > emissionPerLength)
                            {
                                emissionCount = Math::Min(S32(distanceOffset / emissionPerLength), maxCountToEmit);
                                if (emissionCount > 0)
                                {
                                    Vec3DF offsetPerEmission = emissionPerLength * toPosWSDir;

                                    if (ParticleSystemSimulationSpace::World == m_mainModule.getTransformPolicy())
                                    {
                                        m_particles.addAliveCount(emissionCount);

                                        for (S32 i = 0; i < emissionCount; ++i)
                                        {
                                            m_emissionPerDistancePreviousPositionWS += offsetPerEmission;
                                            emitParticles(aliveCount + i, aliveCount + i + 1, m_emissionPerDistancePreviousPositionWS, iterationProgress);
                                        }

                                        postEmitUpdate(emissionCount);
                                    }
                                    else
                                    {
                                        emit(emissionCount, _position);
                                        m_emissionPerDistancePreviousPositionWS += F32(emissionCount) * offsetPerEmission;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!m_mainModule.getLooped())
        {
            if (iterationFinished && aliveCount == 0)
            {
                stop();
            }
        }

        return aliveCount;
    }

    //////////////////////////////////////////
    S32 ParticleSystem3D::calculateEmissionCount(
        S32 _particleSeed,
        F32 _iterationProgress,
        S32 _maxCountToEmit,
        bool _iterationFinished)
    {
        if (_iterationProgress < 0.0f)
            return 0;

        S32 count = 0;

        if (!_iterationFinished)
        {
            // Calculate emission rate
            F32 emissionPerSecond;
            m_mainModule.getEmission().emissionPerSecond.sample(_particleSeed, _iterationProgress, emissionPerSecond);

            if (emissionPerSecond > 0.0f)
            {
                count = Math::Min(_maxCountToEmit, (S32)Math::Floor(m_timeEmission * emissionPerSecond));
                m_timeEmission -= count / emissionPerSecond;

                // Emission time can't be less than zero
                m_timeEmission = Math::Max(m_timeEmission, 0.0f);
            }
            else
            if (emissionPerSecond == 0.0f)
            {
                count = 0;
                m_timeEmission = 0.0f;
            }

            
        }

        //  Emit particles by bursts
        Vector<ParticleSystemBurst> const& bursts = m_mainModule.getEmission().bursts;
        if (!bursts.empty())
        {
            S32 burstsCount = (S32)bursts.size();
            for (S32 i = m_currentBurstIndex; i < burstsCount; i++)
            {
                if (bursts[i].time <= m_time)
                {
                    S32 minCount = bursts[i].minCount;
                    S32 maxCount = bursts[i].maxCount;

                    if (minCount > maxCount)
                        std::swap(minCount, maxCount);

                    count += Random::RangeRandom(minCount, maxCount + 1);
                    ++m_currentBurstIndex;
                }
            }
        }

        count = Math::Min(_maxCountToEmit, count);

        return count;
    }

    //////////////////////////////////////////
    void ParticleSystem3D::emitParticles(
        S32 _first,
        S32 _last,
        Vec3DF const& _emitterPosition,
        F32 _emitterTimePercent)
    {
        m_particles.clearData(_first, _last);

        m_shapeModule.updateInitial(
            _emitterPosition,
            m_particles,
            _first,
            _last,
            _emitterTimePercent,
            m_mainModule.getTransformPolicy(),
            m_mainModule.getScalingMode(),
            m_transform->getLocalTransform(),
            m_transform->getWorldTransform());

        m_mainModule.updateInitial(
            m_particles,
            _first,
            _last,
            _emitterTimePercent,
            m_transform->getWorldTransform());

        m_rendererModule.updateInitial(m_particles, _first, _last, _emitterTimePercent);
        
    }

    //////////////////////////////////////////
    void ParticleSystem3D::updateTime(F32 _dt, bool& _iterationFinished)
    {
        m_time += _dt;

        if (m_mainModule.getEmission().enabled)
            m_timeEmission += _dt;

        if (m_time < m_mainModule.getCurrentDuration())
        {
            _iterationFinished = false;
            return;
        }

        if (m_mainModule.getLooped())
        {
            m_iteration++;
            m_time -= Math::Floor(m_time / m_mainModule.getCurrentDuration()) * m_mainModule.getCurrentDuration();
            m_currentBurstIndex = 0;
        }
        else
        {
            m_time = m_mainModule.getCurrentDuration();
        }

        _iterationFinished = true;
    }

    //////////////////////////////////////////
    void ParticleSystem3D::prepareToRender(
        Vec3DF const& _cameraPosition,
        Vec3DF const& _cameraForward,
        Vec3DF const& _cameraUp)
    {
        m_rendererModule.prepareToRender(
            m_particles,
            m_mainModule.getTransformPolicy(),
            m_mainModule.getScalingMode(),
            m_transform->getLocalTransform(),
            m_transform->getWorldTransform(),
            _cameraPosition,
            _cameraForward,
            _cameraUp);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::processEntityEnabled()
    {
        m_emissionPerDistancePreviousPositionWSDirty = true;
    }

    //////////////////////////////////////////
    void ParticleSystem3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();

        m_emissionPerDistancePreviousPositionWSDirty = true;

        if (m_mainModule.getPlayOnAwake())
            play();
    }

    //////////////////////////////////////////
    void ParticleSystem3D::setMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());
        m_rendererModule.setMaterial(material);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::setMaterial(MaterialPtr const& _material)
    {
        m_rendererModule.setMaterial(_material);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::setMesh(MeshPtr const& _mesh)
    {
        MAZE_ERROR_RETURN_IF(!m_renderSystem, "Render system is null");

        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene()->getMetaClass()->isInheritedFrom<ECSRenderScene>(), "Entity Scene is not Render Scene");
#endif

        RenderTargetPtr const& renderTarget = getEntityRaw()->getECSScene()->castRaw<ECSRenderScene>()->getRenderTarget();

        if (!m_rendererModule.getMaterial())
        {
            m_rendererModule.setRenderMesh(renderTarget->createRenderMeshFromPool((S32)_mesh->getSubMeshesCount()));
        }

        m_rendererModule.getRenderMesh()->loadFromMesh(
            _mesh,
            renderTarget.get());
    }

    //////////////////////////////////////////
    void ParticleSystem3D::play()
    {
        if (getState() == ParticleSystemState::Playing)
        {
            // Reset particle system, but don't remove alive particles
            if (m_time >= m_mainModule.getCurrentDuration())
            {
                m_time = 0.0f;
                m_timeEmission = 0.0f;
                m_iteration = 0;
                m_currentBurstIndex = 0;
            }

            return;
        }

        setState(ParticleSystemState::Playing);
        m_mainModule.generateDuration();

        if (m_mainModule.getPrewarm() && m_mainModule.getLooped())
        {
            while (m_iteration == 0 && getState() == ParticleSystemState::Playing)
            {
                update(1.0f / 40.0f);
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystem3D::play(bool _recursive)
    {
        play();

        if (!_recursive)
            return;

        iterateChildParticleSystems(
            [_recursive](ParticleSystem3D* _particleSystem)
            {
                _particleSystem->play(_recursive);
            });
        
    }

    //////////////////////////////////////////
    void ParticleSystem3D::stop()
    {
        m_time = 0.0f;
        m_timeEmission = 0.0f;
        m_iteration = 0;
        m_currentBurstIndex = 0;

        m_particles.setAliveCount(0);
        setState(ParticleSystemState::None);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::stop(bool _recursive)
    {
        stop();

        if (!_recursive)
            return;

        iterateChildParticleSystems(
            [_recursive](ParticleSystem3D* _particleSystem)
            {
                _particleSystem->stop(_recursive);
            });

    }

    //////////////////////////////////////////
    void ParticleSystem3D::restart()
    {
        stop();
        play();
    }

    //////////////////////////////////////////
    void ParticleSystem3D::restart(bool _recursive)
    {
        restart();

        if (!_recursive)
            return;

        iterateChildParticleSystems(
            [_recursive](ParticleSystem3D* _particleSystem)
            {
                _particleSystem->restart(_recursive);
            });

    }

    //////////////////////////////////////////
    void ParticleSystem3D::pause()
    {
        setState(ParticleSystemState::Pause);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::pause(bool _recursive)
    {
        pause();

        if (!_recursive)            
            return;

        iterateChildParticleSystems(
            [_recursive](ParticleSystem3D* _particleSystem)
            {
                _particleSystem->pause(_recursive);
            });

    }

    //////////////////////////////////////////
    void ParticleSystem3D::setEmissionEnabled(bool _value)
    {
        m_mainModule.getEmission().setEnabled(_value);
    }

    //////////////////////////////////////////
    void ParticleSystem3D::setEmissionEnabled(bool _value, bool _recursive)
    {
        setEmissionEnabled(_value);

        if (!_recursive)
            return;

        iterateChildParticleSystems(
            [_value, _recursive](ParticleSystem3D* _particleSystem)
            {
                _particleSystem->setEmissionEnabled(_value, _recursive);
            });
    }

    //////////////////////////////////////////
    void ParticleSystem3D::setRenderMesh(String const& _renderMeshName)
    {
        m_rendererModule.setRenderMesh(m_renderSystem->getRenderMeshManager()->getRenderMesh(_renderMeshName));
    }

    //////////////////////////////////////////
    void ParticleSystem3D::processEntityRemoved()
    {
        m_rendererModule.setRenderMesh(RenderMeshPtr());
    }

    //////////////////////////////////////////
    void ParticleSystem3D::iterateChildParticleSystems(
        std::function<void(ParticleSystem3D*)> _callback,
        bool _includeInactive)
    {
        for (Transform3D* transform : m_transform->getChildren())
        {
            if (!_includeInactive && !transform->getEntityRaw()->getActiveSelf())
                continue;

            ParticleSystem3D* subParticleSystem = transform->getEntityRaw()->getComponentRaw<ParticleSystem3D>();
            if (subParticleSystem)
            {
                _callback(subParticleSystem);
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
