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
#include "game/level/LevelSpawnDataUnit.hpp"
#include "game/LevelAdapter.hpp"
#include "game/SpaceObject.hpp"
#include "game/SpaceObjectPool.hpp"
#include "game/PlayerOwner.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class LevelSpawnDataUnit
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(LevelSpawnDataUnit, LevelSpawnData,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpaceObjectAvatarType, avatarType, SpaceObjectAvatarType::None, getAvatarType, setAvatarType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, spawnX, 0.0f, getSpawnX, setSpawnX),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, spawnY, 0.0f, getSpawnY, setSpawnY),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, startRotation, Math::c_halfPi, getStartRotation, setStartRotation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(PlayerIndex, player, PlayerIndex::Enemy0, getPlayer, setPlayer));


    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(LevelSpawnDataUnit);


    //////////////////////////////////////////
    LevelSpawnDataUnit::LevelSpawnDataUnit()
        : m_avatarType(SpaceObjectAvatarType::None)
        , m_spawnX(0.0f)
        , m_spawnY(0.5f)
        , m_startRotation(Math::c_halfPi)
        , m_player(PlayerIndex::Enemy0)
    {

    }

    //////////////////////////////////////////
    LevelSpawnDataUnit::~LevelSpawnDataUnit()
    {

    }

    //////////////////////////////////////////
    LevelSpawnDataUnitPtr LevelSpawnDataUnit::Create(
        F32 _time,
        SpaceObjectAvatarType _spaceObject,
        F32 _spawnX,
        F32 _spawnY)
    {
        LevelSpawnDataUnitPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LevelSpawnDataUnit, object, init(_time, _spaceObject, _spawnX, _spawnY));
        return object;
    }
       
    //////////////////////////////////////////
    bool LevelSpawnDataUnit::init(
        F32 _time,
        SpaceObjectAvatarType _avatarType,
        F32 _spawnX,
        F32 _spawnY)
    {
        if (!LevelSpawnData::init())
            return false;

        setTime(_time);
        setAvatarType(_avatarType);
        setSpawnX(_spawnX);
        setSpawnY(_spawnY);
        
        return true;
    }

    //////////////////////////////////////////
    void LevelSpawnDataUnit::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        DeserializeMetaInstanceFromXMLElement(getMetaClass(), getMetaInstance(), _element);
    }

    //////////////////////////////////////////
    void LevelSpawnDataUnit::spawn(LevelAdapterPtr const& _levelAdapter)
    {
        Rect2DF const& gameplayBounds = _levelAdapter->getGameplayBounds();

        SpaceObjectPtr spaceObject = _levelAdapter->getSpaceObjectPool()->createSpaceObject(m_avatarType);
        spaceObject->getTransform()->setLocalRotation(Quaternion(0.0f, m_startRotation, 0.0f));
        spaceObject->getTransform()->setLocalX(gameplayBounds.position.x + gameplayBounds.size.x + 1.0f + m_spawnX);
        spaceObject->getTransform()->setLocalY(gameplayBounds.position.y + m_spawnY * gameplayBounds.size.y);

        spaceObject->getRigidbody2D()->setPosition(spaceObject->getTransform()->getLocalPositionXY());

        spaceObject->getPlayerOwner()->setPlayer(m_player);
        spaceObject->setDirection(SpaceObjectAvatarDirection::Left);
        spaceObject->setThrottle(Vec2DF::c_negativeUnitX);
        spaceObject->setFire(true);
    }
    

} // namespace Maze
//////////////////////////////////////////
