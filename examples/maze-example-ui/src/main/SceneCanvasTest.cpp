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
#include "SceneCanvasTest.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneCanvasTest
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneCanvasTest, EcsRenderScene);

    //////////////////////////////////////////
    SceneCanvasTest::SceneCanvasTest()
    {
    }

    //////////////////////////////////////////
    SceneCanvasTest::~SceneCanvasTest()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneCanvasTestPtr SceneCanvasTest::Create()
    {
        SceneCanvasTestPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneCanvasTest, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneCanvasTest::init()
    {
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneCanvasTest::notifyMainRenderWindowViewportChanged);

        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneCanvasTest::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_canvas0->setViewport(m_canvas0Viewport);
    }

    //////////////////////////////////////////
    void SceneCanvasTest::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

        if (m_text0)
        {
            m_text0->setTextFormatted(
                "Pos: %.2fx%.2f Size: %.2fx%.2f\nPos: %.2fx%.2f Size: %.2fx%.2f",
                m_canvas0->getTransform()->getLocalPosition().x,
                m_canvas0->getTransform()->getLocalPosition().y,
                m_canvas0->getTransform()->getWidth(),
                m_canvas0->getTransform()->getHeight(),
                m_square0->getTransform()->getLocalPosition().x,
                m_square0->getTransform()->getLocalPosition().y,
                m_square0->getTransform()->getWidth(),
                m_square0->getTransform()->getHeight());
        }
    }
    
    //////////////////////////////////////////
    void SceneCanvasTest::create2D()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();

        // Canvas
        Maze::EntityPtr canvasEntity = createEntity();
        Maze::Transform2DPtr canvasTransform2D = canvasEntity->createComponent<Maze::Transform2D>();
        m_canvas0 = canvasEntity->createComponent<Maze::Canvas>();
        m_canvas0->setRenderTarget(renderTarget);
        m_canvas0->setViewport(m_canvas0Viewport);
        m_canvas0->setClearColor(ColorU32::c_red);
        m_canvas0->setClearColorFlag(true);

        m_square0 = SpriteHelper::CreateSprite(
            ColorU32::c_gray,
            m_canvas0->getTransform()->getSize(),
            Vec2F32::c_zero,
            nullptr,
            m_canvas0->getTransform(),
            this);
        m_square0->getEntityRaw()->ensureComponent<SizePolicy2D>();

        FontManager::GetInstancePtr()->setDefaultFont(FontManager::GetInstancePtr()->getOrLoadFont("Editor00.mzfont"));
        FontMaterialPtr fontMaterial = FontMaterialManager::GetInstancePtr()->getFontMaterial("Editor00.mzfontmaterial");
        FontMaterialManager::GetInstancePtr()->setDefaultFontMaterial(fontMaterial);

        m_text0 = UIHelper::CreateText(
            "TEST",
            12,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            m_square0->getTransform()->getSize(),
            Vec2F32::c_zero,
            m_square0->getTransform(),
            this);
    }

} // namespace Maze
//////////////////////////////////////////
