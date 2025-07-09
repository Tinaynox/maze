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
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeSystemCursorManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
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
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-plugin-loader-ogg/MazeLoaderOGGPlugin.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Cursors";
    }

    //////////////////////////////////////////
    void LoadFirstExampleScene(SceneManager* _sceneManager)
    {
        Example::GetInstancePtr()->loadCoreGameAssets();
        _sceneManager->loadScene<SceneExample>();
    }
    
    //////////////////////////////////////////
    bool IsLoadAllAssets()
    {
        return false;
    }

    //////////////////////////////////////////
    bool LoadPlugins()
    {
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderOGG);

        return true;
    }


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, EcsRenderScene);

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
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport)
    {        
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

    }

    //////////////////////////////////////////
    void SceneExample::create2D()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();

        // Canvas
        Maze::EntityPtr canvasEntity = createEntity();
        Maze::Transform2DPtr canvasTransform2D = canvasEntity->createComponent<Maze::Transform2D>();
        m_canvas = canvasEntity->createComponent<Maze::Canvas>();
        m_canvas->setRenderTarget(renderTarget);
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setClearColorFlag(true);

        
        SpriteRenderer2DPtr panel00 = SpriteHelper::CreateSprite(
            ColorU32(203, 203, 203),
            Vec2F32(640, 480),
            Vec2F32(0, 0),
            materialManager->getSpriteMaterial(),
            canvasTransform2D,
            this);

        auto createSystemCursorButton =
            [&](Vec2F const& pos, BuiltinSystemCursorType _type)
            {
                ClickButton2DPtr cursorButton = UIHelper::CreateDefaultClickButton(
                    _type.toCString(),
                    { 70.0f, 18.0f },
                    pos,
                    panel00->getTransform(),
                    this,
                    { 0.0f, 0.0f });
                cursorButton->eventCursorPressIn.subscribe(
                    [_type](Button2D* _button, Vec2F const& _positionOS, CursorInputEvent& _inputEvent)
                    {
                        SystemCursorPtr const& cursor = SystemCursorManager::GetInstancePtr()->ensureBuiltinSystemCursor(_type);
                        Example::GetInstancePtr()->getMainRenderWindow()->getWindow()->setCursor(cursor);
                    });
            };

        createSystemCursorButton({ 50.0f + 80.0f * 0, 420.0f }, BuiltinSystemCursorType::Arrow);
        createSystemCursorButton({ 50.0f + 80.0f * 1, 420.0f }, BuiltinSystemCursorType::EditText);
        createSystemCursorButton({ 50.0f + 80.0f * 2, 420.0f }, BuiltinSystemCursorType::Wait);
        createSystemCursorButton({ 50.0f + 80.0f * 3, 420.0f }, BuiltinSystemCursorType::Cross);
        createSystemCursorButton({ 50.0f + 80.0f * 4, 420.0f }, BuiltinSystemCursorType::ArrowUp);

        createSystemCursorButton({ 50.0f + 80.0f * 0, 370.0f }, BuiltinSystemCursorType::SizeNWSE);
        createSystemCursorButton({ 50.0f + 80.0f * 1, 370.0f }, BuiltinSystemCursorType::SizeNESW);
        createSystemCursorButton({ 50.0f + 80.0f * 2, 370.0f }, BuiltinSystemCursorType::SizeWE);
        createSystemCursorButton({ 50.0f + 80.0f * 3, 370.0f }, BuiltinSystemCursorType::SizeNS);
        createSystemCursorButton({ 50.0f + 80.0f * 4, 370.0f }, BuiltinSystemCursorType::SizeAll);

        createSystemCursorButton({ 50.0f + 80.0f * 0, 320.0f }, BuiltinSystemCursorType::No);
        createSystemCursorButton({ 50.0f + 80.0f * 1, 320.0f }, BuiltinSystemCursorType::HandPress);
        createSystemCursorButton({ 50.0f + 80.0f * 2, 320.0f }, BuiltinSystemCursorType::Help);


        auto createAssetSystemCursorButton =
            [&](Vec2F const& pos, CString _assetName)
            {
                ClickButton2DPtr cursorButton = UIHelper::CreateDefaultClickButton(
                    FileHelper::GetFileNameWithoutExtension(_assetName).toUTF8().c_str(),
                    { 120.0f, 18.0f },
                    pos,
                    panel00->getTransform(),
                    this,
                    { 0.0f, 0.0f });
                cursorButton->eventCursorPressIn.subscribe(
                    [_assetName](Button2D* _button, Vec2F const& _positionOS, CursorInputEvent& _inputEvent)
                    {
                        SystemCursorPtr const& cursor = SystemCursorManager::GetInstancePtr()->getOrLoadSystemCursor(_assetName);
                        Example::GetInstancePtr()->getMainRenderWindow()->getWindow()->setCursor(cursor);
                    });
            };

        createAssetSystemCursorButton({ 70.0f + 130.0f * 0, 220.0f }, "Crosshair2.mzsyscur");
        createAssetSystemCursorButton({ 70.0f + 130.0f * 1, 220.0f }, "Crosshair3.mzsyscur");
        createAssetSystemCursorButton({ 70.0f + 130.0f * 2, 220.0f }, "Cursor002.mzsyscur");
        createAssetSystemCursorButton({ 70.0f + 130.0f * 3, 220.0f }, "Cursor003.mzsyscur");
    }

} // namespace Maze
//////////////////////////////////////////
