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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ToggleButton2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ToggleButton2D, Button2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, checked, false, getChecked, setChecked),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, checkByClick, true, getCheckByClick, setCheckByClick));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ToggleButton2D);

    //////////////////////////////////////////
    ToggleButton2D::ToggleButton2D()
        : m_checked(false)
        , m_checkByClick(true)
    {
    }

    //////////////////////////////////////////
    ToggleButton2D::~ToggleButton2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorReleaseIn.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    ToggleButton2DPtr ToggleButton2D::Create()
    {
        ToggleButton2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ToggleButton2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ToggleButton2D::init()
    {
        if (!Button2D::init())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ToggleButton2D::processEntityAwakened()
    {
        Button2D::processEntityAwakened();

        m_UIElement2D->eventCursorReleaseIn.subscribe(this, &ToggleButton2D::notifyCursorReleaseIn);
    }
    
    //////////////////////////////////////////
    void ToggleButton2D::setChecked(bool _value)
    {
        if (m_checked == _value)
            return;

        m_checked = _value;

        updateTransitionUI();
        eventCheckedChanged(this, m_checked);
    }

    //////////////////////////////////////////
    void ToggleButton2D::notifyCursorReleaseIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (getCheckByClick())
            setChecked(!getChecked());
    }

    //////////////////////////////////////////
    void ToggleButton2D::updateTransitionUI()
    {
        if (!m_transitionSprite)
            return;

        if (!getEntityRaw())
            return;

        if (!m_UIElement2D)
            return;

        if (getChecked())
        {
            m_transitionSprite->setColor(getSelectedColor());
        }
        else
        {
            if (getPressed())
            {
                m_transitionSprite->setColor(getPressedColor());
            }
            else
            {
                if (getFocused())
                {
                    m_transitionSprite->setColor(getFocusedColor());
                }
                else
                {
                    m_transitionSprite->setColor(getNormalColor());
                }
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
