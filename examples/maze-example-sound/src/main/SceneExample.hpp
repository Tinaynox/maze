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
#if (!defined(_SceneExample_hpp_))
#define _SceneExample_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
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
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/MazeSoundSource.hpp"
#include "maze-sound/MazeSoundGroup.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneExample);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    

    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    class SceneExample
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneExamplePtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneExample();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneExample();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void create2D();


        //////////////////////////////////////////
        bool loadPlugins();

    protected:
        CanvasPtr m_canvas;

        MenuListTree2DPtr m_menuListTree;

        ScrollRect2DPtr m_scrollRect;

        SoundManagerPtr m_soundManager;

        ClickButton2DPtr m_playSoundButton;
        ClickButton2DPtr m_playMusicButton;

        Slider2DPtr m_soundVolumeSlider;
        Slider2DPtr m_musicVolumeSlider;


        SoundGroupPtr m_soundGroup;
        SoundGroupPtr m_musicGroup;

        SoundSourcePtr m_music;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneExample_hpp_
//////////////////////////////////////////
