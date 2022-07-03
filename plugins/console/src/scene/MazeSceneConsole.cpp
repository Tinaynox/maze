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
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
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
        if (!ECSRenderScene::init(_renderTarget))
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

        m_consoleText = SystemUIHelper::CreateSystemText(
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

        m_edit = SystemUIHelper::CreateDefaultEditBox(
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
        m_edit->eventTextChanged.subscribe(this, &SceneConsole::notifyTextChanged);

        m_hintText = SystemUIHelper::CreateSystemText(
            "",
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            m_background->getTransform()->getSize(),
            Vec2DF(1.0f, -1.0f),
            m_background->getTransform(),
            this,
            Vec2DF(0.0f, 0.0f),
            Vec2DF(0.0f, 1.0f));
        m_hintText->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());
        updateHintText();
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
    void SceneConsole::notifyTextInput(EditBox2D* _edit)
    {
        String command = m_edit->getText();
        m_edit->setText("");

        if (command.empty())
            return;

        Vector<String> words;
        StringHelper::SplitWords(command, words, ' ');
        String const commandName = words[0];

        if (!ConsoleService::GetInstancePtr()->hasCommand(commandName.c_str()))
        {
            MAZE_WARNING("Undefined command: %s", commandName.c_str());
            return;
        }
        
        if (words.size() > 1)
            ConsoleService::GetInstancePtr()->executeCommand(commandName.c_str(), &words[1], (S32)words.size() - 1);
        else
            ConsoleService::GetInstancePtr()->executeCommand(commandName.c_str(), nullptr, 0);

        m_lastCommandIndex = -1;
    }

    //////////////////////////////////////////
    void SceneConsole::notifyTextChanged(EditBox2D* _edit, String const& _text)
    {
        updateHintText();
    }

    //////////////////////////////////////////
    void SceneConsole::notifyBackgroundClick(Vec2DF const& _positionOS, CursorInputEvent const& _event)
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
            Vector<ConsoleCommand> commands = ConsoleService::GetInstancePtr()->getCommandsStartedWith(command);

            String hintText;
            for (ConsoleCommand const& cmd : commands)
            {
                if (!hintText.empty())
                    hintText += '\n';
                hintText += cmd.command;

                for (S32 i = 0; i < cmd.argsCount; ++i)
                    hintText += " x";
            }

            m_hintText->setText(hintText);
        }
    }

    //////////////////////////////////////////
    void SceneConsole::completeCommand()
    {
        String const& command = m_edit->getText();
        if (command.empty())
            return;

        Vector<ConsoleCommand> commands = ConsoleService::GetInstancePtr()->getCommandsStartedWith(command);
        if (commands.empty())
            return;

        if (commands.size() == 1)
            m_edit->setText(commands.front().command);
        else
        {
            S32 commandsCount = (S32)commands.size();

            String commandPart;
            Size index = 0;
            bool finished = false;
            do
            {
                Char ch = commands.front().command.getString()[index];

                for (S32 i = 1; i < commandsCount; ++i)
                {
                    if (commands[i].command.size() <= index || commands[i].command.getString()[index] != ch)
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
                m_edit->setText(commands.front().command);
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
                S32 lastCommandsSize = (S32)ConsoleService::GetInstancePtr()->getLastCommandsSize();

                if (lastCommandsSize > 0)
                {
                    m_lastCommandIndex = Math::Clamp(m_lastCommandIndex + 1, 0, lastCommandsSize - 1);
                    String const& lastCommand = ConsoleService::GetInstancePtr()->getLastCommand(m_lastCommandIndex);
                    if (!lastCommand.empty())
                        m_edit->setText(lastCommand);
                }
            }
            else
            if (_keyboardData.keyCode == KeyCode::Down)
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

            break;
        }
        default:
            break;
        }
    }

} // namespace Maze
//////////////////////////////////////////
