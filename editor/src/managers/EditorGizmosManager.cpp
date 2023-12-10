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
#include "EditorGizmosManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "Editor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorGizmosManager
    //
    //////////////////////////////////////////
    EditorGizmosManager* EditorGizmosManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorGizmosManager::EditorGizmosManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorGizmosManager::~EditorGizmosManager()
    {
        s_instance = nullptr;

        
    }

    //////////////////////////////////////////
    void EditorGizmosManager::Initialize(EditorGizmosManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorGizmosManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorGizmosManager::init()
    {
        
        return true;
    }

    //////////////////////////////////////////
    void EditorGizmosManager::createGizmosElements()
    {
        S32 const iconSize = 32;
        S32 const gap = 1;

        auto startPosFunc = [&](Vec2S32 const& _pos) { return (iconSize * _pos) + Vec2S32((_pos.x + 1) * gap, (_pos.y + 1) * gap); };

        PixelSheet2D uiElementsSheet(Vec2S32(4, 4) * iconSize, PixelFormat::RGBA_U8);
        uiElementsSheet.fill(ColorU32::c_transparent);

        m_gizmosElementsTexture = Texture2D::Create();
        m_gizmosElementsTexture->setMagFilter(TextureFilter::Linear);
        m_gizmosElementsTexture->setMinFilter(TextureFilter::Linear);
        m_gizmosElementsTexture->setWrapS(TextureWrap::ClampToEdge);
        m_gizmosElementsTexture->setWrapT(TextureWrap::ClampToEdge);


        // Axes
        {
            Vec2S32 sheetPos(0, 0);

            Vec2S32 startPos = startPosFunc(sheetPos);

            for (S32 i = 0; i < 2; ++i)
                for (S32 j = 0; j < 2; ++j)
                {
                    uiElementsSheet.drawLine(
                        startPos + Vec2S32(3 + i, 3 + j),
                        startPos + Vec2S32(28 + i, 3 + j),
                        ColorU32::c_red);

                    uiElementsSheet.drawLine(
                        startPos + Vec2S32(3 + i, 4 + j),
                        startPos + Vec2S32(3 + i, 28 + j),
                        ColorU32::c_green);

                    uiElementsSheet.drawLine(
                        startPos + Vec2S32(4 + i, 4 + j),
                        startPos + Vec2S32(24 + i, 18 + j),
                        ColorU32::c_blue);
                }

            m_editorGizmosSprites[(Size)EditorGizmosSprite::Axes] = Sprite::Create(
                m_gizmosElementsTexture,
                Vec2F32(iconSize * sheetPos),
                Vec2F32(iconSize, iconSize));
        }

        // Grid
        {
            Vec2S32 sheetPos(1, 0);

            Vec2S32 startPos = startPosFunc(sheetPos);

            for (S32 i = 0; i < 2; ++i)
                for (S32 j = 0; j < 2; ++j)
                {
                    for (S32 f = 0; f < 5; ++f)
                    {
                        uiElementsSheet.drawLine(
                            startPos + Vec2S32(2 + i + f * 5, 1 + j),
                            startPos + Vec2S32(2 + i + f * 5, 26 + j),
                            ColorU32::c_lightGray);

                        uiElementsSheet.drawLine(
                            startPos + Vec2S32(1 + i, 3 + j + f * 5),
                            startPos + Vec2S32(26 + i, 3 + j + f * 5),
                            ColorU32::c_lightGray);
                    }
                }

            m_editorGizmosSprites[(Size)EditorGizmosSprite::Grid] = Sprite::Create(
                m_gizmosElementsTexture,
                Vec2F32(iconSize * sheetPos),
                Vec2F32(iconSize, iconSize));
        }

        m_gizmosElementsTexture->loadTexture(uiElementsSheet);

        for (EditorGizmosSprite spriteType = EditorGizmosSprite(0); spriteType < EditorGizmosSprite::MAX; spriteType = EditorGizmosSprite((S32)spriteType + 1))
        {
            if (m_editorGizmosSprites[(Size)spriteType])
                m_editorGizmosSprites[(Size)spriteType]->updateTextureCoords();
        }
    }


} // namespace Maze
//////////////////////////////////////////
