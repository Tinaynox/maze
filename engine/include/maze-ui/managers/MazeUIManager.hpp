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
#if (!defined(_MazeUIManager_hpp_))
#define _MazeUIManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-graphics/MazeTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(UIManager);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(ColorPickerManager);
    MAZE_USING_SHARED_PTR(ColorGradientPickerManager);
    MAZE_USING_SHARED_PTR(AnimationCurveManager);
    MAZE_USING_SHARED_PTR(MaterialPickerManager);
    MAZE_USING_SHARED_PTR(RenderMeshPickerManager);
    MAZE_USING_SHARED_PTR(TexturePickerManager);


    //////////////////////////////////////////
    enum class DefaultUISprite
    {
        DropDownButtonCollapsed = 0,
        DropDownButtonExpanded,
        EntityObject3D,
        EntityObject2D,
        EntityObject,
        Scene,
        MainScene,
        Panel00Default,
        Panel00Focused,
        Panel00Selected,
        Panel00Flat,
        Panel01Default,
        Panel01Focused,
        Panel01Selected,
        Panel02,
        Panel03,
        Frame01,
        ColorPickerCircle,
        ScaleMark,
        ColorSliderTagFrame,
        ColorSliderTagBody,
        SubMenuMark,
        CheckMark,
        TransparentChess,
        SliderHandle,
        FolderClosed,
        FolderOpened,
        File,
        TextFile,
        Material,
        Mesh,
        PhysicsMaterial2D,
        Shader,
        SelectAsset,
        
        MAX
    };


    //////////////////////////////////////////
    // Class UIManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIManager
    {
    public:

        //////////////////////////////////////////
        virtual ~UIManager();

        //////////////////////////////////////////
        static void Initialize(UIManagerPtr& _uiManager);


        //////////////////////////////////////////
        static inline UIManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline UIManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        SpritePtr const& getDefaultUISprite(DefaultUISprite uiSprite) const { return m_defaultUISprites[(Size)uiSprite];}


        //////////////////////////////////////////
        void createUIElements();

    protected:

        //////////////////////////////////////////
        UIManager();

        //////////////////////////////////////////
        bool init();

        
        //////////////////////////////////////////
        void buildPanel(
            PixelSheet2D& _uiElementsSheet,
            Vec2DS const& _startPos,
            Vec2DS const& _size,
            ColorU32 const& _backgroundColor,
            ColorU32 const& _sideBordersColor,
            ColorU32 const& _topBorderColor);

    protected:
        static UIManager* s_instance;

        ColorPickerManagerPtr m_colorPickerManager;
        ColorGradientPickerManagerPtr m_colorGradientPickerManager;
        AnimationCurveManagerPtr m_animationCurveManager;
        MaterialPickerManagerPtr m_materialPickerManager;
        RenderMeshPickerManagerPtr m_renderMeshPickerManager;
        TexturePickerManagerPtr m_texturePickerManager;

        Texture2DPtr m_uiElementsTexture;
        SpritePtr m_defaultUISprites[(Size)DefaultUISprite::MAX];

        Texture2DPtr m_transparentChessTexture;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIManager_hpp_
//////////////////////////////////////////
