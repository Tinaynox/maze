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
#if (!defined(_MazeSystemManagerOSX_hpp_))
#define _MazeSystemManagerOSX_hpp_

//////////////////////////////////////////
#include "maze-core/managers/unix/MazeSystemManagerUnix.hpp"


//////////////////////////////////////////
#ifdef __OBJC__

@class ApplicationOSX;
typedef ApplicationOSX* ApplicationOSXRef;

@class ApplicationDelegateOSX;
typedef ApplicationDelegateOSX* ApplicationDelegateOSXRef;

#else

typedef void* ApplicationOSXRef;
typedef void* ApplicationDelegateOSXRef;

#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerOSX
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemManagerOSX
        : public SystemManagerUnix
    {
    private:
        
        //////////////////////////////////////////
        friend class SystemManager;
        
    public:
        
        //////////////////////////////////////////
        virtual ~SystemManagerOSX();
        
        //////////////////////////////////////////
        virtual bool initApplication() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void shutdownApplication() MAZE_OVERRIDE;
        
        
        ////////////////////////////////////
        ApplicationOSXRef getApplicationOSX() const { return m_applicationOSX; }
        
        ////////////////////////////////////
        ApplicationDelegateOSXRef getApplicationDelegateOSX() const { return m_applicationDelegateOSX; }
        
    protected:
        
        //////////////////////////////////////////
        SystemManagerOSX();
        
        //////////////////////////////////////////
        virtual bool init(Vector<Maze::S8 const*> const& _commandLineArguments ) MAZE_OVERRIDE;
                
        
    protected:
        
        ApplicationOSXRef m_applicationOSX;
        ApplicationDelegateOSXRef m_applicationDelegateOSX;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemManagerOSX_hpp_
//////////////////////////////////////////
