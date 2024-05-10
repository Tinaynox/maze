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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/MazeLightingSettings.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class LightingSettings
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(LightingSettings);

    //////////////////////////////////////////
    LightingSettings::LightingSettings()
    {

    }

    //////////////////////////////////////////
    LightingSettings::~LightingSettings()
    {

    }

    //////////////////////////////////////////
    LightingSettingsPtr LightingSettings::Create()
    {
        LightingSettingsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LightingSettings, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool LightingSettings::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void LightingSettings::setSkyBoxMaterial(String const& _id)
    {
        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_id);
        setSkyBoxMaterial(material);
    }

    
} // namespace Maze
//////////////////////////////////////////
