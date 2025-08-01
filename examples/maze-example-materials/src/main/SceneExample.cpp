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
#include "maze-core/ecs/MazeEcsWorld.hpp"
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
#include "maze-core/settings/MazeSettingsManager.hpp"
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
#include "ExampleCommonSettings.hpp"
#include "LevelBloomController.hpp"


//////////////////////////////////////////
#define DISTORTION_BUFFER_DIV 1u


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
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, BaseSceneExample);

    //////////////////////////////////////////
    SceneExample::SceneExample()
    {

    }

    //////////////////////////////////////////
    SceneExample::~SceneExample()
    {
        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);

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
        if (!BaseSceneExample::init(Vec2F(100.0f, 30.0f)))
            return false;

        Example::GetInstancePtr()->getMainRenderWindow()->setVSync(true);

        m_camera3D->getTransform()->setLocalRotationDegrees(0.0f, 0.0f, 0.0f);
        m_fpsController->setYawAngle(Math::DegreesToRadians(0.0f));
        m_mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneExample::notifyKeyboard);

        getLightingSettings()->setSkyBoxMaterial("Skybox00.mzmaterial");
        m_simpleLevelConfig.floorMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");
        m_simpleLevelConfig.wallMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");
        ExampleHelper::BuildSimpleLevel(
            this,
            m_fpsController->getLevelSize(),
            m_simpleLevelConfig);

        

        F32 const torusKnotScale = 20.0f;

        addMeshPreview("TorusKnotUV.fbx", "Unlit00.mzmaterial", "Unlit", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "Lambert00.mzmaterial", "Lambert Lighting", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "BlinnPhong00.mzmaterial", "Blinn-Phong Lighting", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "BlinnPhongAdd00.mzmaterial", "Additive Blend", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "BlinnPhongAlpha00.mzmaterial", "Alpha Blend", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "Fresnel00.mzmaterial", "Fresnel", torusKnotScale);
        addMeshPreviewSpace();
        
        addMeshPreview("TorusKnotUV.fbx", "XRayFront00.mzmaterial", "XRay", torusKnotScale);
        addMeshPreviewSpace();
        m_meshData.back().renderer->addMaterial("XRayBack00.mzmaterial");
        m_meshData.back().renderer->getEntityRaw()->getComponent<Transform3D>()->translate(Vec3F(0.0f, -1.0f, 0.0f));
        m_meshData.back().renderer->getEntityRaw()->createComponent<SinMovement3D>(
            Vec3F::c_unitY,
            2.0f,
            1.0f);

        addMeshPreview("TorusKnotUV.fbx", "Chessboard01.mzmaterial", "Chessboard", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "GoldTest.mzmaterial", "Gold (SER)", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "SkyboxReflection.mzmaterial", "Skybox Reflection", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "Cel00.mzmaterial", "Cel/Toon", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "BlinnPhongHDR00.mzmaterial", "Emission (HDR)", torusKnotScale);
        addMeshPreviewSpace();

        {
            EntityPtr objectEntity = addMeshPreview("TorusKnotNM.fbx", "NormalMapping.mzmaterial", "Normal Mapping", torusKnotScale);
            addMeshPreviewSpace();

            EntityPtr labelEntity = createEntity("Label");
            SystemTextRenderer3DPtr labelRenderer = labelEntity->ensureComponent<SystemTextRenderer3D>();
            labelRenderer->getTransform()->setLocalPosition(objectEntity->getComponent<Transform3D>()->getLocalPosition());
            labelRenderer->getTransform()->translate(Vec3F(0.0f, -2.0f, 7.75f));
            labelRenderer->getTransform()->setLocalRotationDegrees(40.0f, 0.0f, 0.0f);

            labelRenderer->setText("DAYdiecast\nwas here");
            labelRenderer->setFontSize(16);
            labelRenderer->getTransform()->setLocalScaleX(0.7f);
            labelRenderer->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault3DOutlined());
        }

        addMeshPreview("TorusKnotUV.fbx", "Dissolve00.mzmaterial", "Dissolve", torusKnotScale);
        addMeshPreviewSpace();

        {
            EntityPtr objectEntity = addMeshPreview("TorusKnotUV.fbx", "Distortion00.mzmaterial", "Distortion", torusKnotScale);
            addMeshPreviewSpace();
            m_meshData.back().renderer->getRenderMask()->setMask((S32)DefaultRenderMask::UserMask0);

            Texture2DPtr depthTexture = m_renderBuffer->getDepthTexture()->cast<Texture2D>();

            MeshRenderer* meshRenderer = objectEntity->getComponentRaw<MeshRenderer>();
            meshRenderer->getMaterial()->setUniform(
                MAZE_HCS("u_depthMap"), depthTexture);
        }

        addMeshPreview("TorusKnotUV.fbx", "Snow.mzmaterial", "Snow", torusKnotScale);
        addMeshPreviewSpace();

        addMeshPreview("TorusKnotUV.fbx", "Hologram00.mzmaterial", "Hologram", torusKnotScale);
        addMeshPreviewSpace();

        /*
        addMeshPreview("TorusKnotUV.fbx", "Raymarching00.mzmaterial", "Ray Marching", torusKnotScale);
        addMeshPreviewSpace();
        */

        /*
        addMeshPreview("TorusKnotUV.fbx", "Gem00.mzmaterial", "Gem", torusKnotScale);
        addMeshPreviewSpace();
        */
        

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        BaseSceneExample::update(_dt);
    }

    //////////////////////////////////////////
    EntityPtr SceneExample::addMeshPreview(
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
        
        F32 x = ((S32)m_meshData.size() - 16) * 2.0f + m_meshesOffset;
        transform->setLocalPosition(x, 2.0f, 8.0f);


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

            meshRenderer->setMaterial("Chessboard00.mzmaterial");
        }

        if (renderMesh)
        {
            EntityPtr labelEntity = createEntity("Label");
            SystemTextRenderer3DPtr labelRenderer = labelEntity->ensureComponent<SystemTextRenderer3D>();
            labelRenderer->getTransform()->setLocalPosition(
                transform->getLocalPosition().x,
                0.75f,
                transform->getLocalPosition().z);
            // labelRenderer->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);

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

        return objectEntity;
    }

    //////////////////////////////////////////
    void SceneExample::addMeshPreviewSpace()
    {
        m_meshesOffset += 2.0f;
    }

    //////////////////////////////////////////
    void SceneExample::updateHintText()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();        

        m_hintText->setTextFormatted(
            "[CONTROLS]\n"
            "Movement - WASD, Jump - Space, Camera - RMB (Hold)\n"
            "%s - R\n"
            "%s - P\n"
            "\n"
            "[INFO]\n"
            "Mesh Movement: %s\n"
            "Post Processing: %s",
            m_meshMovementEnabled ? "Disable Mesh Movement" : "Enable Mesh Movement",
            exampleCommonSettings->getBloomEnabled() ? "Disable Post Processing" : "Enable Post Processing",
            m_meshMovementEnabled ? "ON" : "OFF",
            exampleCommonSettings->getBloomEnabled() ? "ON" : "OFF"
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
                    case KeyCode::P:
                    {
                        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

                        exampleCommonSettings->setBloomEnabled(
                            !exampleCommonSettings->getBloomEnabled());
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
