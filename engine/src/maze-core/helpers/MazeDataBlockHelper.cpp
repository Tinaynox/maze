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
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DataBlockHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void SerializeMetaPropertyToDataBlock(
            ConstMetaInstance _metaInstance,
            MetaProperty* _metaProperty,
            HashedCString _propertyName,
            DataBlock& _dataBlock)
        {
            ClassUID valueClassUID = _metaProperty->getValueClassUID();

            if (valueClassUID == ClassInfo<S32>::UID())
            {
                S32 value;
                _metaProperty->getValue<S32>(_metaInstance, value);
                _dataBlock.setS32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<S64>::UID())
            {
                S64 value;
                _metaProperty->getValue<S64>(_metaInstance, value);
                _dataBlock.setS64(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<U32>::UID())
            {
                U32 value;
                _metaProperty->getValue<U32>(_metaInstance, value);
                _dataBlock.setU32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<U64>::UID())
            {
                U64 value;
                _metaProperty->getValue<U64>(_metaInstance, value);
                _dataBlock.setU64(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<F32>::UID())
            {
                F32 value;
                _metaProperty->getValue<F32>(_metaInstance, value);
                _dataBlock.setF32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<F64>::UID())
            {
                F64 value;
                _metaProperty->getValue<F64>(_metaInstance, value);
                _dataBlock.setF64(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Bool>::UID())
            {
                Bool value;
                _metaProperty->getValue<Bool>(_metaInstance, value);
                _dataBlock.setBool(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec2S32>::UID())
            {
                Vec2S32 value;
                _metaProperty->getValue<Vec2S32>(_metaInstance, value);
                _dataBlock.setVec2S32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec3S32>::UID())
            {
                Vec3S32 value;
                _metaProperty->getValue<Vec3S32>(_metaInstance, value);
                _dataBlock.setVec3S32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec4S32>::UID())
            {
                Vec4S32 value;
                _metaProperty->getValue<Vec4S32>(_metaInstance, value);
                _dataBlock.setVec4S32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec2U32>::UID())
            {
                Vec2U32 value;
                _metaProperty->getValue<Vec2U32>(_metaInstance, value);
                _dataBlock.setVec2U32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec3U32>::UID())
            {
                Vec3U32 value;
                _metaProperty->getValue<Vec3U32>(_metaInstance, value);
                _dataBlock.setVec3U32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec4U32>::UID())
            {
                Vec4U32 value;
                _metaProperty->getValue<Vec4U32>(_metaInstance, value);
                _dataBlock.setVec4U32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec2F32>::UID())
            {
                Vec2F32 value;
                _metaProperty->getValue<Vec2F32>(_metaInstance, value);
                _dataBlock.setVec2F32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec3F32>::UID())
            {
                Vec3F32 value;
                _metaProperty->getValue<Vec3F32>(_metaInstance, value);
                _dataBlock.setVec3F32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec4F32>::UID())
            {
                Vec4F32 value;
                _metaProperty->getValue<Vec4F32>(_metaInstance, value);
                _dataBlock.setVec4F32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec2B>::UID())
            {
                Vec2B value;
                _metaProperty->getValue<Vec2B>(_metaInstance, value);
                _dataBlock.setVec2B(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec3B>::UID())
            {
                Vec3B value;
                _metaProperty->getValue<Vec3B>(_metaInstance, value);
                _dataBlock.setVec3B(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Vec4B>::UID())
            {
                Vec4B value;
                _metaProperty->getValue<Vec4B>(_metaInstance, value);
                _dataBlock.setVec4B(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Mat3F32>::UID())
            {
                Mat3F32 value;
                _metaProperty->getValue<Mat3F32>(_metaInstance, value);
                _dataBlock.setMat3F32(_propertyName, value);
            }
            else
            if (valueClassUID == ClassInfo<Mat4F32>::UID())
            {
                Mat4F32 value;
                _metaProperty->getValue<Mat4F32>(_metaInstance, value);
                _dataBlock.setMat4F32(_propertyName, value);
            }
            else
            if (_metaProperty->isDataBlockSerializable(_metaInstance))
            {
                DataBlock& childBlock = _dataBlock[_propertyName];
                _metaProperty->toDataBlock(_metaInstance, childBlock);
            }
            else
            {
                String properyStringValue;
                _metaProperty->toString(_metaInstance, properyStringValue);
                _dataBlock.setString(_propertyName, std::move(properyStringValue));
            }
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeMetaPropertyToDataBlock(
            ConstMetaInstance _metaInstance,
            MetaProperty* _metaProperty,
            DataBlock& _dataBlock)
        {
            return SerializeMetaPropertyToDataBlock(_metaInstance, _metaProperty, _metaProperty->getName(), _dataBlock);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeMetaInstanceToDataBlock(
            MetaClass const* _metaClass,
            ConstMetaInstance _metaInstance,
            DataBlock& _dataBlock)
        {
            for (MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
            {
                for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = metaClass->getProperty(i);
                    SerializeMetaPropertyToDataBlock(_metaInstance, metaProperty, _dataBlock);
                }
            }
        }


        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaPropertyFromDataBlock(
            MetaInstance _metaInstance,
            MetaProperty* _metaProperty,
            HashedCString _propertyName,
            DataBlock const& _dataBlock)
        {
            if (_dataBlock.isParamExists(_propertyName))
            {
                ClassUID valueClassUID = _metaProperty->getValueClassUID();
                if (valueClassUID == ClassInfo<S32>::UID())
                {
                    S32 value = _dataBlock.getS32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<S64>::UID())
                {
                    S64 value = _dataBlock.getS64(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<U32>::UID())
                {
                    U32 value = _dataBlock.getU32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<U64>::UID())
                {
                    U64 value = _dataBlock.getU64(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<F32>::UID())
                {
                    F32 value = _dataBlock.getF32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<F64>::UID())
                {
                    F64 value = _dataBlock.getF64(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Bool>::UID())
                {
                    Bool value = _dataBlock.getBool(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec2S32>::UID())
                {
                    Vec2S32 value = _dataBlock.getVec2S32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec3S32>::UID())
                {
                    Vec3S32 value = _dataBlock.getVec3S32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec4S32>::UID())
                {
                    Vec4S32 value = _dataBlock.getVec4S32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec2U32>::UID())
                {
                    Vec2U32 value = _dataBlock.getVec2U32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec3U32>::UID())
                {
                    Vec3U32 value = _dataBlock.getVec3U32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec4U32>::UID())
                {
                    Vec4U32 value = _dataBlock.getVec4U32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec2F32>::UID())
                {
                    Vec2F32 value = _dataBlock.getVec2F32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec3F32>::UID())
                {
                    Vec3F32 value = _dataBlock.getVec3F32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec4F32>::UID())
                {
                    Vec4F32 value = _dataBlock.getVec4F32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec2B>::UID())
                {
                    Vec2B value = _dataBlock.getVec2B(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec3B>::UID())
                {
                    Vec3B value = _dataBlock.getVec3B(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Vec4B>::UID())
                {
                    Vec4B value = _dataBlock.getVec4B(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Mat3F32>::UID())
                {
                    Mat3F32 value = _dataBlock.getMat3F32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                if (valueClassUID == ClassInfo<Mat4F32>::UID())
                {
                    Mat4F32 value = _dataBlock.getMat4F32(_propertyName);
                    _metaProperty->setValue(_metaInstance, &value);
                }
                else
                {
                    String const& attributeValue = _dataBlock.getString(_propertyName);
                    _metaProperty->setString(_metaInstance, attributeValue);
                }
            }
            else
            if (DataBlock const* childBlock = _dataBlock.getDataBlock(_propertyName))
            {
                _metaProperty->setDataBlock(_metaInstance, *childBlock);
            }
        }

        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaPropertyFromDataBlock(
            MetaInstance _metaInstance,
            MetaProperty* _metaProperty,
            DataBlock const& _dataBlock)
        {
            return DeserializeMetaPropertyFromDataBlock(_metaInstance, _metaProperty, _metaProperty->getName(), _dataBlock);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaInstanceFromDataBlock(
            MetaClass const* _metaClass,
            MetaInstance _metaInstance,
            DataBlock const& _dataBlock)
        {
            for (MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
            {
                for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = metaClass->getProperty(i);
                    DeserializeMetaPropertyFromDataBlock(_metaInstance, metaProperty, _dataBlock);
                }
            }
        }

    } // namespace DataBlockHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
