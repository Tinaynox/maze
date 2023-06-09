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
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
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
#include "ExampleHelper.hpp"
#include "ExampleFPSCameraController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Meshes and Textures";
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
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderDDS);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderTGA);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderJPG);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderFBX);

        return true;
    }


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

        if (m_canvas)
        {
            UIElement2D* canvasUIElement = m_canvas->getEntityRaw()->getComponentRaw<UIElement2D>();
            if (canvasUIElement)
            {
                canvasUIElement->eventCursorPressIn.unsubscribe(this);
                canvasUIElement->eventCursorDrag.unsubscribe(this);
            }
        }

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
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;
                     
        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

        UIElement2DPtr canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        canvasUIElement->eventCursorPressIn.subscribe(this, &SceneExample::processCursorPress);
        canvasUIElement->eventCursorDrag.subscribe(this, &SceneExample::processCursorDrag);
        // m_canvas->setClearColorFlag(false);
        // m_canvas->setClearColor(ColorU32::c_zero);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();



        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity();
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        lightEntity->ensureComponent<Name>("Light");


        Vec2DF levelSize(100.0f, 30.0f);


        // FPS Controller
        EntityPtr fpsControllerEntity = createEntity();
        m_fpsController = fpsControllerEntity->ensureComponent<ExampleFPSCameraController>();
        m_fpsController->setLevelSize(levelSize);


        // Camera
        m_camera3D = m_fpsController->getCamera3D();
        m_camera3D->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        
        getLightingSettings()->setSkyBoxMaterial("Skybox00.mzmaterial");


        ExampleHelper::SimpleLevelConfig config;
        config.floorMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Terrain00.mzmaterial");
        config.wallMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Wall00.mzmaterial");
        ExampleHelper::BuildSimpleLevel(
            this,
            levelSize,
            config);


        // BMP
        addTexturePreview("sp3d_logo_16.bmp");
        addTexturePreview("sp3d_logo_24.bmp");
        addTexturePreview("sp3d_logo_32.bmp");
        addTexturePreviewSpace();

        // PNG
        addTexturePreview("sp3d_logo_32.png");
        addTexturePreview("sp3d_logo_64.png");
        addTexturePreviewSpace();

        // JPG
        addTexturePreview("sp3d_logo_lq.jpg");
        addTexturePreview("sp3d_logo_hq.jpg");
        addTexturePreviewSpace();

        // TGA
        addTexturePreview("sp3d_logo_16.tga");
        addTexturePreview("sp3d_logo_16_rle.tga");
        addTexturePreview("sp3d_logo_24.tga");
        addTexturePreview("sp3d_logo_24_rle.tga");
        addTexturePreview("sp3d_logo_32.tga");
        addTexturePreview("sp3d_logo_32_rle.tga");
        addTexturePreviewSpace();

        // DDS
        addTexturePreview("sp3d_logo_dxt1.dds");
        addTexturePreview("sp3d_logo_dxt3.dds");
        addTexturePreview("sp3d_logo_dxt5.dds");
        addTexturePreviewSpace();



        
        // Drone
        F32 const drone0Scale = 0.35f;
        addMeshPreview("drone0.obj", "drone0.mzmaterial", drone0Scale);
        addMeshPreview("drone0.fbx", "drone0.mzmaterial", drone0Scale);
        // addMeshPreview("drone1.fbx", "drone0.mzmaterial");
        addMeshPreviewSpace();

        // testBox
        F32 const testBoxScale = 0.75f;
        addMeshPreview("testBox.obj", "testBox.mzmaterial", testBoxScale);
        addMeshPreview("testBox.fbx", "testBox.mzmaterial", testBoxScale);


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
        ECSRenderScene::update(_dt);


        m_fpsController->setForward(InputManager::GetInstancePtr()->getKeyState(KeyCode::W));
        m_fpsController->setBackward(InputManager::GetInstancePtr()->getKeyState(KeyCode::S));
        m_fpsController->setRight(InputManager::GetInstancePtr()->getKeyState(KeyCode::A));
        m_fpsController->setLeft(InputManager::GetInstancePtr()->getKeyState(KeyCode::D));
    }

    //////////////////////////////////////////
    void SceneExample::processCursorPress(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        if (_event.button == 1)
        {
            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void SceneExample::processCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        Vec2DF deltaPosition = _positionOS - m_cursorPositionLastFrame;

        if (_event.button == 1)
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
    void SceneExample::addTexturePreview(String const& _textureName)
    {
        EntityPtr objectEntity = createEntity("TexturePreview");
        Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
        MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
        meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultQuadMesh());

        MaterialPtr material = MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::ColorTexture)->createCopy();
        material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::LessEqual);
        Texture2DPtr const& texture2D = TextureManager::GetCurrentInstancePtr()->getTexture2D(_textureName);
        material->setUniform("u_baseMap", texture2D);
        meshRenderer->setMaterial(material);
        
        AssetFilePtr const& textureAssetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);

        F32 x = (m_texturesCount - 10) * 3.0f + m_texturesOffset;
        transform->setLocalPosition(-x, 1.5f, -8.0f);
        transform->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);


        {
            EntityPtr pedestalEntity = createEntity("Pedestal");
            Transform3DPtr pedestalTransform = pedestalEntity->createComponent<Transform3D>();
            pedestalTransform->setLocalScale(1.0f, 0.2f, 1.0f);
            pedestalTransform->setLocalPosition(
                transform->getLocalPosition().x,
                pedestalTransform->getLocalScale().y * 0.5f,
                transform->getLocalPosition().z);
            pedestalTransform->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
            MeshRendererPtr meshRenderer = pedestalEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getBuiltinRenderMesh(BuiltinRenderMeshType::Cylinder));

            meshRenderer->setMaterial("Pedestal00.mzmaterial");
        }

        if (texture2D)
        {
            EntityPtr labelEntity = createEntity("Label");
            SystemTextRenderer3DPtr labelRenderer = labelEntity->ensureComponent<SystemTextRenderer3D>();
            labelRenderer->getTransform()->setLocalPosition(
                transform->getLocalPosition().x,
                0.65f,
                transform->getLocalPosition().z);
            labelRenderer->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);

            S32 bytesCount = PixelFormat::CalculateRequiredBytes(
                texture2D->getWidth(),
                texture2D->getHeight(),
                1,
                texture2D->getInternalPixelFormat());
            labelRenderer->setTextFormatted(
                "%s (%d b)\n"
                "Size: %dx%d (%d b)\n"
                "Format: %s\n"
                "Min: %s Mag: %s\n"
                "WrapS: %s WrapT: %s",
                _textureName.c_str(),
                (S32)textureAssetFile->getFileSize(),
                texture2D->getWidth(),
                texture2D->getHeight(),
                bytesCount,
                PixelFormat::ToString(texture2D->getInternalPixelFormat()).c_str(),
                texture2D->getMinFilter().toCString(), texture2D->getMagFilter().toCString(),
                texture2D->getWrapS().toCString(), texture2D->getWrapT().toCString());

            labelRenderer->setFontSize(8);
            labelRenderer->getTransform()->setLocalScaleX(0.7f);
            labelRenderer->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault3DOutlined());

        }

        ++m_texturesCount;
    }

    //////////////////////////////////////////
    void SceneExample::addTexturePreviewSpace()
    {
        m_texturesOffset += 3.0f;
    }

    //////////////////////////////////////////
    void SceneExample::addMeshPreview(
        String const& _meshName,
        String const& _materialName,
        F32 _scale)
    {
        EntityPtr objectEntity = createEntity("MeshPreview");
        Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
        MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
        AssetFilePtr const& meshAssetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_meshName);
        MeshPtr const& mesh = MeshManager::GetCurrentInstancePtr()->loadMesh(meshAssetFile);
        RenderMeshPtr const& renderMesh = RenderMesh::Create(mesh);
        meshRenderer->setRenderMesh(renderMesh);
        meshRenderer->setMaterial(_materialName);


        transform->setLocalScale(_scale);
        transform->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
        //transform->setLocalRotationDegrees(-90.0f, 0.0f, 0.0f);
        /*
        Rotor3DPtr rotor = objectEntity->ensureComponent<Rotor3D>();
        rotor->setAxis(Vec3DF::c_unitY);
        rotor->setSpeed(0.3f);
        */
        
        F32 x = (m_meshesCount - 1) * 3.0f + m_meshesOffset;
        transform->setLocalPosition(x, 1.5f, 8.0f);


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

            meshRenderer->setMaterial("Pedestal00.mzmaterial");
        }

        if (renderMesh)
        {
            EntityPtr labelEntity = createEntity("Label");
            SystemTextRenderer3DPtr labelRenderer = labelEntity->ensureComponent<SystemTextRenderer3D>();
            labelRenderer->getTransform()->setLocalPosition(
                transform->getLocalPosition().x,
                0.65f,
                transform->getLocalPosition().z);

            S32 verticesCount = 0;

            for (S32 i = 0, in = (S32)mesh->getSubMeshesCount(); i != in; ++i)
            {
                SubMeshPtr const& subMesh = mesh->getSubMesh(i);
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

            labelRenderer->setTextFormatted(
                "%s (%d b)\n"
                "Sub meshes count: %d\n"
                "Vertices count: %d (%d tr)",
                _meshName.c_str(),
                (S32)meshAssetFile->getFileSize(),
                (S32)mesh->getSubMeshesCount(),
                verticesCount, verticesCount/3);

            labelRenderer->setFontSize(8);
            labelRenderer->getTransform()->setLocalScaleX(0.7f);
            labelRenderer->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault3DOutlined());

        }

        ++m_meshesCount;
    }

    //////////////////////////////////////////
    void SceneExample::addMeshPreviewSpace()
    {
        m_meshesOffset += 3.0f;
    }


} // namespace Maze
//////////////////////////////////////////
