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
#include "maze-particles/particle-modules/MazeParticleSystem3DRendererModule.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DRendererModule::TextureSheetAnimation,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DS, tiles, Vec2DS(1, 1), getTiles, setTiles),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, startFrame, ParticleSystemParameterF32(), getStartFrame, setStartFrame),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, frameOverTime, ParticleSystemParameterF32(), getFrameOverTime, setFrameOverTime));


    //////////////////////////////////////////
    // Class ParticleSystem3DRendererModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DRendererModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, particlesMaxCount, 1000, getParticlesMaxCount, setParticlesMaxCount),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RenderMeshPtr, renderMesh, RenderMeshPtr(), getRenderMesh, setRenderMesh),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialPtr, material, MaterialPtr(), getMaterial, setMaterial),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemRenderAlignment, renderAlignment, ParticleSystemRenderAlignment::View, getRenderAlignment, setRenderAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextureSheetAnimation, textureSheetAnimation, TextureSheetAnimation(), getTextureSheetAnimation, setTextureSheetAnimation));


    //////////////////////////////////////////
    void ParticleSystem3DRendererModule::updateInitial(
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _emitterTimePercent)
    {
        // Texture sheet animation
        if (m_textureSheetAnimation.enabled)
        {
            F32 startFrame = 0;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_textureSheetAnimation.startFrame.sample(seed, _emitterTimePercent, startFrame);

                Particles3D::ParticleAnimationFrame& animationFrame = _particles.accessAnimationFrame(i);

                animationFrame.initial = startFrame;
                animationFrame.current = startFrame;
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystem3DRendererModule::updateLifetime(
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _dt)
    {
        // Texture sheet animation
        if (m_textureSheetAnimation.enabled)
        {
            F32 frameOverLifetime = 0.0f;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);

                m_textureSheetAnimation.frameOverTime.sample(seed, life.scalar, frameOverLifetime);

                Particles3D::ParticleAnimationFrame& animationFrame = _particles.accessAnimationFrame(i);

                animationFrame.current = animationFrame.initial + frameOverLifetime;
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystem3DRendererModule::prepareToRender(
        Particles3D& _particles,
        ParticleSystemSimulationSpace _transformPolicy,
        ParticleSystemScalingMode _scalingMode,
        Mat4DF const& _particleSystemLocalTransform,
        Mat4DF const& _particleSystemWorldTransform,
        Vec3DF const& _cameraPosition,
        Vec3DF const& _cameraForward,
        Vec3DF const& _cameraUp)
    {

        S32 aliveCount = _particles.getAliveCount();

        ParticleSystemRenderAlignment renderAlignment = m_renderAlignment;

        FastVector<S32> indices;
        indices.resize(aliveCount);

        // Fill indices array and calculate sqr distance to camera
        switch (_transformPolicy)
        {
            case ParticleSystemSimulationSpace::Local:
            {
                for (S32 i = 0; i < aliveCount; ++i)
                {
                    indices[i] = i;

                    Vec3DF position = _particles.accessPosition(i);
                    F32& sqrDistanceToCamera = _particles.accessSqrDistanceToCamera(i);

                    Vec3DF positionWS = _particleSystemWorldTransform.transformAffine(position);

                    sqrDistanceToCamera = (positionWS - _cameraPosition).squaredLength();
                }
                break;
            }
            case ParticleSystemSimulationSpace::World:
            {
                for (S32 i = 0; i < aliveCount; ++i)
                {
                    indices[i] = i;

                    Vec3DF position = _particles.accessPosition(i);
                    F32& sqrDistanceToCamera = _particles.accessSqrDistanceToCamera(i);

                    sqrDistanceToCamera = (position - _cameraPosition).squaredLength();
                }
                break;
            }
            default:
            {
                break;
            }
        }

        // Sort indices by sqr distance to camera
        std::sort(
            indices.begin(),
            indices.end(),
            [&](S32 _a, S32 _b)
            {
                F32 sqrDistanceToCameraA = _particles.accessSqrDistanceToCamera(_a);
                F32 sqrDistanceToCameraB = _particles.accessSqrDistanceToCamera(_b);

                return sqrDistanceToCameraA > sqrDistanceToCameraB;
            });

        /*
        // Render transform callback
        std::function<Mat4DF(Mat4DF const& _lookAtMatrix)> renderTransformCallback;
        switch (_transformPolicy)
        {
            case ParticleSystemSimulationSpace::Local:
            {
                renderTransformCallback = [&_particleSystemWorldTransform](Mat4DF const& _lookAtMatrix) { return _lookAtMatrix; };
                break;
            }
            case ParticleSystemSimulationSpace::World:
            {
                renderTransformCallback = [](Mat4DF const& _lookAtMatrix) { return _lookAtMatrix; };
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }
        */


        // Render UV callback
        std::function<Vec4DF(S32 _index)> renderUVCallback;
        if (m_textureSheetAnimation.enabled)
        {
            Vec2DS tiles = m_textureSheetAnimation.tiles;
            Vec2DF invTiles = 1.0f / (Vec2DF)tiles;
            S32 frames = tiles.x * tiles.y;

            renderUVCallback = 
                [&_particles, tiles, invTiles, frames](S32 _index)
                {
                    Particles3D::ParticleAnimationFrame& animationFrame = _particles.accessAnimationFrame(_index);
                    S32 animationFrameIndex = (S32)animationFrame.current % frames;

                    S32 r = tiles.y - animationFrameIndex / tiles.x - 1;
                    S32 c = animationFrameIndex % tiles.x;

                    F32 x = c * invTiles.x;
                    F32 y = r * invTiles.y;

                    return Vec4DF(
                        x,
                        x + invTiles.x,
                        y,
                        y + invTiles.y);
                };
        }
        else
        {
            renderUVCallback = [](S32 _index) { return Vec4DF(0.0f, 1.0f, 0.0f, 1.0f); };
        }
        

        // PS Translation
        Vec3DF particleSystemWorldTranslation = _particleSystemWorldTransform.getAffineTranslation();
        Mat4DF particleSystemWorldTranslatonMatrix = Mat4DF::CreateTranslationMatrix(particleSystemWorldTranslation);

        // PS Rotation
        //Vec3DF particleSystemWorldRotation = _particleSystemWorldTransform.getAffineRotationEulerAngles();
        Vec3DF particleSystemWorldRotation = Quaternion::GetEuler(_particleSystemWorldTransform);
        Mat4DF particleSystemWorldRotationMatrix = Mat4DF::CreateRotationMatrix(particleSystemWorldRotation);
        /*
        F32 cx = cosf(particleSystemWorldRotation.x);
        F32 sx = sinf(particleSystemWorldRotation.x);

        F32 cy = cosf(particleSystemWorldRotation.y);
        F32 sy = sinf(particleSystemWorldRotation.y);

        F32 cz = cosf(particleSystemWorldRotation.z);
        F32 sz = sinf(particleSystemWorldRotation.z);

        Mat4DF particleSystemWorldRotationMatrix = 
            Mat4DF(
                (F32)cz, (F32)-sz, (F32)0, (F32)0,
                (F32)sz, (F32)cz, (F32)0, (F32)0,
                (F32)0, (F32)0, (F32)1, (F32)0,
                (F32)0, (F32)0, (F32)0, (F32)1) *
            Mat4DF(
                (F32)cy, (F32)0, (F32)sy, (F32)0,
                (F32)0, (F32)1, (F32)0, (F32)0,
                (F32)-sy, (F32)0, (F32)cy, (F32)0,
                (F32)0, (F32)0, (F32)0, (F32)1) *
            Mat4DF(
                (F32)1, (F32)0, (F32)0, (F32)0,
                (F32)0, (F32)cx, (F32)-sx, (F32)0,
                (F32)0, (F32)sx, (F32)cx, (F32)0,
                (F32)0, (F32)0, (F32)0, (F32)1);*/

        // PS Scale
        Vec3DF particleSystemWorldScale = _particleSystemWorldTransform.getAffineScaleSignless();

        Mat4DF scaleMatrix;
        if (_scalingMode == ParticleSystemScalingMode::Local)
        {
            Vec3DF particleSystemLocalScale = _particleSystemLocalTransform.getAffineScaleSignless();
            scaleMatrix = Mat4DF::CreateScaleMatrix(particleSystemLocalScale);
        }
        else
        {
            scaleMatrix = Mat4DF::CreateScaleMatrix(particleSystemWorldScale);
        }

        Mat4DF localTransformMatrix;
        if (_transformPolicy == ParticleSystemSimulationSpace::Local)
        {
            localTransformMatrix = (particleSystemWorldTranslatonMatrix * particleSystemWorldRotationMatrix * scaleMatrix);
        }


        // Fill final render arrays
        for (S32 i = 0; i < aliveCount; ++i)
        {
            S32 index = indices[i];

            Vec3DF position = _particles.accessPosition(index);
            Vec4DF colorCurrent = _particles.accessColorCurrent(index);
            Particles3D::ParticleSize const& size = _particles.accessSize(index);
            F32 sizeCurrent = size.current;

            Particles3D::ParticleRotation const& rotation = _particles.accessRotation(index);
            F32 rotationCurrent = rotation.current;

            Mat4DF& renderTransform = _particles.accessRenderTransform(i);
            Vec4DF& renderColor = _particles.accessRenderColor(i);
            Vec4DF& renderUV = _particles.accessRenderUV(i);


            // #TODO: Optimize if
            if (_transformPolicy == ParticleSystemSimulationSpace::Local)
            {
                position = localTransformMatrix.transformAffine(position);
            }
            
            // Translation
            Mat4DF mat = Mat4DF::CreateTranslationMatrix(position);

            // Apply world scale
            mat = mat * scaleMatrix;


            // Render Alignment
            if (renderAlignment == ParticleSystemRenderAlignment::View)
            {
                Mat4DF lookAtViewMat = Mat4DF::CreateLookAtMatrix(
                    position,
                    position - _cameraForward,
                    _cameraUp);
                lookAtViewMat[0][3] = 0.0f;
                lookAtViewMat[1][3] = 0.0f;
                lookAtViewMat[2][3] = 0.0f;
                mat = mat * lookAtViewMat;
            }
            else
            if (renderAlignment == ParticleSystemRenderAlignment::Local)
            {
                mat = mat * particleSystemWorldRotationMatrix;
            }
           

            // Apply particle rotation
            F32 c = cosf(rotationCurrent);
            F32 s = sinf(rotationCurrent);

            mat = mat * Mat4DF(
                c, -s, 0.0f, 0.0f,
                s, c, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            

            // Apply particle size
#if 0
            mat = mat * Mat4DF::CreateScaleMatrix(size.current);
#else
            mat[0][0] *= sizeCurrent;
            mat[0][1] *= sizeCurrent;
            mat[0][2] *= sizeCurrent;
            mat[1][0] *= sizeCurrent;
            mat[1][1] *= sizeCurrent;
            mat[1][2] *= sizeCurrent;
            mat[2][0] *= sizeCurrent;
            mat[2][1] *= sizeCurrent;
            mat[2][2] *= sizeCurrent;
#endif
            
            

            renderTransform = mat;
            renderColor = colorCurrent;


            renderUV = renderUVCallback(index);
        }
    }

} // namespace Maze
//////////////////////////////////////////
