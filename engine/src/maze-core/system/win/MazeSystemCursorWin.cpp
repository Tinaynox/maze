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
#include "maze-core/system/win/MazeSystemCursorWin.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemCursorWin
    //
    //////////////////////////////////////////
    SystemCursorWin::SystemCursorWin()
    {
    }

    //////////////////////////////////////////
    SystemCursorWin::~SystemCursorWin()
    {
        setCursorHandle(NULL);
    }

    //////////////////////////////////////////
    SystemCursorWinPtr SystemCursorWin::Create(
        HCURSOR _cursorHandle,
        bool _ownsCursorHandle)
    {
        SystemCursorWinPtr cursor;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(SystemCursorWin, cursor, init(_cursorHandle, _ownsCursorHandle));
        return cursor;
    }

    //////////////////////////////////////////
    bool SystemCursorWin::init(
        HCURSOR _cursorHandle,
        bool _ownsCursorHandle = false)
    {
        setCursorHandle(_cursorHandle);
        return true;
    }

    //////////////////////////////////////////
    void SystemCursorWin::setCursorHandle(
        HCURSOR _cursorHandle,
        bool _ownsCursorHandle)
    {
        if (m_cursorHandle == _cursorHandle)
            return;

        if (m_cursorHandle != NULL && m_ownsCursorHandle)
        {
            DestroyCursor(m_cursorHandle);
        }

        m_cursorHandle = _cursorHandle;
        m_ownsCursorHandle = _ownsCursorHandle;
    }

    //////////////////////////////////////////
    bool SystemCursorWin::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        DataBlock dataBlock = _assetFile->readAsDataBlock();
        CString cursorFileName = dataBlock.getCString(MAZE_HCS(MAZE_PLATFORM_SUFFIX_CAMEL_STR));
        if (!cursorFileName || !cursorFileName[0])
            return false;

        AssetFilePtr const& cursorFile = AssetManager::GetInstancePtr()->getAssetFile(cursorFileName);
        MAZE_ERROR_RETURN_VALUE_IF(!cursorFile, false, "Undefined cursor file: %s", cursorFileName);

        return loadFromWindowsCursorAssetFile(cursorFile);
    }

    //////////////////////////////////////////
    bool SystemCursorWin::loadFromWindowsCursorAssetFile(AssetFilePtr const& _assetFile)
    {
        HCURSOR cursorHandle = NULL;

        if (_assetFile->getMetaClass()->isInheritedFrom<AssetRegularFile>())
        {
            cursorHandle = LoadCursorFromFileW(_assetFile->getFullPath().c_str());
        }
        else
        {
            ByteBufferPtr dataBuffer = _assetFile->readAsByteBuffer();
            Path tempCursorFilePath = FileHelper::GetDefaultTemporaryDirectory() + L"/cursors/" + _assetFile->getFileName();
            ByteBufferHelper::SaveBinaryFile(*dataBuffer.get(), tempCursorFilePath);

            cursorHandle = LoadCursorFromFileW(tempCursorFilePath.c_str());
            FileHelper::DeleteRegularFile(tempCursorFilePath.c_str());
        }

        MAZE_ERROR_RETURN_VALUE_IF(
            cursorHandle == NULL,
            false,
            "Failed to load cursor file %s!",
            _assetFile->getFullPath().toUTF8().c_str());
        setCursorHandle(cursorHandle, true);
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
