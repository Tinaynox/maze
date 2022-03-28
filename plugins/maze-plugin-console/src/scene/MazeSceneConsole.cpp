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
#include "MazeConsoleHeader.hpp"
#include "maze-plugin-console/scene/MazeSceneConsole.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static F32 const c_consoleHeightKoef = 0.5f;


    //////////////////////////////////////////
    // Class SceneConsole
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneConsole, ECSRenderScene);

    //////////////////////////////////////////
    SceneConsole::SceneConsole()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneConsole::~SceneConsole()
    {
        m_canvas.reset();

        ConsoleService::GetInstancePtr()->eventLogChanged.unsubscribe(this);

        if (m_renderTarget)
            m_renderTarget->eventRenderTargetResized.unsubscribe(this);

        if (m_backgroundElement)
            m_backgroundElement->eventClick.unsubscribe(this);

        if (m_edit)
            m_edit->eventTextInput.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneConsolePtr SceneConsole::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneConsolePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneConsole, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneConsole::init(RenderTargetPtr const& _renderTarget)
    {
        if (!ECSRenderScene::init(_renderTarget))
            return false;

        m_renderTarget->eventRenderTargetResized.subscribe(this, &SceneConsole::notifyRenderTargetResized);

        create2D();

        ConsoleService::GetInstancePtr()->eventLogChanged.subscribe(this, &SceneConsole::notifyLogChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneConsole::update(F32 _dt)
    {
        if (m_firstFrame)
        {
            m_firstFrame = false;
            m_edit->setSelected(true);
        }
    }

    //////////////////////////////////////////
    void SceneConsole::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearDepthFlag(false);
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvas->setSortOrder(1400000);
        m_canvas->setViewport(0.0f, 0.0f, 1.0f, 1.0f);

        m_background = SpriteHelper::CreateSprite(
            ColorU32(0, 0, 0, 100),
            Vec2DF(m_canvas->getTransform()->getWidth(), m_canvas->getTransform()->getHeight() * c_consoleHeightKoef),
            Vec2DF(0.0f, 0.0f),
            nullptr,
            m_canvas->getTransform(),
            this,
            Vec2DF(0.5f, 1.0f),
            Vec2DF(0.5f, 1.0f));
        m_backgroundElement = m_background->getEntityRaw()->ensureComponent<UIElement2D>();
        m_backgroundElement->setCaptureCursorHits(true);
        m_backgroundElement->eventClick.subscribe(this, &SceneConsole::notifyBackgroundClick);

        m_consoleText = SpriteHelper::CreateSystemText(
            "",
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Bottom,
            m_background->getTransform()->getSize() - Vec2DF(2.0f, 2.0f),
            Vec2DF(1.0f, 19.0f),
            m_background->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF(0.0f, 0.0f));
        m_consoleText->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        m_consoleText->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());
        updateLogText();

        m_edit = UIHelper::CreateDefaultEditBox(
            "",
            Vec2DF(m_canvas->getTransform()->getWidth(), 18.0f),
            Vec2DF(0.0f, 0.0f),
            m_background->getTransform(),
            this,
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_edit->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        m_edit->getEntityRaw()->ensureComponent<CanvasGroup>()->setLocalAlpha(0.66f);

        m_edit->eventTextInput.subscribe(this, &SceneConsole::notifyTextInput);
    }

    //////////////////////////////////////////
    void SceneConsole::updateSize()
    {
        m_background->getTransform()->setSize(
            Vec2DF(
                m_canvas->getTransform()->getWidth(),
                m_canvas->getTransform()->getHeight() * c_consoleHeightKoef));
    }

    //////////////////////////////////////////
    void SceneConsole::notifyLogChanged(String const& _log)
    {
        updateLogText();
    }

    //////////////////////////////////////////
    void SceneConsole::notifyTextInput(SystemTextEditBox2D* _edit)
    {
        String command = m_edit->getText();
        m_edit->setText("");

        if (command.empty())
            return;


        Debug::Log("%s", command.c_str());
    }

    //////////////////////////////////////////
    void SceneConsole::notifyBackgroundClick(CursorInputEvent const& _event)
    {
        m_edit->setSelected(true);
    }

    //////////////////////////////////////////
    void SceneConsole::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        updateSize();
    }

    //////////////////////////////////////////
    void SceneConsole::updateLogText()
    {
        String const& log = ConsoleService::GetInstancePtr()->getLog();
        if (!log.empty() && log.back() == '\n')
            m_consoleText->setText(String(log.begin(), log.end() - 1));
        else
            m_consoleText->setText(log);
    }

} // namespace Maze
//////////////////////////////////////////
