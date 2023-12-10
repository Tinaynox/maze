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
#include "ecs/components/tools/ParticleEffectInfo.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
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
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePool.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticleEffectInfo
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ParticleEffectInfo, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ParticleEffectInfo);

    //////////////////////////////////////////
    ParticleEffectInfo::ParticleEffectInfo()
    {
    }

    //////////////////////////////////////////
    ParticleEffectInfo::~ParticleEffectInfo()
    {
        if (SelectionManager::GetInstancePtr())
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);

        if (m_stopButton)
            m_stopButton->eventClick.unsubscribe(this);

        if (m_playButton)
            m_playButton->eventClick.unsubscribe(this);

        if (m_restartButton)
            m_restartButton->eventClick.unsubscribe(this);

        if (m_pauseButton)
            m_pauseButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    ParticleEffectInfoPtr ParticleEffectInfo::Create()
    {
        ParticleEffectInfoPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticleEffectInfo, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ParticleEffectInfo::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);
        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(this, &ParticleEffectInfo::notifySelectionChanged);

        return true;
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::update(F32 _dt)
    {
        processDataToUI();
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        m_transform->setAnchor(1.0f, 0.0f);
        m_transform->setPivot(1.0f, 0.0f);
        m_transform->setSize(200.0f, 120.0f);
        m_transform->setLocalPosition(-10.0f, 10.0f);

        
        SpriteRenderer2DPtr backgroundSprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            m_transform->getSize(),
            Vec2F32::c_zero,
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getColorTextureMaterial(),
            m_transform,
            m_transform->getEntityRaw()->getECSScene(),
            { 1.0f, 0.0f },
            { 1.0f, 0.0f });
        backgroundSprite->setColor(255, 255, 255, 140);
    
        SystemTextRenderer2DPtr titleText = SystemUIHelper::CreateSystemText(
            "Particle Effect",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2F32(m_transform->getWidth(), 20.0f),
            Vec2F32::c_zero,
            backgroundSprite->getTransform(),
            m_transform->getEntityRaw()->getECSScene(),
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        titleText->setColor(ColorU32::c_black);

        m_stopButton = UIHelper::CreateDefaultClickButton(
            "Stop",
            { 62.0f, 18.0f },
            { -65.0f, -40.0f },
            backgroundSprite->getTransform(),
            m_transform->getEntityRaw()->getECSScene(),
            { 0.5f, 1.0f },
            { 0.5f, 0.0f });
        m_stopButton->eventClick.subscribe(this, &ParticleEffectInfo::notifyStopButtonClick);

        m_playButton = UIHelper::CreateDefaultClickButton(
            "Play",
            { 62.0f, 18.0f },
            { -65.0f, -40.0f },
            backgroundSprite->getTransform(),
            m_transform->getEntityRaw()->getECSScene(),
            { 0.5f, 1.0f },
            { 0.5f, 0.0f });
        m_playButton->eventClick.subscribe(this, &ParticleEffectInfo::notifyPlayButtonClick);

        m_restartButton = UIHelper::CreateDefaultClickButton(
            "Restart",
            { 62.0f, 18.0f },
            { 0.0f, -40.0f },
            backgroundSprite->getTransform(),
            m_transform->getEntityRaw()->getECSScene(),
            { 0.5f, 1.0f },
            { 0.5f, 0.0f });
        m_restartButton->eventClick.subscribe(this, &ParticleEffectInfo::notifyRestartButtonClick);

        m_pauseButton = UIHelper::CreateDefaultClickButton(
            "Pause",
            { 62.0f, 18.0f },
            { 65.0f, -40.0f },
            backgroundSprite->getTransform(),
            m_transform->getEntityRaw()->getECSScene(),
            { 0.5f, 1.0f },
            { 0.5f, 0.0f });
        m_pauseButton->eventClick.subscribe(this, &ParticleEffectInfo::notifyPauseButtonClick);

        F32 yStart = -45.0f;
        F32 textGap = -14.0f;

        {
            {
                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "Particles",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(8.0f, yStart + textGap * 0),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                text->setColor(ColorU32::c_black);
            }

            {
                m_particlesValueText = SystemUIHelper::CreateSystemText(
                    "0",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(108.0f, yStart + textGap * 0),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                m_particlesValueText->setColor(ColorU32::c_black);
            }
        }

        {
            {
                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "Time",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(8.0f, yStart + textGap * 1),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                text->setColor(ColorU32::c_black);
            }

            {
                m_timeValueText = SystemUIHelper::CreateSystemText(
                    "0",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(108.0f, yStart + textGap * 1),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                m_timeValueText->setColor(ColorU32::c_black);
            }
        }

        {
            {
                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "Iterations",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(8.0f, yStart + textGap * 2),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                text->setColor(ColorU32::c_black);
            }

            {
                m_iteractionsValueText = SystemUIHelper::CreateSystemText(
                    "0",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(108.0f, yStart + textGap * 2),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                m_iteractionsValueText->setColor(ColorU32::c_black);
            }
        }

        {
            {
                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "Recursive",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F32(20.0f, 20.0f),
                    Vec2F32(8.0f, yStart + textGap * 3.5f),
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                text->setColor(ColorU32::c_black);
            }

            {
                m_recursiveButton = UIHelper::CreateDefaultToggleButton(
                    { 108.0f, yStart + textGap * 3.5f },
                    backgroundSprite->getTransform(),
                    m_transform->getEntityRaw()->getECSScene(),
                    { 0.0f, 1.0f },
                    { 0.0f, 1.0f });
                m_recursiveButton->setChecked(true);

            }
        }

        updateActive();
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::updateActive()
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                getEntityRaw()->setActiveSelf(true);
            }
            else
            {
                getEntityRaw()->setActiveSelf(false);
            }
        }
        else
        {
            getEntityRaw()->setActiveSelf(false);
        }    
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::processDataToUI()
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                ParticleSystemState state = particleSystem->getState();
                m_playButton->getEntityRaw()->setActiveSelf(state != ParticleSystemState::Playing);
                m_stopButton->getEntityRaw()->setActiveSelf(state == ParticleSystemState::Playing);

                m_particlesValueText->setText(StringHelper::ToString(particleSystem->getAliveParticles()));
                m_timeValueText->setText(StringHelper::F32ToStringFormatted(particleSystem->getTime(), 1));
                m_iteractionsValueText->setText(StringHelper::ToString(particleSystem->getIteration()));
            }
        }
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::notifySelectionChanged()
    {
        updateActive();
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::notifyStopButtonClick(Button2D* _button, CursorInputEvent const& _cursor)
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                particleSystem->stop(m_recursiveButton->getChecked());
            }
        }
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::notifyPlayButtonClick(Button2D* _button, CursorInputEvent const& _cursor)
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                particleSystem->play(m_recursiveButton->getChecked());
            }
        }
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::notifyRestartButtonClick(Button2D* _button, CursorInputEvent const& _cursor)
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                particleSystem->restart(m_recursiveButton->getChecked());
            }
        }
    }

    //////////////////////////////////////////
    void ParticleEffectInfo::notifyPauseButtonClick(Button2D* _button, CursorInputEvent const& _cursor)
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();

        if (selectedEntities.size() == 1)
        {
            ParticleSystem3D* particleSystem = (*selectedEntities.begin())->getComponentRaw<ParticleSystem3D>();
            if (particleSystem)
            {
                particleSystem->pause(m_recursiveButton->getChecked());
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
