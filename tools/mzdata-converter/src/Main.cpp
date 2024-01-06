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
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeAssetHelper.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
using namespace Maze;


//////////////////////////////////////////
#define CONVERTER_MODE_DEFAULT (1)
#define CONVERTER_MODE_BINALIZER (2)

//////////////////////////////////////////
#define CONVERTER_MODE CONVERTER_MODE_BINALIZER


//////////////////////////////////////////
#if (CONVERTER_MODE == CONVERTER_MODE_DEFAULT)

//////////////////////////////////////////
S32 main(S32 _argc, S8 const* _argv[])
{
    MAZE_ERROR_RETURN_VALUE_IF(_argc < 2, 1, "Incorrect count of params");


    LogService::GetInstancePtr()->setLogFile(FileHelper::GetBinaryDirectory() + "/maze-tool-mzdata-converter.log");

    Path destPath = ".";
   

    Bool keepOriginExtension = false;

    XMLToDataBlockConfig xmlConfig;
    DataBlock config;
    if (config.loadTextFile(FileHelper::GetBinaryDirectory() + "/maze-tool-mzdata-converter.mzdata"))
    {
        keepOriginExtension = config.getBool(MAZE_HS("keepOriginExtension"), false);
        destPath = config.getString(MAZE_HS("destPath"), ".");

        xmlConfig.collapseRootBlock = config[MAZE_HS("xml")].getBool(MAZE_HS("collapseRootBlock"), false);
        xmlConfig.lowerCaseBlockNameCapitalButton = config[MAZE_HS("xml")].getBool(MAZE_HS("lowerCaseBlockNameCapitalButton"), false);
    }

    Debug::Log("destFolder=%s", destPath.toUTF8().c_str());

    for (S32 i = 1; i < _argc; ++i)
    {
        Path srcPath = FileHelper::ConvertLocalPathToFullPath(_argv[i]);
        Debug::Log("srcData=%s", srcPath.toUTF8().c_str());

        if (FileHelper::IsDirectory(srcPath))
        {
            MAZE_ERROR_CONTINUE("%s is not a data file!", FileHelper::GetFileNameInPath(srcPath).toUTF8().c_str());
        }

        Timer timer;
        U32 usStart = timer.getMicroseconds();
        tinyxml2::XMLDocument doc;
        if (XMLHelper::LoadXMLFile(srcPath, doc) == tinyxml2::XMLError::XML_SUCCESS)
        {
            Debug::Log("XML Data");
            Debug::Log("collapseRootBlock=%d", (S32)xmlConfig.collapseRootBlock);
            Debug::Log("lowerCaseBlockNameCapitalButton=%d", (S32)xmlConfig.lowerCaseBlockNameCapitalButton);

            DataBlock dataBlock;
            XMLHelper::ConvertXMLDocumentToDataBlock(&doc, dataBlock, xmlConfig);
            dataBlock.saveTextFile(
                destPath + "/" +
                (keepOriginExtension ? FileHelper::GetFileNameInPath(srcPath) : (FileHelper::GetFileNameWithoutExtension(srcPath) + ".mzdata")));
            U32 usTime = timer.getMicroseconds() - usStart;
            Debug::Log("XML data successfully converted for %.1fms", (F32)(usTime) / 1000.0f);
        }
        else
        {
            MAZE_ERROR_CONTINUE("Unsupported data format for convert - %s", FileHelper::GetFileNameInPath(srcPath).toUTF8().c_str());
        }
    }

    return 0;
}

#elif (CONVERTER_MODE == CONVERTER_MODE_BINALIZER)

//////////////////////////////////////////
S32 main(S32 _argc, S8 const* _argv[])
{
    MAZE_ERROR_RETURN_VALUE_IF(_argc < 3, 1, "Incorrect count of params");

    Path srcPath = FileHelper::ConvertLocalPathToFullPath(_argv[1]);
    Debug::Log("srcData=%s", srcPath.toUTF8().c_str());

    Path destPath = FileHelper::ConvertLocalPathToFullPath(_argv[2]);
    Debug::Log("destPath=%s", destPath.toUTF8().c_str());

    DataBlock dataBlock;
    MAZE_ERROR_RETURN_VALUE_IF(!dataBlock.loadTextFile(srcPath), 1, "Failed to load src file!");

    FileHelper::CreateDirectoryRecursive(FileHelper::GetDirectoryInPath(destPath));
    MAZE_ERROR_RETURN_VALUE_IF(!dataBlock.saveBinaryFile(destPath), 2, "Failed to save file!");

    return 0;
}

#endif