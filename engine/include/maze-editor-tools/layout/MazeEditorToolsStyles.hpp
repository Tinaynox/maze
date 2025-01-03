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
#if (!defined(_MazeEditorToolsStyles_hpp_))
#define _MazeEditorToolsStyles_hpp_


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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorToolsStyles);
    MAZE_USING_SHARED_PTR(FontMaterial);


    //////////////////////////////////////////
    // Class EditorToolsStyles
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorToolsStyles
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        static F32 const c_titleHeightDefault;
        static ColorU32 const c_titleBackgroundColorDefault;
        static U32 const c_titleFontSizeDefault;
        static F32 const c_titleLabelShiftDefault;
        static ColorU32 const c_bodyBackgroundColorDefault;
        static ColorU32 const c_bodySubBackgroundColorDefault;
        static U32 const c_inspectorPropertyFontSizeDefault;
        static ColorU32 const c_inspectorPropertyColorDefault;
        static Rect2F const c_hierarchyViewportDefault;
        static Rect2F const c_inspectorViewportDefault;
        static Rect2F const c_assetsViewportDefault;
        static Rect2F const c_sceneViewportDefault;
        static Rect2F const c_topBarViewportDefault;

        static ColorU32 const c_listObjectTextColorDefault;
        static ColorU32 const c_listObjectTextColorSelected;
        static ColorU32 const c_listObjectTextColorInactive;
        static ColorU32 const c_listObjectBackgroundColorDefault;
        static ColorU32 const c_listObjectBackgroundColorFocused;
        static ColorU32 const c_listObjectBackgroundColorSelected;

    public:
        //////////////////////////////////////////
        virtual ~EditorToolsStyles();

        //////////////////////////////////////////
        static void Initialize(EditorToolsStylesPtr& _gizmosManager);


        //////////////////////////////////////////
        static inline EditorToolsStyles* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorToolsStyles& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline F32 getTitleHeight() const { return m_titleHeight; }

        //////////////////////////////////////////
        inline ColorU32 const& getTitleBackgroundColor() const { return m_titleBackgroundColor; }

        //////////////////////////////////////////
        inline U32 getTitleFontSize() const { return m_titleFontSize; }

        //////////////////////////////////////////
        inline F32 getTitleLabelShift() const { return m_titleLabelShift; }

        //////////////////////////////////////////
        inline ColorU32 const& getBodyBackgroundColor() const { return m_bodyBackgroundColor; }

        //////////////////////////////////////////
        inline ColorU32 const& getBodySubBackgroundColor() const { return m_bodySubBackgroundColor; }

        //////////////////////////////////////////
        inline U32 getInspectorPropertyFontSize() const { return m_inspectorPropertyFontSize; }

        //////////////////////////////////////////
        inline ColorU32 const& getInspectorPropertyColor() const { return m_inspectorPropertyColor; }

        //////////////////////////////////////////
        inline Rect2F const& getHierarchyViewport() const { return m_hierarchyViewport; }

        //////////////////////////////////////////
        inline Rect2F const& getInspectorViewport() const { return m_inspectorViewport; }

        //////////////////////////////////////////
        inline Rect2F const& getAssetsViewport() const { return m_assetsViewport; }

        //////////////////////////////////////////
        inline Rect2F const& getSceneViewport() const { return m_sceneViewport; }

        //////////////////////////////////////////
        inline Rect2F const& getTopBarViewport() const { return m_topBarViewport; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjectTextColorDefault() const { return m_listObjectTextColorDefault; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjectTextColorSelected() const { return m_listObjectTextColorSelected; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjectTextColorInactive() const { return m_listObjectTextColorInactive; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjectBackgroundColorDefault() const { return m_listObjectBackgroundColorDefault; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjectBackgroundColorFocused() const { return m_listObjectBackgroundColorFocused; }

        //////////////////////////////////////////
        inline ColorU32 const& getListObjecBackgroundColorSelected() const { return m_listObjectBackgroundColorSelected; }

        //////////////////////////////////////////
        inline FontMaterialPtr const& getDefaultFontMaterial() const { return m_defaultFontMaterial; }

        //////////////////////////////////////////
        inline FontMaterialPtr const& getDefaultBoldFontMaterial() const { return m_defaultBoldFontMaterial; }


        //////////////////////////////////////////
        void loadStyles(Path const& _fileName);

    protected:
        //////////////////////////////////////////
        EditorToolsStyles();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        bool loadFromDataBlock(DataBlock const& _dataBlock);    


    protected:
        static EditorToolsStyles* s_instance;

    private:
        F32 m_titleHeight;
        ColorU32 m_titleBackgroundColor;
        U32 m_titleFontSize;
        F32 m_titleLabelShift;
        ColorU32 m_bodyBackgroundColor;
        ColorU32 m_bodySubBackgroundColor;
        U32 m_inspectorPropertyFontSize;
        ColorU32 m_inspectorPropertyColor;
        Rect2F m_hierarchyViewport;
        Rect2F m_inspectorViewport;
        Rect2F m_assetsViewport;
        Rect2F m_sceneViewport;
        Rect2F m_topBarViewport;

        ColorU32 m_listObjectTextColorDefault;
        ColorU32 m_listObjectTextColorSelected;
        ColorU32 m_listObjectTextColorInactive;
        ColorU32 m_listObjectBackgroundColorDefault;
        ColorU32 m_listObjectBackgroundColorFocused;
        ColorU32 m_listObjectBackgroundColorSelected;

        String m_defaultFontMaterialName;
        FontMaterialPtr m_defaultFontMaterial;
        String m_defaultBoldFontMaterialName;
        FontMaterialPtr m_defaultBoldFontMaterial;


    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsStyles_hpp_
//////////////////////////////////////////
