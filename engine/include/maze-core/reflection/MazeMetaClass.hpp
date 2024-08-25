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
#if (!defined(_MazeMetaClass_hpp_))
#define _MazeMetaClass_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeStringSerialization.hpp"
#include "maze-core/serialization/MazeBinarySerialization.hpp"
#include "maze-core/serialization/MazeDataBlockSerialization.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include <algorithm>
#include <type_traits>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class MetaProperty;
    class MetaClass;
    class MetaInstance;
    class ConstMetaInstance;
    class EnumClass;


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasMetaClass : std::false_type { };

    template <typename T>
    struct HasMetaClass <T, decltype(&T::GetMetaClass, 0)> : std::true_type { };

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(HasMetaClass<TValue>::value), MetaClass const*>::type GetMetaClass()
    {
        return TValue::GetMetaClass();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!HasMetaClass<TValue>::value), MetaClass const*>::type GetMetaClass()
    {
        return nullptr;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(HasMetaClass<TValue>::value), MetaClass const*>::type GetMetaClass(TValue const& _value)
    {
        return _value.getMetaClass();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!HasMetaClass<TValue>::value), MetaClass const*>::type GetMetaClass(TValue const& _value)
    {
        return nullptr;
    }


    //////////////////////////////////////////
    // Class MetaProperty
    //
    //////////////////////////////////////////
    class MAZE_CORE_API MetaProperty
    {
    public:

        //////////////////////////////////////////
        MetaProperty(CString _name);

        //////////////////////////////////////////
        inline HashedCString getName() const { return m_name; }

        //////////////////////////////////////////
        virtual ClassUID getValueClassUID() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool isEnumClass() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vector<String> getEnumClassValues() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual MetaClass const* getMetaClass() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual MetaClass const* getMetaClass(MetaInstance const& _instance) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool isSharedPtr() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void* getSharedPtrPointer(MetaInstance const& _instance) const MAZE_ABSTRACT;



        //////////////////////////////////////////
        virtual bool isVector() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Size getVectorSize(MetaInstance const& _instance) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setVectorSize(MetaInstance const& _instance, Size _size) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual ClassUID getVectorElementClassUID(MetaInstance const& _instance) const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool hasOperatorEquals() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isEqual(ConstMetaInstance const& _instance0, ConstMetaInstance const& _instance1) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Size getValueSize() const MAZE_ABSTRACT;

        

        //////////////////////////////////////////
        virtual void getValue(ConstMetaInstance const& _instance, void* _buffer) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        template <typename TClass>
        inline void getValue(ConstMetaInstance const& _instance, TClass& _value) const
        {
            getValue(_instance, &_value);
        }

        //////////////////////////////////////////
        virtual void setValue(MetaInstance const& _instance, void const* _buffer) const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void copy(void* _destObject, void const* _srcObject) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void copy(MetaInstance const& _dest, ConstMetaInstance const& _src) const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool setString(MetaInstance const& _instance, CString _value, Size _count) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline bool setString(MetaInstance const& _instance, String const& _value) const
        {
            return setString(_instance, _value.c_str(), _value.size());
        }

        //////////////////////////////////////////
        virtual bool toString(MetaInstance const& _instance, String& _outString) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool toString(ConstMetaInstance const& _instance, String& _outString) const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool getSerializationSize(MetaInstance const& _instance, U32& _outSize) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool getSerializationSize(ConstMetaInstance const& _instance, U32& _outSize) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool serializeTo(MetaInstance const& _instance, U8* _data) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool serializeTo(ConstMetaInstance const& _instance, U8* _data) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool deserializeFrom(MetaInstance const& _instance, U8 const* _data) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool isDataBlockSerializable(ConstMetaInstance const& _instance) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool setDataBlock(MetaInstance const& _instance, DataBlock const& _data) const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool toDataBlock(ConstMetaInstance const& _instance, DataBlock& _data) const MAZE_ABSTRACT;

    private:
        HashedCString m_name;
    };


    //////////////////////////////////////////
    using SuperMetaClassPointerCastFunction = void* (*)(void*);


    //////////////////////////////////////////
    struct SuperMetaClassData
    {
        //////////////////////////////////////////
        SuperMetaClassData(
           MetaClass* _metaClass = nullptr,
           SuperMetaClassPointerCastFunction _superToCurrentPointerCast = nullptr,
           SuperMetaClassPointerCastFunction _currentToSuperPointerCast = nullptr)
            : metaClass(_metaClass)
            , superToCurrentPointerCast(_superToCurrentPointerCast)
            , currentToSuperPointerCast(_currentToSuperPointerCast)
        {}

        MetaClass* metaClass;
        SuperMetaClassPointerCastFunction superToCurrentPointerCast;
        SuperMetaClassPointerCastFunction currentToSuperPointerCast;
    };

    //////////////////////////////////////////
    template <typename TFromClass, typename TToClass>
    void* GenericSuperMetaClassPointerCast(void* _p)
    {
        return static_cast<TToClass*>((TFromClass*)_p);
    }

    //////////////////////////////////////////
    // Struct GenericSuperMetaClassData
    //
    //////////////////////////////////////////
    template <typename TSuperMetaClass, typename TCurrentClass>
    struct GenericSuperMetaClassData
        : public SuperMetaClassData
    {
        //////////////////////////////////////////
        GenericSuperMetaClassData(MetaClass* _metaClass)
            : SuperMetaClassData(
                _metaClass,
                &GenericSuperMetaClassPointerCast<TSuperMetaClass, TCurrentClass>,
                &GenericSuperMetaClassPointerCast<TCurrentClass, TSuperMetaClass>)
        {}

    };


    //////////////////////////////////////////
    // Class MetaClass
    //
    //////////////////////////////////////////
    class MAZE_CORE_API MetaClass
    {
    public:

        //////////////////////////////////////////
        MetaClass(
                  CString _name,
                  MetaProperty** _properties,
                  S32 _propertiesCount);

        //////////////////////////////////////////
        MetaClass(
                  CString _name,
                  Vector<SuperMetaClassData> const& _superMetaClasses,
                  MetaProperty** _properties,
                  S32 _propertiesCount);

        //////////////////////////////////////////
        virtual ClassUID getClassUID() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline HashedCString getName() const { return m_name; }

        //////////////////////////////////////////
        inline Vector<SuperMetaClassData> const& getSuperMetaClassesData() const { return m_superMetaClassesData; }

        //////////////////////////////////////////
        inline S32 getPropertiesCount() const { return m_propertiesCount; }



        //////////////////////////////////////////
        MetaProperty const* getProperty(S32 _index) const;

        //////////////////////////////////////////
        MetaProperty* getProperty(S32 _index);

        //////////////////////////////////////////
        S32 getPropertyIndex(CString _name) const;

        //////////////////////////////////////////
        MetaProperty const* getProperty(CString _name, bool _recursive = true) const;

        //////////////////////////////////////////
        MetaProperty* getProperty(CString _name, bool _recursive = true);

        //////////////////////////////////////////
        inline bool isInheritedFrom(MetaClass const* _superMetaClass) const
        {
            if (this == _superMetaClass)
                return true;

            for (Size i = 0, in = m_allSuperMetaClasses.size(); i <in; ++i)
                if (m_allSuperMetaClasses[i] == _superMetaClass)
                    return true;

            return false;
        }

        //////////////////////////////////////////
        inline bool isInheritedFrom(ClassUID _classUID) const
        {
            if (getClassUID() == _classUID)
                return true;

            for (Size i = 0, in = m_allSuperMetaClasses.size(); i < in; ++i)
                if (m_allSuperMetaClasses[i]->getClassUID() == _classUID)
                    return true;

            return false;
        }

        //////////////////////////////////////////
        inline const Vector<MetaClass*>& getAllSuperMetaClasses() const { return m_allSuperMetaClasses; }

        //////////////////////////////////////////
        inline bool getIsMultipleInheritance() const { return m_isMultipleInheritance; }



        //////////////////////////////////////////
        template <typename TClass>
        inline bool isInheritedFrom() const
        {
            return isInheritedFrom(TClass::GetMetaClass());
        }

        //////////////////////////////////////////
        Vector<SuperMetaClassData> getInheritanceSuccession(MetaClass const* _superMetaClass) const
        {
            Vector<SuperMetaClassData> branchResults;
            if (getInheritanceSuccession(_superMetaClass, branchResults))
                return branchResults;

            return Vector<SuperMetaClassData>();
        }

        //////////////////////////////////////////
        bool getInheritanceSuccession(
            MetaClass const* _superMetaClass,
            Vector<SuperMetaClassData>& _branchResults) const
        {
            if (this == _superMetaClass)
                return true;

            for (auto const& superMetaClassData : getSuperMetaClassesData())
            {
                _branchResults.push_back(superMetaClassData);

                Vector<SuperMetaClassData> otherBranch;
                if (superMetaClassData.metaClass->getInheritanceSuccession(_superMetaClass, otherBranch))
                {
                    for (SuperMetaClassData& d : otherBranch)
                        _branchResults.push_back(d);

                    return true;
                }
            }

            return false;
        }


        //////////////////////////////////////////
        virtual void* createInstance() const MAZE_ABSTRACT;

    public:

        //////////////////////////////////////////
        static MetaClass* GetMetaClass(ClassUID _classUID);

    protected:

        //////////////////////////////////////////
        inline void collectSuperMetaClasses(Vector<MetaClass*>& _result)
        {
            if (std::find(_result.begin(), _result.end(), this) == _result.end())
                _result.push_back(this);

            for (auto superMetaClassData : getSuperMetaClassesData())
                superMetaClassData.metaClass->collectSuperMetaClasses(_result);
        }

    public:

        //////////////////////////////////////////
        static Map<ClassUID, MetaClass*> s_metaClassByUID;

    private:
        HashedCString m_name;
        Vector<SuperMetaClassData> m_superMetaClassesData;
        MetaProperty** m_properties;
        S32 m_propertiesCount;

        Vector<MetaClass*> m_allSuperMetaClasses;

        bool m_isMultipleInheritance;
    };


    //////////////////////////////////////////
    // Class GenericMetaClass
    //
    //////////////////////////////////////////
    template <typename TClass>
    class GenericMetaClass
        : public MetaClass
    {
    public:

        //////////////////////////////////////////
        GenericMetaClass(
                         CString _name,
                         MetaProperty** _properties,
                         S32 _propertiesCount)
            : MetaClass(
                _name,
                _properties,
                _propertiesCount)
        {
            ClassUID classUID = getClassUID();

#if (MAZE_DEBUG)
            auto it = s_metaClassByUID.find(classUID);
            MAZE_ERROR_IF(
                 it != s_metaClassByUID.end(),
                "MetaClass %s duplicate!",
                _name);
#endif

            s_metaClassByUID.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(classUID),
                std::forward_as_tuple(this));
        }

        //////////////////////////////////////////
        GenericMetaClass(
                         CString _name,
                         const Vector<SuperMetaClassData>& _superMetaClassesData,
                         MetaProperty** _properties,
                         S32 _propertiesCount)
            : MetaClass(
                _name,
                _superMetaClassesData,
                _properties,
                _propertiesCount)
        {
            ClassUID classUID = getClassUID();

            MAZE_DEBUG_ERROR_IF(
                s_metaClassByUID.find(classUID) != s_metaClassByUID.end(),
                "MetaClass %s duplicate!",
                _name);

            s_metaClassByUID.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(classUID),
                std::forward_as_tuple(this));
        }


        //////////////////////////////////////////
        virtual ClassUID getClassUID() const MAZE_OVERRIDE
        {
            return ClassInfo<TClass>::UID();
        }

        //////////////////////////////////////////
        virtual void* createInstance() const MAZE_OVERRIDE
        {
            return reinterpret_cast<void*>(createInstanceImpl<TClass>());
        }

    protected:

        //////////////////////////////////////////
        template <typename T>
        typename ::std::enable_if<(::std::is_abstract<T>::value), T*>::type
            createInstanceImpl() const
        {
            return nullptr;
        }

        //////////////////////////////////////////
        template <typename T>
        typename ::std::enable_if<!(::std::is_abstract<T>::value), T*>::type
            createInstanceImpl() const
        {
            return new TClass();
        }
    };


    //////////////////////////////////////////
    // Class ConstMetaInstance
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ConstMetaInstance
    {
    public:

        //////////////////////////////////////////
        ConstMetaInstance();

        //////////////////////////////////////////
        ConstMetaInstance(MetaClass const* _metaClass, void const* _object);

        //////////////////////////////////////////
        ConstMetaInstance(MetaInstance const& _instance);

        //////////////////////////////////////////
        inline operator bool(void) const
        {
            return (m_metaClass != nullptr) && (m_object != nullptr);
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline const TClass* reinterpretObjectCast() const
        {
            return reinterpret_cast<TClass const*>(m_object);
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline TClass const* staticObjectCast() const
        {
            if (m_metaClass->isInheritedFrom<TClass>())
                return static_cast<TClass const*>(m_object);
            
            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline TClass const* dynamicObjectCast() const
        {
            if (TClass::GetMetaClass() == m_metaClass)
                return reinterpret_cast<TClass const*>(m_object);

            Vector<SuperMetaClassData> inheritanceSuccesion = m_metaClass->getInheritanceSuccession(TClass::GetMetaClass());

            if (!inheritanceSuccesion.empty())
            {

                void* p = (void*)m_object;
                for (Vector<SuperMetaClassData>::iterator        it = inheritanceSuccesion.begin(),
                                                                end = inheritanceSuccesion.end();
                                                                it != end;
                                                                ++it)
                {
                    p = it->currentToSuperPointerCast(p);
                }

                return reinterpret_cast<TClass const*>(p);
            }

            return nullptr;
        }

        //////////////////////////////////////////
        inline MetaClass const* getMetaClass() const { return m_metaClass; }

        //////////////////////////////////////////
        void* createCopy();

    protected:
        MetaClass const* m_metaClass;
        void const* m_object;
    };


    //////////////////////////////////////////
    // Class MetaInstance
    //
    //////////////////////////////////////////
    class MAZE_CORE_API MetaInstance
        : public ConstMetaInstance
    {
    public:

        //////////////////////////////////////////
        MetaInstance();

        //////////////////////////////////////////
        MetaInstance(MetaClass const* _metaClass, void* _object);

        //////////////////////////////////////////
        template <typename TClass>
        inline TClass* reinterpretObjectCast() const
        {
            return const_cast<TClass*>(ConstMetaInstance::reinterpretObjectCast<TClass>());
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline TClass* staticObjectCast() const
        {
            return const_cast<TClass*>(ConstMetaInstance::staticObjectCast<TClass>());
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline TClass* dynamicObjectCast() const
        {
            return const_cast<TClass*>(ConstMetaInstance::dynamicObjectCast<TClass>());
        }
    };


    //////////////////////////////////////////
    // Class GenericMetaProperty
    //
    //////////////////////////////////////////
    template <
        typename TObject,
        typename TValue,
        typename TPropertyValue,
        typename TSetterReturnValue = void>
    class GenericMetaProperty
        : public MetaProperty
    {
    public:

        //////////////////////////////////////////
        using Getter = TPropertyValue (TObject::*)(void) const;
        using Setter = TSetterReturnValue (TObject::*)(TPropertyValue);

        //////////////////////////////////////////
        GenericMetaProperty(
            CString _name,
            Getter _getter,
            Setter _setter,
            TValue const& _defaultValue = TValue())
            : MetaProperty(_name)
            , m_getter(_getter)
            , m_setter(_setter)
            , m_defaultValue(_defaultValue)
        {
        }

        //////////////////////////////////////////
        inline TObject* castMetaInstanceObject(MetaInstance const& _metaInstance) const
        {
            if (_metaInstance.getMetaClass()->getIsMultipleInheritance())
            {
                return _metaInstance.dynamicObjectCast<TObject>();
            }
            else
            {
                return _metaInstance.reinterpretObjectCast<TObject>();
            }
        }

        //////////////////////////////////////////
        inline TObject const* castMetaInstanceObject(ConstMetaInstance const& _metaInstance) const
        {
            if (_metaInstance.getMetaClass()->getIsMultipleInheritance())
            {
                return _metaInstance.dynamicObjectCast<TObject>();
            }
            else
            {
                return _metaInstance.reinterpretObjectCast<TObject>();
            }
        }

        //////////////////////////////////////////
        virtual ClassUID getValueClassUID() const MAZE_OVERRIDE
        {
            return ClassInfo<TValue>::UID();
        }

        //////////////////////////////////////////
        virtual bool isEnumClass() const MAZE_OVERRIDE
        {
            return std::is_base_of<EnumClass, TValue>::value;
        }

        //////////////////////////////////////////
        virtual Vector<String> getEnumClassValues() const MAZE_OVERRIDE
        {
            return GetEnumClassValues<TValue>();
        }

        //////////////////////////////////////////
        virtual MetaClass const* getMetaClass() MAZE_OVERRIDE
        {
            return this->getMetaClassImpl<TValue>();
        }

        //////////////////////////////////////////
        virtual MetaClass const* getMetaClass(MetaInstance const& _instance) MAZE_OVERRIDE
        {            
            return this->getMetaClassImpl<TValue>(_instance);
        }


        //////////////////////////////////////////
        virtual bool isSharedPtr() const MAZE_OVERRIDE
        {
            return IsSharedPtr<TValue>::value;
        }

        //////////////////////////////////////////
        virtual void* getSharedPtrPointer(MetaInstance const& _instance) const MAZE_OVERRIDE
        {
            return this->getSharedPtrPointerImpl<TValue>(_instance);
        }


        //////////////////////////////////////////
        virtual bool isVector() const MAZE_OVERRIDE
        {
            return IsVector<TValue>::value;
        }

        //////////////////////////////////////////
        virtual Size getVectorSize(MetaInstance const& _instance) const MAZE_OVERRIDE
        {
            return this->getVectorSizeImpl<TValue>(_instance);
        }

        //////////////////////////////////////////
        virtual void setVectorSize(MetaInstance const& _instance, Size _size) MAZE_OVERRIDE
        {
            return this->setVectorSizeImpl<TValue>(_instance, _size);
        }

        //////////////////////////////////////////
        virtual ClassUID getVectorElementClassUID(MetaInstance const& _instance) const MAZE_OVERRIDE
        {
            return this->getVectorElementClassUIDImpl<TValue>(_instance);
        }


        //////////////////////////////////////////
        virtual bool hasOperatorEquals() const MAZE_OVERRIDE
        {
            return StdHelper::HasOperatorEquals<TValue>::value;
        }

        //////////////////////////////////////////
        virtual bool isEqual(ConstMetaInstance const& _instance0, ConstMetaInstance const& _instance1) const MAZE_OVERRIDE
        {
            return isEqualImpl<TValue>(_instance0, _instance1);
        }

        //////////////////////////////////////////
        virtual Size getValueSize() const MAZE_OVERRIDE
        {
            return sizeof(TValue);
        }

        //////////////////////////////////////////
        virtual void getValue(ConstMetaInstance const& _instance, void* _buffer) const MAZE_OVERRIDE
        {
            TObject const* obj = castMetaInstanceObject(_instance);

            *((TValue*)_buffer) = (obj->*m_getter)();
        }

        //////////////////////////////////////////
        virtual void setValue(MetaInstance const& _instance, void const* _buffer) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            (obj->*m_setter)(*((TValue const*)_buffer));
        }

        //////////////////////////////////////////
        virtual void copy(void* _destObject, void const* _srcObject) const MAZE_OVERRIDE
        {
            TObject const* srcObj = reinterpret_cast<TObject const*>(_srcObject);
            TObject* destObj = reinterpret_cast<TObject*>(_destObject);

            (destObj->*m_setter)(
                (srcObj->*m_getter)());
        }

        //////////////////////////////////////////
        virtual void copy(MetaInstance const& _dest, ConstMetaInstance const& _src) const MAZE_OVERRIDE
        {
            TObject const* srcObj = castMetaInstanceObject(_src);
            TObject* destObj = castMetaInstanceObject(_dest);

            (destObj->*m_setter)(
                (srcObj->*m_getter)());
        }

        //////////////////////////////////////////
        virtual bool setString(MetaInstance const& _instance, CString _data, Size _count) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);
                        
            TValue value;
            if (TryValueFromString<TValue>(value, _data, _count))
            {
               (obj->*m_setter)(value);
               return true;
            }

            return false;
        }

        //////////////////////////////////////////
        virtual bool toString(MetaInstance const& _instance, String& _outString) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            return TryValueToString<TValue>((obj->*m_getter)(), _outString);
        }

        //////////////////////////////////////////
        virtual bool toString(ConstMetaInstance const& _instance, String& _outString) const MAZE_OVERRIDE
        {
            TObject const* obj = castMetaInstanceObject(_instance);

            return TryValueToString<TValue>((obj->*m_getter)(), _outString);
        }

        //////////////////////////////////////////
        virtual bool getSerializationSize(MetaInstance const& _instance, U32& _outSize) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            return TryGetValueSerializationSize<TValue>((obj->*m_getter)(), _outSize);
        }

        //////////////////////////////////////////
        virtual bool getSerializationSize(ConstMetaInstance const& _instance, U32& _outSize) const MAZE_OVERRIDE
        {
            TObject const* obj = castMetaInstanceObject(_instance);

            return TryGetValueSerializationSize<TValue>((obj->*m_getter)(), _outSize);
        }

        //////////////////////////////////////////
        virtual bool serializeTo(MetaInstance const& _instance, U8* _data) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            return TrySerializeValue<TValue>((obj->*m_getter)(), _data);
        }

        //////////////////////////////////////////
        virtual bool serializeTo(ConstMetaInstance const& _instance, U8* _data) const MAZE_OVERRIDE
        {
            TObject const* obj = castMetaInstanceObject(_instance);

            return TrySerializeValue<TValue>((obj->*m_getter)(), _data);
        }

        //////////////////////////////////////////
        virtual bool deserializeFrom(MetaInstance const& _instance, U8 const* _data) MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            TValue value;
            if (TryDeserializeValue<TValue>(value, _data))
            {
                (obj->*m_setter)(value);
                return true;
            }

            return false;
        }


        //////////////////////////////////////////
        virtual bool isDataBlockSerializable(ConstMetaInstance const& _instance) const MAZE_OVERRIDE
        {
            return HasValueToDataBlock<TValue>::value || std::is_base_of<IDataBlockSerializable, TValue>::value;
        }

        //////////////////////////////////////////
        virtual bool setDataBlock(MetaInstance const& _instance, DataBlock const& _data) const MAZE_OVERRIDE
        {
            TObject* obj = castMetaInstanceObject(_instance);

            TValue value;
            if (TryValueFromDataBlock<TValue>(value, _data))
            {
                (obj->*m_setter)(value);
                return true;
            }

            return false;
        }

        //////////////////////////////////////////
        virtual bool toDataBlock(ConstMetaInstance const& _instance, DataBlock& _data) const MAZE_OVERRIDE
        {
            TObject const* obj = castMetaInstanceObject(_instance);

            return TryValueToDataBlock<TValue>((obj->*m_getter)(), _data);
        }


    protected:

        //////////////////////////////////////////
        template <typename TEnumValue>
        inline static typename ::std::enable_if<(std::is_base_of<EnumClass, TEnumValue>::value), Vector<String>>::type GetEnumClassValues()
        {
            return TEnumValue::AllStrings();
        }

        //////////////////////////////////////////
        template <typename TEnumValue>
        inline static typename ::std::enable_if<(!std::is_base_of<EnumClass, TEnumValue>::value), Vector<String>>::type GetEnumClassValues()
        {
            return Vector<String>();
        }


        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(HasMetaClass<TProperty>::value), MetaClass const*>::type getMetaClassImpl() const
        {
            return TProperty::GetMetaClass();
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!HasMetaClass<TProperty>::value&& IsSharedPtr<TProperty>::value), MetaClass const*>::type getMetaClassImpl() const
        {

            return GetMetaClass<typename TProperty::element_type>();
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!HasMetaClass<TProperty>::value && !IsSharedPtr<TProperty>::value), MetaClass const*>::type getMetaClassImpl() const
        {
            return nullptr;
        }


        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(HasMetaClass<TProperty>::value), MetaClass const*>::type getMetaClassImpl(MetaInstance const& _instance) const
        {
            TObject* obj = castMetaInstanceObject(_instance);
            TValue value = (obj->*m_getter)();
            return value.getMetaClass();
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!HasMetaClass<TProperty>::value && IsSharedPtr<TProperty>::value), MetaClass const*>::type getMetaClassImpl(MetaInstance const& _instance) const
        {
            TObject* obj = castMetaInstanceObject(_instance);
            auto ptr = (obj->*m_getter)();
            if (!ptr)
                return nullptr;

            return GetMetaClass(*ptr.get());
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!HasMetaClass<TProperty>::value && !IsSharedPtr<TProperty>::value), MetaClass const*>::type getMetaClassImpl(MetaInstance const& _instance) const
        {
            return nullptr;
        }


        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(IsSharedPtr<TProperty>::value), void*>::type getSharedPtrPointerImpl(MetaInstance const& _instance) const
        {
            TObject* obj = castMetaInstanceObject(_instance);
            return (obj->*m_getter)().get();
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!IsSharedPtr<TProperty>::value), void*>::type getSharedPtrPointerImpl(MetaInstance const& _instance) const
        {
            return nullptr;
        }


        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(IsVector<TVector>::value), Size>::type getVectorSizeImpl(MetaInstance const& _instance) const
        {
            TObject* obj = castMetaInstanceObject(_instance);
            TValue value = (obj->*m_getter)();
            return value.size();
        }

        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(!IsVector<TVector>::value), Size>::type getVectorSizeImpl(MetaInstance const& _instance) const
        {
            return 0;
        }

        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(IsVector<TVector>::value), void>::type setVectorSizeImpl(MetaInstance const& _instance, Size _size)
        {
            TObject* obj = castMetaInstanceObject(_instance);
            TValue value = (obj->*m_getter)();
            value.resize(_size);
            (obj->*m_setter)(value);
        }

        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(!IsVector<TVector>::value), void>::type setVectorSizeImpl(MetaInstance const& _instance, Size _size)
        { }

        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(IsVector<TVector>::value), ClassUID>::type getVectorElementClassUIDImpl(MetaInstance const& _instance) const
        {
            return ClassInfo<typename TVector::value_type>::UID();
        }

        //////////////////////////////////////////
        template <typename TVector>
        inline typename ::std::enable_if<(!IsVector<TVector>::value), ClassUID>::type getVectorElementClassUIDImpl(MetaInstance const& _instance) const
        {
            return 0;
        }


        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(StdHelper::HasOperatorEquals<TProperty>::value), bool>::type isEqualImpl(ConstMetaInstance const& _instance0, ConstMetaInstance const& _instance1) const
        {
            TObject const* obj0 = castMetaInstanceObject(_instance0);
            TObject const* obj1 = castMetaInstanceObject(_instance1);

            return (obj0->*m_getter)() == (obj1->*m_getter)();
        }

        //////////////////////////////////////////
        template <typename TProperty>
        inline typename ::std::enable_if<(!StdHelper::HasOperatorEquals<TProperty>::value), bool>::type isEqualImpl(ConstMetaInstance const& _instance0, ConstMetaInstance const& _instance1) const
        {
            return false;
        }
 
    private:
        Getter m_getter;
        Setter m_setter;
        TValue m_defaultValue;
    };


    //////////////////////////////////////////
    // Class GenericMetaPropertyCreator
    //
    //////////////////////////////////////////
    template <typename TObject>
    struct GenericMetaPropertyCreator
    {
        //////////////////////////////////////////
        template <typename TValue>
        static MetaProperty* Create(CString _name, TValue (TObject::*_getter)(void) const, void (TObject::*_setter)(TValue), TValue const& _defaultValue = TValue())
        {
            return new GenericMetaProperty<TObject, TValue, TValue, void>(_name, _getter, _setter, _defaultValue);
        }

        //////////////////////////////////////////
        template <typename TValue>
        static MetaProperty* Create(CString _name, TValue const& (TObject::*_getter)(void) const, void (TObject::*_setter)(TValue const&), TValue const& _defaultValue = TValue())
        {
            return new GenericMetaProperty<TObject, TValue, const TValue&, void>(_name, _getter, _setter, _defaultValue);
        }

        //////////////////////////////////////////
        template <typename TValue>
        static MetaProperty* Create(CString _name, TValue (TObject::*_getter)(void) const, bool (TObject::*_setter)(TValue), TValue const& _defaultValue = TValue())
        {
            return new GenericMetaProperty<TObject, TValue, TValue, bool>(_name, _getter, _setter, _defaultValue);
        }

        //////////////////////////////////////////
        template <typename TValue>
        static MetaProperty* Create(CString _name, TValue const& (TObject::*_getter)(void) const, bool (TObject::*_setter)(TValue const&), const TValue& _defaultValue = TValue())
        {
            return new GenericMetaProperty<TObject, TValue, TValue const&, bool>(_name, _getter, _setter, _defaultValue);
        }
    };


} // namespace Maze
//////////////////////////////////////////


