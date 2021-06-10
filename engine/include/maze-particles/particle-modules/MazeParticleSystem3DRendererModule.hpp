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
#if (!defined(_MazeParticleSystem3DRendererModule_hpp_))
#define _MazeParticleSystem3DRendererModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-particles/MazeParticleSystemState.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ParticleSystem3DRendererModule);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class ParticleSystem3DRendererModule
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DRendererModule
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DRendererModule);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DRendererModule);

    public:

        //////////////////////////////////////////
        class TextureSheetAnimation
            : public IJSONValueSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(TextureSheetAnimation);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(TextureSheetAnimation);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }


            //////////////////////////////////////////
            inline void setTiles(Vec2DS const& _value) { tiles = _value; }

            //////////////////////////////////////////
            inline Vec2DS const& getTiles() const { return tiles; }


            //////////////////////////////////////////
            inline void setStartFrame(ParticleSystemParameterF32 const& _parameter) { startFrame = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getStartFrame() const { return startFrame; }

            //////////////////////////////////////////
            inline void setFrameOverTime(ParticleSystemParameterF32 const& _parameter) { frameOverTime = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getFrameOverTime() const { return frameOverTime; }


            //////////////////////////////////////////
            inline bool operator==(TextureSheetAnimation const& _value) { return enabled == _value.enabled && tiles == _value.tiles && startFrame == _value.startFrame && frameOverTime == _value.frameOverTime; }

            //////////////////////////////////////////
            inline bool operator!=(TextureSheetAnimation const& _value) { return !this->operator==(_value); }

        public:

            //////////////////////////////////////////
            virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
            {
                DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
            }

            //////////////////////////////////////////
            virtual Json::Value toJSONValue() const MAZE_OVERRIDE
            {
                return SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
            }

        public:

            bool enabled = false;
            Vec2DS tiles = Vec2DS(1, 1);
            ParticleSystemParameterF32 startFrame = ParticleSystemParameterF32(0);
            ParticleSystemParameterF32 frameOverTime = ParticleSystemParameterF32(0);
        };

    public:


        //////////////////////////////////////////
        inline S32 getParticlesMaxCount() const { return m_particlesMaxCount; }

        //////////////////////////////////////////
        inline void setParticlesMaxCount(S32 _value) { m_particlesMaxCount = _value; }

        
        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }

        //////////////////////////////////////////
        inline void setRenderMesh(RenderMeshPtr const& _renderMesh) { m_renderMesh = _renderMesh; }


        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _material) { m_material = _material; }


        //////////////////////////////////////////
        TextureSheetAnimation& getTextureSheetAnimation() { return m_textureSheetAnimation; }

        //////////////////////////////////////////
        TextureSheetAnimation const& getTextureSheetAnimation() const { return m_textureSheetAnimation; }

        //////////////////////////////////////////
        void setTextureSheetAnimation(TextureSheetAnimation const& _value) { m_textureSheetAnimation = _value; }


        //////////////////////////////////////////
        void updateInitial(
            Particles3D& _particles,
            S32 _first,
            S32 _last,
            F32 _emitterTimePercent);

        //////////////////////////////////////////
        void updateLifetime(
            Particles3D& _particles,
            S32 _first,
            S32 _last,
            F32 _dt);

        //////////////////////////////////////////
        void prepareToRender(
            Particles3D& _particles,
            ParticleSystemTransformPolicy _transformPolicy,
            Mat4DF const& _particleSystemWorldTransform,
            Vec3DF const& _cameraPosition,
            Vec3DF const& _cameraForward,
            Vec3DF const& _cameraUp);

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
        {
            DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
        }

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE
        {
            return SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
        }

    protected:
        S32 m_particlesMaxCount = 1000;
        RenderMeshPtr m_renderMesh;
        MaterialPtr m_material;

        TextureSheetAnimation m_textureSheetAnimation;
    };


    //////////////////////////////////////////
    MAZE_STRING_ONLY_SERIALIZATION(ParticleSystem3DRendererModule);
    MAZE_STRING_ONLY_SERIALIZATION(ParticleSystem3DRendererModule::TextureSheetAnimation);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DRendererModule_hpp_
//////////////////////////////////////////
