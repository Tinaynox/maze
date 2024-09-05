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
#if (!defined(_MazeAssetManager_hpp_))
#define _MazeAssetManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include <tinyxml2/tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetManager);
    MAZE_USING_SHARED_PTR(AssetUnitManager);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    using FileChildrenProcessor = std::function<AssetFilePtr(Path const, Vector<AssetFilePtr>*, Vector<AssetFilePtr>*)>;


    //////////////////////////////////////////
    // Class AssetManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetManager
        : public SharedObject<AssetManager>
    {
    public:

        //////////////////////////////////////////
        virtual ~AssetManager();

        //////////////////////////////////////////
        static void Initialize(AssetManagerPtr& _AssetManager, DataBlock const& _config = DataBlock::c_empty);

        
        //////////////////////////////////////////
        virtual Path getDefaultAssetsDirectory() MAZE_ABSTRACT;
        
        
        //////////////////////////////////////////
        bool addAssetsDirectoryPath(Path const& _path);

        //////////////////////////////////////////
        void removeAssetsDirectoryPath(Path const& _path);

        //////////////////////////////////////////
        void updateAssets();

        //////////////////////////////////////////
        String constructAssetsInfo();


        //////////////////////////////////////////
        const AssetFilePtr& getAssetFileByFileName(Path const& _fileName);

        //////////////////////////////////////////
        inline const AssetFilePtr& getAssetFileByFileName(HashedCString _fileName)
        {
            return getAssetFileByFileName(Path(_fileName.str));
        }

        //////////////////////////////////////////
        const AssetFilePtr& getAssetFileByFullPath(Path const& _fileFullPath);

        //////////////////////////////////////////
        const AssetFilePtr& getAssetFile(Path const& _string);

        //////////////////////////////////////////
        inline const AssetFilePtr& getAssetFile(HashedCString _string)
        {
            return getAssetFile(Path(_string.str));
        }

        //////////////////////////////////////////
        void deleteAssetFile(const AssetFilePtr& _assetFile);

        //////////////////////////////////////////
        void moveAssetFile(const AssetFilePtr& _assetFile, Path const& _newFullPath);


        //////////////////////////////////////////
        static inline AssetManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline AssetManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        bool openXMLDocumentAssetFile(
            tinyxml2::XMLDocument& _doc,
            Path const& _fileName,
            bool _warningIfNotExists);


        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtension(Path const& _extension);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtension(Path const& _extension, std::function<bool(AssetFilePtr const&)> _pred);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<Path> const& _extensions);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<Path> const& _extensions, std::function<bool(AssetFilePtr const&)> _pred);

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtension(CString const& _extension)
        {
            return getAssetFilesWithExtension(Path(_extension));
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtension(CString const& _extension, std::function<bool(AssetFilePtr const&)> _pred)
        {
            return getAssetFilesWithExtension(Path(_extension), _pred);
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<CString> const& _extensions)
        {
            Set<Path> extensions;
            for (CString const& ext : _extensions)
                extensions.insert(ext);
            return getAssetFilesWithExtensions(extensions);
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<CString> const& _extensions, std::function<bool(AssetFilePtr const&)> _pred)
        {
            Set<Path> extensions;
            for (CString const& ext : _extensions)
                extensions.insert(ext);
            return getAssetFilesWithExtensions(extensions, _pred);
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtension(String const& _extension)
        {
            return getAssetFilesWithExtension(Path(_extension));
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtension(String const& _extension, std::function<bool(AssetFilePtr const&)> _pred)
        {
            return getAssetFilesWithExtension(Path(_extension), _pred);
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<String> const& _extensions)
        {
            Set<Path> extensions;
            for (String const& ext : _extensions)
                extensions.insert(ext);
            return getAssetFilesWithExtensions(extensions);
        }

        //////////////////////////////////////////
        inline Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<String> const& _extensions, std::function<bool(AssetFilePtr const&)> _pred)
        {
            Set<Path> extensions;
            for (String const& ext : _extensions)
                extensions.insert(ext);
            return getAssetFilesWithExtensions(extensions, _pred);
        }

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFiles(ClassUID _uid);

        //////////////////////////////////////////
        template <typename TAssetFile>
        inline Vector<AssetFilePtr> getAssetFiles()
        {
            return getAssetFiles(ClassInfo<TAssetFile>::UID());
        }

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesInFolder(Path const& _folderFullPath);

        //////////////////////////////////////////
        Path getMetaDataFullPath(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        AssetFilePtr getMetaDataFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool getMetaData(AssetFilePtr const& _assetFile, DataBlock& _metaData);

        //////////////////////////////////////////
        void saveMetaData(AssetFilePtr const& _assetFile, DataBlock const& _metaData);


        //////////////////////////////////////////
        inline void registerFileChildrenProcessor(String const& _extension, FileChildrenProcessor const& _processor)
        {
            m_fileChildrenProcessors.insert(_extension, _processor);
        }

        //////////////////////////////////////////
        inline void clearFileChildrenProcessor(String const& _extension)
        {
            m_fileChildrenProcessors.erase(_extension);
        }

        //////////////////////////////////////////
        inline FileChildrenProcessor getFileChildrenProcessor(String const& _extension) const
        {
            auto it = m_fileChildrenProcessors.find(_extension);
            if (it != m_fileChildrenProcessors.end())
                return it->second;

            return FileChildrenProcessor();
        }

        //////////////////////////////////////////
        inline Set<Path> const& getAssetDirectoryPathes() const { return m_assetDirectoryPathes; }

    public:
        MultiDelegate<Path const&> eventAssetsDirectoryPathAdded;
        MultiDelegate<Path const&> eventAssetsDirectoryPathRemoved;
        MultiDelegate<AssetFilePtr const&> eventAssetFileAdded;
        MultiDelegate<AssetFilePtr const&> eventAssetFileRemoved;
        MultiDelegate<AssetFilePtr const&, Path const&> eventAssetFileMoved;

    protected:

        //////////////////////////////////////////
        AssetManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);


        //////////////////////////////////////////
        void addFilesInPath(
            Path const& _path,
            bool _recursive = false,
            Vector<AssetFilePtr>* _addedFiles = nullptr);
        
        //////////////////////////////////////////
        void processAddFile(AssetFilePtr const& _file);

        //////////////////////////////////////////
        void processRemoveFile(AssetFilePtr const& _file);

        //////////////////////////////////////////
        void updateFileInfo(AssetFilePtr const& _file);


        //////////////////////////////////////////
        bool addAssetsDirectory(Path const& _path, bool _recursive = true);

        //////////////////////////////////////////
        void removeAssetsDirectory(Path const& _path, bool _recursive = true);
    
        //////////////////////////////////////////
        Set<Path> collectRootAssetDirectoryPathes();

    protected:
        static AssetManager* s_instance;
        
    protected:
        AssetUnitManagerPtr m_assetUnitManager;

        Set<Path> m_assetDirectoryPathes;

        UnorderedMap<Path, AssetFilePtr> m_assetFilesByFileName;
        UnorderedMap<Path, AssetFilePtr> m_assetFilesByFullPath;
        Map<AssetFilePtr, UnixTime> m_assetFilesUpdateTimeUTC;

        StringKeyMap<FileChildrenProcessor> m_fileChildrenProcessors;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetManager_hpp_
//////////////////////////////////////////
