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
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
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
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AssetsController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetsController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AssetsController);

    //////////////////////////////////////////
    AssetsController::AssetsController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    AssetsController::~AssetsController()
    {
        if (SelectionManager::GetInstancePtr())
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.unsubscribe(this);
            AssetManager::GetInstancePtr()->eventAssetFileRemoved.unsubscribe(this);
            AssetManager::GetInstancePtr()->eventAssetFileMoved.unsubscribe(this);
        }

        clearAssetsTree();
        clearSelectedAssetsFolder();
    }

    //////////////////////////////////////////
    AssetsControllerPtr AssetsController::Create(Canvas* _canvas)
    {
        AssetsControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetsController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool AssetsController::init(Canvas* _canvas)
    {
        m_canvas = _canvas;
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(this, &AssetsController::notifySelectionManagerSelectionChanged);

        AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(this, &AssetsController::notifyAssetFileAdded);
        AssetManager::GetInstancePtr()->eventAssetFileRemoved.subscribe(this, &AssetsController::notifyAssetFileRemoved);
        AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(this, &AssetsController::notifyAssetFileMoved);

        return true;
    }

    //////////////////////////////////////////
    void AssetsController::update(F32 _dt)
    {
        if (!getEntityRaw() || !getEntityRaw()->getEcsScene())
            return;

        MAZE_PROFILE_EVENT("AssetsController::update");

        if (m_assetsTreeDirty)
        {
            updateAssetsTree();
            m_assetsTreeDirty = false;
        }

        if (m_selectedAssetsFolder)
        {
            updateSelectedAssetsFolder();
            m_selectedAssetsFolder = false;
        }
    }

    //////////////////////////////////////////
    void AssetsController::processEntityAwakened()
    {
        AssetsControllerWPtr controllerWeak = cast<AssetsController>();

        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        Transform2DPtr canvasTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2F(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        canvasTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        SpriteRenderer2DPtr titleBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                m_canvas->getTransform()->getSize().x,
                EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(0.0f, 0.0f),
            materialManager->getSpriteMaterial(),
            canvasTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        titleBackground->setColor(EditorToolsStyles::GetInstancePtr()->getTitleBackgroundColor());
        titleBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            "Assets",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getTitleFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(100, EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(EditorToolsStyles::GetInstancePtr()->getTitleLabelShift(), 0),
            titleBackground->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        titleText->setColor(ColorU32::c_black);

        m_bodyBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                canvasTransform->getSize().x, 
                canvasTransform->getSize().y - EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(0.0f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_bodyBackground->setColor(EditorToolsStyles::GetInstancePtr()->getBodyBackgroundColor());
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setSizeDelta(0.0f, -EditorToolsStyles::GetInstancePtr()->getTitleHeight());
        
        m_layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Top,
            m_bodyBackground->getTransform()->getSize(),
            Vec2F::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_layout->setExpand(true);
        m_layout->setControlChildWidth(true);
        m_layout->setAutoWidth(false);
        m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();

        m_assetsTreeRoot = UIHelper::CreateDefaultScrollRect(
            Vec2F(m_bodyBackground->getTransform()->getWidth() / 2.0f, m_bodyBackground->getTransform()->getHeight()),
            Vec2F::c_zero,
            m_layout->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f),
            true,
            true);
        
        m_assetsTreeRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setColor(198, 198, 198);
        m_assetsTreeRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));

        m_assetsTreeLayoutTransform = m_assetsTreeRoot->getContentTransform();
        m_assetsTreeLayoutTransform->setSize(Vec2F::c_zero);
        VerticalLayout2DPtr assetsTreeLayoutTransformLayout = m_assetsTreeLayoutTransform->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        assetsTreeLayoutTransformLayout->setPadding(0.0f, 0.0f, 4.0f, 4.0f);
        // assetsTreeLayoutTransformLayout->setSpacing(5.0f);
        SizePolicy2DPtr assetsTreeLayoutSizePolicy = m_assetsTreeLayoutTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        assetsTreeLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        m_selectedAssetsFolderRoot = UIHelper::CreateDefaultScrollRect(
            Vec2F(m_bodyBackground->getTransform()->getWidth() / 2.0f, m_bodyBackground->getTransform()->getHeight()),
            Vec2F(m_bodyBackground->getTransform()->getWidth() / 2.0f, 0.0f),
            m_layout->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f),
            true,
            true);
        
        m_selectedAssetsFolderRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setColor(198, 198, 198);
        m_selectedAssetsFolderRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));

        ContextMenu2DPtr selectedAssetsFolderRootContextMenu = m_selectedAssetsFolderRoot->getEntityRaw()->ensureComponent<ContextMenu2D>();
        selectedAssetsFolderRootContextMenu->setCallbackFunction(
            [controllerWeak](MenuListTree2DPtr const& _menuListTree)
            {
                AssetsControllerPtr controller = controllerWeak.lock();
                if (controller && !controller->m_selectedAssetFolder.empty())
                {
                    AssetEditorToolsManager::GetInstancePtr()->callAssetFileContextMenuCallback(
                        controller.get(),
                        controller->m_selectedAssetFolder,
                        _menuListTree);
                }
            });

        m_selectedAssetsFolderLayoutTransform = m_selectedAssetsFolderRoot->getContentTransform();
        m_selectedAssetsFolderLayoutTransform->setSize(Vec2F::c_zero);
        VerticalLayout2DPtr assetsFolderLayoutTransformLayout = m_selectedAssetsFolderLayoutTransform->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        assetsFolderLayoutTransformLayout->setPadding(0.0f, 0.0f, 4.0f, 4.0f);
        // assetsFolderLayoutTransformLayout->setSpacing(5.0f);
        SizePolicy2DPtr assetsFolderLayoutSizePolicy = m_selectedAssetsFolderLayoutTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        assetsFolderLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        setAssetsFullPath(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory());
    }

    //////////////////////////////////////////
    void AssetsController::setAssetsFullPath(Path const& _assetsFullPath)
    {
        if (m_assetsFullPath == _assetsFullPath)
            return;

        m_assetsFullPath = _assetsFullPath;


        updateAssetsTree();

        Set<AssetFilePtr> const& assetFiles = SelectionManager::GetInstancePtr()->getSelectedAssetFiles();
        if (!assetFiles.empty())
        {
            Path selectedDirectory;
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                Path directory = FileHelper::GetDirectoryInPath(assetFile->getFullPath());

                if (selectedDirectory.empty())
                    selectedDirectory = directory;
                else
                if (selectedDirectory != directory)
                {
                    selectedDirectory.clear();
                    break;
                }
            }

            if (!selectedDirectory.empty())
                setSelectedAssetFolder(selectedDirectory);
        }

        notifySelectionManagerSelectionChanged();
    }

    //////////////////////////////////////////
    void AssetsController::clearAssetsTree()
    {
        for (auto assetTreeLine : m_assetsTreeLines)
        {
            assetTreeLine.second->eventExpandedChanged.unsubscribe(this);
            assetTreeLine.second->eventLineClick.unsubscribe(this);
            assetTreeLine.second->eventLineDoubleClick.unsubscribe(this);
        }

        m_assetsTreeLines.clear();
        m_assetsTreeLayoutTransform->destroyAllChildren();
    }

    //////////////////////////////////////////
    void AssetsController::updateAssetsTree()
    {
        AssetsControllerWPtr controllerWeak = cast<AssetsController>();

        clearAssetsTree();

        Path assetsFullPath = m_assetsFullPath;
        
        const AssetFilePtr& assetDirectory = AssetManager::GetInstancePtr()->getAssetFileByFullPath(assetsFullPath);
        if (!assetDirectory)
            return;

        Vector<AssetFilePtr> assetDirectories = assetDirectory->getChildrenAssets<AssetDirectory>();
        assetDirectories.push_back(assetDirectory);

        std::sort(
            assetDirectories.begin(),
            assetDirectories.end(),
            [](AssetFilePtr const& _a, AssetFilePtr const& _b) -> bool
            {
                return _a->getFullPath() < _b->getFullPath();
            });

        for (AssetFilePtr const& assetFile : assetDirectories)
        {
            Path fileName = assetFile->getFileName();
            Path fullPath = assetFile->getFullPath();

            if (!StringHelper::IsStartsWith(fullPath.c_str(), assetsFullPath.c_str()))
                continue;

            Transform2DPtr lineParent = m_assetsTreeLayoutTransform;

            EntityPtr assetLineObject = getEntityRaw()->getEcsScene()->createEntity();
            AssetLinePtr line = assetLineObject->ensureComponent<AssetLine>(this, assetFile);
            line->setDragAndDropEnabled(false);

            Size fullPathLastSlashPosition = fullPath.getPath().find_last_of('/');
            if (fullPathLastSlashPosition != String::npos)
            {
                Path parentFullPath = fullPath.getPath().substr(0, fullPathLastSlashPosition);
                if (!parentFullPath.empty()/* && parentFullPath != assetsFullPath*/)
                {
                    auto assetsTreeLinesIt = m_assetsTreeLines.find(parentFullPath);
                    if (assetsTreeLinesIt != m_assetsTreeLines.end())
                    {
                        AssetLinePtr const& parentLine = assetsTreeLinesIt->second;
                        lineParent = parentLine->getChildrenTransform();

                        parentLine->setDropDownVisible(true);
                    }
                }

            }

            line->getTransform()->setParent(lineParent);
            line->setDropDownVisible(false);
            line->getIconRenderer()->setColor(ColorU32(192, 192, 192));

            m_assetsTreeLines.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(assetFile->getFullPath()),
                std::forward_as_tuple(line));
            line->eventExpandedChanged.subscribe(this, &AssetsController::notifyAssetTreeLineExpandedChanged);            
            line->eventLineClick.subscribe(this, &AssetsController::notifyAssetTreeLineClick);
            line->eventLineDoubleClick.subscribe(this, &AssetsController::notifyAssetTreeLineDoubleClick);

            bool selectedFolder = StringHelper::IsStartsWith(
                m_selectedAssetFolder.getPath(),
                assetFile->getFullPath().getPath());

            line->setSelected(m_selectedAssetFolder == assetFile->getFullPath());
            line->setExpanded(selectedFolder);
            notifyAssetTreeLineExpandedChanged(line.get(), getAssetFileExpanded(assetFile));

            ContextMenu2DPtr const& lineContextMenu = line->getContextMenu();
            lineContextMenu->setCallbackFunction(
                [controllerWeak, fullPath](MenuListTree2DPtr const& _menuListTree)
                {
                    AssetsControllerPtr controller = controllerWeak.lock();
                    if (controller)
                    {
                        AssetEditorToolsManager::GetInstancePtr()->callAssetFileContextMenuCallback(
                            controller.get(),
                            fullPath,
                            _menuListTree);
                    }
                });
        }
        
    }

    //////////////////////////////////////////
    void AssetsController::setSelectedAssetFolder(Path const& _value)
    {
        if (m_selectedAssetFolder == _value)
            return;

        if (!m_selectedAssetFolder.empty())
        {
            auto it = m_assetsTreeLines.find(m_selectedAssetFolder);
            if (it != m_assetsTreeLines.end())
            {
                it->second->setSelected(false);
            }
        }

        m_selectedAssetFolder = _value;

        if (!m_selectedAssetFolder.empty())
        {
            auto it = m_assetsTreeLines.find(_value);
            if (it != m_assetsTreeLines.end())
            {
                it->second->setSelected(true);
            }
        }


        updateSelectedAssetsFolder();
    }

    //////////////////////////////////////////
    void AssetsController::notifyAssetTreeLineExpandedChanged(AssetLine* _line, bool _value)
    {
        _line->setIcon(
            _value ? UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::FolderOpened)
                   : UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::FolderClosed));
    }

    //////////////////////////////////////////
    void AssetsController::notifyAssetTreeLineClick(AssetLine* _line)
    {
        String fullPath = _line->getAssetFile()->getFullPath();
        
        if (!fullPath.empty())
        {
            if (m_selectedAssetFolder == fullPath)
            {
                // Do nothing
                // setSelectedAssetFolder(String());
            }
            else
                setSelectedAssetFolder(fullPath);
        }
    }

    //////////////////////////////////////////
    void AssetsController::notifyAssetTreeLineDoubleClick(AssetLine* _line)
    {
        Path fullPath = _line->getAssetFile()->getFullPath();

        const AssetFilePtr& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFullPath(fullPath);
        if (assetFile)
        {
            auto it = m_assetsTreeLines.find(fullPath);
            if (it != m_assetsTreeLines.end())
            {
                if (it->second->getDropDownVisible())
                {
                    if (assetFile->getMetaClass()->isInheritedFrom<AssetDirectory>())
                        setAssetFileExpanded(assetFile, !getAssetFileExpanded(assetFile));
                }
                else
                if (assetFile->getMetaClass()->isInheritedFrom<AssetDirectory>())
                {
                    SelectionManager::GetInstancePtr()->unselectObject(assetFile);
                    setSelectedAssetFolder(fullPath);
                    return;
                }
            }
        }

        eventAssetDoubleClick(fullPath);
    }

    //////////////////////////////////////////
    void AssetsController::clearSelectedAssetsFolder()
    {
        for (auto assetTreeLine : m_selectedAssetsFolderLines)
        {
            assetTreeLine.second->eventExpandedChanged.unsubscribe(this);
            assetTreeLine.second->eventLineClick.unsubscribe(this);
            assetTreeLine.second->eventLineDoubleClick.unsubscribe(this);
        }

        m_selectedAssetsFolderLines.clear();
        m_selectedAssetsFolderLayoutTransform->destroyAllChildren();
    }

    //////////////////////////////////////////
    void AssetsController::updateSelectedAssetsFolder()
    {
        clearSelectedAssetsFolder();

        if (m_selectedAssetFolder.empty())
            return;

        AssetsControllerWPtr controllerWeak = cast<AssetsController>();

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesInFolder(m_selectedAssetFolder, false);

        std::sort(
            assetFiles.begin(),
            assetFiles.end(),
            [](AssetFilePtr const& _a, AssetFilePtr const& _b) -> bool
            {
                return _a->getFullPath() < _b->getFullPath();
            });

        for (AssetFilePtr const& assetFile : assetFiles)
        {
            Path fileName = assetFile->getFileName();
            Path fullPath = assetFile->getFullPath();

            if (assetFile->getExtension() == "mzmeta")
                continue;

            // TODO: Ignored extensions
            if (assetFile->getExtension() == "mztangents")
                continue;

            Transform2DPtr lineParent = m_selectedAssetsFolderLayoutTransform;

            EntityPtr assetLineObject = getEntityRaw()->getEcsScene()->createEntity();
            AssetLinePtr line = assetLineObject->ensureComponent<AssetLine>(this, assetFile);
            line->setDragAndDropEnabled(true);

            line->getTransform()->setParent(lineParent);
            line->setDropDownVisible(false);
            AssetFileEditorIconData iconData = AssetEditorToolsManager::GetInstancePtr()->getIconForAssetFile(assetFile);
            line->setIcon(iconData.sprite, iconData.color);
            line->setSelectAssetFileByClick(true);

            line->eventLineDoubleClick.subscribe(this, &AssetsController::notifyAssetTreeLineDoubleClick);

            m_selectedAssetsFolderLines.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(fullPath),
                std::forward_as_tuple(line));

            ContextMenu2DPtr const& lineContextMenu = line->getContextMenu();
            lineContextMenu->setCallbackFunction(
                [controllerWeak, fullPath](MenuListTree2DPtr const& _menuListTree)
                {
                    AssetsControllerPtr controller = controllerWeak.lock();
                    if (controller && !controller->m_selectedAssetFolder.empty())
                    {
                        AssetEditorToolsManager::GetInstancePtr()->callAssetFileContextMenuCallback(
                            controller.get(),
                            fullPath,
                            _menuListTree);
                    }
                });
        }

        updateSelectedAssetsFolderSelection();
    }

    //////////////////////////////////////////
    void AssetsController::updateSelectedAssetsFolderSelection()
    {
        Set<AssetFilePtr> const& assetFiles = SelectionManager::GetInstancePtr()->getSelectedAssetFiles();

        for (auto const& selectedAssetFolderLineData : m_selectedAssetsFolderLines)
        {
            selectedAssetFolderLineData.second->setSelected(
                assetFiles.find(selectedAssetFolderLineData.second->getAssetFile()) != assetFiles.end());
        }
    }

    //////////////////////////////////////////
    void AssetsController::notifySelectionManagerSelectionChanged()
    {
        if (SelectionManager::GetInstancePtr()->getSelectionType() == SelectionType::Objects)
        {
            updateSelectedAssetsFolderSelection();

            Set<AssetFilePtr> const& assetFiles = SelectionManager::GetInstancePtr()->getSelectedAssetFiles();

            for (auto const& assetTreeLineData : m_assetsTreeLines)
            {
                auto const& assetTreeLineDataPath = assetTreeLineData.second->getAssetFile()->getFullPath().getPath();

                AssetFilePtr selectedAssetFile;
                for (AssetFilePtr const& assetFile : assetFiles)
                {
                    auto const& assetFilePath = assetFile->getFullPath().getPath();
                    if (StringHelper::IsStartsWith(
                        assetFilePath,
                        assetTreeLineDataPath))
                    {
                        selectedAssetFile = assetFile;
                        break;
                    }
                }

                if (selectedAssetFile)
                    setSelectedAssetFolder(FileHelper::GetParentDirectoryInPath(selectedAssetFile->getFullPath()));
            }
        }
    }
    
    //////////////////////////////////////////
    void AssetsController::notifyAssetFileAdded(AssetFilePtr const& _file, HashedString const& _extension)
    {
        m_assetsTreeDirty = true;
        m_selectedAssetsFolder = true;
    }

    //////////////////////////////////////////
    void AssetsController::notifyAssetFileRemoved(AssetFilePtr const& _file)
    {
        m_assetsTreeDirty = true;
        m_selectedAssetsFolder = true;

        if (_file->getFullPath() == m_selectedAssetFolder)
            m_selectedAssetFolder.clear();

        m_assetFilesInfo.erase(_file);
    }

    //////////////////////////////////////////
    void AssetsController::notifyAssetFileMoved(AssetFilePtr const& _file, Path const& _prevFullPath)
    {
        m_assetsTreeDirty = true;
        m_selectedAssetsFolder = true;

        if (m_selectedAssetFolder == _prevFullPath)
            setSelectedAssetFolder(_file->getFullPath());
    }

    //////////////////////////////////////////
    void AssetsController::setAssetFileExpanded(AssetFilePtr const& _assetFile, bool _expanded)
    {
        m_assetFilesInfo[_assetFile].expanded = _expanded;

        for (auto assetLineData : m_assetsTreeLines)
            if (assetLineData.second->getAssetFile() == _assetFile)
            {
                assetLineData.second->setExpanded(_expanded);
                break;
            }

        for (auto assetLineData : m_selectedAssetsFolderLines)
            if (assetLineData.second->getAssetFile() == _assetFile)
            {
                assetLineData.second->setExpanded(_expanded);
                break;
            }
    }

    //////////////////////////////////////////
    bool AssetsController::getAssetFileExpanded(AssetFilePtr const& _assetFile)
    {
        auto it = m_assetFilesInfo.find(_assetFile);
        if (it != m_assetFilesInfo.end())
            return it->second.expanded;

        return false;
    }

    //////////////////////////////////////////
    void AssetsController::setAssetFileRename(AssetFilePtr const& _assetFile, bool _rename)
    {
        m_assetFilesInfo[_assetFile].rename = _rename;

        for (auto assetLineData : m_assetsTreeLines)
            if (assetLineData.second->getAssetFile() == _assetFile)
            {
                assetLineData.second->setEditMode(_rename);
                break;
            }

        for (auto assetLineData : m_selectedAssetsFolderLines)
            if (assetLineData.second->getAssetFile() == _assetFile)
            {
                assetLineData.second->setEditMode(_rename);
                break;
            }
    }

    //////////////////////////////////////////
    bool AssetsController::getAssetFileRename(AssetFilePtr const& _assetFile)
    {
        auto it = m_assetFilesInfo.find(_assetFile);
        if (it != m_assetFilesInfo.end())
            return it->second.rename;

        return false;
    }
    
} // namespace Maze
//////////////////////////////////////////
