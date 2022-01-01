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
#include "maze-core/assets/MazeAssetObfuscation.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeAssetHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"

#define INCLUDECRYPTINGCODE_IFCRYPTALLOWED
#include <mz.h>
#include <mz_compat.h>
#include <mz_crypt.h>


//////////////////////////////////////////
namespace Maze
{ 

    ////////////////////////////////////
    ObfuscationPasswordFunction g_obfuscationPasswordFunction = nullptr;

    ////////////////////////////////////
    MAZE_CORE_API void SetObfuscationPasswordFunction(ObfuscationPasswordFunction _func)
    {
        g_obfuscationPasswordFunction = _func;
    }

    ////////////////////////////////////
    MAZE_CORE_API String CallObfuscationPasswordFunction(String const& _fileName)
    {
        return g_obfuscationPasswordFunction(_fileName);
    }

    //////////////////////////////////////////
    inline void StartMinizipObfuscation()
    {
        /*
        zipFile zf = zipOpen("minizip-dummy", APPEND_STATUS_CREATE);

        if (ZIP_OK == zipOpenNewFileInZip3(
            zf,
            "dummy-file",
            nullptr,
            nullptr,
            0,
            nullptr,
            0,
            nullptr,
            Z_DEFLATED,
            MZ_COMPRESS_METHOD_STORE,
            0,
            -MAX_WBITS,
            MZ_COMPRESS_LEVEL_DEFAULT,
            0,
            "dummy-password",
            0
        ))
        {
            CString dummyData = "dummy-data";
            zipWriteInFileInZip(zf, dummyData, sizeof(dummyData));
            zipCloseFileInZip(zf);
        }

        zipClose(zf, nullptr);

        FileHelper::DeleteRegularFile("minizip-dummy");

        srand(ZCR_SEED2);
        */
    }

    //////////////////////////////////////////
    inline void EndMinizipObfuscation()
    {

    }

    //////////////////////////////////////////
    inline void AddAssetFilesIntoZip(zipFile zf, Vector<AssetFilePtr>& _files, String _path)
    {
        for (AssetFilePtr const& file : _files)
        {
            String fileName = file->getFileName();

            if (file->getMetaClass()->isInheritedFrom<AssetDirectory>())
            {
                String directoryPath = file->castRaw<AssetDirectory>()->getFullPath();
                Vector<AssetFilePtr> folderFiles = AssetHelper::GetAllAssetFilesInDirectory(directoryPath);

                AddAssetFilesIntoZip(zf, folderFiles, _path + "/" + fileName);
            }
            else
            {
                ByteBufferPtr byteBuffer = file->readAsByteBuffer();

                S32 result = zipOpenNewFileInZip3(
                    zf,
                    (_path + '/' + fileName).c_str(),
                    nullptr,
                    nullptr,
                    0,
                    nullptr,
                    0,
                    nullptr,
                    Z_DEFLATED,
                    MZ_COMPRESS_METHOD_STORE,
                    0,
                    -MAX_WBITS,
                    DEF_MEM_LEVEL,
                    0,
                    CallObfuscationPasswordFunction(fileName).c_str(),
                    0);

                if (result == ZIP_OK)
                {
                    Debug::Log("Packing %s...", fileName.c_str());

                    if (zipWriteInFileInZip(zf, byteBuffer->getData(), (U32)byteBuffer->getSize()))
                    {
                        MAZE_WARNING("Can not write file %s into zip archive", fileName.c_str());
                    }

                    if (zipCloseFileInZip(zf))
                    {
                        MAZE_WARNING("Can not close file %s in zip archive", fileName.c_str());
                    }
                }
                else
                {
                    MAZE_WARNING("Append zip file error - %d", result);
                }
            }
        }
    }

    //////////////////////////////////////////
    MAZE_CORE_API void ObfuscateAssetPacks(
        String const& _srcPath,
        String const& _destPath)
    {
        FileHelper::CreateDirectoryRecursive(_destPath.c_str());

        Vector<AssetFilePtr> files = AssetHelper::GetAllAssetFilesInDirectory(_srcPath);
        for (AssetFilePtr const& file : files)
        {
            if (file->getMetaClass()->isInheritedFrom<AssetDirectory>())
            {
                if (file->getExtension() == "mzap")
                {
                    zipFile zf = zipOpen((_destPath + "/" + file->getFileName()).c_str(), APPEND_STATUS_CREATE);
                    if (!zf)
                        continue;

                    Vector<AssetFilePtr > filesInPack = AssetHelper::GetAllAssetFilesInDirectory(file->getFullPath());
                    AddAssetFilesIntoZip(zf, filesInPack, "");

                    if (zipClose(zf, nullptr))
                    {
                        MAZE_WARNING("Zip archive closing error!");
                        continue;
                    }
                }
                else
                {
                    ObfuscateAssetPacks(file->getFullPath(), _destPath);
                }
            }
            else
            {
                FileHelper::CopyRegularFile(file->getFullPath().c_str(), (_destPath + "/" + file->getFileName()).c_str());
            }
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
