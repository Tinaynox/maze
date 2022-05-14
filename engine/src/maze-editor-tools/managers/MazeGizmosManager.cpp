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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeLight3DGizmos.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeCamera3DGizmos.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeMeshRendererGizmos.hpp"
#include "maze-editor-tools/managers/MazeGizmoToolsManager.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class GizmosManager
    //
    //////////////////////////////////////////
    GizmosManager* GizmosManager::s_instance = nullptr;

    //////////////////////////////////////////
    GizmosManager::GizmosManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    GizmosManager::~GizmosManager()
    {
        setCamera(nullptr);
        setCanvas(nullptr);
        m_gizmoToolsManager.reset();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void GizmosManager::Initialize(GizmosManagerPtr& _graphicsManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmosManager, _graphicsManager, init());
    }

    //////////////////////////////////////////
    bool GizmosManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);
        
        GraphicsUtilsHelper::ConstructAsciiSymbolsSheet8x8();

        GizmoToolsManager::Initialize(m_gizmoToolsManager);
        if (!m_gizmoToolsManager)
            return false;

        registerGizmos<Light3D, Light3DGizmos>();
        registerGizmos<Camera3D, Camera3DGizmos>();
        registerGizmos<MeshRenderer, MeshRendererGizmos>();

        return true;
    }

    //////////////////////////////////////////
    void GizmosManager::update(F32 dt)
    {
        
    }

    //////////////////////////////////////////
    void GizmosManager::setCanvas(CanvasPtr const& _canvas)
    {
        if (m_canvas == _canvas)
            return;

        eventCanvasWillBeChanged(m_canvas);

        m_canvas = _canvas;

        eventCanvasChanged(m_canvas);
    }

    //////////////////////////////////////////
    void GizmosManager::createGizmosElements()
    {
        S32 const iconSize = 32;
        S32 const gap = 1;

        auto startPosFunc = [&](Vec2DS const& _pos) { return (iconSize * _pos) + Vec2DS((_pos.x + 1) * gap, (_pos.y + 1) * gap); };

        PixelSheet2D uiElementsSheet(Vec2DS(4, 4) * iconSize, PixelFormat::RGBA_U8);
        uiElementsSheet.fill(ColorU32::c_transparent);

        m_gizmosElementsTexture = Texture2D::Create();
        m_gizmosElementsTexture->setMagFilter(TextureFilter::Linear);
        m_gizmosElementsTexture->setMinFilter(TextureFilter::Linear);
        m_gizmosElementsTexture->setWrapS(TextureWrap::ClampToEdge);
        m_gizmosElementsTexture->setWrapT(TextureWrap::ClampToEdge);


        // Light Gizmo
        {
            Vec2DS sheetPos(0, 1);

            Vec2DS startPos = startPosFunc(sheetPos);

            F32 const radius = 7.0f;
            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 16) + startPos,
                (S32)radius,
                ColorU32::c_white);

            Vec2DS centerPoint = startPos + Vec2DS(16, 16);

            MAZE_CONSTEXPR F32 const angleDelta = Math::DegreesToRadians(30.0f);
            for (F32 i = 0; i < Math::c_twoPi - angleDelta * 0.5f; i += angleDelta)
            {
                Vec2DF dir(Math::Cos(i), Math::Sin(i));

                Vec2DF crossDir = dir.perpendicular();

                uiElementsSheet.drawLine(
                    (Vec2DS)((Vec2DF)centerPoint + dir * (radius + 3.0f) + 0.5f),
                    (Vec2DS)((Vec2DF)centerPoint + dir * (radius + 6.0f) + 0.5f),
                    ColorU32::c_white);
                uiElementsSheet.drawLine(
                    (Vec2DS)((Vec2DF)centerPoint - crossDir * 0.5f + dir * (radius + 3.0f) + 0.5f),
                    (Vec2DS)((Vec2DF)centerPoint - crossDir * 0.5f + dir * (radius + 6.0f) + 0.5f),
                    ColorU32::c_white);
                uiElementsSheet.drawLine(
                    (Vec2DS)((Vec2DF)centerPoint + crossDir * 0.5f + dir * (radius + 3.0f) + 0.5f),
                    (Vec2DS)((Vec2DF)centerPoint + crossDir * 0.5f + dir * (radius + 6.0f) + 0.5f),
                    ColorU32::c_white);
            }

            m_defaultGizmosSprites[(Size)DefaultGizmosSprite::LightGizmo] = Sprite::Create(
                m_gizmosElementsTexture,
                Vec2DF(iconSize * sheetPos),
                Vec2DF(iconSize, iconSize));
        }

        // Camera Gizmo
        {
            Vec2DS sheetPos(1, 1);

            Vec2DS startPos = startPosFunc(sheetPos);

            uiElementsSheet.drawFilledRect(
                startPos + Vec2DS(8, 8),
                Vec2DS(17, 11),
                ColorU32::c_white);

            F32 const radius = 5.0f;
            uiElementsSheet.drawFilledCircle(
                Vec2DS(7, 17) + startPos,
                (S32)radius,
                ColorU32::c_white);

            uiElementsSheet.drawFilledCircle(
                Vec2DS(16, 22) + startPos,
                (S32)radius,
                ColorU32::c_white);

            uiElementsSheet.drawFilledTriangle(
                Vec2DS(23, 14) + startPos,
                Vec2DS(29, 17) + startPos,
                Vec2DS(29, 11) + startPos,
                ColorU32::c_white);

            m_defaultGizmosSprites[(Size)DefaultGizmosSprite::CameraGizmo] = Sprite::Create(
                m_gizmosElementsTexture,
                Vec2DF(iconSize * sheetPos),
                Vec2DF(iconSize, iconSize));
        }

        m_gizmosElementsTexture->loadTexture(uiElementsSheet);

        for (DefaultGizmosSprite spriteType = DefaultGizmosSprite(0); spriteType < DefaultGizmosSprite::MAX; spriteType = DefaultGizmosSprite((S32)spriteType + 1))
        {
            if (m_defaultGizmosSprites[(Size)spriteType])
                m_defaultGizmosSprites[(Size)spriteType]->updateTextureCoords();
        }
    }


} // namespace Maze
//////////////////////////////////////////
