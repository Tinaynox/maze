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
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderController.hpp"
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
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#include <GL/GL.h>
#endif


//////////////////////////////////////////
extern bool g_applicationActive;
extern Maze::RenderTargetPtr g_defaultRenderTarget;


//////////////////////////////////////////
// Class SceneExample
//
//////////////////////////////////////////
MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, Maze::EcsRenderScene);

//////////////////////////////////////////
SceneExample::SceneExample()
    : m_timer(0.0f)
    , m_modelMatrix(Maze::Mat4F32::c_identity)
    , m_modelMatrix2(Maze::Mat4F32::c_identity)
    , m_modelMatrix3(Maze::Mat4F32::c_identity)
    , m_yawAngle(0.0f)
    , m_pitchAngle(0.0f)
    , m_cursorPositionLastFrame(Maze::Vec2F32::c_zero)
    , m_cursorDrag(false)
{
}

//////////////////////////////////////////
SceneExample::~SceneExample()
{
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    if (inputManager)
    {
        inputManager->eventMouse.unsubscribe(this);
    }
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
    if (!Maze::EcsRenderScene::init(g_defaultRenderTarget))
        return false;

    Maze::S32 p0 = Maze::EntityManager::GetInstancePtr()->getComponentPriority<Maze::Transform3D>();
    Maze::S32 p1 = Maze::EntityManager::GetInstancePtr()->getComponentPriority<Maze::Camera3D>();
   

    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    inputManager->eventMouse.subscribe(this, &SceneExample::notifyMouse);

    Maze::RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
    Maze::ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();


    Maze::SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();

    Maze::EntityManager* entityManager = Maze::EntityManager::GetInstancePtr();
    Maze::EcsWorldPtr const& world = entityManager->getDefaultWorld();

    createAndAddEntityWithComponent<Maze::RenderController>(MAZE_HCS("RenderController"),renderSystem);

    for (int i = 0; i < 4; ++i)
    {
        Maze::EntityPtr cameraEntity = Maze::Entity::Create();
        m_cameraTransform3D = cameraEntity->createComponent<Maze::Transform3D>();
        m_cameraTransform3D->setLocalPosition(Maze::Vec3F32(0.0f, 0.0f, -3.0f));
        Maze::Camera3DPtr camera3D = cameraEntity->createComponent<Maze::Camera3D>();
        camera3D->setRenderTarget(g_defaultRenderTarget);
        camera3D->setClipViewport(true);

        camera3D->setClearColor(Maze::ColorU32::Random());

        switch (i)
        {
            case 0: camera3D->setViewport(0.0f, 0.0f, 0.5f, 0.5f); break;
            case 1: camera3D->setViewport(0.0f, 0.5f, 0.5f, 0.5f); break;
            case 2: camera3D->setViewport(0.5f, 0.5f, 0.5f, 0.5f); break;
            case 3: camera3D->setViewport(0.5f, 0.0f, 0.5f, 0.5f); break;
        }
        world->addEntity(cameraEntity);
    }
    

    Maze::SpritePtr sprite = spriteManager->getSprite("Panel00.png");
    if (sprite)
        sprite->setSliceBorder(
            40.0f,
            34.0f,
            40.0f,
            34.0f);


    ////////////////////////////////////////////
    // 2D

    // Canvas
    for (int i = 0; i < 4; ++i)
    {
        Maze::EntityPtr canvasEntity = createEntity();
        Maze::Transform2DPtr canvasTransform2D = canvasEntity->createComponent<Maze::Transform2D>();
        Maze::CanvasPtr canvas = canvasEntity->createComponent<Maze::Canvas>();
        canvas->setRenderTarget(g_defaultRenderTarget);
        //canvas->setViewport(Maze::Rect2DF(0.5f, 0.5f, 0.5f, 0.5f));

        switch (i)
        {
            case 0: canvas->setViewport(0.0f, 0.0f, 0.5f, 0.5f); break;
            case 1: canvas->setViewport(0.0f, 0.5f, 0.5f, 0.5f); break;
            case 2: canvas->setViewport(0.5f, 0.5f, 0.5f, 0.5f); break;
            case 3: canvas->setViewport(0.5f, 0.0f, 0.5f, 0.5f); break;
        }

    
        Maze::MaterialPtr material = Maze::Material::Create("UI00.mzmaterial");
        {
            Maze::EntityPtr spriteRendererEntity = createEntity();
            Maze::Transform2DPtr transform = spriteRendererEntity->createComponent<Maze::Transform2D>();
            transform->setParent(canvasTransform2D);
            transform->setAnchor(Maze::Vec2F32(0.0f, 0.0f));
            transform->setPivot(Maze::Vec2F32::c_zero);
            transform->setSize(50, 50);
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(sprite);
            spriteRenderer->setMaterial(material);
        }
        {
            Maze::EntityPtr spriteRendererEntity = createEntity();
            Maze::Transform2DPtr transform = spriteRendererEntity->createComponent<Maze::Transform2D>();
            transform->setParent(canvasTransform2D);
            transform->setAnchor(Maze::Vec2F32(1.0f, 1.0f));
            transform->setPivot(Maze::Vec2F32(0.0f, 1.0f));
            transform->setSize(50, 50);
            transform->setLocalScale(Maze::Vec2F32(-1.0f, 1.0f));
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(sprite);
            spriteRenderer->setMaterial(Maze::Material::Create("UI00.mzmaterial"));
        }
        {
            Maze::EntityPtr spriteRendererEntity = createEntity();
            Maze::Transform2DPtr transform = spriteRendererEntity->createComponent<Maze::Transform2D>();
            transform->setParent(canvasTransform2D);
            transform->setAnchor(Maze::Vec2F32(0.0f, 1.0f));
            transform->setSize(50, 50);
            transform->setPivot(Maze::Vec2F32(0.0f, 1.0f));
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(sprite);
            spriteRenderer->setMaterial(Maze::Material::Create("UI00.mzmaterial"));
        }
        {
            Maze::EntityPtr spriteRendererEntity = createEntity();
            Maze::Transform2DPtr transform = spriteRendererEntity->createComponent<Maze::Transform2D>();
            transform->setParent(canvasTransform2D);
            transform->setAnchor(Maze::Vec2F32(1.0f, 0.0f));
            transform->setPivot(Maze::Vec2F32(0.0f, 0.0f));
            transform->setSize(50, 50);
            transform->setLocalScale(Maze::Vec2F32(-1.0f, 1.0f));
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(sprite);
            spriteRenderer->setMaterial(Maze::Material::Create("UI00.mzmaterial"));
        }
    }
    

    ////////////////////////////////////////////
    // 3D

    m_modelMatrix2 = Maze::Mat4F32::c_identity;
    m_modelMatrix3 = Maze::Mat4F32::c_identity;
    
    
    // Axes
    Maze::EntityPtr axesMeshRendererEntity = Maze::Entity::Create();
    axesMeshRendererEntity->createComponent<Maze::Transform3D>();
    m_axesMeshRenderer = axesMeshRendererEntity->createComponent<Maze::MeshRenderer>();
    m_axesMeshRenderer->setRenderMesh(Maze::RenderMesh::Create(Maze::MeshHelper::CreateCoordinateAxes()));
    m_axesMeshRenderer->setMaterial(Maze::Material::Create("Axes00.mzmaterial"));
    world->addEntity(axesMeshRendererEntity);

    // Main Light
    Maze::EntityPtr lightEntity = createEntity();
    Maze::Light3DPtr mainLight3D = lightEntity->createComponent<Maze::Light3D>();
    mainLight3D->setColor(Maze::ColorU32(255, 244, 214));
    mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
    mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
    lightEntity->ensureComponent<Maze::Name>("Light");

    // SMG
    Maze::RenderMeshPtr smgRenderMesh = Maze::RenderMesh::Create("SMG.obj");
    Maze::MaterialPtr smgMaterial = Maze::Material::Create("Test00.mzmaterial");
    {
        Maze::EntityPtr meshRendererEntity = createEntity();
        meshRendererEntity->createComponent<Maze::Transform3D>();
        m_meshRenderer = meshRendererEntity->createComponent<Maze::MeshRenderer>();
        m_meshRenderer->setRenderMesh(smgRenderMesh);
        m_meshRenderer->setMaterial(smgMaterial);
    }
    
    return true;
}

