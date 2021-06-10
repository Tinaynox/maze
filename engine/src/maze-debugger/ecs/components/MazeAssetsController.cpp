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
#include "maze-debugger/ecs/components/MazeAssetsController.hpp"
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
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/managers/MazeAssetDebuggerManager.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
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
        SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);

        clearAssetsTree();
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

        return true;
    }

    //////////////////////////////////////////
    void AssetsController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void AssetsController::processEntityAwakened()
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
            "Assets",
            DebuggerLayout::c_titleFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(100, DebuggerLayout::c_titleHeight),
            Vec2DF(DebuggerLayout::c_titleLabelShift, 0),
            materialManager->getColorTextureMaterial(),
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
        
        m_layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Top,
            m_bodyBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_layout->setExpand(true);
        m_layout->setControlChildWidth(true);
        m_layout->setAutoWidth(false);
        m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();

        m_assetsTreeRoot = UIHelper::CreateDefaultScrollRect(
            Vec2DF(m_bodyBackground->getTransform()->getWidth() / 2.0f, m_bodyBackground->getTransform()->getHeight()),
            Vec2DF::c_zero,
            m_layout->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f),
            true,
            true);
        
        m_assetsTreeRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setColor(198, 198, 198);
        m_assetsTreeRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));

        m_assetsTreeLayoutTransform = m_assetsTreeRoot->getContentTransform();
        m_assetsTreeLayoutTransform->setSize(Vec2DF::c_zero);
        VerticalLayout2DPtr assetsTreeLayoutTransformLayout = m_assetsTreeLayoutTransform->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        assetsTreeLayoutTransformLayout->setPadding(0.0f, 0.0f, 4.0f, 4.0f);
        assetsTreeLayoutTransformLayout->setSpacing(3.0f);
        SizePolicy2DPtr assetsTreeLayoutSizePolicy = m_assetsTreeLayoutTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        assetsTreeLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        m_selectedAssetsFolderRoot = UIHelper::CreateDefaultScrollRect(
            Vec2DF(m_bodyBackground->getTransform()->getWidth() / 2.0f, m_bodyBackground->getTransform()->getHeight()),
            Vec2DF(m_bodyBackground->getTransform()->getWidth() / 2.0f, 0.0f),
            m_layout->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f),
            true,
            true);
        
        m_selectedAssetsFolderRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setColor(198, 198, 198);
        m_selectedAssetsFolderRoot->getEntityRaw()->getComponent<SpriteRenderer2D>()->setSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));

        m_selectedAssetsFolderLayoutTransform = m_selectedAssetsFolderRoot->getContentTransform();
        m_selectedAssetsFolderLayoutTransform->setSize(Vec2DF::c_zero);
        VerticalLayout2DPtr assetsFolderLayoutTransformLayout = m_selectedAssetsFolderLayoutTransform->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        assetsFolderLayoutTransformLayout->setPadding(0.0f, 0.0f, 4.0f, 4.0f);
        assetsFolderLayoutTransformLayout->setSpacing(3.0f);
        SizePolicy2DPtr assetsFolderLayoutSizePolicy = m_selectedAssetsFolderLayoutTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        assetsFolderLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        updateAssetsTree();
    }

    //////////////////////////////////////////
    void AssetsController::clearAssetsTree()
    {
        for (auto assetTreeLine : m_assetsTreeLines)
        {
            assetTreeLine.second->eventExpandedChanged.unsubscribe(this);
            assetTreeLine.second->eventLinePressed.unsubscribe(this);
        }

        m_assetsTreeLines.clear();
        m_assetsTreeLayoutTransform->destroyAllChildren();
    }

    //////////////////////////////////////////
    void AssetsController::updateAssetsTree()
    {
        Vector<AssetFilePtr> assetDirectories;

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFiles<AssetDirectory>();
        String defaultAssetsDirectory = AssetManager::GetInstancePtr()->getDefaultAssetsDirectory();
        for (AssetFilePtr assetFile : assetFiles)
        {
            String fullPath = assetFile->getFullPath();
            if (fullPath == defaultAssetsDirectory)
                continue;

            assetDirectories.emplace_back(assetFile);
        }

        std::sort(
            assetDirectories.begin(),
            assetDirectories.end(),
            [](AssetFilePtr const& _a, AssetFilePtr const& _b) -> bool
            {
                return _a->getFullPath() < _b->getFullPath();
            });

        for (AssetFilePtr const& assetFile : assetDirectories)
        {
            String fileName = assetFile->getFileName();
            String fullPath = assetFile->getFullPath();

            if (!StringHelper::IsStartsWith(fullPath, defaultAssetsDirectory))
                continue;

            Transform2DPtr lineParent = m_assetsTreeLayoutTransform;

            EntityPtr assetLineObject = getEntityRaw()->getECSScene()->createEntity();
            AssetLinePtr line = assetLineObject->ensureComponent<AssetLine>(fileName);
            line->setIcon(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::FolderClosed));

            Size fullPathLastSlashPosition = fullPath.find_last_of('/');
            if (fullPathLastSlashPosition != String::npos)
            {
                String parentFullPath = fullPath.substr(0, fullPathLastSlashPosition);
                if (!parentFullPath.empty() && parentFullPath != defaultAssetsDirectory)
                {
                    auto assetsTreeLinesIt = m_assetsTreeLines.find(parentFullPath);
                    if (assetsTreeLinesIt != m_assetsTreeLines.end())
                    {
                        AssetLinePtr const& parentLine = assetsTreeLinesIt->second;
                        lineParent = parentLine->getChildrenTransform();

                        parentLine->setDropDownVisible(true);
                        parentLine->setExpanded(true);
                    }
                    else
                    {
                        MAZE_ERROR("Parent is not found!");
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
            line->setAssetFile(assetFile);
            line->eventExpandedChanged.subscribe(this, &AssetsController::notifyAssetTreeLineExpandedChanged);            
            line->eventLinePressed.subscribe(this, &AssetsController::notifyAssetTreeLinePressed);
        }
        
    }

    //////////////////////////////////////////
    void AssetsController::setSelectedAssetFolder(String const& _value)
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
    void AssetsController::notifyAssetTreeLinePressed(AssetLine* _line)
    {
        String fullPath = _line->getAssetFile()->getFullPath();
        
        if (!fullPath.empty())
        {
            if (m_selectedAssetFolder == fullPath)
                setSelectedAssetFolder(String());
            else
                setSelectedAssetFolder(fullPath);
        }
    }

    //////////////////////////////////////////
    void AssetsController::clearSelectedAssetsFolder()
    {
        for (auto assetTreeLine : m_selectedAssetsFolderLines)
        {
            assetTreeLine.second->eventExpandedChanged.unsubscribe(this);
            assetTreeLine.second->eventLinePressed.unsubscribe(this);
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

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesInFolder(m_selectedAssetFolder);

        std::sort(
            assetFiles.begin(),
            assetFiles.end(),
            [](AssetFilePtr const& _a, AssetFilePtr const& _b) -> bool
            {
                return _a->getFullPath() < _b->getFullPath();
            });

        for (AssetFilePtr const& assetFile : assetFiles)
        {
            String fileName = assetFile->getFileName();
            String fullPath = assetFile->getFullPath();

            Transform2DPtr lineParent = m_selectedAssetsFolderLayoutTransform;

            EntityPtr assetLineObject = getEntityRaw()->getECSScene()->createEntity();
            AssetLinePtr line = assetLineObject->ensureComponent<AssetLine>(fileName);

            line->getTransform()->setParent(lineParent);
            line->setDropDownVisible(false);
            line->setIcon(
                AssetDebuggerManager::GetInstancePtr()->getIconForAssetFile(assetFile));
            line->setSelectAssetFileByPress(true);

            m_selectedAssetsFolderLines.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(fullPath),
                std::forward_as_tuple(line));
            line->setAssetFile(assetFile);
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
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
