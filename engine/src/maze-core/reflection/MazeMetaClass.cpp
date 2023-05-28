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
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MetaProperty
    //
    //////////////////////////////////////////
    MetaProperty::MetaProperty(CString _name)
        : m_name(_name)
    {
    }
    
    
    //////////////////////////////////////////
    // Class MetaClass
    //
    //////////////////////////////////////////
    Map<ClassUID, MetaClass*> MetaClass::s_metaClassByUID = Map<ClassUID, MetaClass*>();

    //////////////////////////////////////////
    MetaClass::MetaClass(
        CString _name,
        MetaProperty** _properties,
        S32 _propertiesCount)
        : m_name(_name)
        , m_properties(_properties)
        , m_propertiesCount(_propertiesCount)
        , m_isMultipleInheritance(false)
    {
        collectSuperMetaClasses(m_allSuperMetaClasses);
    }
    
    //////////////////////////////////////////
    MetaClass::MetaClass(
        CString _name,
        Vector<SuperMetaClassData> const& _superMetaClassesData,
        MetaProperty** _properties,
        S32 _propertiesCount)
        : m_name(_name)
        , m_superMetaClassesData(_superMetaClassesData)
        , m_properties(_properties)
        , m_propertiesCount(_propertiesCount)
        , m_isMultipleInheritance(_superMetaClassesData.size() >= 2)
    {
        collectSuperMetaClasses(m_allSuperMetaClasses);

        if (!m_isMultipleInheritance) 
        {
            for (SuperMetaClassData const& superMetaClassData : _superMetaClassesData)
            {
                m_isMultipleInheritance |= superMetaClassData.metaClass->getIsMultipleInheritance();
            }
        }
    }

    //////////////////////////////////////////
    MetaProperty const* MetaClass::getProperty(S32 _index) const
    {
#if (MAZE_DEBUG)
        MAZE_BP_RETURN_VALUE_IF(_index >= m_propertiesCount, nullptr);
#endif
        return m_properties[_index];
    }

    //////////////////////////////////////////
    MetaProperty* MetaClass::getProperty(S32 _index)
    {
#if (MAZE_DEBUG)
        MAZE_BP_RETURN_VALUE_IF(_index >= m_propertiesCount, nullptr);
#endif
        return m_properties[_index];
    }

    //////////////////////////////////////////
    S32 MetaClass::getPropertyIndex(CString _name) const
    {
        for (S32 i = 0; i < m_propertiesCount; i++)
        {
            MetaProperty* m = m_properties[i];
            if (strcmp(m->getName(), _name) == 0)
                return i;
        }

        return -1;
    }

    //////////////////////////////////////////
    MetaProperty const* MetaClass::getProperty(CString _name, bool _recursive) const
    {
        for (S32 i = 0; i < m_propertiesCount; i++) 
        {
            MetaProperty* m = m_properties[i];
            if (strcmp(m->getName(), _name) == 0)
                return m;
        }

        if (_recursive)
            for (auto superMetaClassData : getSuperMetaClassesData())
            {
                const MetaProperty* metaProperty = superMetaClassData.metaClass->getProperty(_name, _recursive);
                if (metaProperty)
                    return metaProperty;
            }

        return nullptr;
    }

    //////////////////////////////////////////
    MetaProperty* MetaClass::getProperty(CString _name, bool _recursive)
    {
        for (S32 i = 0; i < m_propertiesCount; i++) 
        {
            MetaProperty* m = m_properties[i];
            if (strcmp(m->getName(), _name) == 0)
                return m;
        }

        if (_recursive)
            for (auto superMetaClassData : getSuperMetaClassesData())
            {
                MetaProperty* metaProperty = superMetaClassData.metaClass->getProperty(_name, _recursive);
                if (metaProperty)
                    return metaProperty;
            }

        return nullptr;
    }

    //////////////////////////////////////////
    MetaClass* MetaClass::GetMetaClass(ClassUID _classUID)
    {
        auto it = s_metaClassByUID.find(_classUID);
        if (it != s_metaClassByUID.end())
            return it->second;

        return nullptr;
    }
    
    
    

    //////////////////////////////////////////
    // Class ConstMetaInstance
    //
    //////////////////////////////////////////
    ConstMetaInstance::ConstMetaInstance()
        : m_metaClass(nullptr)
        , m_object(nullptr)
    {}

    //////////////////////////////////////////
    ConstMetaInstance::ConstMetaInstance(MetaClass const* _metaClass, void const* _pointer)
        : m_metaClass(_metaClass)
        , m_object(_pointer)
    {}

    //////////////////////////////////////////
    ConstMetaInstance::ConstMetaInstance(MetaInstance const& _instance)
        : m_metaClass(_instance.m_metaClass)
        , m_object(_instance.m_object)
    {
    }

    //////////////////////////////////////////
    void* ConstMetaInstance::createCopy()
    {
        void* obj = m_metaClass->createInstance();
        if (obj)
        {
            MetaClassHelper::CopyProperties(obj, m_metaClass, m_object);
        }

        return obj;
    }


    //////////////////////////////////////////
    // Class MetaInstance
    //
    //////////////////////////////////////////
    MetaInstance::MetaInstance()
        : ConstMetaInstance()
    {
    }

    //////////////////////////////////////////
    MetaInstance::MetaInstance(MetaClass const* _metaClass, void* _object)
        : ConstMetaInstance(_metaClass, _object)
    {
    }


} // namespace Maze
//////////////////////////////////////////
