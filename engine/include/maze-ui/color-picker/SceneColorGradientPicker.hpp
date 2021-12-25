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
#if (!defined(_SceneColorGradientPicker_hpp_))
#define _SceneColorGradientPicker_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
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
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-ui/ecs/components/MazeColorSliderTag2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneColorGradientPicker);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SceneColorGradientPicker
    //
    //////////////////////////////////////////
    class MAZE_UI_API SceneColorGradientPicker
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneColorGradientPicker, ECSRenderScene);

        //////////////////////////////////////////
        enum EditMode
        {
            None,
            AlphaTag,
            ColorTag,
        };

    public:

        //////////////////////////////////////////
        static SceneColorGradientPickerPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneColorGradientPicker();

        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneColorGradientPicker();

        //////////////////////////////////////////
        // using ECSRenderScene::init;

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();


        //////////////////////////////////////////
        void notifyGradientChanged(ColorGradient const& _gradient);


        //////////////////////////////////////////
        void updateUI();


        //////////////////////////////////////////
        void notifyModeChanged(SystemTextDropdown2D* _dropdown, S32 _index);


        //////////////////////////////////////////
        ColorSliderTag2DPtr createColorSliderTag(bool _down = true);

        //////////////////////////////////////////
        void updateColorSliderTags();


        //////////////////////////////////////////
        void notifyColorSliderTagClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyColorSliderTagPressedChanged(Button2D* _button, bool _pressed);

        //////////////////////////////////////////
        void notifyColorSliderTagCursorPressIn(Button2D* _button, Vec2DF const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void processAlphaColorSliderTagClick(Size _index, S32 _buttonId);

        //////////////////////////////////////////
        void processRGBColorSliderTagClick(Size _index, S32 _buttonId);


        //////////////////////////////////////////
        void setEditMode(EditMode _value);

        //////////////////////////////////////////
        void updateEditModeUI();


        //////////////////////////////////////////
        void updateColorTagUI();


        //////////////////////////////////////////
        void notifyColorEditColorChanged(ColorHDREdit2D* _edit, ColorF128 const& _color);

        //////////////////////////////////////////
        void notifyAlphaSliderValueChanged(Slider2D* _slider, F32 _value);
        
        //////////////////////////////////////////
        void notifyAlphaEditTextInput(SystemTextEditBox2D* _edit);

        //////////////////////////////////////////
        void notifyLocationEditTextInput(SystemTextEditBox2D* _edit);

        //////////////////////////////////////////
        void changeCurrentAlphaTagTime(F32 _newTime);

        //////////////////////////////////////////
        void changeCurrentColorTagTime(F32 _newTime);


        //////////////////////////////////////////
        void notifyCanvasCursorReleaseIn(Vec2DF const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyCanvasCursorReleaseOut(CursorInputEvent const& _event);


        //////////////////////////////////////////
        void notifyGradientClickButtonClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyGradientClickButtonCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _event);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_canvasUIElement;

        SystemTextDropdown2DPtr m_modeDropdown;
        ClickButton2DPtr m_copyButton;

        SpriteRenderer2DPtr m_gradientRendererHolder;
        SpriteRenderer2DPtr m_gradientRenderer;
        ClickButton2DPtr m_gradientClickButton;
        Vec2DF m_gradientClickPosition = Vec2DF::c_zero;

        EntityPtr m_alphaHolder;
        Slider2DPtr m_alphaSlider;
        SystemTextEditBox2DPtr m_alphaEdit;

        EntityPtr m_colorHolder;
        ColorHDREdit2DPtr m_colorEdit;

        EntityPtr m_locationHolder;
        SystemTextEditBox2DPtr m_locationEdit;

        Vector<ColorSliderTag2DPtr> m_alphaTags;
        Vector<ColorSliderTag2DPtr> m_colorTags;

        EditMode m_editMode = EditMode::None;
        Size m_colorTagIndex = -1;
        bool m_draggingTag = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneColorGradientPicker_hpp_
//////////////////////////////////////////
