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
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-particles/MazeParticleSystemState.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ParticleSystem3DRendererModule);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class ParticleSystem3DRendererModule
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DRendererModule
        : public IJSONValueSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DRendererModule);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DRendererModule);

    public:

        //////////////////////////////////////////
        class MAZE_PARTICLES_API TextureSheetAnimation
            : public IJSONValueSerializable
            , public IDataBlockSerializable
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
            inline void setTiles(Vec2S const& _value) { tiles = _value; }

            //////////////////////////////////////////
            inline Vec2S const& getTiles() const { return tiles; }


            //////////////////////////////////////////
            inline void setStartFrame(ParticleSystemParameterF32 const& _parameter) { startFrame = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getStartFrame() const { return startFrame; }

            //////////////////////////////////////////
            inline void setFrameOverTime(ParticleSystemParameterF32 const& _parameter) { frameOverTime = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getFrameOverTime() const { return frameOverTime; }


            //////////////////////////////////////////
            inline bool operator==(TextureSheetAnimation const& _value) const { return enabled == _value.enabled && tiles == _value.tiles && startFrame == _value.startFrame && frameOverTime == _value.frameOverTime; }

            //////////////////////////////////////////
            inline bool operator!=(TextureSheetAnimation const& _value) const { return !this->operator==(_value); }

        public:

            //////////////////////////////////////////
            virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
            {
                JSONHelper::DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
            }

            //////////////////////////////////////////
            virtual Json::Value toJSONValue() const MAZE_OVERRIDE
            {
                return JSONHelper::SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
            }

        public:
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            Vec2S tiles = Vec2S(1, 1);
            ParticleSystemParameterF32 startFrame = ParticleSystemParameterF32(0);
            ParticleSystemParameterF32 frameOverTime = ParticleSystemParameterF32(0);
        };

    public:


        //////////////////////////////////////////
        inline S32 getParticlesMaxCount() const { return m_particlesMaxCount; }

        //////////////////////////////////////////
        inline void setParticlesMaxCount(S32 _value) { m_particlesMaxCount = _value; }

        
        //////////////////////////////////////////
        inline RenderMeshAssetRef const& getRenderMeshRef() const { return m_renderMeshRef; }

        //////////////////////////////////////////
        inline void setRenderMeshRef(RenderMeshAssetRef const& _renderMesh) { m_renderMeshRef.setRenderMesh(_renderMesh.getRenderMesh()); }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMeshRef.getRenderMesh(); }

        //////////////////////////////////////////
        inline void setRenderMesh(RenderMeshPtr const& _renderMesh) { m_renderMeshRef.setRenderMesh(_renderMesh); }

        //////////////////////////////////////////
        inline MaterialAssetRef const& getMaterialRef() const { return m_materialRef; }

        //////////////////////////////////////////
        inline void setMaterialRef(MaterialAssetRef const& _value) { m_materialRef.setMaterial(_value.getMaterial()); }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_materialRef.getMaterial(); }

        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _material) { m_materialRef.setMaterial(_material); }


        //////////////////////////////////////////
        TextureSheetAnimation& getTextureSheetAnimation() { return m_textureSheetAnimation; }

        //////////////////////////////////////////
        TextureSheetAnimation const& getTextureSheetAnimation() const { return m_textureSheetAnimation; }

        //////////////////////////////////////////
        void setTextureSheetAnimation(TextureSheetAnimation const& _value) { m_textureSheetAnimation = _value; }


        //////////////////////////////////////////
        inline ParticleSystemRenderAlignment getRenderAlignment() const { return m_renderAlignment; }

        //////////////////////////////////////////
        inline void setRenderAlignment(ParticleSystemRenderAlignment _value) { m_renderAlignment = _value; }


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
            ParticleSystemSimulationSpace _transformPolicy,
            ParticleSystemScalingMode _scalingMode,
            TMat const& _particleSystemLocalTransform,
            TMat const& _particleSystemWorldTransform,
            Vec3F const& _cameraPosition,
            Vec3F const& _cameraForward,
            Vec3F const& _cameraUp);



        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DRendererModule const& _value) const
        {
            return 
                m_particlesMaxCount == _value.m_particlesMaxCount &&
                m_renderMeshRef.getRenderMesh() == _value.m_renderMeshRef.getRenderMesh() &&
                m_materialRef.getMaterial() == _value.m_materialRef.getMaterial() &&
                m_renderAlignment == _value.m_renderAlignment &&
                m_textureSheetAnimation == _value.m_textureSheetAnimation;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DRendererModule const& _value) const { return !this->operator==(_value); }

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
        {
            JSONHelper::DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
        }

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE
        {
            return JSONHelper::SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
        }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
        {
            DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            return true;
        }

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
        {
            DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
        }

    protected:
        S32 m_particlesMaxCount = 1000;
        RenderMeshAssetRef m_renderMeshRef;
        MaterialAssetRef m_materialRef;

        ParticleSystemRenderAlignment m_renderAlignment = ParticleSystemRenderAlignment::View;

        TextureSheetAnimation m_textureSheetAnimation;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DRendererModule_hpp_
//////////////////////////////////////////
