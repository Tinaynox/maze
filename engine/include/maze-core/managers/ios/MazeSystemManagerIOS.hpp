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
#if (!defined(_MazeSystemManagerIOS_hpp_))
#define _MazeSystemManagerIOS_hpp_

//////////////////////////////////////////
#include "maze-core/managers/unix/MazeSystemManagerUnix.hpp"


//////////////////////////////////////////
#ifdef __OBJC__

@class ApplicationIOS;
typedef ApplicationIOS* ApplicationIOSRef;

@class ApplicationDelegateIOS;
typedef ApplicationDelegateIOS* ApplicationDelegateIOSRef;

#else

typedef void* ApplicationIOSRef;
typedef void* ApplicationDelegateIOSRef;

#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerIOS
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemManagerIOS
        : public SystemManagerUnix
    {
    private:
        
        //////////////////////////////////////////
        friend class SystemManager;
        
    public:
        
        //////////////////////////////////////////
        virtual ~SystemManagerIOS();
        
        //////////////////////////////////////////
        virtual bool initApplication() MAZE_OVERRIDE;
        
                
        ////////////////////////////////////
        void processApplicationWillTerminate();
        
    protected:
        
        //////////////////////////////////////////
        SystemManagerIOS();
        
        //////////////////////////////////////////
        virtual bool init(Vector<Maze::S8 const*> const& _commandLineArguments ) MAZE_OVERRIDE;
        
        
    protected:
        
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemManagerIOS_hpp_
//////////////////////////////////////////
