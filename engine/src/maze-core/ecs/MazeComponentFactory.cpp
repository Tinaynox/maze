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
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct ComponentCreationData
    //
    //////////////////////////////////////////
    ComponentCreationData::ComponentCreationData()
        : name(nullptr)
        , staticId(0)
        , createComponentMethod(nullptr)
        , metaClass(nullptr)
        , group(nullptr)
    {

    }

    //////////////////////////////////////////
    // Class ComponentFactory
    //
    //////////////////////////////////////////
    ComponentFactory::ComponentFactory()
    {
    }

    //////////////////////////////////////////
    ComponentFactory::~ComponentFactory()
    {

    }

    //////////////////////////////////////////
    ComponentFactoryPtr ComponentFactory::Create()
    {
        ComponentFactoryPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentFactory, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ComponentFactory::init()
    {
        return true;
    }

    //////////////////////////////////////////
    ComponentPtr ComponentFactory::createComponent(CString _className)
    {
        for (Size i = 0; i < m_sceneObjectCreationData.size(); ++i)
        {
            for (auto const& sceneObjectCreationData : m_sceneObjectCreationData)
                if (strcmp(sceneObjectCreationData.second.name, _className) == 0)
                    return (this->*sceneObjectCreationData.second.createComponentMethod)();
        }

        MAZE_ERROR("Undefined component: %s", _className);
        return ComponentPtr();
    }

    //////////////////////////////////////////
    ComponentPtr ComponentFactory::createComponent(ComponentId _id)
    {
        Map<ComponentId, ComponentCreationData>::const_iterator it = m_sceneObjectCreationData.find(_id);

        MAZE_ERROR_RETURN_VALUE_IF(it == m_sceneObjectCreationData.end(), ComponentPtr(), "Undefined component index: %u", _id);

        return (this->*it->second.createComponentMethod)();
    }

    //////////////////////////////////////////
    ComponentPtr ComponentFactory::createDynamicComponent(ComponentId _id, HashedCString _dynamicComponentName)
    {
        Map<ComponentId, ComponentCreationData>::const_iterator it = m_sceneObjectCreationData.find(_id);

        MAZE_ERROR_RETURN_VALUE_IF(it == m_sceneObjectCreationData.end(), ComponentPtr(), "Undefined component index: %u", _id);
        MAZE_ERROR_RETURN_VALUE_IF(!it->second.createDynamicComponentFunc, ComponentPtr(), "Component is not dynamic: %u", _id);

        return it->second.createDynamicComponentFunc(_dynamicComponentName);
    }

    //////////////////////////////////////////
    ComponentPtr ComponentFactory::createDynamicComponent(CString _staticComponentName, HashedCString _dynamicComponentName)
    {
        ComponentId componentId = getComponentId(_staticComponentName);
        return createDynamicComponent(componentId, _dynamicComponentName);
    }

    //////////////////////////////////////////
    ComponentId ComponentFactory::getComponentId(CString _className)
    {
        for (auto const& sceneObjectCreationData : m_sceneObjectCreationData)
            if (strcmp(sceneObjectCreationData.second.name, _className) == 0)
                return sceneObjectCreationData.second.staticId;

        MAZE_ERROR("Undefined component: %s", _className);
        return 0;
    }
                
    //////////////////////////////////////////
    CString ComponentFactory::getComponentName(ComponentId _id)
    {
        for (auto const& sceneObjectCreationData : m_sceneObjectCreationData)
            if (sceneObjectCreationData.second.staticId == _id)
                return sceneObjectCreationData.second.name;
        
        MAZE_ERROR("Undefined component with uid=%u", _id);
        return nullptr;
    }

    //////////////////////////////////////////
    MetaClass* ComponentFactory::getComponentMetaClass(ComponentId _id)
    {
        for (auto const& sceneObjectCreationData : m_sceneObjectCreationData)
            if (sceneObjectCreationData.second.staticId == _id)
                return sceneObjectCreationData.second.metaClass;

        MAZE_ERROR("Undefined component with uid=%u", _id);
        return nullptr;
    }


} // namespace Maze
//////////////////////////////////////////
