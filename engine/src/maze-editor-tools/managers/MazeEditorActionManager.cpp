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
#include "maze-editor-tools/managers/MazeEditorActionManager.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionManager
    //
    //////////////////////////////////////////
    EditorActionManager* EditorActionManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorActionManager::EditorActionManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorActionManager::~EditorActionManager()
    {
        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
    }

    //////////////////////////////////////////
    void EditorActionManager::Initialize(EditorActionManagerPtr& _graphicsManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionManager, _graphicsManager, init());
    }

    //////////////////////////////////////////
    bool EditorActionManager::init()
    {
        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &EditorActionManager::notifyKeyboard);

        return true;
    }

    //////////////////////////////////////////
    void EditorActionManager::setCurrentHistoryIndex(S32 _index)
    {
        if (m_currentHistoryIndex == _index)
            return;

        // Security check
        MAZE_DEBUG_BP_IF(m_historyIndexSetting);
        m_historyIndexSetting = true;


        if (_index >= (S32)m_history.size())
        {
            m_historyIndexSetting = false;
            MAZE_DEBUG_BP_RETURN;
        }

        if (_index < m_currentHistoryIndex)
        {
            for (S32 i = m_currentHistoryIndex; i > _index; --i)
                m_history[i]->revert();
        }
        else
        {
            for (S32 i = m_currentHistoryIndex + 1; i <= _index; i++)
                m_history[i]->apply();
        }

        m_currentHistoryIndex = _index;

        // Security check
        m_historyIndexSetting = false;

        Debug::Log("%d/%d", m_currentHistoryIndex, (S32)m_history.size());
    }

    //////////////////////////////////////////
    void EditorActionManager::applyAction(EditorActionPtr const& _action)
    {
        if (!_action)
            return;

        Vector<EditorActionPtr>::iterator it = std::find(m_history.begin(), m_history.end(), _action);
        MAZE_DEBUG_BP_RETURN_IF(it != m_history.end());

        m_history.erase(m_history.begin() + (m_currentHistoryIndex + 1), m_history.end());

        if (m_history.size() >= m_historyMaxCapacity)
        {
            m_history.erase(m_history.begin() + 1, m_history.begin() + 2);
            --m_currentHistoryIndex;

        }

        m_history.push_back(_action);
        setCurrentHistoryIndex((S32)m_history.size() - 1);
    }

    //////////////////////////////////////////
    void EditorActionManager::undoAction()
    {
        if (m_history.empty())
            return;

        if (m_currentHistoryIndex < 0)
            return;

        setCurrentHistoryIndex(m_currentHistoryIndex - 1);
    }

    //////////////////////////////////////////
    void EditorActionManager::redoAction()
    {
        if (!m_history.size())
            return;

        if (m_currentHistoryIndex >= (S32)m_history.size() - 1)
            return;

        setCurrentHistoryIndex(m_currentHistoryIndex + 1);
    }

    ////////////////////////////////////
    void EditorActionManager::clearHistory()
    {
        m_history.clear();
        m_currentHistoryIndex = -1;
    }

    ////////////////////////////////////
    EditorActionPtr const& EditorActionManager::getLastEditorAction() const
    {
        static EditorActionPtr nullPointer;

        if (m_currentHistoryIndex < 0)
            return nullPointer;

        return m_history[m_currentHistoryIndex];
    }

    //////////////////////////////////////////
    void EditorActionManager::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::Z:
                    {
                        if (_data.isControlDown())
                        {
                            undoAction();
                        }

                        break;
                    }
                    case KeyCode::Y:
                    {
                        if (_data.isControlDown())
                        {
                            redoAction();
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
            default:
                break;
        }
    }


} // namespace Maze
//////////////////////////////////////////
