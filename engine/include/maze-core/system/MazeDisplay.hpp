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
#if (!defined(_MazeDisplayInfo_hpp_))
#define _MazeDisplayInfo_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Display);


    //////////////////////////////////////////
    // Class Display
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Display
    {
    public:

        //////////////////////////////////////////
        enum Flags
        {
            Primary = MAZE_BIT(0)
        };
        

    public:

        //////////////////////////////////////////
        virtual ~Display();

        //////////////////////////////////////////
        static DisplayPtr Create(
            String const& _name,
            U64 _handle = 0,
            U32 _flags = 0,
            Vector<WindowVideoMode> const& _fullscreenModes = Vector<WindowVideoMode>());

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline U64 getHandle() const { return m_handle; }
        
        //////////////////////////////////////////
        Vector<WindowVideoMode> const& getFullscreenModes() const { return m_fullscreenModes; }

        //////////////////////////////////////////
        inline bool isPrimary() const { return m_flags | Flags::Primary; }

        //////////////////////////////////////////
        WindowVideoMode getCurrentMode();

        //////////////////////////////////////////
        inline void* getUserData() const { return m_userData; }

        //////////////////////////////////////////
        inline void setUserData(void* _userData) { m_userData = _userData; }

    protected:

        ////////////////////////////////////
        Display();

        ////////////////////////////////////
        bool init(
            String const& _name,
            U64 _handle = 0,
            U32 _flags = 0,
            Vector<WindowVideoMode> const& _fullscreenModes = Vector<WindowVideoMode>());


    protected:
        String m_name;
        U64 m_handle;
        U32 m_flags;

        Vector<WindowVideoMode> m_fullscreenModes;

        void* m_userData;
    };


} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeWindow_hpp_
//////////////////////////////////////////
