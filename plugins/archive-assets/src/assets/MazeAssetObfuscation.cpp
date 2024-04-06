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
#include "MazeArchiveAssetsHeader.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetObfuscation.hpp"
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

    //////////////////////////////////////////
    ObfuscationPasswordFunction g_obfuscationPasswordFunction = nullptr;

    //////////////////////////////////////////
    MAZE_PLUGIN_ARCHIVE_ASSETS_API void SetObfuscationPasswordFunction(ObfuscationPasswordFunction _func)
    {
        g_obfuscationPasswordFunction = _func;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_ARCHIVE_ASSETS_API String CallObfuscationPasswordFunction(Path const& _fileName)
    {
        if (g_obfuscationPasswordFunction)
            return g_obfuscationPasswordFunction(_fileName);

        return String();
    }

    //////////////////////////////////////////
    inline void AddAssetFilesIntoZip(zipFile zf, Vector<AssetFilePtr>& _files, Path _path)
    {
        for (AssetFilePtr const& file : _files)
        {
            Path fileName = file->getFileName();

            if (file->getMetaClass()->isInheritedFrom<AssetDirectory>())
            {
                Path directoryPath = file->castRaw<AssetDirectory>()->getFullPath();
                Vector<AssetFilePtr> folderFiles = AssetHelper::GetAllAssetFilesInDirectory(directoryPath);

                AddAssetFilesIntoZip(zf, folderFiles, _path + "/" + fileName);
            }
            else
            {
                ByteBufferPtr byteBuffer = file->readAsByteBuffer();

                S32 result = zipOpenNewFileInZip3(
                    zf,
                    (_path + '/' + fileName).toUTF8().c_str(),
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
                    Debug::Log("Packing %s...", fileName.toUTF8().c_str());

                    if (zipWriteInFileInZip(zf, byteBuffer->getDataRO(), (U32)byteBuffer->getSize()))
                    {
                        MAZE_WARNING("Can not write file %s into zip archive", fileName.toUTF8().c_str());
                    }

                    if (zipCloseFileInZip(zf))
                    {
                        MAZE_WARNING("Can not close file %s in zip archive", fileName.toUTF8().c_str());
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
    MAZE_PLUGIN_ARCHIVE_ASSETS_API void ObfuscateAssetPacks(
        Path const& _srcPath,
        Path const& _destPath)
    {
        ObfuscateAssetPacks(
            _srcPath,
            _destPath,
            _srcPath,
            _destPath);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_ARCHIVE_ASSETS_API void ObfuscateAssetPacks(
        Path const& _srcPath,
        Path const& _destPath,
        Path const& _targetSrcPath,
        Path const& _targetDestPath)
    {
        Path targetSrcFullPath = FileHelper::ConvertLocalPathToFullPath(_targetSrcPath);
        Path targetDestFullPath = FileHelper::ConvertLocalPathToFullPath(_targetDestPath);

        FileHelper::CreateDirectoryRecursive(_destPath);

        CString mzapMetaExt = "mzap.meta";
        Path::StringType mzapMetaExtPath = Path::StringType((Char*)mzapMetaExt, (Char*)mzapMetaExt + strlen(mzapMetaExt));

        Vector<AssetFilePtr> files = AssetHelper::GetAllAssetFilesInDirectory(_srcPath);
        for (AssetFilePtr const& file : files)
        {
            if (file->getMetaClass()->isInheritedFrom<AssetDirectory>())
            {
                if (file->getExtension() == "mzap")
                {
                    Path fileFullPath = file->getFullPath();
                    Path::StringType relPath = fileFullPath;
                    StringHelper::RemoveSubstring(relPath, (Path::StringType)_targetSrcPath);
                    Path targetPackFullPath = _targetDestPath + relPath;
                    Path targetPackDirectoryFullPath = FileHelper::GetDirectoryInPath(targetPackFullPath);

                    Path packName = FileHelper::GetFileNameWithoutExtension(file->getFileName());
                    targetPackDirectoryFullPath += "/";
                    targetPackDirectoryFullPath += packName;
                    FileHelper::CreateDirectoryRecursive(targetPackDirectoryFullPath);
                    targetPackDirectoryFullPath += "/";
                    targetPackDirectoryFullPath += file->getFileName();

                    zipFile zf = zipOpen(targetPackDirectoryFullPath.toUTF8().c_str(), APPEND_STATUS_CREATE);
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
                    ObfuscateAssetPacks(file->getFullPath(), _destPath, _targetSrcPath, _targetDestPath);
                }
            }
            else
            {
                Path fileFullPath = file->getFullPath();
                Path::StringType relPath = fileFullPath;
                StringHelper::RemoveSubstring(relPath, (Path::StringType)_targetSrcPath);
                Path targetPackFullPath = _targetDestPath + relPath;

                if (StringHelper::IsEndsWith(fileFullPath.getPath(), mzapMetaExtPath))
                {
                    Path targetPackDirectoryFullPath = FileHelper::GetDirectoryInPath(targetPackFullPath);

                    Path packName = FileHelper::GetFileNameWithoutExtension(FileHelper::GetFileNameWithoutExtension(file->getFileName()));
                    targetPackDirectoryFullPath += "/";
                    targetPackDirectoryFullPath += packName;
                    FileHelper::CreateDirectoryRecursive(targetPackDirectoryFullPath);
                    targetPackDirectoryFullPath += "/";
                    targetPackDirectoryFullPath += file->getFileName();

                    targetPackFullPath = targetPackDirectoryFullPath;
                }

                FileHelper::CopyRegularFile(file->getFullPath(), targetPackFullPath);
            }
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
