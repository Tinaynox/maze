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
#include "LevelSpawnController.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "game/level/LevelSpawnDataUnit.hpp"
#include "scenes/SceneGame.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class LevelSpawnController
    //
    //////////////////////////////////////////
    LevelSpawnController::LevelSpawnController()
        : m_sceneGame(nullptr)
    {
    }

    //////////////////////////////////////////
    LevelSpawnController::~LevelSpawnController()
    {

    }

    //////////////////////////////////////////
    LevelSpawnControllerPtr LevelSpawnController::Create(SceneGame* _sceneGame)
    {
        LevelSpawnControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LevelSpawnController, object, init(_sceneGame));
        return object;
    }

    //////////////////////////////////////////
    bool LevelSpawnController::init(SceneGame* _sceneGame)
    {
        m_sceneGame = _sceneGame;
        
        return true;
    }

    //////////////////////////////////////////
    void LevelSpawnController::update(F32 _dt)
    {
        while (!m_spawnData.empty() && m_spawnData.front()->getTime() <= m_sceneGame->getGameTimer())
        {
            m_spawnData.front()->spawn(m_sceneGame->getLevelAdapter());
            m_spawnData.pop_front();
        }
    }

    //////////////////////////////////////////
    void LevelSpawnController::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        m_spawnData.clear();

        tinyxml2::XMLNode const* node = _element->FirstChild();
        while (node)
        {
            tinyxml2::XMLElement const* childElement = node->ToElement();
            if (!childElement)
            {
                node = node->NextSibling();
                continue;
            }

            if (strcmp(childElement->Name(), ClassInfo<LevelSpawnDataUnit>::Name()) == 0)
            {
                LevelSpawnDataUnitPtr spawnData = LevelSpawnDataUnit::Create();
                spawnData->loadFromXMLElement(childElement);
                m_spawnData.push_back(spawnData);
            }

            node = node->NextSibling();
        }

        sortSpawnData();
    }

    //////////////////////////////////////////
    void LevelSpawnController::sortSpawnData()
    {
        std::sort(
            m_spawnData.begin(),
            m_spawnData.end(),
            [](LevelSpawnDataPtr const& _a, LevelSpawnDataPtr const& _b) -> bool
            {
                return _a->getTime() < _b->getTime();
            });
    }

} // namespace Maze
//////////////////////////////////////////
