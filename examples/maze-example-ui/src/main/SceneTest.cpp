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
#include "SceneTest.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
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
    // Class SceneTest
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneTest, EcsRenderScene);

    //////////////////////////////////////////
    SceneTest::SceneTest()
    {
    }

    //////////////////////////////////////////
    SceneTest::~SceneTest()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneTestPtr SceneTest::Create()
    {
        SceneTestPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneTest, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneTest::init()
    {
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create2D();

        m_inputSystemSample = m_world->requestInclusiveSample<InputSystem2D>();

        return true;
    }

    //////////////////////////////////////////
    void SceneTest::notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport)
    {
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneTest::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

        m_inputSystemSample->query(
            [&](Entity*, InputSystem2D* _inputSystem)
        {
            Vec2F renderTargetCursorPos = InputManager::GetInstancePtr()->getCursorPosition(0);
            bool traced = _inputSystem->traceElement(
                m_traceTest->getEntityRaw()->getComponentRaw<UIElement2D>(),
                renderTargetCursorPos);

            m_traceTest->setColor(traced ? ColorU32::c_yellow : ColorU32::c_red);
        });
    }

    //////////////////////////////////////////
    void SceneTest::create2D()
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
        m_canvas = canvasEntity->createComponent<Maze::Canvas>();
        m_canvas->setRenderTarget(renderTarget);
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setClearColor(ColorU32::c_darkGray);
        m_canvas->setClearColorFlag(true);

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneTest::notifyMainRenderWindowViewportChanged);

        /*
        SpriteRenderer2DPtr square = SpriteHelper::CreateSprite(
            ColorU32::c_gray,
            Vec2F32(800.0f, 600.0f),
            Vec2F32::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);
        */

        /*
        UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(100.0f),
            Vec2F::c_zero,
            m_canvas->getTransform(),
            this);
        */

        SpriteRenderer2DPtr back = SpriteHelper::CreateSprite(
            ColorU32::c_green,
            Vec2F32(150.0f, 150.0f),
            Vec2F32::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);
        back->getEntityRaw()->ensureComponent<ScissorMask2D>();

        EntityPtr buttonEntity = createEntity();
        buttonEntity->ensureComponent<StaticName>("ClickButton");

        ClickButton2DPtr clickButton = buttonEntity->createComponent<ClickButton2D>();
        clickButton->setNormalColor(ColorU32{ 245, 245, 245 });
        clickButton->setFocusedColor(ColorU32{ 255, 160, 0 });
        clickButton->setSelectedColor(ColorU32{ 0, 220, 240 });
        clickButton->setPressedColor(ColorU32{ 213, 0, 0 });

        Transform2DPtr const& transform = clickButton->getTransform();
        transform->setParent(back->getTransform());
        transform->setLocalY(75.0f);
        transform->setSize(Vec2F(100.0f));

        Maze::SpriteRenderer2DPtr spriteRenderer = buttonEntity->createComponent<Maze::SpriteRenderer2D>();
        spriteRenderer->setSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
        spriteRenderer->setMaterialCopy(SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial());

        clickButton->setTransitionSprite(spriteRenderer);

        {
            m_traceTest = SpriteHelper::CreateSprite(
                ColorU32::c_red,
                Vec2F32(20.0f, 120.0f),
                Vec2F32(10.0f, 25.0f),
                nullptr,
                back->getTransform(),
                this);
            m_traceTest->getEntityRaw()->ensureComponent<UIElement2D>()->setCaptureCursorHits(true);
        }

        {
            auto btn0 = UIHelper::CreateClickButton(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(100.0f),
                Vec2F(60.0f, 0.0f),
                back->getTransform(),
                this);
            btn0->setPressedColor(ColorU32{ 213, 160, 0 });
            btn0->getEntity()->ensureComponent<StaticName>()->setStaticName("btn0");

            auto btn1 = UIHelper::CreateClickButton(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
                Vec2F(50.0f),
                Vec2F(0.0f),
                btn0->getTransform(),
                this);
            btn1->setPressedColor(ColorU32{ 0, 0, 255 });
            btn1->getEntity()->ensureComponent<StaticName>()->setStaticName("btn1");
        }
    }

} // namespace Maze
//////////////////////////////////////////
