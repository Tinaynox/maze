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
#include "maze-core/managers/android/MazeAssetManagerAndroid.hpp"
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/helpers/android/MazeWindowHelperAndroid.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/android/MazeFileHelperAndroid.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AssetManagerAndroid
    //
    //////////////////////////////////////////
    AssetManagerAndroid::AssetManagerAndroid()
    {
    }
    
    //////////////////////////////////////////
    AssetManagerAndroid::~AssetManagerAndroid()
    {
    }
    
    //////////////////////////////////////////
    bool AssetManagerAndroid::init(DataBlock const& _config)
    {
        if (!AssetManager::init(_config))
            return false;

        if (_config.getBool(MAZE_HS("extractAssetsFromAPK"), true))
            extractAssetsFromAPK();
        
        return true;
    }
    
    //////////////////////////////////////////
    Path AssetManagerAndroid::getDefaultAssetsDirectory()
    {
        return FileHelper::GetDocumentsDirectory();
    }
    
    //////////////////////////////////////////
    Path AssetManagerAndroid::getAssetsHashFullPath() const
    {
        return FileHelper::GetUnpackedAssetsDirectory() + "/assets.hash";
    }

    //////////////////////////////////////////
    String AssetManagerAndroid::readSavedAssetsHash()
    {
        Path const assetsHashFullPath = getAssetsHashFullPath();
        String unpackedAssetsHash;
        std::ifstream ifs(assetsHashFullPath.c_str());
        unpackedAssetsHash.assign((std::istreambuf_iterator<S8>(ifs)),
                                  (std::istreambuf_iterator<S8>()));
        ifs.close();
        
        return std::move(unpackedAssetsHash);
    }

    //////////////////////////////////////////
    String AssetManagerAndroid::calculateCurrentAssetsHash()
    {
        SystemManagerAndroid* systemManagerAndroid = SystemManager::GetInstancePtr()->castRaw<SystemManagerAndroid>();
        return systemManagerAndroid->callActivityMethodString("getAssetsHash");
    }
    
    //////////////////////////////////////////
    Vector<String> AssetManagerAndroid::getAssetsNames()
    {
        SystemManagerAndroid* systemManagerAndroid = SystemManager::GetInstancePtr()->castRaw<SystemManagerAndroid>();
        return systemManagerAndroid->callActivityMethodStringVector("getAssetsNames");
    }
        
    //////////////////////////////////////////
    void AssetManagerAndroid::extractAssetsFromAPK()
    {
        Debug::log << "extractAssetsFromAPK started..." << endl;

        SystemManagerAndroid* systemManagerAndroid = SystemManager::GetInstancePtr()->castRaw<SystemManagerAndroid>();

        String unpackedAssetsHash = readSavedAssetsHash();
        String apkAssetsHash = calculateCurrentAssetsHash();
        
        Debug::log << "Assets hash: " << apkAssetsHash << endl;

        if  (unpackedAssetsHash != apkAssetsHash)
        {
            Debug::log << "Extracting assets from APK..." << endl;

            FileHelper::DeleteDirectory(FileHelper::GetUnpackedAssetsDirectory().c_str());

            Vector<String> assetsFiles = getAssetsNames();
            if (assetsFiles.empty())
            {
                Debug::log << "Assets is empty!" << endl;
            }
            else
            {
                Debug::log << "Extracting " << assetsFiles.size() << " assets..." << endl;
                
                Size i = 0;
                for (String const& assetFilePath : assetsFiles)
                {
                    extractFileFromAPK(assetFilePath, true);
                    
                    ++i;
                    Debug::log << "[" << S32(100.0f * (F32)i / (Size)assetsFiles.size()) << "%] => " << assetFilePath << endl;
                }

                Debug::log << "All assets extracted." << endl;
            }

            saveAssetsHash(apkAssetsHash);
        }
        else
        {
            Debug::log << "Assets is up to date." << endl;
        }
        
        addAssetsDirectory(FileHelper::GetUnpackedAssetsDirectory(), true);
    }
    
    //////////////////////////////////////////
    void AssetManagerAndroid::saveAssetsHash(String const& _hash)
    {
        Path const assetsHashFullPath = getAssetsHashFullPath();
        std::ofstream ofs(assetsHashFullPath.c_str());
        ofs << _hash;
        ofs.close();
    }
        

    //////////////////////////////////////////
    bool AssetManagerAndroid::extractFileFromAPK(Path const& _assetFilePath, bool _errorNoExists)
    {
        char buffer[BUFSIZ];

        Path fullPath = FileHelper::GetUnpackedAssetsDirectory() + "/" + _assetFilePath;
        Path fullPathDir = FileHelper::GetDirectoryInPath(fullPath);
        FileHelper::CreateDirectoryRecursive(fullPathDir);

        SystemManagerAndroid* systemManagerAndroid = SystemManager::GetInstancePtr()->castRaw<SystemManagerAndroid>();
        AAssetManager* assetManager = systemManagerAndroid->getAssetManager();

        AAsset* inputFile = AAssetManager_open(assetManager, _assetFilePath.c_str(), AASSET_MODE_BUFFER);

        if (!inputFile)
        {
            MAZE_ERROR_IF(_errorNoExists, "Failed to extract file '%s' from APK", _assetFilePath.c_str());
            return false;
        }

        FILE* outputFile = fopen(fullPath.c_str(), "wb");
        if (!outputFile)
        {
            AAsset_close(inputFile);
            MAZE_ERROR("Failed to open file '%s' for writing", fullPath.c_str());
            return false;
        }

        int count, totalCount = 0;
        while (((count = AAsset_read(inputFile, buffer, BUFSIZ)) > 0))
        {
            fwrite(buffer, 1, count, outputFile);
            totalCount += count;
        }

        AAsset_close(inputFile);
        fclose(outputFile);

        return true;
    }
   
    
} // namespace Maze
//////////////////////////////////////////
