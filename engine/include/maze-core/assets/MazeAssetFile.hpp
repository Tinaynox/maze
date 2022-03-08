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
#if (!defined(_MazeAssetFile_hpp_))
#define _MazeAssetFile_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/MazeObject.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetFile);   


    //////////////////////////////////////////
    // Class AssetFile
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetFile
        : public SharedObject<AssetFile>
        , public Object
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetFile, Object);
        
    public:

        //////////////////////////////////////////
        virtual ~AssetFile();


        //////////////////////////////////////////
        virtual String const& getFullPath() const MAZE_ABSTRACT;
        
        //////////////////////////////////////////
        virtual String const& getFileName() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Size getFileSize() MAZE_ABSTRACT;
        

        //////////////////////////////////////////
        virtual UnorderedMap<String, AssetFilePtr> const* getChildrenAssets() const;

        //////////////////////////////////////////
        virtual Size readToString(String& _string) { return 0; }
        
        //////////////////////////////////////////
        virtual String readAsString();

        //////////////////////////////////////////
        virtual bool readToXMLDocument(tinyxml2::XMLDocument& _doc) { return false; }

        //////////////////////////////////////////
        virtual bool readToByteBuffer(ByteBuffer& _byteBuffer) { MAZE_NOT_IMPLEMENTED; return 0; }

        //////////////////////////////////////////
        virtual bool readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) { MAZE_NOT_IMPLEMENTED; return 0; }


        //////////////////////////////////////////
        bool readToByteBuffer(ByteBufferPtr const& _byteBuffer);

        //////////////////////////////////////////
        bool readHeaderToByteBuffer(ByteBufferPtr const& _byteBuffer, Size _size);

        //////////////////////////////////////////
        ByteBufferPtr readAsByteBuffer();

        //////////////////////////////////////////
        ByteBufferPtr readHeaderAsByteBuffer(Size _size);

        //////////////////////////////////////////
        String getExtension() const;


        //////////////////////////////////////////
        inline void setTags(Set<String> const& _tags) { m_tags = _tags; }

        //////////////////////////////////////////
        inline Set<String> const& getTags() const { return m_tags; }

        //////////////////////////////////////////
        inline bool hasTag(String const& _tag) { return m_tags.find(_tag) != m_tags.end(); }

        //////////////////////////////////////////
        inline bool hasAnyOfTags(Set<String> const& _tags)
        {
            for (auto it = m_tags.begin(), end = m_tags.end(); it != end; ++it)
                return _tags.find(*it) != _tags.end();

            return false;
        }

    protected:

        //////////////////////////////////////////
        AssetFile();

        //////////////////////////////////////////
        virtual bool init();

    
    protected:
        Set<String> m_tags;
    };


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<AssetFilePtr>::value), void>::type
        ValueToString(AssetFilePtr const& _value, String& _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<AssetFilePtr>::value), void>::type
        ValueFromString(AssetFilePtr& _value, CString _data, Size _count)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<AssetFilePtr>::value), U32>::type
        GetValueSerializationSize(AssetFilePtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<AssetFilePtr>::value), void>::type
        SerializeValue(AssetFilePtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<AssetFilePtr>::value), void>::type
        DeserializeValue(AssetFilePtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetFile_hpp_
//////////////////////////////////////////
