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
#if (!defined(_SceneSettings_hpp_))
#define _SceneSettings_hpp_


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
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneSettings);
    MAZE_USING_SHARED_PTR(UITweenTransitionAlpha);


    //////////////////////////////////////////
    enum class SettingsMode
    {
        Video = 0,
        Sound,
        Game,
        Controls,

        MAX
    };


    //////////////////////////////////////////
    // Class SceneSettings
    //
    //////////////////////////////////////////
    class SceneSettings
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneSettings, ECSRenderScene);


    public:

        //////////////////////////////////////////
        static SceneSettingsPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneSettings();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneSettings();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        virtual void processSceneStateChanged(ECSSceneState _state) MAZE_OVERRIDE;



        //////////////////////////////////////////
        void notifyResetButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCloseButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyTopMenuButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        void setMode(SettingsMode _value);

        //////////////////////////////////////////
        void updateMenus();

        //////////////////////////////////////////
        void updateVideoMenuProperties();


        //////////////////////////////////////////
        void notifyVideoMenuPropertyChanged(bool const& _value);

        //////////////////////////////////////////
        void notifyVideoMenuPropertyChanged(int const& _value);

    protected:
        CanvasPtr m_canvas;

        UITweenTransitionAlphaPtr m_canvasTransition;

        SettingsMode m_settingsMode = SettingsMode::Video;
        Array<ToggleButton2DPtr, (Size)SettingsMode::MAX> m_menuButtons;
        Array<Transform2DPtr, (Size)SettingsMode::MAX> m_menus;
        
        // Video settings
        ToggleButton2DPtr m_fullscreenToggle;
        ToggleButton2DPtr m_vsyncToggle;
        ToggleButton2DPtr m_postProcessFXToggle;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneSettings_hpp_
//////////////////////////////////////////
