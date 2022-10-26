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
#if (!defined(_MazeSceneColorPicker_hpp_))
#define _MazeSceneColorPicker_hpp_


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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneColorPicker);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SceneColorPicker
    //
    //////////////////////////////////////////
    class MAZE_UI_API SceneColorPicker
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneColorPicker, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneColorPickerPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneColorPicker();

        //////////////////////////////////////////
        void setup(bool _hdr);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneColorPicker();

        //////////////////////////////////////////
        // using ECSRenderScene::init;

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void createSystems();


        //////////////////////////////////////////
        void notifyColorChanged(ColorU32 const& _color);

        //////////////////////////////////////////
        void notifyColorHDRChanged(ColorF128 const& _color);

        //////////////////////////////////////////
        void updateUI();

        //////////////////////////////////////////
        void notifyHSVRectCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyHSVRectCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyHSVRectCursorMoveOut(CursorInputEvent const& _inputEvent);
        
        //////////////////////////////////////////
        void processHSVRectPick(Vec2DF const& _positionOS);


        //////////////////////////////////////////
        void notifyHSVBandCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyHSVBandCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyHSVBandCursorMoveOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void processHSVBandPick(Vec2DF const& _positionOS);


        //////////////////////////////////////////
        void notifyRGBBandCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyRGBBandCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyRGBBandCursorMoveOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void processRGBBandPick(S32 _channel, Vec2DF const& _positionOS);

        //////////////////////////////////////////
        void notifyRGBATextInput(EditBox2D* _editBox);

        //////////////////////////////////////////
        void notifyRGBATextSelectedChanged(EditBox2D* _editBox, bool _selected);

        //////////////////////////////////////////
        void notifyIntensitySliderValueChanged(Slider2D* _slider, F32 _value);

        //////////////////////////////////////////
        void notifyIntensityTextInput(EditBox2D* _editBox);

        //////////////////////////////////////////
        void notifyIntensityTextSelectedChanged(EditBox2D* _editBox, bool _selected);

        //////////////////////////////////////////
        void processIntensityFromEditBox();

        //////////////////////////////////////////
        void processIntensityFromSlider();

        //////////////////////////////////////////
        void processRGBAFromEditBox(EditBox2D* _editBox);

        //////////////////////////////////////////
        void processRGBtoHSV();

        //////////////////////////////////////////
        void processHSVtoRGB();


        //////////////////////////////////////////
        void setHSV(
            Vec3DF const& _hsv,
            U8 _alpha,
            F32 _intensity);

    protected:
        bool m_hdr;

        CanvasPtr m_canvas;

        ClickButton2DPtr m_copyButton;
        ClickButton2DPtr m_pasteButton;

        ColorU32 m_prevColor;
        F32 m_prevColorIntensity;

        SpriteRenderer2DPtr m_prevColorRenderer;
        SpriteRenderer2DPtr m_finalColorRenderer;

        SpriteRenderer2DPtr m_hsvRectRenderer;
        UIElement2DPtr m_hsvRectButton;
        SpriteRenderer2DPtr m_hsvRectCircleRenderer;

        UIElement2DPtr m_hsvBandButton;
        Transform2DPtr m_hsvBandScale;

        SpriteRenderer2DPtr m_hsvBandRenderer;

        SystemTextRenderer2DPtr m_floatLabel;

        SpriteRenderer2DPtr m_rgbaRenderers[4];
        UIElement2DPtr m_rgbaButtons[4];
        SpriteRenderer2DPtr m_rgbaCarriages[4];
        EditBox2DPtr m_rgbaTextEdits[4];

        EditBox2DPtr m_hexadecimalTextEdit;


        Vec3DF m_hsv;
        U8 m_alpha;
        F32 m_intensity;

        EntityPtr m_intensityEntity;
        Slider2DPtr m_intensitySlider;
        EditBox2DPtr m_intensityTextEdit;

        bool m_setup = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneColorPicker_hpp_
//////////////////////////////////////////