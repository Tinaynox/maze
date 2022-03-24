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
#if (!defined(_MazeArchiveAssetsPlugin_hpp_))
#define _MazeArchiveAssetsPlugin_hpp_


//////////////////////////////////////////
#include "maze-plugin-archive-assets/MazeArchiveAssetsHeader.hpp"
#include "maze-core/system/MazePlugin.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ArchiveAssetsPlugin);
    MAZE_USING_SHARED_PTR(ArchiveAssets);


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallArchiveAssetsPlugin();

    //////////////////////////////////////////
    void UninstallArchiveAssetsPlugin();

#endif


    //////////////////////////////////////////
    // Class ArchiveAssetsPlugin
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_ARCHIVE_ASSETS_API ArchiveAssetsPlugin
        : public Plugin
        , public std::enable_shared_from_this<ArchiveAssetsPlugin>
    {
    public:

        //////////////////////////////////////////
        virtual ~ArchiveAssetsPlugin();

        //////////////////////////////////////////
        static ArchiveAssetsPluginPtr Create();

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void install() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void uninstall() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ArchiveAssetsPlugin();

        //////////////////////////////////////////
        bool init();

    protected:
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeArchiveAssetsPlugin_hpp_
//////////////////////////////////////////
