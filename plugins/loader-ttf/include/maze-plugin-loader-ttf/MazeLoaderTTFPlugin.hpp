//////////////////////////////////////////
//
// Maze SoundSystemAL
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
#if (!defined(_MazeLoaderTTFPlugin_hpp_))
#define _MazeLoaderTTFPlugin_hpp_


//////////////////////////////////////////
#include "maze-plugin-loader-ttf/MazeLoaderTTFHeader.hpp"
#include "maze-core/system/MazePlugin.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(LoaderTTFPlugin);
    MAZE_USING_SHARED_PTR(LoaderTTF);


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderTTFPlugin();

    //////////////////////////////////////////
    void UninstallLoaderTTFPlugin();

#endif


    //////////////////////////////////////////
    // Class LoaderTTFPlugin
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_LOADER_TTF_API LoaderTTFPlugin
        : public Plugin
        , public std::enable_shared_from_this<LoaderTTFPlugin>
    {
    public:
        //////////////////////////////////////////
        static constexpr CString const c_libraryName = "maze-plugin-loader-ttf";

    public:

        //////////////////////////////////////////
        virtual ~LoaderTTFPlugin();

        //////////////////////////////////////////
        static LoaderTTFPluginPtr Create();

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void install() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void uninstall() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        LoaderTTFPlugin();

        //////////////////////////////////////////
        bool init();

    protected:
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLoaderTTFPlugin_hpp_
//////////////////////////////////////////
