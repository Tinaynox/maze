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
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static F32 const c_consoleHeightKoef = 0.5f;
    static F32 const c_editBoxHeight = 18.0f;
    static F32 const c_logPadding = 1.0f;
    static S32 const c_wheelScrollLines = 3;


    //////////////////////////////////////////
    // Class SceneConsole
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneConsole, EcsRenderScene);

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
        {
            m_backgroundElement->eventClick.unsubscribe(this);
            m_backgroundElement->eventCursorWheel.unsubscribe(this);
        }

        if (m_edit)
        {
            m_edit->eventTextInput.unsubscribe(this);
            m_edit->eventTextChanged.unsubscribe(this);
        }

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
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
        if (!EcsRenderScene::init(_renderTarget))
            return false;

        m_renderTarget->eventRenderTargetResized.subscribe(this, &SceneConsole::notifyRenderTargetResized);

        create2D();

        ConsoleService::GetInstancePtr()->eventLogChanged.subscribe(this, &SceneConsole::notifyLogChanged);

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneConsole::notifyKeyboard);

        return true;
    }

    //////////////////////////////////////////
    void SceneConsole::update(F32 _dt)
    {
        if (m_firstFrame)
        {
            m_firstFrame = false;
            m_edit->setSelected(true);

            // Text renderers can't measure until their entities are awakened
            updateLogLayout();
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
            Vec2F(m_canvas->getTransform()->getWidth(), m_canvas->getTransform()->getHeight() * c_consoleHeightKoef),
            Vec2F(0.0f, 0.0f),
            nullptr,
            m_canvas->getTransform(),
            this,
            Vec2F(0.5f, 1.0f),
            Vec2F(0.5f, 1.0f));
        m_backgroundElement = m_background->getEntityRaw()->ensureComponent<UIElement2D>();
        m_backgroundElement->setCaptureCursorHits(true);
        m_backgroundElement->eventClick.subscribe(this, &SceneConsole::notifyBackgroundClick);

        m_backgroundElement->eventCursorWheel.subscribe(this, &SceneConsole::notifyBackgroundCursorWheel);

        m_logViewport = SpriteHelper::CreateTransform2D(
            calculateLogViewportSize(),
            Vec2F(c_logPadding, c_editBoxHeight + c_logPadding),
            m_background->getTransform(),
            this,
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_logViewport->getEntityRaw()->ensureComponent<ScissorMask2D>();

        m_consoleText = UIHelper::CreateTextOrSystemText(
            "",
            FontMaterialPtr(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            m_logViewport->getSize(),
            Vec2F::c_zero,
            m_logViewport,
            this,
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));

        updateLogText();

        m_edit = UIHelper::CreateDefaultEditBox(
            "",
            FontMaterialPtr(),
            12,
            Vec2F(m_canvas->getTransform()->getWidth(), c_editBoxHeight),
            Vec2F(0.0f, 0.0f),
            m_background->getTransform(),
            this,
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_edit->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        m_edit->getEntityRaw()->ensureComponent<CanvasGroup>()->setLocalAlpha(0.66f);
        m_edit->setEditBoxFlag(EditBox2D::EditBoxFlags::DontDeselectOnTextInput, true);

        m_edit->eventTextInput.subscribe(this, &SceneConsole::notifyTextInput);
        m_edit->eventTextChanged.subscribe(this, &SceneConsole::notifyTextChanged);

        m_hintText = UIHelper::CreateTextOrSystemText(
            "",
            FontMaterialPtr(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            m_background->getTransform()->getSize(),
            Vec2F(1.0f, -1.0f),
            m_background->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, 1.0f));
        m_hintText->setColor(ColorU32(255, 255, 255, 255));
        if (m_hintText->getClassUID() == ClassInfo<TextRenderer2D>::UID())
        {
            TextRenderer2D* hintText = m_hintText->castRaw<TextRenderer2D>();
            hintText->setOutlineThickness(1.0f);
            hintText->setOutlineColor(ColorU32(20, 20, 20, 255));
        }
        else
        if (m_hintText->getClassUID() == ClassInfo<SystemTextRenderer2D>::UID())
        {
            m_hintText->castRaw<SystemTextRenderer2D>()->setSystemFont(
                SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());
        }

        updateHintText();
    }

    //////////////////////////////////////////
    void SceneConsole::updateSize()
    {
        m_background->getTransform()->setSize(
            Vec2F(
                m_canvas->getTransform()->getWidth(),
                m_canvas->getTransform()->getHeight() * c_consoleHeightKoef));

        m_logViewport->setSize(calculateLogViewportSize());
        updateLogLayout();
    }

    //////////////////////////////////////////
    Vec2F SceneConsole::calculateLogViewportSize() const
    {
        Vec2F size = m_background->getTransform()->getSize() - Vec2F(c_logPadding * 2.0f, c_editBoxHeight + c_logPadding * 2.0f);
        return Vec2F(Math::Max(size.x, 0.0f), Math::Max(size.y, 0.0f));
    }

    //////////////////////////////////////////
    void SceneConsole::updateLogLayout()
    {
        Vec2F const& viewportSize = m_logViewport->getSize();

        Transform2DPtr const& textTransform = m_consoleText->getTransform();
        textTransform->setWidth(viewportSize.x);

        F32 contentHeight = m_consoleText->calculatePreferredSize().y;
        textTransform->setHeight(contentHeight);

        S32 linesCount = 1;
        for (Char ch : m_consoleText->getText())
            if (ch == '\n')
                ++linesCount;
        if (contentHeight > 0.0f)
            m_logLineHeight = contentHeight / (F32)linesCount;

        if (m_logScrollFromBottom > 0.0f && contentHeight > m_logContentHeight)
            m_logScrollFromBottom += contentHeight - m_logContentHeight;
        m_logContentHeight = contentHeight;

        F32 overflow = Math::Max(contentHeight - viewportSize.y, 0.0f);
        m_logScrollFromBottom = Math::Clamp(m_logScrollFromBottom, 0.0f, overflow);

        textTransform->setLocalPosition(0.0f, overflow - m_logScrollFromBottom);
    }

    //////////////////////////////////////////
    void SceneConsole::scrollLog(F32 _delta)
    {
        m_logScrollFromBottom += _delta;
        updateLogLayout();
    }

    //////////////////////////////////////////
    void SceneConsole::notifyBackgroundCursorWheel(CursorWheelInputEvent const& _event)
    {
        scrollLog(Math::Sign(_event.deltaWheel) * m_logLineHeight * (F32)c_wheelScrollLines);
    }

    //////////////////////////////////////////
    void SceneConsole::notifyLogChanged(String const& _log)
    {
        updateLogText();
    }

    //////////////////////////////////////////
    void SceneConsole::notifyTextInput(EditBox2D* _edit)
    {
        String command = m_edit->getText();
        m_edit->setText("");

        if (command.empty())
            return;

        m_logScrollFromBottom = 0.0f;
        ConsoleService::GetInstancePtr()->executeCommand(command);

        m_lastCommandIndex = -1;
    }

    //////////////////////////////////////////
    void SceneConsole::notifyTextChanged(EditBox2D* _edit, String const& _text)
    {
        updateHintText();
    }

    //////////////////////////////////////////
    void SceneConsole::notifyBackgroundClick(Vec2F const& _positionOS, CursorInputEvent& _event)
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

        updateLogLayout();
    }

    //////////////////////////////////////////
    void SceneConsole::updateHintText()
    {
        String const& command = m_edit->getText();

        if (command.empty())
        {
            m_hintText->setText("");
        }
        else
        {
            Vector<ConsoleCommandHint> commandHints = ConsoleService::GetInstancePtr()->getCommandHintsStartedWith(command);

            StringStream hintText;
            S32 hintsAdded = 0;
            for (ConsoleCommandHint const& cmdHint : commandHints)
            {
                if (hintsAdded > 0)
                    hintText << '\n';
                hintText << cmdHint.command;

                for (S32 i = 0; i < cmdHint.argsCount; ++i)
                    hintText << " x";

                if (!cmdHint.description.empty())
                    hintText << " // " << cmdHint.description;

                hintsAdded++;
            }

            m_hintText->setText(String(hintText.str().c_str()));
        }
    }

    //////////////////////////////////////////
    void SceneConsole::completeCommand()
    {
        String const& command = m_edit->getText();
        if (command.empty())
            return;

        Vector<ConsoleCommandHint> commandHints = ConsoleService::GetInstancePtr()->getCommandHintsStartedWith(command);
        if (commandHints.empty())
            return;

        if (commandHints.size() == 1)
            m_edit->setText(commandHints.front().command);
        else
        {
            S32 commandsCount = (S32)commandHints.size();

            String commandPart;
            Size index = 0;
            bool finished = false;
            do
            {
                Char ch = commandHints.front().command.getString()[index];

                for (S32 i = 1; i < commandsCount; ++i)
                {
                    if (commandHints[i].command.size() <= index || commandHints[i].command.getString()[index] != ch)
                    {
                        finished = true;
                        break;
                    }
                }

                if (!finished)
                    commandPart += ch;
                ++index;
            }
            while (!finished);

            if (commandPart == command)
                m_edit->setText(commandHints.front().command);
            else
                m_edit->setText(commandPart);
        }
    }

    //////////////////////////////////////////
    void SceneConsole::notifyKeyboard(InputEventKeyboardData const& _keyboardData)
    {
        switch (_keyboardData.type)
        {
        case InputEventKeyboardType::KeyDown:
        {
            if (_keyboardData.keyCode == KeyCode::Tab)
            {
                if (m_edit->getSelected())
                {
                    completeCommand();
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::Up)
            {
                if (m_edit->getSelected())
                {
                    S32 lastCommandsSize = (S32)ConsoleService::GetInstancePtr()->getLastCommandsSize();

                    if (lastCommandsSize > 0)
                    {
                        m_lastCommandIndex = Math::Clamp(m_lastCommandIndex + 1, 0, lastCommandsSize - 1);
                        String const& lastCommand = ConsoleService::GetInstancePtr()->getLastCommand(m_lastCommandIndex);
                        if (!lastCommand.empty())
                        {
                            m_edit->setText(lastCommand);
                        }
                    }
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::Down)
            {
                if (m_edit->getSelected())
                {
                    S32 lastCommandsSize = (S32)ConsoleService::GetInstancePtr()->getLastCommandsSize();

                    if (lastCommandsSize > 0)
                    {
                        if (m_lastCommandIndex <= 0)
                        {
                            m_lastCommandIndex = -1;
                            m_edit->setText("");
                        }
                        else
                        {
                            m_lastCommandIndex = Math::Clamp(m_lastCommandIndex - 1, 0, lastCommandsSize - 1);
                            String const& lastCommand = ConsoleService::GetInstancePtr()->getLastCommand(m_lastCommandIndex);
                            if (!lastCommand.empty())
                                m_edit->setText(lastCommand);
                        }
                    }
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::PageUp || _keyboardData.keyCode == KeyCode::PageDown)
            {
                if (m_edit->getSelected())
                {
                    F32 page = Math::Max(m_logViewport->getHeight() - m_logLineHeight, m_logLineHeight);
                    scrollLog(_keyboardData.keyCode == KeyCode::PageUp ? page : -page);
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::Home || _keyboardData.keyCode == KeyCode::End)
            {
                if (m_edit->getSelected() && _keyboardData.isControlDown())
                {
                    m_logScrollFromBottom = _keyboardData.keyCode == KeyCode::Home ? m_logContentHeight : 0.0f;
                    updateLogLayout();
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::C)
            {
                if (m_edit->getSelected())
                {
                    if (_keyboardData.isControlDown())
                    {
                        if (!m_edit->getText().empty())
                            SystemManager::GetInstancePtr()->setClipboardString(m_edit->getText());
                    }
                }
            }

            break;
        }
        default:
            break;
        }
    }

} // namespace Maze
//////////////////////////////////////////
