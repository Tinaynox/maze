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
#include "maze-core/system/MazeFileStats.hpp"
#include "maze-core/system/MazePath.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/assets/MazeAssetFileId.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include <tinyxml2/tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(AssetUnit);


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
        void setAssetFileId(AssetFileId _id);

        //////////////////////////////////////////
        inline AssetFileId getAssetFileId() const { return m_assetFileId; }


        //////////////////////////////////////////
        virtual Path const& getFullPath() const MAZE_ABSTRACT;
        
        //////////////////////////////////////////
        virtual Path const& getFileName() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Size getFileSize() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual FileStats getFileStats() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isFileExists() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _renamedFiles) MAZE_ABSTRACT;
        

        //////////////////////////////////////////
        virtual UnorderedMap<Path, AssetFilePtr> const* getChildrenAssets() const;


        //////////////////////////////////////////
        virtual void saveInfoToMetaData(DataBlock& _metaData) const;

        //////////////////////////////////////////
        virtual void saveInfoToMetaData();

        //////////////////////////////////////////
        virtual void loadInfoFromMetaData(DataBlock const& _metaData);

        //////////////////////////////////////////
        virtual bool loadInfoFromMetaData();


        //////////////////////////////////////////
        void updateAssetUnitsFromMetaData(DataBlock const& _metaData);

        //////////////////////////////////////////
        void updateAssetUnitsFromMetaData();

        //////////////////////////////////////////
        template <typename TAssetUnit>
        inline SharedPtr<TAssetUnit> getAssetUnit() const
        {
            for (AssetUnitPtr const& assetUnit : m_assetUnits)
                if (assetUnit->getClassUID() == ClassInfo<TAssetUnit>::UID())
                    return assetUnit->cast<TAssetUnit>();

            return nullptr;
        }

        //////////////////////////////////////////
        void addAssetUnit(AssetUnitPtr const& _assetUnit);


        //////////////////////////////////////////
        virtual bool updateChildrenAssets(
            bool _recursive,
            Vector<AssetFilePtr>* _addedFiles,
            Vector<AssetFilePtr>* _removedFiles)
        {
            return true;
        };

        //////////////////////////////////////////
        void getChildrenAssets(
            Vector<AssetFilePtr>& _outResult,
            ClassUID _classUID,
            bool _recursive = true) const;

        //////////////////////////////////////////
        void getChildrenAssets(
            Vector<AssetFilePtr>& _outResult,
            bool _recursive = true) const;

        //////////////////////////////////////////
        template <typename TAssetFile>
        inline Vector<AssetFilePtr> getChildrenAssets(bool _recursive = true) const
        {
            Vector<AssetFilePtr> result;
            getChildrenAssets(result, ClassInfo<TAssetFile>::UID(), _recursive);
            return result;
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getChildrenAssets(bool _recursive) const
        {
            Vector<AssetFilePtr> result;
            getChildrenAssets(result, _recursive);
            return result;
        }

        //////////////////////////////////////////
        virtual Size readToString(String& _string) const { return 0; }
        
        //////////////////////////////////////////
        virtual String readAsString() const;

        //////////////////////////////////////////
        virtual bool readToXMLDocument(tinyxml2::XMLDocument& _doc) const { return false; }

        //////////////////////////////////////////
        virtual bool readToByteBuffer(ByteBuffer& _byteBuffer) const { MAZE_NOT_IMPLEMENTED; return 0; }

        //////////////////////////////////////////
        virtual bool readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) const { MAZE_NOT_IMPLEMENTED; return 0; }


        //////////////////////////////////////////
        bool readToByteBuffer(ByteBufferPtr const& _byteBuffer) const;

        //////////////////////////////////////////
        bool readHeaderToByteBuffer(ByteBufferPtr const& _byteBuffer, Size _size) const;

        //////////////////////////////////////////
        ByteBufferPtr readAsByteBuffer() const;

        //////////////////////////////////////////
        ByteBufferPtr readHeaderAsByteBuffer(Size _size) const;

        //////////////////////////////////////////
        Path getExtension() const;


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

    public:
        MultiDelegate<AssetFile*, AssetFileId, AssetFileId> eventAssetFileIdChanged;

    protected:

        //////////////////////////////////////////
        AssetFile();

        //////////////////////////////////////////
        virtual bool init();


        //////////////////////////////////////////
        void removeAssetUnits();
    
    protected:
        AssetFileId m_assetFileId = 0u;
        Set<String> m_tags;

        Vector<AssetUnitPtr> m_assetUnits;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetFile_hpp_
//////////////////////////////////////////