////////////////////////////////////
#define MAZE_DECLARE_METACLASS_METHODS_BASE(DClassName)\
            public:\
            friend class Maze::GenericMetaClass<DClassName>;\
            static Maze::MetaClass* GetMetaClass();\
            virtual Maze::ClassUID getClassUID() const;\
            virtual Maze::CString getClassName() const;\
            virtual const Maze::MetaClass* getMetaClass() const;\
            virtual Maze::MetaClass* getMetaClass();\
            virtual Maze::MetaInstance getMetaInstance();\
            virtual Maze::ConstMetaInstance getMetaInstance(void) const;\
            template <typename TClass> \
            inline TClass* dynamicCastRaw() { return getMetaInstance().template dynamicObjectCast<TClass>(); }\
            template <typename TClass> \
            inline const TClass* dynamicCastRaw() const { return getMetaInstance().template dynamicObjectCast<TClass>(); }

////////////////////////////////////
#define MAZE_IMPLEMENT_METACLASS_METHODS_BASE(DExplicitSpecialization, DClassName, ...)\
            DExplicitSpecialization\
            Maze::MetaClass* DClassName::GetMetaClass()\
            {\
                using Class = DClassName;\
                static Class* unused = nullptr;\
                MAZE_UNUSED(unused);\
                static Maze::MetaProperty* m[] = { nullptr, __VA_ARGS__ };\
                static Maze::GenericMetaClass<Class> metaClass(Maze::ClassInfo<DClassName>::Name(), m + 1, sizeof(m) / sizeof(m[0]) - 1);\
                return static_cast<Maze::MetaClass*>(&metaClass);\
            }\
            DExplicitSpecialization\
            Maze::ClassUID DClassName::getClassUID() const { return Maze::ClassInfo<DClassName>::UID(); };\
            DExplicitSpecialization\
            Maze::CString DClassName::getClassName() const { return Maze::ClassInfo<DClassName>::Name(); };\
            DExplicitSpecialization\
            Maze::MetaClass const* DClassName::getMetaClass() const { return GetMetaClass(); }\
            DExplicitSpecialization\
            Maze::MetaClass* DClassName::getMetaClass() { return GetMetaClass(); }\
            DExplicitSpecialization\
            Maze::MetaInstance DClassName::getMetaInstance() { return Maze::MetaInstance(getMetaClass(), this); }\
            DExplicitSpecialization\
            Maze::ConstMetaInstance DClassName::getMetaInstance(void) const { return Maze::ConstMetaInstance(getMetaClass(), this); }

