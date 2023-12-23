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
        MAZE_CORE_API void SerializeMetaInstanceToDataBlock(
            MetaClass const* _metaClass,
            ConstMetaInstance _metaInstance,
            DataBlock& _dataBlock)
        {
            HashedCString className = _metaClass->getName();

            for (MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
            {
                for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = metaClass->getProperty(i);

                    HashedCString propertyName = metaProperty->getName();

                    ClassUID valueClassUID = metaProperty->getValueClassUID();

                    if (valueClassUID == ClassInfo<S32>::UID())
                    {
                        S32 value;
                        metaProperty->getValue<S32>(_metaInstance, value);
                        _dataBlock.setS32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<S64>::UID())
                    {
                        S64 value;
                        metaProperty->getValue<S64>(_metaInstance, value);
                        _dataBlock.setS64(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<U32>::UID())
                    {
                        U32 value;
                        metaProperty->getValue<U32>(_metaInstance, value);
                        _dataBlock.setU32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<U64>::UID())
                    {
                        U64 value;
                        metaProperty->getValue<U64>(_metaInstance, value);
                        _dataBlock.setU64(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<F32>::UID())
                    {
                        F32 value;
                        metaProperty->getValue<F32>(_metaInstance, value);
                        _dataBlock.setF32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<F64>::UID())
                    {
                        F64 value;
                        metaProperty->getValue<F64>(_metaInstance, value);
                        _dataBlock.setF64(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Bool>::UID())
                    {
                        Bool value;
                        metaProperty->getValue<Bool>(_metaInstance, value);
                        _dataBlock.setBool(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec2S32>::UID())
                    {
                        Vec2S32 value;
                        metaProperty->getValue<Vec2S32>(_metaInstance, value);
                        _dataBlock.setVec2S32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec3S32>::UID())
                    {
                        Vec3S32 value;
                        metaProperty->getValue<Vec3S32>(_metaInstance, value);
                        _dataBlock.setVec3S32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec4S32>::UID())
                    {
                        Vec4S32 value;
                        metaProperty->getValue<Vec4S32>(_metaInstance, value);
                        _dataBlock.setVec4S32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec2U32>::UID())
                    {
                        Vec2U32 value;
                        metaProperty->getValue<Vec2U32>(_metaInstance, value);
                        _dataBlock.setVec2U32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec3U32>::UID())
                    {
                        Vec3U32 value;
                        metaProperty->getValue<Vec3U32>(_metaInstance, value);
                        _dataBlock.setVec3U32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec4U32>::UID())
                    {
                        Vec4U32 value;
                        metaProperty->getValue<Vec4U32>(_metaInstance, value);
                        _dataBlock.setVec4U32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec2F32>::UID())
                    {
                        Vec2F32 value;
                        metaProperty->getValue<Vec2F32>(_metaInstance, value);
                        _dataBlock.setVec2F32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec3F32>::UID())
                    {
                        Vec3F32 value;
                        metaProperty->getValue<Vec3F32>(_metaInstance, value);
                        _dataBlock.setVec3F32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec4F32>::UID())
                    {
                        Vec4F32 value;
                        metaProperty->getValue<Vec4F32>(_metaInstance, value);
                        _dataBlock.setVec4F32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec2B>::UID())
                    {
                        Vec2B value;
                        metaProperty->getValue<Vec2B>(_metaInstance, value);
                        _dataBlock.setVec2B(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec3B>::UID())
                    {
                        Vec3B value;
                        metaProperty->getValue<Vec3B>(_metaInstance, value);
                        _dataBlock.setVec3B(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Vec4B>::UID())
                    {
                        Vec4B value;
                        metaProperty->getValue<Vec4B>(_metaInstance, value);
                        _dataBlock.setVec4B(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Mat3F32>::UID())
                    {
                        Mat3F32 value;
                        metaProperty->getValue<Mat3F32>(_metaInstance, value);
                        _dataBlock.setMat3F32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<Mat4F32>::UID())
                    {
                        Mat4F32 value;
                        metaProperty->getValue<Mat4F32>(_metaInstance, value);
                        _dataBlock.setMat4F32(propertyName, value);
                    }
                    else
                    if (valueClassUID == ClassInfo<String>::UID() || valueClassUID == ClassInfo<CString>::UID() ||
                        valueClassUID == ClassInfo<HashedCString>::UID() || valueClassUID == ClassInfo<HashedString>::UID())
                    {
                        String properyStringValue = metaProperty->toString(_metaInstance);
                        _dataBlock.setString(propertyName, std::move(properyStringValue));
                    }
                    else
                    {
                        DataBlock& childBlock = _dataBlock[propertyName];
                        metaProperty->toDataBlock(_metaInstance, childBlock);
                    }
                }
            }
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

                    HashedCString propertyName = metaProperty->getName();

                    if (_dataBlock.isParamExists(propertyName))
                    {
                        ClassUID valueClassUID = metaProperty->getValueClassUID();
                        if (valueClassUID == ClassInfo<S32>::UID())
                        {
                            S32 value = _dataBlock.getS32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<S64>::UID())
                        {
                            S64 value = _dataBlock.getS64(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<U32>::UID())
                        {
                            U32 value = _dataBlock.getU32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<U64>::UID())
                        {
                            U64 value = _dataBlock.getU64(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<F32>::UID())
                        {
                            F32 value = _dataBlock.getF32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<F64>::UID())
                        {
                            F64 value = _dataBlock.getF64(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Bool>::UID())
                        {
                            Bool value = _dataBlock.getBool(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec2S32>::UID())
                        {
                            Vec2S32 value = _dataBlock.getVec2S32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec3S32>::UID())
                        {
                            Vec3S32 value = _dataBlock.getVec3S32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec4S32>::UID())
                        {
                            Vec4S32 value = _dataBlock.getVec4S32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec2U32>::UID())
                        {
                            Vec2U32 value = _dataBlock.getVec2U32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec3U32>::UID())
                        {
                            Vec3U32 value = _dataBlock.getVec3U32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec4U32>::UID())
                        {
                            Vec4U32 value = _dataBlock.getVec4U32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec2F32>::UID())
                        {
                            Vec2F32 value = _dataBlock.getVec2F32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec3F32>::UID())
                        {
                            Vec3F32 value = _dataBlock.getVec3F32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec4F32>::UID())
                        {
                            Vec4F32 value = _dataBlock.getVec4F32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec2B>::UID())
                        {
                            Vec2B value = _dataBlock.getVec2B(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec3B>::UID())
                        {
                            Vec3B value = _dataBlock.getVec3B(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Vec4B>::UID())
                        {
                            Vec4B value = _dataBlock.getVec4B(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Mat3F32>::UID())
                        {
                            Mat3F32 value = _dataBlock.getMat3F32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<Mat4F32>::UID())
                        {
                            Mat4F32 value = _dataBlock.getMat4F32(propertyName);
                            metaProperty->setValue(_metaInstance, &value);
                        }
                        else
                        if (valueClassUID == ClassInfo<String>::UID() || valueClassUID == ClassInfo<CString>::UID() ||
                            valueClassUID == ClassInfo<HashedCString>::UID() || valueClassUID == ClassInfo<HashedString>::UID())
                        {
                            String const& attributeValue = _dataBlock.getString(propertyName);
                            metaProperty->setString(_metaInstance, attributeValue);
                        }
                        else
                        {
                            MAZE_ERROR("Unknown class %s", static_cast<CString>(ClassInfo<String>::Name()));
                        }
                    }
                    else
                    if (DataBlock const* childBlock = _dataBlock.getDataBlock(propertyName))
                    {
                        metaProperty->setDataBlock(_metaInstance, *childBlock);
                    }
                }
            }
        }

    } // namespace DataBlockHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
