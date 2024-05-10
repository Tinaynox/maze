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
#if (!defined(_MazeSceneColorGradientPicker_hpp_))
#define _MazeSceneColorGradientPicker_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-ui/ecs/components/MazeColorSliderTag2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-editor-tools/ecs/components/MazeColorHDREdit2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneColorGradientPicker);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SceneColorGradientPicker
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SceneColorGradientPicker
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneColorGradientPicker, EcsRenderScene);

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
        // using EcsRenderScene::init;

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();


        //////////////////////////////////////////
        void notifyGradientChanged(ColorGradient const& _gradient);


        //////////////////////////////////////////
        void updateUI();


        //////////////////////////////////////////
        void notifyModeChanged(Dropdown2D* _dropdown, S32 _index);


        //////////////////////////////////////////
        ColorSliderTag2DPtr createColorSliderTag(bool _down = true);

        //////////////////////////////////////////
        void updateColorSliderTags();


        //////////////////////////////////////////
        void notifyColorSliderTagClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyColorSliderTagPressedChanged(Button2D* _button, bool _pressed);

        //////////////////////////////////////////
        void notifyColorSliderTagCursorPressIn(Button2D* _button, Vec2F const& _positionOS, CursorInputEvent const& _event);

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
        void notifyAlphaEditTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void notifyLocationEditTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void changeCurrentAlphaTagTime(F32 _newTime);

        //////////////////////////////////////////
        void changeCurrentColorTagTime(F32 _newTime);

        //////////////////////////////////////////
        void notifyCanvasCursorRelease(CursorInputEvent const& _event);


        //////////////////////////////////////////
        void notifyGradientClickButtonClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyGradientClickButtonCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _event);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_canvasUIElement;

        Dropdown2DPtr m_modeDropdown;
        ClickButton2DPtr m_copyButton;
        ClickButton2DPtr m_pasteButton;
        ClickButton2DPtr m_copyXMLButton;
        ClickButton2DPtr m_pasteXMLButton;

        SpriteRenderer2DPtr m_gradientRendererHolder;
        SpriteRenderer2DPtr m_gradientRenderer;
        ClickButton2DPtr m_gradientClickButton;
        Vec2F m_gradientClickPosition = Vec2F::c_zero;

        EntityPtr m_alphaHolder;
        Slider2DPtr m_alphaSlider;
        EditBox2DPtr m_alphaEdit;

        EntityPtr m_colorHolder;
        ColorHDREdit2DPtr m_colorEdit;

        EntityPtr m_locationHolder;
        EditBox2DPtr m_locationEdit;

        Vector<ColorSliderTag2DPtr> m_alphaTags;
        Vector<ColorSliderTag2DPtr> m_colorTags;

        EditMode m_editMode = EditMode::None;
        Size m_colorTagIndex = -1;
        bool m_draggingTag = false;

        Vec2F m_draggingTagPrevCursorPositionGradientSpace = Vec2F::c_zero;
        F32 m_draggingTagDistance = 0.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneColorGradientPicker_hpp_
//////////////////////////////////////////