#define MAZE_IMPLEMENT_METACLASS_METHODS(DClassName, ...)\
    MAZE_IMPLEMENT_METACLASS_METHODS_BASE(, DClassName, __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_METHODS_TEMPLATE(DClassName, ...)\
    MAZE_IMPLEMENT_METACLASS_METHODS_BASE(MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName, __VA_ARGS__)






////////////////////////////////////
#define MAZE_DECLARE_METACLASS_WITH_PARENT_METHODS_BASE(DClassName)\
            public:\
            friend class Maze::GenericMetaClass<DClassName>;\
            static Maze::MetaClass* GetMetaClass();\
            virtual Maze::ClassUID getClassUID() const MAZE_OVERRIDE;\
            virtual Maze::CString getClassName() const MAZE_OVERRIDE;\
            virtual const Maze::MetaClass* getMetaClass() const MAZE_OVERRIDE;\
            virtual Maze::MetaClass* getMetaClass() MAZE_OVERRIDE;\
            virtual Maze::MetaInstance getMetaInstance() MAZE_OVERRIDE;\
            virtual Maze::ConstMetaInstance getMetaInstance(void) const MAZE_OVERRIDE;\
            template <typename TClass> \
            inline TClass* dynamicCastRaw() { return getMetaInstance().template dynamicObjectCast<TClass>(); }\
            template <typename TClass> \
            inline const TClass* dynamicCastRaw() const { return getMetaInstance().template dynamicObjectCast<TClass>(); }

////////////////////////////////////
#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_BASE(DExplicitSpecialization, DClassName, DSuperClasses, ...)\
            DExplicitSpecialization\
            Maze::MetaClass* DClassName::GetMetaClass()\
            {\
                using Class = DClassName;\
                static Class* unused = nullptr;\
                MAZE_UNUSED(unused);\
                static Maze::MetaProperty* m[] = { nullptr, __VA_ARGS__ };\
                static Maze::GenericMetaClass<Class> metaClass(Maze::ClassInfo<DClassName>::Name(),\
                DSuperClasses,\
                m + 1,\
                sizeof(m) / sizeof(m[0]) - 1);\
                return static_cast<Maze::MetaClass*>(&metaClass);\
            }\
            DExplicitSpecialization\
            Maze::ClassUID DClassName::getClassUID() const { return Maze::ClassInfo<DClassName>::UID(); };\
            DExplicitSpecialization\
            Maze::CString DClassName::getClassName() const { return Maze::ClassInfo<DClassName>::Name(); };\
            DExplicitSpecialization\
            Maze::MetaClass const* DClassName::getMetaClass() const { return GetMetaClass(); }\
            DExplicitSpecialization\
            Maze::MetaClass* DClassName::getMetaClass() { return GetMetaClass(); }\
            DExplicitSpecialization\
            Maze::MetaInstance DClassName::getMetaInstance() { return Maze::MetaInstance(getMetaClass(), this); }\
            DExplicitSpecialization\
            Maze::ConstMetaInstance DClassName::getMetaInstance(void) const { return Maze::ConstMetaInstance(getMetaClass(), this); }

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS(DClassName)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_BASE(, DClassName)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_TEMPLATE(DClassName)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_BASE(MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName)


//////////////////////////////////////////
// Metaclass without parent
//
//////////////////////////////////////////
#define MAZE_DECLARE_METACLASS(DClassName)\
    MAZE_DECLARE_METACLASS_METHODS_BASE(DClassName)

#define MAZE_IMPLEMENT_METACLASS_BASE(DExplicitSpecialization, DClassName, ...)\
    MAZE_IMPLEMENT_METACLASS_METHODS_BASE(DExplicitSpecialization, DClassName, __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS(DClassName, ...)\
    MAZE_IMPLEMENT_METACLASS_BASE(, DClassName, __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_TEMPLATE(DClassName, ...)\
    MAZE_IMPLEMENT_METACLASS_BASE(MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName, __VA_ARGS__)


//////////////////////////////////////////
// Metaclass with one parent
//
//////////////////////////////////////////
#define MAZE_DECLARE_METACLASS_WITH_PARENT(DClassName, DSuperClass0)\
    MAZE_DECLARE_METACLASS_WITH_PARENT_METHODS_BASE(DClassName)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT_BASE(DExplicitSpecialization, DClassName, DSuperClass0, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_BASE(\
        DExplicitSpecialization,\
        DClassName,\
        { (MAZE_MACRO_SPLIT_COMMA(Maze::GenericSuperMetaClassData<DSuperClass0, DClassName>(DSuperClass0 ::GetMetaClass()))) },\
        __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DClassName, DSuperClass0, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_BASE(\
        ,\
        DClassName,\
        DSuperClass0,\
        __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(DClassName, DSuperClass0, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_BASE(\
        MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName,\
        DSuperClass0,\
        __VA_ARGS__)


//////////////////////////////////////////
// Metaclass with two parents
//
//////////////////////////////////////////
#define MAZE_DECLARE_METACLASS_WITH_PARENTS_2(DClassName, DSuperClass0, DSuperClass1)\
    MAZE_DECLARE_METACLASS_WITH_PARENT_METHODS_BASE(DClassName)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENTS_2_BASE(DExplicitSpecialization, DClassName, DSuperClass0, DSuperClass1, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_METHODS_BASE(\
    DExplicitSpecialization,\
    DClassName,\
    (Maze::Vector<Maze::SuperMetaClassData>({ MAZE_MACRO_SPLIT_COMMA(\
        (MAZE_MACRO_SPLIT_COMMA(Maze::GenericSuperMetaClassData<DSuperClass0, DClassName>(DSuperClass0 ::GetMetaClass()))), \
        (MAZE_MACRO_SPLIT_COMMA(Maze::GenericSuperMetaClassData<DSuperClass1, DClassName>(DSuperClass1 ::GetMetaClass())))) })), \
    __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENTS_2(DClassName, DSuperClass0, DSuperClass1, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENTS_2_BASE(\
    ,\
    DClassName,\
    DSuperClass0,\
    DSuperClass1,\
    __VA_ARGS__)

#define MAZE_IMPLEMENT_METACLASS_WITH_PARENTS_2_TEMPLATE(DClassName, DSuperClass0, DSuperClass1, ...)\
    MAZE_IMPLEMENT_METACLASS_WITH_PARENTS_2_BASE(\
    MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName,\
    DSuperClass0,\
    DSuperClass1,\
    __VA_ARGS__)


//////////////////////////////////////////
#define MAZE_IMPLEMENT_METACLASS_PROPERTY(DPropertyType, DPropertyName, DDefaultValue, DGetter, DSetter)\
    Maze::GenericMetaPropertyCreator<Class>::template Create<DPropertyType>(#DPropertyName, &Class::DGetter, &Class::DSetter, DDefaultValue)


//////////////////////////////////////////
#endif // _MazeMetaClass_hpp_
