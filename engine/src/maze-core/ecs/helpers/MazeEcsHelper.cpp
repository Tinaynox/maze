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
#include "MazeCoreHeader.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/components/MazeName.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EcsHelper
    {

        //////////////////////////////////////////
        MAZE_CORE_API CString GetName(Entity* _entity)
        {
            StaticName* staticName = _entity->getComponentRaw<StaticName>();
            if (staticName)
                return staticName->getName();

            Name* name = _entity->getComponentRaw<Name>();
            if (name)
                return name->getName().c_str();

            return "";
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SetName(Entity* _entity, String const& _name)
        {
            _entity->removeComponent<StaticName>();
            _entity->ensureComponent<Name>()->setName(_name);
        }

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
