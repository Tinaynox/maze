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
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MetaClassHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void CopyProperties(void* _destObject, MetaClass const* _metaClass, void const* _srcObject)
        {
            for (S32 i = 0, c = _metaClass->getPropertiesCount(); i < c; ++i)
            {
                MetaProperty const* metaProperty = _metaClass->getProperty(i);
                metaProperty->copy(_destObject, _srcObject);
            }

            for (SuperMetaClassData const& superMetaClassData : _metaClass->getSuperMetaClassesData())
            {
                CopyProperties(
                    superMetaClassData.currentToSuperPointerCast(_destObject),
                    superMetaClassData.metaClass,
                    superMetaClassData.currentToSuperPointerCast(const_cast<void*>(_srcObject)));
            }
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyProperties(MetaInstance _dstMetaInstance, ConstMetaInstance _srcMetaInstance)
        {
            MetaClass const* srcMetaClass = _srcMetaInstance.getMetaClass();

#if (MAZE_DEBUG)
            MetaClass const* dstMetaClass = _dstMetaInstance.getMetaClass();
            if (!dstMetaClass->isInheritedFrom(srcMetaClass))
            {
                MAZE_ERROR_RETURN_VALUE( 
                    false,
                    "Trying initialize class '%s' with class '%s'", 
                    dstMetaClass->getName(),
                    srcMetaClass->getName());
            }
#endif

            Vector<MetaClass*> const& srcSuperMetaClasses = srcMetaClass->getAllSuperMetaClasses();
            
            for (MetaClass* srcSuperMetaClass : srcSuperMetaClasses)
            {
                for (S32 i = 0; i < srcSuperMetaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = srcSuperMetaClass->getProperty(i);
                    metaProperty->copy(_dstMetaInstance, _srcMetaInstance);
                }
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsEqual(ConstMetaInstance _metaInstance0, ConstMetaInstance _metaInstance1)
        {
            MetaClass const* metaClass = _metaInstance0.getMetaClass();
            if (metaClass != _metaInstance1.getMetaClass())
                return false;

            Vector<MetaClass*> const& superMetaClasses = metaClass->getAllSuperMetaClasses();

            for (MetaClass* superMetaClass : superMetaClasses)
            {
                for (S32 i = 0; i < superMetaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = superMetaClass->getProperty(i);
                    if (!metaProperty->isEqual(_metaInstance0, _metaInstance1))
                        return false;
                }
            }

            return true;
        }

    } // namespace MetaClassHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