//////////////////////////////////////////
void SceneExample::update(Maze::F32 _dt)
{
    
    if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::Z))
    {    
        m_renderTarget->castRaw<Maze::RenderWindowOpenGL>()->getContext()->setWireframeRender(true);
    }
    else
    {
        m_renderTarget->castRaw<Maze::RenderWindowOpenGL>()->getContext()->setWireframeRender(false);
    }

    Maze::EcsScene::update(_dt);

    m_timer += _dt;

    Maze::RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
    if (renderSystem->getCurrentRenderTarget())
    {
        float f = (Maze::Math::Sin(m_timer) + 1)/2.0f;        

        m_modelMatrix = Maze::Mat4F32::CreateRotationYMatrix(m_timer);
        m_modelMatrix2 = Maze::Mat4F32::CreateTranslationMatrix(Maze::Math::Sin(m_timer), 0.0f, 0.0f) * Maze::Mat4F32::CreateRotationYMatrix(Maze::Math::c_pi);
        m_modelMatrix3 = Maze::Mat4F32::CreateTranslationMatrix(0.0f, 1.5f, 0.0f);

        Maze::Vec3F32 cameraForwardDirection = m_cameraTransform3D->getLocalRotation() * Maze::Vec3F32::c_unitZ;
        Maze::Vec3F32 cameraRightDirection = m_cameraTransform3D->getLocalRotation() * Maze::Vec3F32::c_unitX;

        Maze::F32 speed = 2.0f;
        
        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::W))
        {
            m_cameraTransform3D->translate(cameraForwardDirection *_dt * speed);
        }
        else
        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::S))
        {
            m_cameraTransform3D->translate(-cameraForwardDirection * _dt * speed);
        }

        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::A))
        {
            m_cameraTransform3D->translate(-cameraRightDirection * _dt * speed);
        }
        else
        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::D))
        {
            m_cameraTransform3D->translate(cameraRightDirection * _dt * speed);
        }
        else
        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::Q))
        {
            m_pitchAngle -= _dt * Maze::Math::c_pi;
        }
        else
        if (Maze::InputManager::GetInstancePtr()->getKeyState(Maze::KeyCode::E))
        {
            m_pitchAngle += _dt * Maze::Math::c_pi;
        }

        m_cameraTransform3D->setLocalRotation(Maze::Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        
        Maze::EntityManager* entityManager = Maze::EntityManager::GetInstancePtr();
        Maze::EcsWorldPtr const& world = entityManager->getDefaultWorld();
        
        world->requestInclusiveSample<Maze::MeshRenderer, Maze::Transform3D>()->process(
            [&](Maze::Entity* _entity, Maze::MeshRenderer* _meshRenderer, Maze::Transform3D* _transform)
            {
                if (m_meshRenderer.get() == _meshRenderer)
                    _transform->setLocalTransform(m_modelMatrix);
            });

    }
}

