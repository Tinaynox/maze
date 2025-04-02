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
#if (!defined(_MazeEcsAssetScene_hpp_))
#define _MazeEcsAssetScene_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(EcsAssetScene);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class EcsAssetScene
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API EcsAssetScene
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EcsAssetScene, EcsRenderScene);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EcsAssetScene);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        static EcsAssetScenePtr Create(
            AssetFilePtr const& _file,
            RenderTargetPtr const& _renderTarget,
            EcsWorld* _world);

        //////////////////////////////////////////
        virtual ~EcsAssetScene();


        //////////////////////////////////////////
        virtual HashedCString getSceneName() const MAZE_OVERRIDE { return getName(); }


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(HashedString const& _name) { m_name = _name; }


        //////////////////////////////////////////
        virtual void serializeSceneCommonInfo(DataBlock& _info) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void deserializeSceneCommonInfo(DataBlock const& _info) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline void setFile(AssetFilePtr const& _file) { m_file = _file; }

        //////////////////////////////////////////
        inline AssetFilePtr const& getFile() const { return m_file; }

    protected:

        //////////////////////////////////////////
        EcsAssetScene();
        
        //////////////////////////////////////////
        bool init(
            AssetFilePtr const& _file,
            RenderTargetPtr const& _renderTarget,
            EcsWorld* _world);


        //////////////////////////////////////////
        virtual EcsWorld* assignWorld() MAZE_OVERRIDE { return m_world; }

    protected:

        //////////////////////////////////////////
        using EcsRenderScene::init;

    protected:
        AssetFilePtr m_file;
        HashedString m_name;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsAssetScene_hpp_
//////////////////////////////////////////
