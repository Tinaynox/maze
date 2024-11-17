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
#include "SceneDragAndDrop.hpp"
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
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
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
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"
#include "main/components/InventorySlot.hpp"
#include "main/components/InventoryItem.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDragAndDrop
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDragAndDrop, EcsRenderScene);

    //////////////////////////////////////////
    SceneDragAndDrop::SceneDragAndDrop()
    {
    }

    //////////////////////////////////////////
    SceneDragAndDrop::~SceneDragAndDrop()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneDragAndDropPtr SceneDragAndDrop::Create()
    {
        SceneDragAndDropPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDragAndDrop, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneDragAndDrop::init()
    {
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create3D();
        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport)
    {
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

    }

    //////////////////////////////////////////
    void SceneDragAndDrop::create3D()
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
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneDragAndDrop::notifyMainRenderWindowViewportChanged);

    }

    //////////////////////////////////////////
    void SceneDragAndDrop::create2D()
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
        m_canvas->setClearColor(ColorU32::c_blackSoft);
        m_canvas->setClearColorFlag(true);

        SpriteRenderer2DPtr square = SpriteHelper::CreateSprite(
            ColorU32::c_gray,
            Vec2F32(800.0f, 600.0f),
            Vec2F32::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);


        InventorySlotPtr leftSlot = createSlot(Vec2F(-32.0f, 0.0f));
        InventorySlotPtr rightSlot = createSlot(Vec2F(32.0f, 0.0f));
        InventorySlotPtr rightSlot2 = createSlot(Vec2F(96.0f, 0.0f));

        Debug::Log("leftSlot=%d", (S32)leftSlot->getEntityId());
        Debug::Log("rightSlot=%d", (S32)rightSlot->getEntityId());

        m_item = createItem();

        rightSlot->setItem(m_item);
    }

    //////////////////////////////////////////
    InventorySlotPtr SceneDragAndDrop::createSlot(Vec2F const& _pos)
    {
        ClickButton2DPtr button = UIHelper::CreateClickButton(
            SpriteManager::GetCurrentInstance()->getOrLoadSprite(MAZE_HCS("cell00.png")),
            Vec2F32(64.0f, 64.0f),
            _pos,
            m_canvas->getTransform(),
            this);
        button->setNormalColor(ColorU32{ 245, 245, 245 });
        button->setFocusedColor(ColorU32{ 163, 181, 248 });
        button->setPressedColor(ColorU32{ 255, 160, 0 });
        

        SpriteRenderer2DPtr glow = SpriteHelper::CreateSprite(
            MAZE_HCS("cell00_glow.png"),
            Vec2F(64.0f, 64.0f) * (255.0f / 166.0f),
            Vec2F::c_zero,
            button->getTransform(),
            this);
        glow->setColor(ColorU32(30, 30, 30, 255));
        glow->setMaterial(
            MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::SpriteAdditivePA));
        glow->getMeshRenderer()->setEnabled(false);

        InventorySlotPtr inventorySlot = button->getEntityRaw()->ensureComponent<InventorySlot>();
        inventorySlot->setGlow(glow);
        return inventorySlot;
    }

    //////////////////////////////////////////
    InventoryItemPtr SceneDragAndDrop::createItem()
    {
        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            MAZE_HCS("item_gun00.png"),
            Vec2F(64.0f, 64.0f) * 0.8f,
            Vec2F::c_zero,
            nullptr,
            this);

        InventoryItemPtr inventoryItem = sprite->getEntityRaw()->ensureComponent<InventoryItem>();
        return inventoryItem;
    }

} // namespace Maze
//////////////////////////////////////////
