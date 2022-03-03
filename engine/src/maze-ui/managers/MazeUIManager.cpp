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
#include "MazeUIHeader.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/managers/MazeColorGradientPickerManager.hpp"
#include "maze-ui/managers/MazeAnimationCurveManager.hpp"
#include "maze-ui/managers/MazeMaterialPickerManager.hpp"
#include "maze-ui/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-ui/managers/MazeTexturePickerManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeColorEdit2D.hpp"
#include "maze-ui/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-ui/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-ui/ecs/components/MazeAnimationCurveEdit2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-ui/ecs/components/MazeMenuBar2D.hpp"
#include "maze-ui/ecs/components/MazeMenuList2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeExperimental.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UIManager
    //
    //////////////////////////////////////////
    UIManager* UIManager::s_instance = nullptr;

    //////////////////////////////////////////
    UIManager::UIManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    UIManager::~UIManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void UIManager::Initialize(UIManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(UIManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool UIManager::init()
    {
        ColorPickerManager::Initialize(m_colorPickerManager);
        if (!m_colorPickerManager)
            return false;

        ColorGradientPickerManager::Initialize(m_colorGradientPickerManager);
        if (!m_colorGradientPickerManager)
            return false;

        AnimationCurveManager::Initialize(m_animationCurveManager);
        if (!m_animationCurveManager)
            return false;

        MaterialPickerManager::Initialize(m_materialPickerManager);
        if (!m_materialPickerManager)
            return false;

        RenderMeshPickerManager::Initialize(m_renderMeshPickerManager);
        if (!m_renderMeshPickerManager)
            return false;

        TexturePickerManager::Initialize(m_texturePickerManager);
        if (!m_texturePickerManager)
            return false;

        
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ClickButton2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorEdit2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorHDREdit2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorGradientEdit2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<AnimationCurveEdit2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ContextMenu2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ContextMenuCanvas2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<HorizontalLayout2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<VerticalLayout2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MenuBar2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MenuList2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MenuListItem2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MenuListTree2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Slider2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SystemTextDropdown2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SystemTextEditBox2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ToggleButton2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<UIElement2D>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<UITweenTransitionAlpha>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<UITweenTransitionScale>("UI");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Experimental>("UI");

        return true;
    }

    //////////////////////////////////////////
    void UIManager::createUIElements()
    {
        S32 const chunkSize = 32;
        S32 const gap = 1;

        auto startPosFunc = [&](Vec2DS const& _pos) { return (chunkSize * _pos) + Vec2DS((_pos.x + 1) * gap, (_pos.y + 1) * gap); };

        PixelSheet2D uiElementsSheet(Vec2DS(8, 8) * chunkSize, PixelFormat::RGBA_U8);
        uiElementsSheet.fill(ColorU32::c_transparent);

        m_uiElementsTexture = Texture2D::Create();
        m_uiElementsTexture->setMagFilter(TextureFilter::Nearest);
        m_uiElementsTexture->setMinFilter(TextureFilter::Linear);

        // Drop down button collapsed
        {
            Vec2DS sheetPos(0, 0);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(10, 6) + startPos,
                Vec2DS(22, 14) + startPos,
                Vec2DS(10, 22) + startPos,
                ColorU32::c_white);
            
            m_defaultUISprites[(Size)DefaultUISprite::DropDownButtonCollapsed] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Drop down button expanded
        {
            Vec2DS sheetPos(1, 0);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(8, 20) + startPos,
                Vec2DS(16, 6) + startPos,
                Vec2DS(24, 20) + startPos,
                ColorU32::c_white);

            m_defaultUISprites[(Size)DefaultUISprite::DropDownButtonExpanded] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Entity Object
        {
            Vec2DS sheetPos(2, 0);
            Vec2DS startPos = startPosFunc(sheetPos);

            Vec2DS outerBottom(14, 2);
            Vec2DS outerTop(14, 29);
            Vec2DS outerBottomLeft(2, 7);
            Vec2DS outerTopLeft(2, 24);
            Vec2DS outerBottomRight(26, 7);
            Vec2DS outerTopRight(26, 24);

            uiElementsSheet.drawFilledTriangle(
                outerTopLeft + startPos,
                outerTopRight + startPos,
                outerTop + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawFilledTriangle(
                outerTopLeft + startPos,
                outerBottomLeft + startPos,
                outerBottomRight + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawFilledTriangle(
                outerTopRight + startPos,
                outerTopLeft + startPos,
                outerBottomRight + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawFilledTriangle(
                outerBottomLeft + startPos,
                outerBottom + startPos,
                outerBottomRight + startPos,
                ColorU32::c_white);
            
            uiElementsSheet.drawFilledTriangle(
                outerTopLeft + Vec2DS(2 + 1, - 2) + startPos,
                outerTopRight + Vec2DS(-2 - 1, - 2) + startPos,
                outerTop  + Vec2DS(0, -2 - 1) + startPos,
                ColorU32::c_transparent);
            uiElementsSheet.drawFilledTriangle(
                outerTopLeft + Vec2DS(2 + 1, - 2) + startPos,
                outerTopRight + Vec2DS(-2 - 1, - 2) + startPos,
                outerTop + Vec2DS(0, -(outerTop.y - outerTopLeft.y) * 2)  + Vec2DS(0, +2 - 3) + startPos,
                ColorU32::c_transparent);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(4, 9) + startPos,
                Vec2DS(4, 21 - 2) + startPos,
                Vec2DS(12, 18 - 2) + startPos,
                ColorU32::c_transparent);
            uiElementsSheet.drawFilledTriangle(
                Vec2DS(4, 9) + startPos,
                Vec2DS(12, 6) + startPos,
                Vec2DS(12, 18 - 2) + startPos,
                ColorU32::c_transparent);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(4 + 12, 6) + startPos,
                Vec2DS(4 + 12, 18 - 2) + startPos,
                Vec2DS(12 + 12, 21 - 2) + startPos,
                ColorU32::c_transparent);
            uiElementsSheet.drawFilledTriangle(
                Vec2DS(4 + 12, 6) + startPos,
                Vec2DS(12 + 12, 9) + startPos,
                Vec2DS(12 + 12, 21 - 2) + startPos,
                ColorU32::c_transparent);

            m_defaultUISprites[(Size)DefaultUISprite::EntityObject3D] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Scene
        {
            Vec2DS sheetPos(3, 0);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2, 2) + startPos,
                Vec2DS(28, 28),
                ColorU32::c_white);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 3, 2 + 3) + startPos,
                Vec2DS(28 - 6, 28 - 6),
                ColorU32::c_transparent);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 6, 2 + 6) + startPos,
                Vec2DS(28 - 12, 28 - 12),
                ColorU32::c_white);

            m_defaultUISprites[(Size)DefaultUISprite::Scene] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Panel00
        {
            Vec2DS sheetPos(0, 1);
            Vec2DS startPos = startPosFunc(sheetPos);
            Vec2DS size(7, 7);
            SpriteSliceBorder sliceBorder(3, 3, 3, 3);

            // Default
            {
                Vec2DS pos = startPos + Vec2DS(0, 0);
                static ColorU32 const backgroundColor = ColorU32(240, 240, 240);
                static ColorU32 const sideBordersColor = ColorU32(184, 184, 184);
                static ColorU32 const topBorderColor = ColorU32(161, 161, 161);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel00Default] = sprite;
            }

            // Focused
            {
                Vec2DS pos = startPos + Vec2DS(8, 0);
                static ColorU32 const backgroundColor = ColorU32(240, 240, 240);
                static ColorU32 const sideBordersColor = ColorU32(108, 108, 108);
                static ColorU32 const topBorderColor = ColorU32(108, 108, 108);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel00Focused] = sprite;
            }

            // Selected
            {
                Vec2DS pos = startPos + Vec2DS(16, 0);
                static ColorU32 const backgroundColor = ColorU32(237, 237, 237);
                static ColorU32 const sideBordersColor = ColorU32(76, 76, 76);
                static ColorU32 const topBorderColor = ColorU32(76, 76, 76);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel00Selected] = sprite;
            }

            // Flat
            {
                Vec2DS pos = startPos + Vec2DS(0, 8);
                static ColorU32 const backgroundColor = ColorU32(255, 255, 255);
                static ColorU32 const sideBordersColor = ColorU32(255, 255, 255);
                static ColorU32 const topBorderColor = ColorU32(255, 255, 255);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel00Flat] = sprite;
            }
        }

        // Panel01
        {
            Vec2DS sheetPos(1, 1);
            Vec2DS startPos = startPosFunc(sheetPos);
            Vec2DS size(7, 7);
            SpriteSliceBorder sliceBorder(3, 3, 3, 3);

            // Default
            {
                Vec2DS pos = startPos + Vec2DS(0, 0);
                static ColorU32 const backgroundColor = ColorU32::c_transparent;
                static ColorU32 const sideBordersColor = ColorU32(184, 184, 184);
                static ColorU32 const topBorderColor = ColorU32(161, 161, 161);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel01Default] = sprite;
            }

            // Focused
            {
                Vec2DS pos = startPos + Vec2DS(8, 0);
                static ColorU32 const backgroundColor = ColorU32::c_transparent;
                static ColorU32 const sideBordersColor = ColorU32(108, 108, 108);
                static ColorU32 const topBorderColor = ColorU32(108, 108, 108);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel01Focused] = sprite;
            }

            // Selected
            {
                Vec2DS pos = startPos + Vec2DS(16, 0);
                static ColorU32 const backgroundColor = ColorU32::c_transparent;
                static ColorU32 const sideBordersColor = ColorU32(76, 76, 76);
                static ColorU32 const topBorderColor = ColorU32(76, 76, 76);

                buildPanel(
                    uiElementsSheet,
                    pos,
                    size,
                    backgroundColor,
                    sideBordersColor,
                    topBorderColor);

                SpritePtr sprite = Sprite::Create(
                    m_uiElementsTexture,
                    pos,
                    size);
                sprite->setSliceBorder(sliceBorder);
                m_defaultUISprites[(Size)DefaultUISprite::Panel01Selected] = sprite;
            }
        }

        // Check Mark
        {
            Vec2DS sheetPos(0, 1);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(0, 16);

            uiElementsSheet.drawLine(
                Vec2DS(4, 6) + startPos,
                Vec2DS(6, 4) + startPos,
                ColorU32(255, 255, 255, 100));

            uiElementsSheet.drawLine(
                Vec2DS(7, 5) + startPos,
                Vec2DS(11, 9) + startPos,
                ColorU32(255, 255, 255, 100));

            uiElementsSheet.drawLine(
                Vec2DS(5, 7) + startPos,
                Vec2DS(6, 6) + startPos,
                ColorU32(255, 255, 255, 100));

            uiElementsSheet.drawLine(
                Vec2DS(7, 7) + startPos,
                Vec2DS(10, 10) + startPos,
                ColorU32(255, 255, 255, 100));

            uiElementsSheet.drawLine(
                Vec2DS(6, 5) + startPos,
                Vec2DS(11, 10) + startPos,
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(4, 7) + startPos,
                Vec2DS(6, 5) + startPos,
                ColorU32::c_white);


            m_defaultUISprites[(Size)DefaultUISprite::CheckMark] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(16.0f, 16.0f));
        }

        // Slider Handle
        {
            Vec2DS sheetPos(1, 1);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(0, 16);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(8, 8) + startPos,
                8,
                ColorU32::c_white);

            uiElementsSheet.drawCircle(
                Vec2DS(8, 8) + startPos,
                8,
                ColorU32(100, 100, 100));

            m_defaultUISprites[(Size)DefaultUISprite::SliderHandle] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(17.0f, 17.0f));
        }

        // ScaleMark
        {
            Vec2DS sheetPos(0, 2);
            Vec2DS startPos = startPosFunc(sheetPos);

            ColorU32 borderColor(116, 116, 116);

            uiElementsSheet.drawFilledRect(
                Vec2DS(1, 1) + startPos,
                Vec2DS(3, 7),
                ColorU32::c_white);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(3, 1) + startPos,
                Vec2DS(6, 4) + startPos,
                Vec2DS(3, 7) + startPos,
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(0, 1) + startPos,
                Vec2DS(0, 7) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(1, 0) + startPos,
                Vec2DS(3, 0) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(1, 0) + startPos,
                Vec2DS(3, 0) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(1, 8) + startPos,
                Vec2DS(3, 8) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(4, 1) + startPos,
                Vec2DS(7, 4) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(4, 7) + startPos,
                Vec2DS(6, 5) + startPos,
                borderColor);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(8, 9));

            m_defaultUISprites[(Size)DefaultUISprite::ScaleMark] = sprite;
        }

        // ColorSliderTagFrame
        {
            Vec2DS sheetPos(0, 2);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(0, 10);

            ColorU32 borderColor(255, 255, 255);

            uiElementsSheet.drawLine(
                Vec2DS(4, 0) + startPos,
                Vec2DS(0, 4) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(0, 4) + startPos,
                Vec2DS(0, 13) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(0, 13) + startPos,
                Vec2DS(8, 13) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(8, 13) + startPos,
                Vec2DS(8, 4) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(8, 4) + startPos,
                Vec2DS(4, 0) + startPos,
                borderColor);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(9, 14));

            m_defaultUISprites[(Size)DefaultUISprite::ColorSliderTagFrame] = sprite;
        }

        // ColorSliderTagBody
        {
            Vec2DS sheetPos(0, 2);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(16, 10);

            ColorU32 color(255, 255, 255);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(4, 0) + startPos,
                Vec2DS(0, 4) + startPos,
                Vec2DS(8, 4) + startPos,
                color);

            uiElementsSheet.drawFilledRect(
                Vec2DS(0, 4) + startPos,
                Vec2DS(9, 10),
                color);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(9, 14));

            m_defaultUISprites[(Size)DefaultUISprite::ColorSliderTagBody] = sprite;
        }

        // SubMenuMark
        {
            Vec2DS sheetPos(0, 2);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(16, 0);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(0, 0) + startPos,
                Vec2DS(3, 3) + startPos,
                Vec2DS(0, 6) + startPos,
                ColorU32::c_white);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(4, 7));

            m_defaultUISprites[(Size)DefaultUISprite::SubMenuMark] = sprite;
        }

        // Panel02
        {
            Vec2DS sheetPos(2, 1);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(0, 16);
            SpriteSliceBorder sliceBorder(3, 3, 3, 3);

            ColorU32 const borderColor(230, 230, 230);

            uiElementsSheet.drawFilledRect(
                Vec2DS(0, 0) + startPos,
                Vec2DS(7, 7),
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(0, 6) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(6, 0) + startPos,
                Vec2DS(6, 6) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(6, 0) + startPos,
                borderColor);
            uiElementsSheet.drawLine(
                Vec2DS(0, 6) + startPos,
                Vec2DS(6, 6) + startPos,
                borderColor);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(7, 7));
            sprite->setSliceBorder(sliceBorder);

            m_defaultUISprites[(Size)DefaultUISprite::Panel02] = sprite;
        }

        // Panel02
        {
            Vec2DS sheetPos(2, 1);
            Vec2DS startPos = startPosFunc(sheetPos) + Vec2DS(16, 16);
            SpriteSliceBorder sliceBorder(3, 3, 3, 3);

            ColorU32 const borderColor(160, 160, 160);

            uiElementsSheet.drawFilledRect(
                Vec2DS(0, 0) + startPos,
                Vec2DS(7, 7),
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(0, 6) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(6, 0) + startPos,
                Vec2DS(6, 6) + startPos,
                borderColor);

            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(6, 0) + startPos,
                borderColor);
            uiElementsSheet.drawLine(
                Vec2DS(0, 6) + startPos,
                Vec2DS(6, 6) + startPos,
                borderColor);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(7, 7));
            sprite->setSliceBorder(sliceBorder);

            m_defaultUISprites[(Size)DefaultUISprite::Panel03] = sprite;
        }

        // Frame01
        {
            Vec2DS sheetPos(2, 1);
            Vec2DS startPos = startPosFunc(sheetPos);
            SpriteSliceBorder sliceBorder(3, 3, 3, 3);

            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(0, 6) + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawLine(
                Vec2DS(1, 0) + startPos,
                Vec2DS(1, 6) + startPos,
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(5, 0) + startPos,
                Vec2DS(5, 6) + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawLine(
                Vec2DS(6, 0) + startPos,
                Vec2DS(6, 6) + startPos,
                ColorU32::c_white);


            uiElementsSheet.drawLine(
                Vec2DS(0, 0) + startPos,
                Vec2DS(6, 0) + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawLine(
                Vec2DS(0, 1) + startPos,
                Vec2DS(6, 1) + startPos,
                ColorU32::c_white);

            uiElementsSheet.drawLine(
                Vec2DS(0, 5) + startPos,
                Vec2DS(6, 5) + startPos,
                ColorU32::c_white);
            uiElementsSheet.drawLine(
                Vec2DS(0, 6) + startPos,
                Vec2DS(6, 6) + startPos,
                ColorU32::c_white);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(7, 7));
            sprite->setSliceBorder(sliceBorder);

            m_defaultUISprites[(Size)DefaultUISprite::Frame01] = sprite;
        }

        // ColorPickerCircle
        {
            Vec2DS sheetPos(3, 1);
            Vec2DS startPos = startPosFunc(sheetPos);


            uiElementsSheet.drawCircle(
                6 + startPos.x,
                6 + startPos.y,
                6,
                ColorU32::c_white);

            SpritePtr sprite = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(13, 13));

            m_defaultUISprites[(Size)DefaultUISprite::ColorPickerCircle] = sprite;
        }

        // MainScene
        {
            Vec2DS sheetPos(0, 3);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2, 2) + startPos,
                Vec2DS(28, 28),
                ColorU32::c_white);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 3, 2 + 3) + startPos,
                Vec2DS(28 - 6, 28 - 6),
                ColorU32::c_transparent);

            Vec2DS starStartPos = Vec2DS(2 + 6, 2 + 6) + startPos;
            
            uiElementsSheet.drawFilledTriangle(
                starStartPos + Vec2DS(3, 0),
                starStartPos + Vec2DS(8, 15),
                starStartPos + Vec2DS(10, 6),
                ColorU32::c_white);

            uiElementsSheet.drawFilledTriangle(
                starStartPos + Vec2DS(12, 0),
                starStartPos + Vec2DS(8, 15),
                starStartPos + Vec2DS(5, 6),
                ColorU32::c_white);

            uiElementsSheet.drawFilledTriangle(
                starStartPos + Vec2DS(0, 9),
                starStartPos + Vec2DS(15, 9),
                starStartPos + Vec2DS(7, 4),
                ColorU32::c_white);

            uiElementsSheet.setPixel(
                starStartPos + Vec2DS(11, 6),
                ColorU32::c_white);

            m_defaultUISprites[(Size)DefaultUISprite::MainScene] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // EntityObject2D
        {
            Vec2DS sheetPos(1, 3);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2, 2) + startPos,
                Vec2DS(28, 28),
                ColorU32::c_white);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 3, 2 + 3) + startPos,
                Vec2DS(28 - 6, 28 - 6),
                ColorU32::c_transparent);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 6, 2 + 6) + startPos,
                Vec2DS(28 - 12, 28 - 12),
                ColorU32::c_white);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 9, 2 + 9) + startPos,
                Vec2DS(28 - 18, 28 - 18),
                ColorU32::c_transparent);

            m_defaultUISprites[(Size)DefaultUISprite::EntityObject2D] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // EntityObject
        {
            Vec2DS sheetPos(2, 3);
            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2, 2) + startPos,
                Vec2DS(28, 28),
                ColorU32::c_white);

            uiElementsSheet.drawFilledRect(
                Vec2DS(2 + 3, 2 + 3) + startPos,
                Vec2DS(28 - 6, 28 - 6),
                ColorU32::c_transparent);

            m_defaultUISprites[(Size)DefaultUISprite::EntityObject] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // FolderClosed
        {
            Vec2DS sheetPos(3, 3);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(112, 112, 112);

            uiElementsSheet.drawFilledRect(
                Vec2DS(3, 6) + startPos,
                Vec2DS(24, 16),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(3, 22) + startPos,
                Vec2DS(10, 4),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(13, 22) + startPos,
                Vec2DS(2, 2),
                mainColor);

            m_defaultUISprites[(Size)DefaultUISprite::FolderClosed] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // FolderOpened
        {
            Vec2DS sheetPos(3, 2);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(112, 112, 112);
            ColorU32 innerColor(255, 255, 255);

            uiElementsSheet.drawFilledRect(
                Vec2DS(3, 6) + startPos,
                Vec2DS(24, 16),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(3, 22) + startPos,
                Vec2DS(10, 4),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(13, 22) + startPos,
                Vec2DS(2, 2),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(5, 8) + startPos,
                Vec2DS(8, 16),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(13, 8) + startPos,
                Vec2DS(12, 12),
                innerColor);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(5, 7) + startPos,
                Vec2DS(9, 17) + startPos,
                Vec2DS(30, 17) + startPos,
                mainColor);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(30, 17) + startPos,
                Vec2DS(26, 7) + startPos,
                Vec2DS(5, 7) + startPos,
                mainColor);

            m_defaultUISprites[(Size)DefaultUISprite::FolderOpened] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // File
        {
            Vec2DS sheetPos(4, 0);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(173, 173, 173);
            ColorU32 innerColor(216, 216, 216);
            ColorU32 fillColor(255, 255, 255);

            uiElementsSheet.drawFilledRect(
                Vec2DS(4, 2) + startPos,
                Vec2DS(18, 28),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 2) + startPos,
                Vec2DS(6, 22),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 26) + startPos,
                Vec2DS(2, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 24) + startPos,
                Vec2DS(2, 2),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(16, 24),
                fillColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 4) + startPos,
                Vec2DS(4, 20),
                fillColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 22) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 24) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);

            m_defaultUISprites[(Size)DefaultUISprite::File] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // TextFile
        {
            Vec2DS sheetPos(4, 1);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(173, 173, 173);
            ColorU32 innerColor(216, 216, 216);
            ColorU32 fillColor(255, 255, 255);

            uiElementsSheet.drawFilledRect(
                Vec2DS(4, 2) + startPos,
                Vec2DS(18, 28),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 2) + startPos,
                Vec2DS(6, 22),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 26) + startPos,
                Vec2DS(2, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 24) + startPos,
                Vec2DS(2, 2),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(16, 24),
                fillColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 4) + startPos,
                Vec2DS(4, 20),
                fillColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 22) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 24) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 10) + startPos,
                Vec2DS(4, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 10) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(16, 10) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 14) + startPos,
                Vec2DS(8, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 14) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 14) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 18) + startPos,
                Vec2DS(8, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 18) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 18) + startPos,
                Vec2DS(2, 2),
                innerColor);

            m_defaultUISprites[(Size)DefaultUISprite::TextFile] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Material
        {
            Vec2DS sheetPos(4, 2);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(14, 109, 203);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 16) + startPos,
                12,
                mainColor);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(13, 19) + startPos,
                6,
                ColorU32::c_transparent);

            m_defaultUISprites[(Size)DefaultUISprite::Material] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Mesh
        {
            Vec2DS sheetPos(4, 3);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 color0(14, 109, 203);
            ColorU32 color1(97, 174, 254);
            ColorU32 color2(62, 143, 225);

            S8 const image[32][32 + 1] =
            {
                "________________________________",
                "________________________________",
                "_____________111111_____________",
                "___________1111111111___________",
                "__________111111111111__________",
                "________11_1111111111_11________",
                "______111111_111111_111111______",
                "____1111111111____1111111111____",
                "____1111111111____1111111111____",
                "___2__111111__0022__111111__0___",
                "___222__11__00002222__11__000___",
                "___22222__000000222222__00000___",
                "___222222_000000222222_000000___",
                "___22222220000002222220000000___",
                "___22222220000002222220000000___",
                "___222222200000__222220000000___",
                "____2222220000____2222000000____",
                "___2__222200_111111_220000__0___",
                "___222__22_1111111111_00__000___",
                "___22222__111111111111__00000___",
                "___222222__1111111111__000000___",
                "___222222222_111111_000000000___",
                "___22222222222____00000000000___",
                "___22222222222220000000000000___",
                "____222222222222000000000000____",
                "______22222222220000000000______",
                "________2222222200000000________",
                "__________222222000000__________",
                "____________22220000____________",
                "______________2200______________",
                "________________________________",
                "________________________________"
            };

            for (S32 r = 0; r < 32; ++r)
            {
                for (S32 c = 0; c < 32; ++c)
                {
                    ColorU32 color;

                    S8 const value = image[31 - r][c];
                    switch (value)
                    {
                        case '0': color = color0; break;
                        case '1': color = color1; break;
                        case '2': color = color2; break;
                        default: continue;
                    }

                    uiElementsSheet.setPixel(c + startPos.x, r + startPos.y, color);
                }
            }

            m_defaultUISprites[(Size)DefaultUISprite::Mesh] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // PhysicsMaterial2D
        {
            Vec2DS sheetPos(5, 3);
            Vec2DS startPos = startPosFunc(sheetPos);

            ColorU32 color0(48, 126, 52);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(20, 24) + startPos,
                9,
                color0);

            uiElementsSheet.drawFilledRect(
                Vec2DS(3, 3) + startPos,
                Vec2DS(28, 4),
                color0);

            m_defaultUISprites[(Size)DefaultUISprite::PhysicsMaterial2D] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }
        
        // Shader
        {
            Vec2DS sheetPos(5, 2);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(173, 173, 173);
            ColorU32 innerColor(216, 216, 216);
            ColorU32 fillColor(255, 255, 255);

            uiElementsSheet.drawFilledRect(
                Vec2DS(4, 2) + startPos,
                Vec2DS(18, 28),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 2) + startPos,
                Vec2DS(6, 22),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 26) + startPos,
                Vec2DS(2, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 24) + startPos,
                Vec2DS(2, 2),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(16, 24),
                fillColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 4) + startPos,
                Vec2DS(4, 20),
                fillColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(6, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 4) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(24, 22) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(22, 24) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 26) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 10) + startPos,
                Vec2DS(4, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 10) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(16, 10) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 14) + startPos,
                Vec2DS(8, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 14) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 14) + startPos,
                Vec2DS(2, 2),
                innerColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(12, 18) + startPos,
                Vec2DS(8, 2),
                mainColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(10, 18) + startPos,
                Vec2DS(2, 2),
                innerColor);
            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 18) + startPos,
                Vec2DS(2, 2),
                innerColor);

            for (S32 r = 0; r < 32; ++r)
            {
                for (S32 c = 0; c < 32; ++c)
                {
                    ColorU32 clr = uiElementsSheet.getPixelRGBA_U8(c + startPos.x, r + startPos.y);
                    if (clr == fillColor)
                    {
                        ColorU32 newColor =
                            ColorHelper::ConvertHSVToRGB(
                                Math::Clamp(140.0f * (r - 4.0f) / 24.0f, 0.0f, 140.0f), 1.0f, 1.0f);
                        uiElementsSheet.setPixel(c + startPos.x, r + startPos.y, newColor);
                    }
                    else if (clr == innerColor || clr == mainColor)
                    {
                        ColorU32 newColor =
                            ColorHelper::ConvertHSVToRGB(
                                Math::Clamp(140.0f * (r - 4.0f) / 24.0f, 0.0f, 140.0f), 1.0f, 1.0f);
                        uiElementsSheet.setPixel(c + startPos.x, r + startPos.y, newColor.blendedCopy(clr));
                    }
                }
            }

            m_defaultUISprites[(Size)DefaultUISprite::Shader] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // SelectAsset
        {

            Vec2DS sheetPos(5, 1);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(255, 255, 255);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 16) + startPos,
                7,
                mainColor);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 16) + startPos,
                5,
                ColorU32::c_transparent);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 16) + startPos,
                1,
                mainColor);

            m_defaultUISprites[(Size)DefaultUISprite::SelectAsset] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // Pause
        {

            Vec2DS sheetPos(5, 0);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(255, 255, 255);

            uiElementsSheet.drawFilledRect(
                Vec2DS(11, 9) + startPos,
                Vec2DS(3, 14),
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(18, 9) + startPos,
                Vec2DS(3, 14),
                mainColor);

            m_defaultUISprites[(Size)DefaultUISprite::Pause] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        // StepUpdate
        {

            Vec2DS sheetPos(6, 0);
            Vec2DS startPos = startPosFunc(sheetPos);
            ColorU32 mainColor(255, 255, 255);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(10, 9) + startPos,
                Vec2DS(19, 15) + startPos,
                Vec2DS(10, 22) + startPos,
                mainColor);

            uiElementsSheet.drawFilledRect(
                Vec2DS(20, 9) + startPos,
                Vec2DS(2, 14),
                mainColor);

            m_defaultUISprites[(Size)DefaultUISprite::StepUpdate] = Sprite::Create(
                m_uiElementsTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        m_uiElementsTexture->loadTexture(uiElementsSheet);

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        // m_uiElementsTexture->saveToFileAsTGA("uiElements.tga");
#endif



        PixelSheet2D transparentChessSheet(Vec2DS(8, 8), PixelFormat::RGBA_U8);
        transparentChessSheet.fill(ColorU32::c_transparent);

        m_transparentChessTexture = Texture2D::Create();
        m_transparentChessTexture->setMagFilter(TextureFilter::Linear);
        m_transparentChessTexture->setMinFilter(TextureFilter::Linear);
        m_transparentChessTexture->setWrapS(TextureWrap::Repeat);
        m_transparentChessTexture->setWrapT(TextureWrap::Repeat);

        // TransparentChess
        {
            transparentChessSheet.drawFilledRect(
                Vec2DS(0, 0),
                Vec2DS(4, 4),
                ColorU32::c_white);

            transparentChessSheet.drawFilledRect(
                Vec2DS(4, 4),
                Vec2DS(4, 4),
                ColorU32::c_white);

            transparentChessSheet.drawFilledRect(
                Vec2DS(0, 4),
                Vec2DS(4, 4),
                ColorU32(214, 214, 214));

            transparentChessSheet.drawFilledRect(
                Vec2DS(4, 0),
                Vec2DS(4, 4),
                ColorU32(214, 214, 214));

            SpritePtr sprite = Sprite::Create(
                m_transparentChessTexture,
                Vec2DS(0, 0),
                Vec2DF(8, 8));

            m_defaultUISprites[(Size)DefaultUISprite::TransparentChess] = sprite;
        }

        m_transparentChessTexture->loadTexture(transparentChessSheet);
        m_transparentChessTexture->generateMipmaps();

        for (DefaultUISprite spriteType = DefaultUISprite(0); spriteType < DefaultUISprite::MAX; spriteType = DefaultUISprite((S32)spriteType + 1))
        {
            if (m_defaultUISprites[(Size)spriteType])
                m_defaultUISprites[(Size)spriteType]->updateTextureCoords();
        }
    }

    //////////////////////////////////////////
    void UIManager::buildPanel(
        PixelSheet2D& _uiElementsSheet,
        Vec2DS const& _startPos,
        Vec2DS const& _size,
        ColorU32 const& _backgroundColor,
        ColorU32 const& _sideBordersColor,
        ColorU32 const& _topBorderColor)
    {
        _uiElementsSheet.drawFilledRect(
            Vec2DS(1, 1) + _startPos,
            Vec2DS(_size.x - 2, _size.y - 2),
            _backgroundColor);

        _uiElementsSheet.drawLine(
            Vec2DS(0, 1) + _startPos,
            Vec2DS(0, _size.y - 2) + _startPos,
            _sideBordersColor);

        _uiElementsSheet.drawLine(
            Vec2DS(_size.x - 1, 1) + _startPos,
            Vec2DS(_size.x - 1, _size.y - 2) + _startPos,
            _sideBordersColor);

        _uiElementsSheet.drawLine(
            Vec2DS(1, 0) + _startPos,
            Vec2DS(_size.x - 2, 0) + _startPos,
            _sideBordersColor);


        _uiElementsSheet.drawLine(
            Vec2DS(1, _size.y - 1) + _startPos,
            Vec2DS(_size.x - 2, _size.y - 1) + _startPos,
            _topBorderColor);

        _uiElementsSheet.drawLine(
            Vec2DS(1, _size.y - 2) + _startPos,
            Vec2DS(1, _size.y - 1) + _startPos,
            _sideBordersColor);

        _uiElementsSheet.drawLine(
            Vec2DS(_size.x - 2, _size.y - 2) + _startPos,
            Vec2DS(_size.x - 2, _size.y - 1) + _startPos,
            _sideBordersColor);

        _uiElementsSheet.setPixel(
            Vec2DS(1, 1) + _startPos,
            _sideBordersColor);

        _uiElementsSheet.setPixel(
            Vec2DS(_size.x - 2, 1) + _startPos,
            _sideBordersColor);
    }
    
} // namespace Maze
//////////////////////////////////////////
