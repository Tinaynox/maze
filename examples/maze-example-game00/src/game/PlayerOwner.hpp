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
#if (!defined(_PlayerOwner_hpp_))
#define _PlayerOwner_hpp_


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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_8(PlayerIndex,
        Player0,
        Player1,
        Player2,
        Player3,
        Enemy0,
        Enemy1,
        Enemy2,
        Enemy3);


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PlayerOwner);


    //////////////////////////////////////////
    // Class PlayerOwner
    //
    //////////////////////////////////////////
    class PlayerOwner
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PlayerOwner, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PlayerOwner);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~PlayerOwner();

        //////////////////////////////////////////
        static PlayerOwnerPtr Create(PlayerIndex _originalOwner = PlayerIndex::Player0);


        //////////////////////////////////////////
        void setOriginalOwner(PlayerIndex _playerIndex);

        //////////////////////////////////////////
        PlayerIndex getOriginalOwner() const { return m_originalOwner; }


        //////////////////////////////////////////
        void setCurrentOwner(PlayerIndex _playerIndex);

        //////////////////////////////////////////
        PlayerIndex getCurrentOwner() const { return m_currentOwner; }


        //////////////////////////////////////////
        inline void setPlayer(PlayerIndex _playerIndex)
        {
            setOriginalOwner(_playerIndex);
            setCurrentOwner(_playerIndex);
        }

    public:

        MultiDelegate<PlayerIndex> eventOriginalOwnerChanged;
        MultiDelegate<PlayerIndex> eventCurrentOwnerChanged;

    protected:

        //////////////////////////////////////////
        PlayerOwner();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(PlayerIndex _originalOwner);

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        PlayerIndex m_originalOwner;
        PlayerIndex m_currentOwner;
    };


    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS_SERIALIZATION(PlayerIndex);


} // namespace Maze
//////////////////////////////////////////


#endif // _PlayerOwner_hpp_
//////////////////////////////////////////
