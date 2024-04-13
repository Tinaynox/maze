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
#if (!defined(_MazeSceneTexturePicker_hpp_))
#define _MazeSceneTexturePicker_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTexture.hpp"
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
    MAZE_USING_SHARED_PTR(SceneTexturePicker);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);


    //////////////////////////////////////////
    // Class SceneTexturePicker
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SceneTexturePicker
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneTexturePicker, ECSRenderScene);

    public:

        //////////////////////////////////////////
        struct TexturePreviewData
        {
            Texture2DPtr texture;
            Transform2DPtr bodyTransform;
            ToggleButton2DPtr button;
            SystemTextRenderer2DPtr titleText;
        };

    public:

        //////////////////////////////////////////
        static SceneTexturePickerPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneTexturePicker();

        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneTexturePicker();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyTextureChanged(Texture2DPtr const& _material);

        //////////////////////////////////////////
        void updateTextures();

        //////////////////////////////////////////
        void updateUI();


        //////////////////////////////////////////
        void notifyCanvasCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyCanvasCursorReleaseOut(CursorInputEvent const& _event);


        //////////////////////////////////////////
        TexturePreviewData createTexturePreview(Texture2DPtr const& _material);


        //////////////////////////////////////////
        void clearPreviews();

        //////////////////////////////////////////
        void notifyButtonClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyFilterTextInput(EditBox2D* _editBox);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_canvasUIElement;

        EditBox2DPtr m_filterEditBox;

        Vector<TexturePreviewData> m_previews;
        VerticalLayout2DPtr m_layout;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneTexturePicker_hpp_
//////////////////////////////////////////