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
#if (!defined(_MazeSceneCurveEditor_hpp_))
#define _MazeSceneCurveEditor_hpp_


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
    MAZE_USING_SHARED_PTR(SceneCurveEditor);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SceneCurveEditor
    //
    //////////////////////////////////////////
    class MAZE_UI_API SceneCurveEditor
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneCurveEditor, ECSRenderScene);


        //////////////////////////////////////////
        enum CurveElement
        {
            None,
            Key,
            InTangent,
            OutTangent,
        };

    public:

        //////////////////////////////////////////
        static SceneCurveEditorPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneCurveEditor();

        //////////////////////////////////////////
        void setup(AnimationCurveMinMaxMode _mode);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneCurveEditor();       

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyCurveChanged(AnimationCurve const& _curve);

        //////////////////////////////////////////
        void updateUI();


        //////////////////////////////////////////
        void notifyModeChanged(Dropdown2D* _dropdown, S32 _index);


        //////////////////////////////////////////
        void processCurve();


        //////////////////////////////////////////
        void notifyCurveClickButtonClick(Button2D* _button, CursorInputEvent const& _event);
        
        //////////////////////////////////////////
        void notifyCurveClickButtonCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _event);


        //////////////////////////////////////////
        Vec2DF getCurveOrigin();

        //////////////////////////////////////////
        F32 getCurveHeightScalar();

        //////////////////////////////////////////
        CurveElement getFocusedElement(S32& _key);


        //////////////////////////////////////////
        void changeCurrentKey(F32 _newTime, F32 _newValue);


        //////////////////////////////////////////
        void notifyCanvasCursorReleaseIn(Vec2DF const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyCanvasCursorReleaseOut(CursorInputEvent const& _event);

        //////////////////////////////////////////
        void processDragging();

        //////////////////////////////////////////
        void startDragging(CurveElement _element);


        //////////////////////////////////////////
        Vec2DF getKeyPoint(Size _keyIndex);

        //////////////////////////////////////////
        Vec2DF getInTangentPoint(Size _keyIndex);

        //////////////////////////////////////////
        Vec2DF getOutTangentPoint(Size _keyIndex);


        //////////////////////////////////////////
        void notifyCurveScalarEditTextInput(EditBox2D* _edit);


        //////////////////////////////////////////
        void notifyValueEditTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void notifyLocationEditTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void notifyInTangentEditTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void notifyOutTangentEditTextInput(EditBox2D* _edit);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_canvasUIElement;

        AnimationCurveMinMaxMode m_minMaxMode = AnimationCurveMinMaxMode::None;

        Dropdown2DPtr m_modeDropdown;
        ClickButton2DPtr m_copyButton;
        ClickButton2DPtr m_pasteButton;
        ClickButton2DPtr m_copyXMLButton;
        ClickButton2DPtr m_pasteXMLButton;

        SpriteRenderer2DPtr m_curveRendererHolder;
        SpriteRenderer2DPtr m_curveRenderer;
        ClickButton2DPtr m_curveClickButton;
        Vec2DF m_curveClickPosition = Vec2DF::c_zero;

        PixelSheet2D m_curvePixelSheet;
        Texture2DPtr m_curveTexture;
        SpritePtr m_curveSprite;

        EditBox2DPtr m_curveScalarEdit;

        S32 m_keyIndex = -1;

        CurveElement m_draggingElement = CurveElement::None;
        Vec2DF m_draggingPrevCursorPositionCurveSpace = Vec2DF::c_zero;
        F32 m_draggingDistance = 0.0f;

        EntityPtr m_keyHolder;
        EditBox2DPtr m_valueEdit;
        EditBox2DPtr m_locationEdit;
        EntityPtr m_tangentsHolder;
        EditBox2DPtr m_inTangentEdit;
        EditBox2DPtr m_outTangentEdit;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneCurveEditor_hpp_
//////////////////////////////////////////
