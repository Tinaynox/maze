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
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include <tinyxml2/tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetManager);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class AssetManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetManager
    {
    public:

        //////////////////////////////////////////
        virtual ~AssetManager();

        //////////////////////////////////////////
        static void Initialize(AssetManagerPtr& _AssetManager);

        
        //////////////////////////////////////////
        virtual String getDefaultAssetsDirectory() MAZE_ABSTRACT;
        
        
        //////////////////////////////////////////
        void addAssetsDirectory(String const& _path, bool _recursive = true);
        
        //////////////////////////////////////////
        void removeAssetsDirectory(String const& _path, bool _recursive = true);
        
        //////////////////////////////////////////
        String constructAssetsInfo();


        //////////////////////////////////////////
        const AssetFilePtr& getAssetFileByFileName(String const& _fileName);
        
        //////////////////////////////////////////
        const AssetFilePtr& getAssetFileByFullPath(String const& _fileFullPath);


        //////////////////////////////////////////
        static inline AssetManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline AssetManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        bool openXMLDocumentAssetFile(
            tinyxml2::XMLDocument& _doc,
            String const& _fileName,
            bool _warningIfNotExists);


        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtension(String const& _extension);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtension(String const& _extension, std::function<bool(AssetFilePtr const&)> _pred);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesWithExtensions(Set<String> const& _extensions);

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFiles(ClassUID _uid);

        //////////////////////////////////////////
        template <typename TAssetFile>
        inline Vector<AssetFilePtr> getAssetFiles()
        {
            return getAssetFiles(ClassInfo<TAssetFile>::UID());
        }

        //////////////////////////////////////////
        Vector<AssetFilePtr> getAssetFilesInFolder(String& _folderFullPath);

        //////////////////////////////////////////
        UnorderedMap<String, String> getMetaData(AssetFilePtr const& _assetFile);

    protected:

        //////////////////////////////////////////
        AssetManager();

        //////////////////////////////////////////
        virtual bool init();


        ////////////////////////////////////
        void addFilesInPath(
            String const& _path,
            bool _recursive = false,
            Vector<AssetFilePtr>* _addedFiles = nullptr);
        
        ////////////////////////////////////
        void processAddFile(AssetFilePtr const& _file);

        ////////////////////////////////////
        void processRemoveFile(AssetFilePtr const& _file);

        ////////////////////////////////////
        void updateFileInfo(AssetFilePtr const& _file);

    
    protected:
        static AssetManager* s_instance;
        
    protected:
        UnorderedMap<String, AssetFilePtr> m_assetFilesByFileName;
        UnorderedMap<String, AssetFilePtr> m_assetFilesByFullPath;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetManager_hpp_
//////////////////////////////////////////
