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
#include "SceneSystemTextTest.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
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
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneSystemTextTest
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSystemTextTest, ECSRenderScene);

    //////////////////////////////////////////
    SceneSystemTextTest::SceneSystemTextTest()
    {
    }

    //////////////////////////////////////////
    SceneSystemTextTest::~SceneSystemTextTest()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);

        m_lb->eventClick.unsubscribe(this);
        m_l->eventClick.unsubscribe(this);
        m_lt->eventClick.unsubscribe(this);
        m_t->eventClick.unsubscribe(this);
        m_rt->eventClick.unsubscribe(this);
        m_r->eventClick.unsubscribe(this);
        m_rb->eventClick.unsubscribe(this);
        m_b->eventClick.unsubscribe(this);

        m_decreaseFont->eventClick.unsubscribe(this);
        m_increaseFont->eventClick.unsubscribe(this);

        m_changeFont->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneSystemTextTestPtr SceneSystemTextTest::Create()
    {
        SceneSystemTextTestPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSystemTextTest, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSystemTextTest::init()
    {
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create3D();
        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneSystemTextTest::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneSystemTextTest::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

    }

    //////////////////////////////////////////
    void SceneSystemTextTest::create3D()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneSystemTextTest::notifyMainRenderWindowViewportChanged);


        // Light
        EntityPtr lightEntity = createEntity();
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        lightEntity->ensureComponent<Name>("Light");


        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3F32(0.0f, 0.5f, -18.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        
        m_camera3D->setRenderTarget(renderTarget);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(30.0f);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneSystemTextTest::create2D()
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

        SpriteRenderer2DPtr square = SpriteHelper::CreateSprite(
            ColorU32::c_gray,
            Vec2F32(200.0f, 200.0f),
            Vec2F32::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);

        m_text = SystemUIHelper::CreateSystemText(
            "Hello, World!",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            square->getTransform()->getSize(),
            Vec2F32::c_zero,
            square->getTransform(),
            this);

        m_lb = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(0.0f, 0.0f),
            Vec2F32(1.0f, 1.0f));
        m_lb->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_l = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(0.0f, 0.5f),
            Vec2F32(1.0f, 0.5f));
        m_l->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_lt = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(0.0f, 1.0f),
            Vec2F32(1.0f, 0.0f));
        m_lt->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_t = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(0.5f, 1.0f),
            Vec2F32(0.5f, 0.0f));
        m_t->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_rt = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(1.0f, 1.0f),
            Vec2F32(0.0f, 0.0f));
        m_rt->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_r = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(1.0f, 0.5f),
            Vec2F32(0.0f, 0.5f));
        m_r->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_rb = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(1.0f, 0.0f),
            Vec2F32(0.0f, 1.0f));
        m_rb->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_b = UIHelper::CreateDefaultClickButton(
            "",
            Vec2F32(20.0f, 20.0f),
            Vec2F32::c_zero,
            square->getTransform(),
            this,
            Vec2F32(0.5f, 0.0f),
            Vec2F32(0.5f, 1.0f));
        m_b->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_decreaseFont = UIHelper::CreateDefaultClickButton(
            "-",
            Vec2F32(20.0f, 20.0f),
            Vec2F32(-12.0f, -50.0f),
            square->getTransform(),
            this,
            Vec2F32(0.5f, 0.0f),
            Vec2F32(0.5f, 1.0f));
        m_decreaseFont->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_increaseFont = UIHelper::CreateDefaultClickButton(
            "+",
            Vec2F32(20.0f, 20.0f),
            Vec2F32(12.0f, -50.0f),
            square->getTransform(),
            this,
            Vec2F32(0.5f, 0.0f),
            Vec2F32(0.5f, 1.0f));
        m_increaseFont->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);

        m_changeFont = UIHelper::CreateDefaultClickButton(
            "F",
            Vec2F32(20.0f, 20.0f),
            Vec2F32(40.0f, -50.0f),
            square->getTransform(),
            this,
            Vec2F32(0.5f, 0.0f),
            Vec2F32(0.5f, 1.0f));
        m_changeFont->eventClick.subscribe(this, &SceneSystemTextTest::notifyButtonClick);
    }

    //////////////////////////////////////////
    void SceneSystemTextTest::notifyButtonClick(Button2D* _button, CursorInputEvent const& _event)
    {
        if (_button == m_lb.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Left);
            m_text->setVerticalAlignment(VerticalAlignment2D::Bottom);
        }
        else
        if (_button == m_l.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Left);
            m_text->setVerticalAlignment(VerticalAlignment2D::Middle);
        }
        else
        if (_button == m_lt.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Left);
            m_text->setVerticalAlignment(VerticalAlignment2D::Top);
        }
        else
        if (_button == m_t.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Center);
            m_text->setVerticalAlignment(VerticalAlignment2D::Top);
        }
        else
        if (_button == m_rt.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Right);
            m_text->setVerticalAlignment(VerticalAlignment2D::Top);
        }
        else
        if (_button == m_r.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Right);
            m_text->setVerticalAlignment(VerticalAlignment2D::Middle);
        }
        else
        if (_button == m_rb.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Right);
            m_text->setVerticalAlignment(VerticalAlignment2D::Bottom);
        }
        else
        if (_button == m_b.get())
        {
            m_text->setHorizontalAlignment(HorizontalAlignment2D::Center);
            m_text->setVerticalAlignment(VerticalAlignment2D::Bottom);
        }
        else
        if (_button == m_decreaseFont.get())
        {
            if (m_text->getFontSize() > 6)
                m_text->setFontSize(m_text->getFontSize() - 2);
        }
        else
        if (_button == m_increaseFont.get())
        {
            if (m_text->getFontSize() < 14)
                m_text->setFontSize(m_text->getFontSize() + 2);
        }
        else
        if (_button == m_changeFont.get())
        {
            m_text->setSystemFont(
                m_text->getSystemFont() == SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault() ?
                SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined() :
                SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault());
        }
    }


} // namespace Maze
//////////////////////////////////////////
