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
#if (!defined(_SpecialEffect_hpp_))
#define _SpecialEffect_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SpecialEffect);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(PlayerOwner);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);
    MAZE_USING_SHARED_PTR(ParticleSystem3D);


    //////////////////////////////////////////
    // Class SpecialEffect
    //
    //////////////////////////////////////////
    class SpecialEffect
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SpecialEffect, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SpecialEffect);

        //////////////////////////////////////////
        friend class Entity;


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }
        

        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        void prepare();

    public:

        //////////////////////////////////////////
        virtual ~SpecialEffect();

        //////////////////////////////////////////
        static SpecialEffectPtr Create();


        //////////////////////////////////////////
        void release();

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventPrepare;

    protected:

        //////////////////////////////////////////
        SpecialEffect();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        SharedPtr<ComponentPoolObject<SpecialEffect>> m_poolObject;
        Transform3DPtr m_transform;     

        Vector<ParticleSystem3DPtr> m_particleSystems;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SpecialEffect_hpp_
//////////////////////////////////////////
