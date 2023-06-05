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
#if (!defined(_MazeWindowVideoMode_hpp_))
#define _MazeWindowVideoMode_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct WindowVideoMode
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API WindowVideoMode
    {
        //////////////////////////////////////////
        WindowVideoMode()
            : width(0)
            , height(0)
            , bpp(0)
        {
    
        }

        //////////////////////////////////////////
        WindowVideoMode(
            U32 _width,
            U32 _height,
            U32 _bpp,
            F32 _pixelScale = 1.0f)
            : width(_width)
            , height(_height)
            , bpp(_bpp)
            , pixelScale(_pixelScale)
        {
    
        }

        //////////////////////////////////////////
        static WindowVideoMode FromString(const String& _videoMode)
        {
            WindowVideoMode result;

            Vector<String> words;
            StringHelper::SplitWords(_videoMode, words, 'x');

            Size wordsCount = words.size();
            if (wordsCount >= 2)
            {
                result.width = atoi(words[0].c_str());
                result.height = atoi(words[1].c_str());
            }
            
            if (wordsCount >= 3)
            {
                result.bpp = atoi(words[2].c_str());
            }

            return result;
        }

        //////////////////////////////////////////
        static String ToString(WindowVideoMode _resolution)
        {
            return    StringHelper::ToString(_resolution.width) + "x" 
                    + StringHelper::ToString(_resolution.height) + "x"
                    + StringHelper::ToString(_resolution.bpp);
        }

        //////////////////////////////////////////
        inline String toString() const
        {
            return ToString(*this);
        }

        //////////////////////////////////////////
        inline bool operator==(const WindowVideoMode& _videoMode) const
        {
            return (
                   width == _videoMode.width 
                && height == _videoMode.height 
                && bpp == _videoMode.bpp);
        }

        //////////////////////////////////////////
        inline bool operator!=(const WindowVideoMode& _videoMode) const
        {
            return !(*this == _videoMode);
        }

        //////////////////////////////////////////
        inline bool operator<(const WindowVideoMode& _videoMode) const
        {
            return ((width * height * bpp) < (_videoMode.width * _videoMode.height * _videoMode.bpp));
        }

        U32 width;
        U32 height;
        U32 bpp;
        F32 pixelScale = 1.0f;
    };


} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeWindow_hpp_
//////////////////////////////////////////
