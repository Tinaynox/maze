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
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/emscripten/MazeFileHelperEmscripten.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include <unistd.h>
#include <emscripten.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        static std::function<void(bool)> g_mountEmscriptenLocalStorageCallback = nullptr;
        static std::function<void(bool)> g_syncEmscriptenLocalStorageCallback = nullptr;
        
    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
extern "C"
{
    //////////////////////////////////////////
    void EMSCRIPTEN_KEEPALIVE OnMountEmscriptenLocalStorageFinished(bool _result)
    {
        Maze::Debug::Log("OnMountEmscriptenLocalStorageFinished: %d", (Maze::S32)_result);
        
        if (Maze::FileHelper::g_mountEmscriptenLocalStorageCallback)
        {
            std::function<void(bool)> tmpCb = Maze::FileHelper::g_mountEmscriptenLocalStorageCallback;
            Maze::FileHelper::g_mountEmscriptenLocalStorageCallback = nullptr;
            tmpCb(_result);
        }
    }
    
    //////////////////////////////////////////
    void EMSCRIPTEN_KEEPALIVE OnSyncEmscriptenLocalStorageFinished(bool _result)
    {
        Maze::Debug::Log("OnSyncEmscriptenLocalStorageFinished: %d", (Maze::S32)_result);
        
        if (Maze::FileHelper::g_syncEmscriptenLocalStorageCallback)
        {
            std::function<void(bool)> tmpCb = Maze::FileHelper::g_syncEmscriptenLocalStorageCallback;
            Maze::FileHelper::g_syncEmscriptenLocalStorageCallback = nullptr;
            tmpCb(_result);
        }
    }
}


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        
        //////////////////////////////////////////
        MAZE_CORE_API Path GetWorkingDirectory()
        {
            S8 buff[PATH_MAX + 1];
            getcwd(buff, sizeof(buff));
            return buff;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryFullPath()
        {
            return GetWorkingDirectory();
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryDirectory()
        {
            return GetDirectoryInPath(GetBinaryFullPath());
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDocumentsDirectory()
        {
            return MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR "/documents";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultTemporaryDirectory()
        {
            return MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR "/tmp";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultLogDirectory()
        {
            return MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR "/.log";
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void MountEmscriptenLocalStorage(std::function<void(bool)> _mountCb)
        {
            MAZE_ERROR_RETURN_IF(g_mountEmscriptenLocalStorageCallback != nullptr, "Local storage mounting is in progress already!");
            
            g_mountEmscriptenLocalStorageCallback = _mountCb;
            
            Debug::Log("Mounting Emscripten localStorage as '%s' started...", MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR);
            
            EM_ASM_({
                
                FS.mkdir(UTF8ToString($0));
                FS.mount(IDBFS, {}, UTF8ToString($0));
                
                FS.syncfs(true, function (err) {
                    if (err) {
                        console.warn("Mounting Emscripten localStorage failed:", err);
                        
                        ccall('OnMountEmscriptenLocalStorageFinished', 'void', ['number'], [0]);
                    }
                    else {
                        console.log("Mounting Emscripten localStorage successful.");
                        
                        ccall('OnMountEmscriptenLocalStorageFinished', 'void', ['number'], [1]);
                    }
                });
            }, MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void SyncEmscriptenLocalStorage(std::function<void(bool)> _syncCb)
        {
            MAZE_ERROR_RETURN_IF(g_syncEmscriptenLocalStorageCallback != nullptr, "Local storage syncing is in progress already!");
            
            g_syncEmscriptenLocalStorageCallback = _syncCb;
            
            Debug::Log("Syncing Emscripten localStorage started...");
            
            EM_ASM(
                FS.syncfs(function (err) {
                    if (err) {
                        console.warn("Syncing Emscripten localStorage failed:", err);
                        
                        ccall('OnSyncEmscriptenLocalStorageFinished', 'void', ['number'], [0]);
                    }
                    else {
                        console.log("Syncing Emscripten localStorage successful.");
                        
                        ccall('OnSyncEmscriptenLocalStorageFinished', 'void', ['number'], [1]);
                    }
                });
            );
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API bool IsSyncEmscriptenLocalStorageInProgress()
        {
            return g_syncEmscriptenLocalStorageCallback != nullptr;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void TrySyncEmscriptenLocalStorage()
        {
            if (IsSyncEmscriptenLocalStorageInProgress())
                return;
            
            SyncEmscriptenLocalStorage();
        }


    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
