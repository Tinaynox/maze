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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/MazePreviewController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-editor-tools/preview-inspectors/MazeMaterialsPreviewInspector.hpp"
#include "maze-editor-tools/preview-inspectors/MazeTexture2DPreviewInspector.hpp"
#include "maze-editor-tools/preview-inspectors/MazeRenderMeshPreviewInspector.hpp"
#include "maze-editor-tools/preview-inspectors/MazeDirectoryPreviewInspector.hpp"
#include "maze-editor-tools/preview-inspectors/MazeFilePreviewInspector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PreviewController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PreviewController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PreviewController);

    //////////////////////////////////////////
    PreviewController::PreviewController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    PreviewController::~PreviewController()
    {
        if (SelectionManager::GetInstancePtr())
        {
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);
        }

        if (UpdateManager::GetInstancePtr())
            UpdateManager::GetInstancePtr()->removeUpdatable(this);

        if (GraphicsManager::GetInstancePtr())
        {
            if (GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw())
            {
                if (GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager())
                {
                    GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager()->eventTextureLoaderAdded.unsubscribe(this);
                }
            }
        }

        if (AssetManager::GetInstancePtr())
            AssetManager::GetInstancePtr()->eventAssetFileRemoved.unsubscribe(this);

        if (m_scene)
        {
            SceneManager::GetInstancePtr()->destroyScene(m_scene);
            m_scene.reset();
        }

        m_bodyBackgroundElement->eventCursorPressIn.unsubscribe(this);
        m_bodyBackgroundElement->eventCursorDrag.unsubscribe(this);
        m_bodyBackgroundElement->eventCursorWheel.unsubscribe(this);
    }

    //////////////////////////////////////////
    PreviewControllerPtr PreviewController::Create(Canvas* _canvas)
    {
        PreviewControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PreviewController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool PreviewController::init(Canvas* _canvas)
    {
        m_canvas = _canvas;
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32(49, 49, 49, 255));

        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(this, &PreviewController::notifySelectionChanged);
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        registerPreviewInspectorByExtension<MaterialsPreviewInspector>(MAZE_HS("mzmaterial"));
        registerPreviewInspectorByClassUID<MaterialsPreviewInspector, Material>();
        
        registerPreviewInspectorByExtension<Texture2DPreviewInspector>(MAZE_HS("mztexture"));
        registerPreviewInspectorByClassUID<Texture2DPreviewInspector, Texture2D>();

        TextureManager::GetCurrentInstancePtr()->eventTextureLoaderAdded.subscribe(
            this, &PreviewController::notifyTextureLoaderAdded);

        Vector<String> textureExtensions = TextureManager::GetCurrentInstancePtr()->getTextureLoaderExtensions();
        for (String const& textureExtension : textureExtensions)
            registerPreviewInspectorByExtension<Texture2DPreviewInspector>(MAZE_HS(textureExtension.c_str()));

        registerPreviewInspectorByExtension<RenderMeshPreviewInspector>(MAZE_HS("obj"));
        registerPreviewInspectorByClassUID<RenderMeshPreviewInspector, RenderMesh>();

        AssetManager::GetInstancePtr()->eventAssetFileRemoved.subscribe(this, &PreviewController::notifyAssetFileRemoved);

        return true;
    }

    //////////////////////////////////////////
    void PreviewController::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("PreviewController::update");

        if (m_canvas && m_renderBuffer)
        {
            Vec2U size = (Vec2U)m_bodyBackground->getTransform()->getSize();
            if (size.x != 0u && size.y != 0u)
                m_renderBuffer->setSize(size);
        }

        if (m_inspectorsDirty)
        {
            udpateInspectors();
            m_inspectorsDirty = false;
        }
    }

    //////////////////////////////////////////
    void PreviewController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();       
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        Transform2DPtr canvasTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2F(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        canvasTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();


        m_renderBuffer = RenderBuffer::Create(
            {
                m_canvas->getTransform()->getSize(),
                PixelFormat::RGBA_U8,
                PixelFormat::DEPTH_U24
            });
        m_renderBuffer->getColorTexture2D()->setMinFilter(TextureFilter::Nearest);
        m_renderBuffer->getColorTexture2D()->setMagFilter(TextureFilter::Nearest);

        m_scene = SceneManager::GetInstancePtr()->loadScene<SceneDebugPreview>(true, m_renderBuffer);

        
        SpriteRenderer2DPtr titleBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                m_canvas->getTransform()->getSize().x,
                EditorToolsLayout::c_titleHeight),
            Vec2F(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            canvasTransform,
            getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        titleBackground->setColor(EditorToolsLayout::c_titleBackgroundColor);
        titleBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        SystemTextRenderer2DPtr hierarchyText = SystemUIHelper::CreateSystemText(
            "Preview",
            EditorToolsLayout::c_titleFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(100, EditorToolsLayout::c_titleHeight),
            Vec2F(EditorToolsLayout::c_titleLabelShift, 0),
            titleBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        hierarchyText->setColor(ColorU32::c_black);

        m_bodySprite = Sprite::Create(m_renderBuffer->getColorTexture()->cast<Texture2D>());
        m_bodySprite->setName("PreviewController");
        m_bodyBackground = SpriteHelper::CreateSprite(
            // m_bodySprite,
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                canvasTransform->getSize().x, 
                canvasTransform->getSize().y - EditorToolsLayout::c_titleHeight),
            Vec2F(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_bodyBackground->setColor(EditorToolsLayout::c_bodyBackgroundColor);
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setSizeDelta(0.0f, -EditorToolsLayout::c_titleHeight);
        m_bodyBackgroundElement = m_bodyBackground->getEntityRaw()->ensureComponent<UIElement2D>();
        m_bodyBackgroundElement->setCaptureCursorHits(true);
        m_bodyBackgroundElement->eventCursorPressIn.subscribe(this, &PreviewController::notifyBodyBackgroundElementCursorPress);
        m_bodyBackgroundElement->eventCursorDrag.subscribe(this, &PreviewController::notifyBodyBackgroundElementCursorDrag);
        m_bodyBackgroundElement->eventCursorWheel.subscribe(this, &PreviewController::notifyBodyBackgroundElementCursorWheel);
        
        m_layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            m_bodyBackground->getTransform()->getSize(),
            Vec2F::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        SizePolicy2DPtr layoutSizePolicy = m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();

    }        

    //////////////////////////////////////////
    void PreviewController::processEntityAdded()
    {
        
    }

    //////////////////////////////////////////
    void PreviewController::processEntityRemoved()
    {
        if (m_scene)
        {
            SceneManager::GetInstancePtr()->destroyScene(m_scene);
            m_scene.reset();
        }
    }

    //////////////////////////////////////////
    void PreviewController::udpateInspectors()
    {
        switch (SelectionManager::GetInstancePtr()->getSelectionType())
        {
        case SelectionType::Entities:
        {
            clearInspector();


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

                        if (objectsMetaClass->isInheritedFrom<AssetDirectory>())
                        {
                            setupInspector<DirectoryPreviewInspector>(assetFiles);
                            break;
                        }

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
                            clearInspector();
                        }
                        else
                        {
                            auto it = m_editorByExtension.find(extension);
                            if (it != m_editorByExtension.end())
                            {
                                PreviewInspectorPtr inspector = it->second(this);
                                if (inspector)
                                {
                                    if (inspector->setAssetFiles(assetFiles))
                                    {
                                        m_layout->alignChildren();
                                    }
                                }
                            }
                            else
                            {
                                setupInspector<FilePreviewInspector>(assetFiles);
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (auto editorByClassUIDData : m_editorByClassUID)
                        {
                            if (objectsMetaClass->isInheritedFrom(editorByClassUIDData.first))
                            {
                                PreviewInspectorPtr inspector = editorByClassUIDData.second(this);
                                if (inspector)
                                {
                                    inspector->setObjects(objects);
                                    m_layout->alignChildren();
                                }
                                break;
                            }
                        }
                    }
                }
                else
                {
                    clearInspector();
                }
            }
            else
            {
                clearInspector();
            }

            break;
        }
        default:
        {
            clearInspector();
            break;
        }
        }
    }

    //////////////////////////////////////////
    void PreviewController::notifySelectionChanged()
    {
        m_inspectorsDirty = true;
    }
    
    //////////////////////////////////////////
    void PreviewController::clearInspector()
    {
        m_layout->getTransform()->destroyAllChildren();
        m_layout->alignChildren();
        m_previewInspector.reset();

        if (m_scene)
            m_scene->clear();
        

        setSceneVisibleSettings(false, false);
    }

    //////////////////////////////////////////
    void PreviewController::setSceneVisibleSettings(bool _camera, bool _canvas)
    {
        if (_camera || _canvas)
        {
            m_bodyBackground->setSprite(m_bodySprite);
            m_bodyBackground->setColor(ColorU32::c_white);
        }
        else
        {
            m_bodyBackground->setSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));
            m_bodyBackground->setColor(EditorToolsLayout::c_bodyBackgroundColor);
        }

        if (m_scene)
        {
            m_scene->getCamera()->getEntityRaw()->setActiveSelf(_camera);
            m_scene->getCanvas()->getEntityRaw()->setActiveSelf(_canvas);
        }
    }

    //////////////////////////////////////////
    void PreviewController::notifyBodyBackgroundElementCursorPress(
        Vec2F const& _positionOS, CursorInputEvent const& _event)
    {
        if (m_previewInspector)
            m_previewInspector->processCursorPress(_positionOS, _event);
    }

    //////////////////////////////////////////
    void PreviewController::notifyBodyBackgroundElementCursorDrag(
        Vec2F const& _positionOS, CursorInputEvent const& _event)
    {
        if (m_previewInspector)
            m_previewInspector->processCursorDrag(_positionOS, _event);
    }

    //////////////////////////////////////////
    void PreviewController::notifyBodyBackgroundElementCursorWheel(
        CursorWheelInputEvent const& _event)
    {
        if (m_previewInspector)
            m_previewInspector->processCursorWheel(_event);
    }

    //////////////////////////////////////////
    void PreviewController::notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _loader)
    {
        registerPreviewInspectorByExtension<Texture2DPreviewInspector>(_extension);
    }

    //////////////////////////////////////////
    void PreviewController::notifyAssetFileRemoved(AssetFilePtr const& _file)
    {
        m_inspectorsDirty = true;
    }

} // namespace Maze
//////////////////////////////////////////
