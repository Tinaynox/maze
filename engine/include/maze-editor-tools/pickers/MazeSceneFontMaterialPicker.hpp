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
#if (!defined(_MazeSceneFontMaterialPicker_hpp_))
#define _MazeSceneFontMaterialPicker_hpp_


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
#include "maze-ui/fonts/MazeFontMaterial.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneFontMaterialPicker);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);


    //////////////////////////////////////////
    // Class SceneFontMaterialPicker
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SceneFontMaterialPicker
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneFontMaterialPicker, EcsRenderScene);

    public:

        //////////////////////////////////////////
        struct FontMaterialPreviewData
        {
            FontMaterialPtr material;
            Transform2DPtr bodyTransform;
            ToggleButton2DPtr button;
            AbstractTextRenderer2DPtr titleText;
        };

    public:

        //////////////////////////////////////////
        static SceneFontMaterialPickerPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneFontMaterialPicker();

        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneFontMaterialPicker();


        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyFontMaterialChanged(FontMaterialPtr const& _material);

        //////////////////////////////////////////
        void updateFontMaterials();

        //////////////////////////////////////////
        void updateUI();


        //////////////////////////////////////////
        void notifyCanvasCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyCanvasCursorReleaseOut(CursorInputEvent& _event);


        //////////////////////////////////////////
        FontMaterialPreviewData createFontMaterialPreview(FontMaterialPtr const& _material);


        //////////////////////////////////////////
        void clearPreviews();

        //////////////////////////////////////////
        void notifyButtonClick(Button2D* _button, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyFilterTextInput(EditBox2D* _editBox);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_canvasUIElement;

        EditBox2DPtr m_filterEditBox;

        Vector<FontMaterialPreviewData> m_previews;
        VerticalLayout2DPtr m_layout;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneFontMaterialPicker_hpp_
//////////////////////////////////////////