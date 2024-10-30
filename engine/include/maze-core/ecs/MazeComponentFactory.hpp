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
#if (!defined(_MazeFactory_hpp_))
#define _MazeFactory_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ComponentFactory);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(ComponentComponentFactoryLinker);


    //////////////////////////////////////////
    // Struct ComponentCreationData
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API ComponentCreationData
    {
        ////////////////////////////////////
        ComponentCreationData();

        CString name;
        ComponentId id;
        ComponentPtr (ComponentFactory::*createComponentMethod)(void);
        MetaClass* metaClass;

        CString group;
    };


    //////////////////////////////////////////
    // Class ComponentFactory
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ComponentFactory
    {
    public:

        //////////////////////////////////////////
        virtual ~ComponentFactory();

        //////////////////////////////////////////
        static ComponentFactoryPtr Create();

        //////////////////////////////////////////
        template <class T>
        void registerComponent(CString _group = nullptr)
        {
            MAZE_ERROR_IF(strcmp(T::GetMetaClass()->getName(), static_cast<CString>(ClassInfo<T>::Name())) != 0, "Class %s should have his own MetaClass! Current MetaClass is %s", ClassInfo<T>::Name(), T::GetMetaClass()->getName());

            ComponentId staticId = GetStaticComponentId<T>();

            ComponentCreationData objectData;
            objectData.name = ClassInfo<T>::Name();
            objectData.id = staticId;
            objectData.createComponentMethod = &ComponentFactory::createComponent<T>;
            objectData.metaClass = T::GetMetaClass();
            objectData.group = _group;

            m_sceneObjectCreationData.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(staticId),
                std::forward_as_tuple(objectData));
        }

        //////////////////////////////////////////
        template <typename T, typename ...TArgs> 
        ComponentPtr createComponent(TArgs... _args)
        {
            return T::Create(_args...);
        }

        //////////////////////////////////////////
        ComponentPtr createComponent(CString _className);

        //////////////////////////////////////////
        inline ComponentPtr createComponent(String const& _className) { return createComponent(_className.c_str()); }

        //////////////////////////////////////////
        ComponentPtr createComponent(ComponentId _componentUID);

        //////////////////////////////////////////
        ComponentId getComponentId(CString _className);
        
        //////////////////////////////////////////
        ComponentId getComponentId(String const& _className) { return getComponentId(_className.c_str()); }
        
        //////////////////////////////////////////
        CString getComponentName(ComponentId _id);

        //////////////////////////////////////////
        MetaClass* getComponentMetaClass(ComponentId _uid);

        //////////////////////////////////////////
        Map<ComponentId, ComponentCreationData> const& getSceneObjectCreationData() const { return m_sceneObjectCreationData; }

    protected:

        //////////////////////////////////////////
        ComponentFactory();

        //////////////////////////////////////////
        virtual bool init();


    protected:
        Map<ComponentId, ComponentCreationData> m_sceneObjectCreationData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFactory_hpp_
//////////////////////////////////////////
