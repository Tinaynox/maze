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
#pragma once
#if (!defined(_MazeParticleSystem3D_hpp_))
#define _MazeParticleSystem3D_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-particles/MazeParticleSystemState.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DShapeModule.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DRendererModule.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ParticleSystem3D);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class ParticleSystem3D
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ParticleSystem3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ParticleSystem3D);


        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ParticleSystem3D();

        //////////////////////////////////////////
        static ParticleSystem3DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        void processEntityRemoved();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline RenderMaskPtr const& getRenderMask() const { return m_renderMask; }


        //////////////////////////////////////////
        void setMainModule(ParticleSystem3DMainModule const& _module) { m_mainModule = _module; }

        //////////////////////////////////////////
        inline ParticleSystem3DMainModule const& getMainModule() const { return m_mainModule; }

        //////////////////////////////////////////
        inline ParticleSystem3DMainModule& getMainModule() { return m_mainModule; }



        //////////////////////////////////////////
        void setShapeModule(ParticleSystem3DShapeModule const& _module) { m_shapeModule = _module; }

        //////////////////////////////////////////
        inline ParticleSystem3DShapeModule const& getShapeModule() const { return m_shapeModule; }

        //////////////////////////////////////////
        inline ParticleSystem3DShapeModule& getShapeModule() { return m_shapeModule; }


        //////////////////////////////////////////
        void setRendererModule(ParticleSystem3DRendererModule const& _module) { m_rendererModule = _module; }

        //////////////////////////////////////////
        inline ParticleSystem3DRendererModule const& getRendererModule() const { return m_rendererModule; }

        //////////////////////////////////////////
        inline ParticleSystem3DRendererModule& getRendererModule() { return m_rendererModule; }


        //////////////////////////////////////////
        void play();

        //////////////////////////////////////////
        void play(bool _recursive);

        //////////////////////////////////////////
        void stop();

        //////////////////////////////////////////
        void stop(bool _recursive);

        //////////////////////////////////////////
        void restart();

        //////////////////////////////////////////
        void restart(bool _recursive);

        //////////////////////////////////////////
        void pause();

        //////////////////////////////////////////
        void pause(bool _recursive);

        //////////////////////////////////////////
        void setEmissionEnabled(bool _value);

        //////////////////////////////////////////
        void setEmissionEnabled(bool _value, bool _recursive);


        //////////////////////////////////////////
        void setRenderMesh(String const& _renderMeshName);

        //////////////////////////////////////////
        void setMesh(MeshPtr const& _mesh);


        //////////////////////////////////////////
        void setMaterial(String const& _materialName);

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);


        //////////////////////////////////////////
        inline ParticleSystemState getState() const { return m_state; }


        //////////////////////////////////////////
        inline F32 getTime() const { return m_time; }

        //////////////////////////////////////////
        inline S32 getIteration() const { return m_iteration; }

        //////////////////////////////////////////
        inline S32 getAliveParticles() const { return m_particles.getAliveCount(); }

        //////////////////////////////////////////
        inline TMat const* getRenderTransforms() const { return m_particles.getRenderTransforms(); }

        //////////////////////////////////////////
        inline Vec4F const* getRenderColors() const { return m_particles.getRenderColors(); }

        //////////////////////////////////////////
        inline Vec4F const* getRenderUVs() const { return m_particles.getRenderUVs(); }


        //////////////////////////////////////////
        void prepareToRender(
            Vec3F const& _cameraPosition,
            Vec3F const& _cameraForward,
            Vec3F const& _cameraUp);

    protected:

        //////////////////////////////////////////
        ParticleSystem3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EcsWorld* _world,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline void setState(ParticleSystemState _state) { m_state = _state; }


        //////////////////////////////////////////
        virtual void processEntityEnabled() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

       

        //////////////////////////////////////////
        S32 updateEmitter(F32 _dt, const Vec3F& _position);

        //////////////////////////////////////////
        S32 calculateEmissionCount(
            S32 _particleSeed,
            F32 _scalar,
            S32 _maxCountToEmit,
            bool _ended);

        //////////////////////////////////////////
        void emitParticles(
            S32 _first,
            S32 _last,
            Vec3F const& _position,
            F32 _emitterTimePercent);

        //////////////////////////////////////////
        void updateTime(F32 _dt, bool& _iterationFinished);

        //////////////////////////////////////////
        void iterateChildParticleSystems(
            std::function<void(ParticleSystem3D*)> _callback,
            bool _includeInactive = false);

    protected:
        RenderSystem* m_renderSystem;

        Transform3DPtr m_transform;
        RenderMaskPtr m_renderMask;

        F32 m_time;
        F32 m_timeEmission;
        S32 m_iteration = 0;
        S32 m_currentBurstIndex = 0;
        Vec3F m_emissionPerDistancePreviousPositionWS = Vec3F::c_zero;
        bool m_emissionPerDistancePreviousPositionWSDirty = true;

        ParticleSystem3DMainModule m_mainModule;
        ParticleSystem3DShapeModule m_shapeModule;
        ParticleSystem3DRendererModule m_rendererModule;

        ParticleSystemState m_state;
        Particles3D m_particles;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3D_hpp_
//////////////////////////////////////////
