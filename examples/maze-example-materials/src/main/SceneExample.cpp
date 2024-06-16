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
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer3D.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "maze-plugin-loader-dds/MazeLoaderDDSPlugin.hpp"
#include "maze-plugin-loader-tga/MazeLoaderTGAPlugin.hpp"
#include "maze-plugin-loader-jpg/MazeLoaderJPGPlugin.hpp"
#include "maze-plugin-loader-fbx/MazeLoaderFBXPlugin.hpp"
#include "Example.hpp"
#include "ExampleFPSCameraController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Materials";
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
        MAZE_LOAD_PLATFORM_PLUGIN(ProfilerView);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG);
        // MAZE_LOAD_PLATFORM_PLUGIN(LoaderDDS);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderTGA);
        // MAZE_LOAD_PLATFORM_PLUGIN(LoaderJPG);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderFBX);

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

        if (m_canvas)
        {
            UIElement2D* canvasUIElement = m_canvas->getEntityRaw()->getComponentRaw<UIElement2D>();
            if (canvasUIElement)
            {
                canvasUIElement->eventCursorPressIn.unsubscribe(this);
                canvasUIElement->eventCursorDrag.unsubscribe(this);
            }
        }

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
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
                     
        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

        UIElement2DPtr canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        canvasUIElement->eventCursorPressIn.subscribe(this, &SceneExample::processCursorPress);
        canvasUIElement->eventCursorDrag.subscribe(this, &SceneExample::processCursorDrag);


        m_hintText = SystemUIHelper::CreateSystemText(
            "",
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F32::c_zero,
            Vec2F32(10.0f, -10.0f),
            m_canvas->getTransform(),
            this,
            Vec2F32(0.0f, 1.0f),
            Vec2F32(0.0f, 1.0f));
        m_hintText->setColor(ColorU32(255, 255, 255, 220));
        m_hintText->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getBuiltinSystemFont(BuiltinSystemFontType::DefaultOutlined));
        updateHintText();


        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneExample::notifyKeyboard);

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity();
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        //mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        mainLight3D->getTransform()->setLocalRotation(0.408979f, 0.906161f, -0.068055f, -0.083529f);
        
        lightEntity->ensureComponent<Name>("Light");


        Vec2F32 levelSize(100.0f, 30.0f);


        // FPS Controller
        EntityPtr fpsControllerEntity = createEntity();
        m_fpsController = fpsControllerEntity->ensureComponent<ExampleFPSCameraController>();
        m_fpsController->setLevelSize(levelSize);
        m_fpsController->setYawAngle(Math::DegreesToRadians(180.0f));


        // Camera
        m_camera3D = m_fpsController->getCamera3D();
        m_camera3D->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_camera3D->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
        
        getLightingSettings()->setSkyBoxMaterial("Skybox02.mzmaterial");


        m_simpleLevelConfig.floorMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Checkerboard00.mzmaterial");
        m_simpleLevelConfig.wallMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Checkerboard00.mzmaterial");
        ExampleHelper::BuildSimpleLevel(
            this,
            levelSize,
            m_simpleLevelConfig);

        

        F32 const torusKnotScale = 20.0f;

        addMeshPreview("TorusKnot.fbx", "Unlit00.mzmaterial", "Unlit", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "BlinnPhong00.mzmaterial", "Blinn-Phong Lighting", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "BlinnPhongAdd00.mzmaterial", "Additive Blend", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "BlinnPhongAlpha00.mzmaterial", "Alpha Blend", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "Fresnel00.mzmaterial", "Fresnel", torusKnotScale);
        addMeshPreviewSpace();
        
        addMeshPreview("TorusKnot.fbx", "XRayFront00.mzmaterial", "XRay", torusKnotScale);
        addMeshPreviewSpace();
        m_meshData.back().renderer->addMaterial("XRayBack00.mzmaterial");
        m_meshData.back().renderer->getEntityRaw()->getComponent<Transform3D>()->translate(Vec3F(0.0f, -1.0f, 0.0f));
        m_meshData.back().renderer->getEntityRaw()->createComponent<SinMovement3D>(
            Vec3F::c_unitY,
            2.0f,
            1.0f);

        addMeshPreview("TorusKnot.fbx", "Checkerboard01.mzmaterial", "Checkerboard", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "GoldTest.mzmaterial", "Gold", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnot.fbx", "Cel00.mzmaterial", "Cel/Toon", torusKnotScale);
        addMeshPreviewSpace();
        

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(_mainRenderWindowViewport);
        m_camera3D->setViewport(_mainRenderWindowViewport);
    }

    //////////////////////////////////////////
    void SceneExample::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);


        m_fpsController->setForward(InputManager::GetInstancePtr()->getKeyState(KeyCode::W));
        m_fpsController->setBackward(InputManager::GetInstancePtr()->getKeyState(KeyCode::S));
        m_fpsController->setRight(InputManager::GetInstancePtr()->getKeyState(KeyCode::A));
        m_fpsController->setLeft(InputManager::GetInstancePtr()->getKeyState(KeyCode::D));
        m_fpsController->setJump(InputManager::GetInstancePtr()->getKeyState(KeyCode::Space));
    }

    //////////////////////////////////////////
    void SceneExample::processCursorPress(Vec2F32 const& _positionOS, CursorInputEvent const& _event)
    {
        if (_event.button == 0 || _event.button == 1)
        {
            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void SceneExample::processCursorDrag(Vec2F32 const& _positionOS, CursorInputEvent const& _event)
    {
        Vec2F32 deltaPosition = _positionOS - m_cursorPositionLastFrame;

        if (_event.button == 0 || _event.button == 1)
        {
            F32 yawAngle = m_fpsController->getYawAngle();
            F32 pitchAngle = m_fpsController->getPitchAngle();
            yawAngle += deltaPosition.x * 0.0075f;
            pitchAngle -= deltaPosition.y * 0.0075f;
            pitchAngle = Math::Clamp(pitchAngle, -Math::c_halfPi, Math::c_halfPi);
            m_fpsController->setYawAngle(yawAngle);
            m_fpsController->setPitchAngle(pitchAngle);

            m_cursorPositionLastFrame = _positionOS;
        }
    }


    //////////////////////////////////////////
    void SceneExample::addMeshPreview(
        String const& _meshName,
        String const& _materialName,
        String const& _text,
        F32 _scale)
    {
        Timer timer;

        EntityPtr objectEntity = createEntity("MeshPreview");
        Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
        MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
        AssetFilePtr const& meshAssetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_meshName);

        U32 timerStart = timer.getMilliseconds();
        MeshPtr const& mesh = MeshManager::GetInstancePtr()->loadMesh(meshAssetFile);
        U32 loadTime = timer.getMilliseconds() - timerStart;


        RenderMeshPtr const& renderMesh = RenderMesh::Create(mesh);
        meshRenderer->setRenderMesh(renderMesh);
        meshRenderer->setMaterial(_materialName);


        transform->setLocalScale(_scale);
        transform->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
        
        Rotor3DPtr rotor = objectEntity->ensureComponent<Rotor3D>();
        rotor->setAxis(Vec3F32::c_unitY);
        rotor->setSpeed(0.3f);
        rotor->setActive(m_meshMovementEnabled);

        ExampleMeshData meshData;
        meshData.renderer = meshRenderer;
        meshData.file = meshAssetFile;
        meshData.loadTime = loadTime;
        meshData.material = meshRenderer->getMaterial();
        
        meshData.rotor = rotor;
        
        F32 x = ((S32)m_meshData.size() - 6) * 3.0f + m_meshesOffset;
        transform->setLocalPosition(-x, 2.0f, -8.0f);


        {
            EntityPtr pedestalEntity = createEntity("Pedestal");
            Transform3DPtr pedestalTransform = pedestalEntity->createComponent<Transform3D>();
            pedestalTransform->setLocalScale(1.0f, 0.2f, 1.0f);
            pedestalTransform->setLocalPosition(
                transform->getLocalPosition().x,
                pedestalTransform->getLocalScale().y * 0.5f,
                transform->getLocalPosition().z);
            MeshRendererPtr meshRenderer = pedestalEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getBuiltinRenderMesh(BuiltinRenderMeshType::Cylinder));

            meshRenderer->setMaterial("Checkerboard00.mzmaterial");
        }

        if (renderMesh)
        {
            EntityPtr labelEntity = createEntity("Label");
            SystemTextRenderer3DPtr labelRenderer = labelEntity->ensureComponent<SystemTextRenderer3D>();
            labelRenderer->getTransform()->setLocalPosition(
                transform->getLocalPosition().x,
                0.75f,
                transform->getLocalPosition().z);
            labelRenderer->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);

            S32 indicesCount = 0;
            S32 verticesCount = 0;

            for (S32 i = 0, in = (S32)mesh->getSubMeshesCount(); i != in; ++i)
            {
                SubMeshPtr const& subMesh = mesh->getSubMesh(i);
                indicesCount += (S32)subMesh->getIndicesCount();
                verticesCount += (S32)subMesh->getVerticesCount(VertexAttributeSemantic::Position);
                switch (subMesh->getRenderDrawTopology())
                {
                    case RenderDrawTopology::Points:
                    {
                        break;
                    }
                    case RenderDrawTopology::Lines:
                    {
                        break;
                    }
                    case RenderDrawTopology::Triangles:
                    {
                        break;
                    }
                    default:
                    {
                        MAZE_NOT_IMPLEMENTED;
                    }
                }
            }

            labelRenderer->setText(_text);

            labelRenderer->setFontSize(16);
            labelRenderer->getTransform()->setLocalScaleX(0.7f);
            labelRenderer->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault3DOutlined());

        }

        m_meshData.emplace_back(meshData);
    }

    //////////////////////////////////////////
    void SceneExample::addMeshPreviewSpace()
    {
        m_meshesOffset += 3.0f;
    }

    //////////////////////////////////////////
    void SceneExample::updateHintText()
    {
        m_hintText->setTextFormatted(
            "[CONTROLS]\n"
            "Movement - WASD, Jump - Space, Camera - RMB (Hold)\n"
            "%s - R\n"
            "\n"
            "[INFO]\n"
            "Mesh Movement: %s",
            m_meshMovementEnabled ? "Disable Mesh Movement" : "Enable Mesh Movement",
            m_meshMovementEnabled ? "ON" : "OFF"
        );
    }


    //////////////////////////////////////////
    void SceneExample::setMeshMovementEnabled(bool _value)
    {
        if (m_meshMovementEnabled == _value)
            return;

        m_meshMovementEnabled = _value;

        for (ExampleMeshData const& meshData : m_meshData)
        {
            meshData.rotor->setActive(m_meshMovementEnabled);

            SinMovement3D* sinMovement = meshData.rotor->getEntityRaw()->getComponentRaw<SinMovement3D>();
            if (sinMovement)
                sinMovement->setActive(m_meshMovementEnabled);
        }
        updateHintText();
    }

    //////////////////////////////////////////
    void SceneExample::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::R:
                    {
                        setMeshMovementEnabled(!m_meshMovementEnabled);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }


} // namespace Maze
//////////////////////////////////////////
