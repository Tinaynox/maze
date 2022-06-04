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
#if (!defined(_MazeSceneProfilerView_hpp_))
#define _MazeSceneProfilerView_hpp_


//////////////////////////////////////////
#include "maze-plugin-profiler-view/MazeProfilerViewHeader.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSimpleLineRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneProfilerView);
    


    //////////////////////////////////////////
    // Class SceneProfilerView
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PROFILER_VIEW_API SceneProfilerView
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneProfilerView, ECSRenderScene);

    public:

        //////////////////////////////////////////
        // Struct ProfilerViewData
        //
        //////////////////////////////////////////
        struct ProfilerViewData
        {
            SpriteRenderer2DPtr background;
            SystemTextRenderer2DPtr label0;
            SystemTextRenderer2DPtr label1;
            SimpleLineRenderer2DPtr graph;
        };

    public:

        //////////////////////////////////////////
        static SceneProfilerViewPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneProfilerView();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneProfilerView();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void updateUI();

    protected:
        CanvasPtr m_canvas;

        FastVector<ProfilerViewData> m_views;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneProfilerView_hpp_
//////////////////////////////////////////