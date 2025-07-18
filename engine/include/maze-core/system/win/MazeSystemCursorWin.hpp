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
#if (!defined(_MazeSystemCursorWin_hpp_))
#define _MazeSystemCursorWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeSystemCursor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(SystemCursorWin);


    //////////////////////////////////////////
    // Class SystemCursorWin
    //
    //////////////////////////////////////////
    class SystemCursorWin
        : public SystemCursor
    {
    public:

        //////////////////////////////////////////
        virtual ~SystemCursorWin();

        //////////////////////////////////////////
        static SystemCursorWinPtr Create(
            HCURSOR _cursorHandle = NULL,
            bool _ownsCursorHandle = false);

        //////////////////////////////////////////
        void setCursorHandle(
            HCURSOR _cursorHandle,
            bool _ownsCursorHandle = false);

        //////////////////////////////////////////
        inline HCURSOR getCursorHandle() const { return m_cursorHandle; }


        //////////////////////////////////////////
        virtual bool loadFromAssetFile(AssetFilePtr const& _assetFile) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool loadFromWindowsCursorAssetFile(AssetFilePtr const& _assetFile);

    protected:

        //////////////////////////////////////////
        SystemCursorWin();

        //////////////////////////////////////////
        bool init(
            HCURSOR _cursorHandle,
            bool _ownsCursorHandle);

    protected:
        HCURSOR m_cursorHandle = 0;
        bool m_ownsCursorHandle = false;
    };
    
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeSystemCursorWin_hpp_
//////////////////////////////////////////
