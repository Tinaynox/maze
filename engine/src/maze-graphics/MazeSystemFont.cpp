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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeSystemFont.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    void SystemFont::FromString(SystemFontPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("SystemFont::FromString");

        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SystemFontManagerPtr const& systemFontManager = renderSystem->getSystemFontManager();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = systemFontManager->getSystemFont(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void SystemFont::ToString(SystemFont const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("SystemFont::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SystemFontManagerPtr const& systemFontManager = renderSystem->getSystemFontManager();

        CString materialName = systemFontManager->getSystemFontName(_value);
        if (materialName != nullptr)
        {
            _data = materialName;
        }
        else
        {
            // #TODO: Replace with ResourceId
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

    //////////////////////////////////////////
    String SystemFont::toString() const
    {
        String value;
        SystemFont::ToString(this, value);
        return value;
    }

    //////////////////////////////////////////
    void SystemFont::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }

} // namespace Maze
//////////////////////////////////////////
