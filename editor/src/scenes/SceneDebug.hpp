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
#if (!defined(_SceneDebug_hpp_))
#define _SceneDebug_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "settings/MazeEditorSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDebug);


    //////////////////////////////////////////
    // Class SceneDebug
    //
    //////////////////////////////////////////
    class SceneDebug
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDebug, EcsRenderScene);

    public:

        //////////////////////////////////////////
        static SceneDebugPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneDebug();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneDebug();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyMainRenderWindowResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void notifyDebugInfoEnabledChanged(bool const& _value);

        //////////////////////////////////////////
        void updateTextRendererEnabled();

    protected:
        AbstractTextRenderer2DPtr m_fpsSystemTextRenderer;
        F32 m_fpsUpdateInterval;
        F32 m_fpsLowest;
        F32 m_fpsAcc;
        S32 m_framesCount;
        F32 m_fpsTimeLeft;

        CanvasPtr m_canvas;

        EditorSettings* m_editorSettings = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDebug_hpp_
//////////////////////////////////////////
