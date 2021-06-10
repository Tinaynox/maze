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
#if (!defined(_ProjectileManager_hpp_))
#define _ProjectileManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
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
#include "input/PlayerGamepad.hpp"
#include "game/ProjectileAvatarType.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ProjectileManager);
    MAZE_USING_SHARED_PTR(RenderMesh);


    //////////////////////////////////////////
    // Class ProjectileManager
    //
    //////////////////////////////////////////
    class ProjectileManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~ProjectileManager();

        //////////////////////////////////////////
        static void Initialize(ProjectileManagerPtr& _gameManager);
        

        //////////////////////////////////////////
        static inline ProjectileManager* GetInstancePtr(){ return s_instance; }

        //////////////////////////////////////////
        static inline ProjectileManager& GetInstance(){ return *s_instance; }


        /////////////////////////////////////////
        inline EntityPtr const& getProjectilePrefab(ProjectileAvatarType _type) const { return m_projectilePrefabs[_type]; }

        /////////////////////////////////////////
        EntityPtr createProjectile(ProjectileAvatarType _type, ECSScene* _scene);

        //////////////////////////////////////////
        void createProjectilePrefabs();

    protected:

        //////////////////////////////////////////
        ProjectileManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyCoreGameResourcesLoaded();

    protected:
        static ProjectileManager* s_instance;

        EntityPtr m_projectilePrefabs[ProjectileAvatarType::MAX];
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _ProjectileManager_hpp_
//////////////////////////////////////////
