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
#include "SceneExample.hpp"
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
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    //////////////////////////////////////////
    SceneExample::SceneExample()
    {
    }

    //////////////////////////////////////////
    SceneExample::~SceneExample()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneExamplePtr SceneExample::Create()
    {
        SceneExamplePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneExample, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneExample::init()
    {
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create3D();
        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

    }

    //////////////////////////////////////////
    void SceneExample::create3D()
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
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);


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
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(0.0f, 0.5f, -18.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        
        m_camera3D->setRenderTarget(renderTarget);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(30.0f);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::create2D()
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

        SpriteHelper::CreateSystemText(
            "SOME TEST TEXT",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2DF::c_zero,
            Vec2DF::c_zero,
            m_canvas->getTransform(),
            this);

        SpriteHelper::CreateSystemText(
            "SECOND TEXT HERE",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2DF::c_zero,
            { 0.0f, -20.0f },
            m_canvas->getTransform(),
            this,
            { 0.5f, 0.5f },
            { 0.5f, 0.5f });
        /*
        Maze::SpritePtr sprite = spriteManager->getSprite("Panel00.mztexture");
        sprite->setSliceBorder(
            40.0f,
            34.0f,
            40.0f,
            34.0f);

        
        SpriteRenderer2DPtr panel00 = SpriteHelper::CreateSprite(
            ColorU32(203, 203, 203),
            Vec2DF(640, 480),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            canvasTransform2D,
            this);
        */
    
        /*
        Scrollbar2DPtr scrollbarH = UIHelper::CreateDefaultScrollbarHorizontal(
            Vec2DF(200.0f, 18.0f),
            Vec2DF::c_zero,
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        scrollbarH->setValue(1.0f);

        Scrollbar2DPtr scrollbarV = UIHelper::CreateDefaultScrollbarVertical(
            Vec2DF(18.0f, 200.0f),
            Vec2DF(200.0f, 100.0f),
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        scrollbarV->setValue(0.5f);
        */

        /*
        m_scrollRect = UIHelper::CreateDefaultScrollRect(
            Vec2DF(200.0f, 200.0f),
            Vec2DF(0.0f, 0.0f),
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f));

        SpriteRenderer2DPtr spriteRenderer = m_scrollRect->getContentTransform()->getEntityRaw()->ensureComponent<SpriteRenderer2D>();
        spriteRenderer->setMaterial(renderSystem->getMaterialManager()->getColorTextureMaterial());
        spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::MainScene));
        m_scrollRect->getContentTransform()->setSize(300.0f, 150.0f);
        */

        /*
        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(450, 18 * 3),
            Vec2DF(0, 0),
            panel00->getTransform(),
            this);
        layout->setSpacing(5.0f);

        createF32Edit("X", layout->getTransform())
            */

        
        /*
        SystemTextDropdown2DPtr dropdown = UIHelper::CreateDefaultDropdown(
            Vec2DF(150, 18),
            Vec2DF(270, 300),
            panel00->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF::c_zero);
        dropdown->addOptions({"Option 1", "Option 2", "Option 3", "Option 4", "Option 5" });
        */

        /*
        MenuListItem2DPtr menuListItem = UIHelper::CreateDefaultMenuListItem(
            "Option Test",
            Vec2DF(170.0f, 20.0f),
            Vec2DF(200, 100),
            panel00->getTransform(),
            this,
            Vec2DF::c_zero,
            Vec2DF::c_zero);
            */

        /*
        MenuList2DPtr menuList = UIHelper::CreateDefaultMenuList(
            Vec2DF(170.0f, 140.0f),
            Vec2DF(270, 300),
            panel00->getTransform(),
            this,
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        menuList->addItem(
            "Option 1",
            [](String const& _option)
            {
                Log("11");
            });
        */
        
        

            /*
        menuList->addItem(
            "Option 2",
            [](String const& _option)
            {
                Log("22");
            });
        menuList->addItem(
            "Option 3",
            [](String const& _option)
            {
                Log("33");
            });
        menuList->addItem(
            "Option 4",
            [](String const& _option)
            {
                Log("44");
            });*/
        
        
        /*
        m_menuListTree = UIHelper::CreateDefaultMenuListTree(
            Vec2DF(0.0f, 0.0f),
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.0f, 1.0f));
        
        m_menuListTree->addItem("Option 1", [](String const& _option) { Log("Option 1"); });
        m_menuListTree->addItem("Option 2/Option 4", [](String const& _option) { Log("Option 4"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 6",    [](String const& _option) {    Log("Option 6"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 7", [](String const& _option) { Log("Option 7"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 8", [](String const& _option) {    Log("Option 8"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9", [](String const& _option) {    Log("Option 9"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 10", [](String const& _option) {    Log("Option 10"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 11", [](String const& _option) {    Log("Option 11"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12", [](String const& _option) {    Log("Option 12"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 13", [](String const& _option) {    Log("Option 13"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14", [](String const& _option) {    Log("Option 14"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 15", [](String const& _option) {    Log("Option 15"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 16", [](String const& _option) {    Log("Option 16"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 17", [](String const& _option) {    Log("Option 17"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 18", [](String const& _option) {    Log("Option 18"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 19", [](String const& _option) {    Log("Option 19"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 20", [](String const& _option) {    Log("Option 20"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 21", [](String const& _option) {    Log("Option 21"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 22", [](String const& _option) {    Log("Option 22"); });
        m_menuListTree->addItem("Option 2/Option 5/Option 9/Option 12/Option 14/Option 22/That's all!", [](String const& _option) {    Log("Option 23"); });
        m_menuListTree->addItem("Option 3", [](String const& _option)    { Log("Option 3"); });
        */
        
        //menuListTree->addItem("Option 1", [](String const& _option) { Log("Option 1"); });
        
        /*
        ColorEdit2DPtr colorEdit = UIHelper::CreateDefaultColorEdit(
            ColorU32::c_red,
            Vec2DF(150, 18),
            Vec2DF(270, 300),
            panel00->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF::c_zero);

        ColorHDREdit2DPtr colorHDREdit = UIHelper::CreateDefaultColorHDREdit(
            ColorF128::c_green,
            Vec2DF(150, 18),
            Vec2DF(270, 270),
            panel00->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF::c_zero);
        */

        /*
        ColorEdit2DPtr colorEdit = UIHelper::CreateDefaultColorEdit(
            ColorU32::c_red,
            Vec2DF(150, 18),
            Vec2DF(270, 300),
            panel00->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF::c_zero);
        ColorEdit2D* colorEditRaw = colorEdit.get();
        colorEdit->getEntityRaw()->ensureComponent<ContextMenu2D>()->setCallbackFunction(
            [colorEditRaw](MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem("Change Color/Red", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_red); });
                _menuListTree->addItem("Change Color/Green", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_green); });
                _menuListTree->addItem("Change Color/Blue", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_blue); });

                _menuListTree->addItem("Change Alpha/0%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 0)); });
                _menuListTree->addItem("Change Alpha/33%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255 * 0.33)); });
                _menuListTree->addItem("Change Alpha/66%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255 * 0.66)); });
                _menuListTree->addItem("Change Alpha/100%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255)); });
            });
        */

        /*
        MenuBar2DPtr menuBar = UIHelper::CreateDefaultMenuBarList(
            Vec2DF(500.0f, 20.0f),
            Vec2DF::c_zero,
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        menuBar->addOption("File", "Option 1", [](String const& _text) { Log("Option 1"); });
        menuBar->addOption("File", "Option 2", [](String const& _text) { Log("Option 2"); });
        menuBar->addOption("File", "Option 3", [](String const& _text) { Log("Option 3"); });

        menuBar->addOption("Edit", "Option 4", [](String const& _text) { Log("Option 4"); });
        menuBar->addOption("Edit", "Option 5", [](String const& _text) { Log("Option 5"); });
        menuBar->addOption("Edit", "Option 6", [](String const& _text) { Log("Option 6"); });
        */
        

        /*
        menuBar->getEntityRaw()->ensureComponent<ContextMenu2D>()->setCallbackFunction(
            [colorEditRaw](MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem("Change Color/Red", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_red); });
                _menuListTree->addItem("Change Color/Green", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_green); });
                _menuListTree->addItem("Change Color/Blue", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32::c_blue); });

                _menuListTree->addItem("Change Alpha/0%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 0)); });
                _menuListTree->addItem("Change Alpha/33%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255 * 0.33)); });
                _menuListTree->addItem("Change Alpha/66%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255 * 0.66)); });
                _menuListTree->addItem("Change Alpha/100%", [colorEditRaw](String const& _option) { colorEditRaw->setColor(ColorU32(colorEditRaw->getColor(), 255)); });
            });
            */
        //MenuListTree2DPtr menuList = menuBar->ensureMenu("File");

        /*
        Slider2DPtr slider = UIHelper::CreateDefaultSlider(
            0.25f,
            Vec2DF(150, 18),
            Vec2DF(270, 200),
            panel00->getTransform(),
            this,
            Vec2DF::c_zero,
            Vec2DF::c_zero);
            */
        

        /*
        ColorPickerManager::GetInstancePtr()->openColorPicker(
            [](ColorU32 const& _color)
            {
            });
        */
        

        /*
        Transform2DPtr const& positionEdit = createVec3DFEdit("Position", layout->getTransform());
        Transform2DPtr const& rotationEdit = createVec3DFEdit("Rotation", layout->getTransform());
        Transform2DPtr const& scaleEdit = createVec3DFEdit("Scale", layout->getTransform());
        */

        /*
        SpriteHelper::CreateSprite(
            ColorU32(255, 0, 0),
            Vec2DF(160, 30),
            Vec2DF(100, 100),
            materialManager->getColorTextureMaterial(),
            panel00->getTransform(),
            this);

        SpriteHelper::CreateSprite(
            ColorU32(0, 255, 0),
            Vec2DF(30, 50),
            Vec2DF(200, 200),
            materialManager->getColorTextureMaterial(),
            panel00->getTransform(),
            this);

        SpriteHelper::CreateSprite(
            ColorU32(0, 0, 255),
            Vec2DF(160, 90),
            Vec2DF(300, 300),
            materialManager->getColorTextureMaterial(),
            panel00->getTransform(),
            this);
            */

        /*
        HorizontalLayout2DPtr horizontalLayout = panel00->getEntityRaw()->ensureComponent<HorizontalLayout2D>();
        horizontalLayout->setHorizontalAlignment(HorizontalAlignment2D::Left);
        horizontalLayout->setVerticalAlignment(VerticalAlignment2D::Top);
        horizontalLayout->setExpand(true);
        horizontalLayout->setSpacing(0.0f);
        // horizontalLayout->setPaddingTop(50.0f);
        horizontalLayout->setPaddingRight(100.0f);
        horizontalLayout->setControlChildHeight(false);
        horizontalLayout->setControlChildWidth(true);
        */

        /*
        VerticalLayout2DPtr verticalLayout = panel00->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        verticalLayout->setHorizontalAlignment(HorizontalAlignment2D::Right);
        verticalLayout->setVerticalAlignment(VerticalAlignment2D::Bottom);
        verticalLayout->setExpand(false);
        verticalLayout->setSpacing(0.0f);
        // verticalLayout->setPaddingTop(50.0f);
        verticalLayout->setPaddingRight(0.0f);
        verticalLayout->setControlChildHeight(false);
        verticalLayout->setControlChildWidth(false);
        */


        /*
        UIHelper::CreateDefaultEditBox(
            "123456",
            Vec2DF(182, 18),
            Vec2DF(-90, 0),
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);

        SystemTextEditBox2DPtr editBox = UIHelper::CreateDefaultEditBox(
            "EditBox2",
            Vec2DF(100, 18),
            Vec2DF(-90, -20),
            panel00->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        editBox->setText("Tinaynox!");
        */
    }

    //////////////////////////////////////////
    Transform2DPtr const& SceneExample::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(53, 18),
            Vec2DF(0, 0),
            _parent,
            this);
        layout->setSpacing(5.0f);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            _name,
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            this,
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(ColorU32::c_black);

        SystemTextEditBox2DPtr xEditBox = UIHelper::CreateDefaultEditBox(
            "0",
            Vec2DF(85, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            this,
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);

        

        return layout->getTransform();
    }

    //////////////////////////////////////////
    Transform2DPtr const& SceneExample::createVec3DFEdit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(450, 18),
            Vec2DF(0, 0),
            _parent,
            this);
        layout->setExpand(true);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            _name,
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            this,
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(ColorU32::c_black);

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2DF(200, 18),
                Vec2DF(0, 0),
                layout->getTransform(),
                this);
            layout2->setSpacing(10.0f);

            Transform2DPtr const& editX = createF32Edit("X", layout2->getTransform());
            Transform2DPtr const& editY = createF32Edit("Y", layout2->getTransform());
            Transform2DPtr const& editZ = createF32Edit("Z", layout2->getTransform());
        }

        return layout->getTransform();
    }


} // namespace Maze
//////////////////////////////////////////
