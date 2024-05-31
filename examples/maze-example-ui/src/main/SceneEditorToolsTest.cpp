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
#include "SceneEditorToolsTest.hpp"
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
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneEditorToolsTest
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneEditorToolsTest, EcsRenderScene);

    //////////////////////////////////////////
    SceneEditorToolsTest::SceneEditorToolsTest()
    {
    }

    //////////////////////////////////////////
    SceneEditorToolsTest::~SceneEditorToolsTest()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventKeyboard.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneEditorToolsTestPtr SceneEditorToolsTest::Create()
    {
        SceneEditorToolsTestPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneEditorToolsTest, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneEditorToolsTest::init()
    {
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneEditorToolsTest::notifyKeyboard);

        create3D();
        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneEditorToolsTest::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneEditorToolsTest::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

    }

    //////////////////////////////////////////
    void SceneEditorToolsTest::create3D()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        EcsWorldPtr const& world = entityManager->getDefaultWorld();

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneEditorToolsTest::notifyMainRenderWindowViewportChanged);


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
    void SceneEditorToolsTest::create2D()
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


        SpriteRenderer2DPtr hierarchyHolderSprite = SpriteHelper::CreateSprite(
            ColorU32::c_lightGray,
            Vec2F(300.0f, 400.0f),
            Vec2F(0.0f, 0.0f),
            renderSystem->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this);
        VerticalLayout2DPtr verticalLayout = hierarchyHolderSprite->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        verticalLayout->setAutoHeight(false);

        auto createHierarchyLine = 
            [this](HierarchyLineType _type)
            {
                EntityPtr const& hierarchyLineEntity = createEntity();
                HierarchyLinePtr hierarchyLine = hierarchyLineEntity->createComponent<HierarchyLine>(_type);
                hierarchyLineEntity->ensureComponent<Name>("HIERARCHY LINE");

                hierarchyLine->getContextMenu()->setCallbackFunction(
                    [](MenuListTree2DPtr const& _menuListTree)
                    {
                        _menuListTree->addItem(
                            "Hello!",
                            [](String const& _text)
                            {
                                Debug::Log(_text.c_str());
                            });
                    });

                return hierarchyLine;
            };

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->getTransform()->setParent(hierarchyHolderSprite->getTransform());

        HierarchyLinePtr hierarchyLine1 = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->addChild(hierarchyLine1);

        HierarchyLinePtr hierarchyLine2 = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->addChild(hierarchyLine2);

        HierarchyLinePtr hierarchyLine22 = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine2->addChild(hierarchyLine22);

        HierarchyLinePtr hierarchyLine3 = createHierarchyLine(HierarchyLineType::Scene);
        hierarchyLine->addChild(hierarchyLine3);


        HierarchyLinePtr hierarchyLine4 = createHierarchyLine(HierarchyLineType::Scene);
        hierarchyLine4->getTransform()->setParent(hierarchyHolderSprite->getTransform());

        HierarchyLinePtr hierarchyLine5 = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine4->addChild(hierarchyLine5);
    }

    //////////////////////////////////////////
    Transform2DPtr const& SceneEditorToolsTest::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F32(53, 18),
            Vec2F32(0, 0),
            _parent,
            this);
        layout->setSpacing(5.0f);

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            _name,
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F32(8, 18),
            Vec2F32(0, 0),
            layout->getTransform(),
            this,
            Vec2F32(0.0f, 0.5f),
            Vec2F32::c_zero);
        systemText->setColor(ColorU32::c_black);

        EditBox2DPtr xEditBox = SystemUIHelper::CreateDefaultEditBox(
            "0",
            Vec2F32(85, 18),
            Vec2F32(0, 0),
            layout->getTransform(),
            this,
            Vec2F32(0.5f, 0.5f),
            Vec2F32::c_zero);

        

        return layout->getTransform();
    }

    //////////////////////////////////////////
    Transform2DPtr const& SceneEditorToolsTest::createVec3F32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F32(450, 18),
            Vec2F32(0, 0),
            _parent,
            this);
        layout->setExpand(true);

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            _name,
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F32(8, 18),
            Vec2F32(0, 0),
            layout->getTransform(),
            this,
            Vec2F32(0.0f, 0.5f),
            Vec2F32::c_zero);
        systemText->setColor(ColorU32::c_black);

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F32(200, 18),
                Vec2F32(0, 0),
                layout->getTransform(),
                this);
            layout2->setSpacing(10.0f);

            Transform2DPtr const& editX = createF32Edit("X", layout2->getTransform());
            Transform2DPtr const& editY = createF32Edit("Y", layout2->getTransform());
            Transform2DPtr const& editZ = createF32Edit("Z", layout2->getTransform());
        }

        return layout->getTransform();
    }

    //////////////////////////////////////////
    void SceneEditorToolsTest::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::Number9:
                    {
                        SceneManager::GetInstancePtr()->unloadScene<SceneEditorToolsTest>();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
