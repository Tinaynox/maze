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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/ecs/components/MazeInspectorController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-debugger/inspectors/entities/MazeEntitiesInspector.hpp"
#include "maze-debugger/inspectors/asset-materials/MazeAssetMaterialsInspector.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class InspectorController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InspectorController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(InspectorController);

    //////////////////////////////////////////
    InspectorController::InspectorController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    InspectorController::~InspectorController()
    {
        if (SelectionManager::GetInstancePtr())
        {
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);
        }

        if (UpdateManager::GetInstancePtr())
            UpdateManager::GetInstancePtr()->removeUpdatable(this);
    }

    //////////////////////////////////////////
    InspectorControllerPtr InspectorController::Create(Canvas* _canvas)
    {
        InspectorControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InspectorController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool InspectorController::init(Canvas* _canvas)
    {
        m_canvas = _canvas;

        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(this, &InspectorController::notifySelectionChanged);
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void InspectorController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void InspectorController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        Transform2DPtr canvasTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2DF(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        canvasTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        SpriteRenderer2DPtr titleBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                m_canvas->getTransform()->getSize().x,
                DebuggerLayout::c_titleHeight),
            Vec2DF(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            canvasTransform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        titleBackground->setColor(DebuggerLayout::c_titleBackgroundColor);
        titleBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        SystemTextRenderer2DPtr hierarchyText = SpriteHelper::CreateSystemText(
            "Inspector",
            DebuggerLayout::c_titleFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(100, DebuggerLayout::c_titleHeight),
            Vec2DF(DebuggerLayout::c_titleLabelShift, 0),
            titleBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.0f, 0.5f));
        hierarchyText->setColor(ColorU32::c_black);

        m_bodyBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                canvasTransform->getSize().x, 
                canvasTransform->getSize().y - DebuggerLayout::c_titleHeight),
            Vec2DF(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_bodyBackground->setColor(DebuggerLayout::c_bodyBackgroundColor);
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setSizeDelta(0.0f, -DebuggerLayout::c_titleHeight);
        

        ScrollRect2DPtr scrollRect = UIHelper::CreateDefaultScrollRect(
            m_bodyBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f),
            false,
            true);
        scrollRect->getViewportTransform()->getEntityRaw()->getComponent<ScissorMask2D>()->setPadding(0, 0, 0, 0);
        scrollRect->getEntityRaw()->ensureComponent<SizePolicy2D>();
        scrollRect->getEntityRaw()->getComponent<MeshRenderer>()->setEnabled(false);

        m_layout = scrollRect->getContentTransform()->getEntityRaw()->createComponent<VerticalLayout2D>();
        m_layout->setHorizontalAlignment(HorizontalAlignment2D::Left);
        m_layout->setVerticalAlignment(VerticalAlignment2D::Top);
        m_layout->setAutoWidth(false);
        m_layout->setAutoHeight(true);
        SizePolicy2DPtr layoutSizePolicy = m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        layoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        
        notifySelectionChanged();
    }

    //////////////////////////////////////////
    void InspectorController::notifySelectionChanged()
    {
        switch (SelectionManager::GetInstancePtr()->getSelectionType())
        {
            case SelectionType::Entities:
            {
                Set<EntityPtr> const& selectedObjects = SelectionManager::GetInstancePtr()->getSelectedEntities();

                if (!selectedObjects.empty())
                {
                    EntitiesInspectorPtr inspector = setupEditor<EntitiesInspector>();
                    inspector->setEntities(selectedObjects);

                    inspector->update(0.0f);
                    m_layout->alignChildren();
                }
                else
                {
                    clearEditor();
                }

                break;
            }
            case SelectionType::Objects:
            {
                Set<ObjectPtr> const& objects = SelectionManager::GetInstancePtr()->getSelectedObjects();

                if (!objects.empty())
                {
                    MetaClass* objectsMetaClass = (*objects.begin())->getMetaClass();
                    for (auto it = ++objects.begin(); it != objects.end(); ++it)
                    {
                        if ((*it)->getMetaClass() != objectsMetaClass)
                        {
                            objectsMetaClass = nullptr;
                            break;
                        }
                    }

                    if (objectsMetaClass != nullptr)
                    {
                        // Asset files
                        if (objectsMetaClass->isInheritedFrom<AssetFile>())
                        {
                            Set<AssetFilePtr> assetFiles;
                            for (ObjectPtr const& object : objects)
                                assetFiles.insert(std::static_pointer_cast<AssetFile>(object));

                            String extension = (*assetFiles.begin())->getExtension();
                            bool multiExtension = false;
                            for (auto it = ++assetFiles.begin(); it != assetFiles.end(); ++it)
                            {
                                if ((*it)->getExtension() != extension)
                                {
                                    multiExtension = true;
                                    break;
                                }
                            }

                            if (multiExtension)
                            {
                                clearEditor();
                            }
                            else
                            {
                                if (extension == "mzmaterial")
                                {
                                    AssetMaterialsInspectorPtr inspector = setupEditor<AssetMaterialsInspector>();
                                    inspector->setAssetFiles(assetFiles);

                                    inspector->update(0.0f);
                                    m_layout->alignChildren();
                                }
                                else
                                {
                                    // Not supported extension
                                    clearEditor();
                                }
                            }
                        }
                        else
                        // Material
                        if (objectsMetaClass->isInheritedFrom<Material>())
                        {
                            Set<MaterialPtr> materials;
                            for (ObjectPtr const& object : objects)
                                materials.insert(std::static_pointer_cast<Material>(object));

                            MaterialsInspectorPtr inspector = setupEditor<MaterialsInspector>();
                            inspector->setMaterials(materials);

                            inspector->update(0.0f);
                            m_layout->alignChildren();
                        }
                    }
                    else
                    {
                        clearEditor();
                    }
                }
                else
                {
                    clearEditor();
                }

                break;
            }
            default:
            {
                clearEditor();
                break;
            }
        }
    }

    //////////////////////////////////////////
    void InspectorController::clearEditor()
    {
        if (m_layout)
        {
            m_layout->getTransform()->destroyAllChildren();
            m_layout->alignChildren();
        }

        m_inspector.reset();
    }
    
    
} // namespace Maze
//////////////////////////////////////////
