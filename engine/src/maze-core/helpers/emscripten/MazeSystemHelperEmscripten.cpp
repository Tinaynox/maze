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
#include "maze-core/helpers/emscripten/MazeSystemHelperEmscripten.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <emscripten.h>


//////////////////////////////////////////
inline Maze::String GetSystemLocale()
{
    Maze::String result;
    
    const char* language = (const char*)EM_ASM_INT({
        var language = navigator.language;
        var buffer = Module._malloc(language.length + 1);
        stringToUTF8(language, buffer, language.length + 1);
        return buffer;
    });
    
    if (language)
    {
        result = language;
    
        EM_ASM_INT({
            Module._free($0);
        }, language);
    
    }
    
    return result;
}


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void OpenURL(Path const& _url)
        {
            EM_ASM_({
                window.open(UTF8ToString($0), '_blank');
            }, _url.c_str());
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API GeoLocation GetGeoLocation()
        {
            return GeoLocation::Unknown;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API GeoLanguage GetUserLanguage()
        {
            return GetSystemLanguage();
        }

        //////////////////////////////////////////
        MAZE_CORE_API GeoLanguage GetSystemLanguage()
        {
            Maze::String localeLanguage = GetSystemLocale();
            if (localeLanguage.empty())
                return GeoLanguage::Unknown;
            
            Vector<String> words;
            StringHelper::SplitWords(localeLanguage, words, '-');
            
            return IETFTagToGeoLanguage(words[0]);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void SetupMazeDynLibDirectoriesPolicy()
        {
            
        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