//////////////////////////////////////////
void SceneExample::notifyMouse(Maze::InputEventMouseData const& _data)
{
    switch (_data.type)
    {
        case Maze::InputEventMouseType::Move:
        {
            Maze::Vec2F32 cursorPosition = Maze::Vec2F32((Maze::F32)_data.x, (Maze::F32)_data.y);

            if (m_cursorDrag)
            {
                Maze::Vec2F32 deltaPosition = cursorPosition - m_cursorPositionLastFrame;

                m_yawAngle += deltaPosition.x * 0.0075f;
                m_pitchAngle -= deltaPosition.y * 0.0075f;
            }

            m_cursorPositionLastFrame = cursorPosition;
            break;
        }
        case Maze::InputEventMouseType::ButtonDown:
        {
            m_cursorDrag = true;
            break;
        }
        case Maze::InputEventMouseType::ButtonUp:
        {
            m_cursorDrag = false;
            break;
        }
        default:
        {
            break;
        }
    }
}

//////////////////////////////////////////
void SceneExample::testRender()
{
    Maze::RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        
    Maze::RenderQueuePtr const& renderQueue = renderSystem->getCurrentRenderTarget()->getRenderQueue();
    Maze::RenderQueueOpenGL* renderQueueOpenGL = renderQueue->castRaw<Maze::RenderQueueOpenGL>();

}

