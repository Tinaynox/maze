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
#if (!defined(_MazeECSRenderScene_hpp_))
#define _MazeECSRenderScene_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/ecs/MazeLightingSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ECSRenderScene);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class ECSRenderScene
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ECSRenderScene
        : public ECSScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ECSRenderScene, ECSScene);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ECSRenderScene);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ECSRenderScene();

        //////////////////////////////////////////
        inline RenderTargetPtr const& getRenderTarget() const { return m_renderTarget; }

        //////////////////////////////////////////
        LightingSettingsPtr const& getLightingSettings() const { return m_lightingSettings; }

    protected:

        //////////////////////////////////////////
        ECSRenderScene();
        
        //////////////////////////////////////////
        bool init(RenderTargetPtr const& _renderTarget);

    private:

        //////////////////////////////////////////
        using ECSScene::init;

    protected:
        RenderTargetPtr m_renderTarget;

        LightingSettingsPtr m_lightingSettings;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSRenderScene_hpp_
//////////////////////////////////////////
