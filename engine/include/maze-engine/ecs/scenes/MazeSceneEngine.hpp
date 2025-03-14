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
#if (!defined(_SceneEngine_hpp_))
#define _SceneEngine_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneEngine);
    MAZE_USING_MANAGED_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(RenderController);
    MAZE_USING_SHARED_PTR(ParticlesDrawerController);
    MAZE_USING_SHARED_PTR(InputSystem2D);
    MAZE_USING_SHARED_PTR(GizmosController);    


    //////////////////////////////////////////
    // Struct SceneEngineInitConfig
    //
    //////////////////////////////////////////
    struct SceneEngineInitConfig
    {
        RenderTargetPtr renderTarget;
        RenderSystemPtr renderSystem;

        bool createRenderController = true;
        bool createParticlesDrawerController = true;
        bool createInputSystem2D = true;
        bool createGizmosController = true;
    };


    //////////////////////////////////////////
    // Class SceneEngine
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API SceneEngine
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneEngine, EcsRenderScene);

    public:

        //////////////////////////////////////////
        static SceneEnginePtr Create(SceneEngineInitConfig const& _config);
    
        //////////////////////////////////////////
        virtual ~SceneEngine();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline RenderControllerPtr const& getRenderController() const { return m_renderController; }

        //////////////////////////////////////////
        inline ParticlesDrawerControllerPtr const& getParticlesDrawerController() const { return m_particlesDrawerController; }

        //////////////////////////////////////////
        inline InputSystem2DPtr const& getInputSystem2D() const { return m_inputSystem2D; }

    protected:

        //////////////////////////////////////////
        SceneEngine();

        //////////////////////////////////////////
        virtual bool init(SceneEngineInitConfig const& _config);


    protected:
        RenderControllerPtr m_renderController;
        ParticlesDrawerControllerPtr m_particlesDrawerController;
        InputSystem2DPtr m_inputSystem2D;
        GizmosControllerPtr m_gizmosController;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneEngine_hpp_
//////////////////////////////////////////
