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
#include "maze-graphics/MazeRenderCommandsBuffer.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderCommandsBuffer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(RenderCommandsBuffer);

    //////////////////////////////////////////
    RenderCommandsBuffer::RenderCommandsBuffer()
        : m_commandsBufferData{0}
        , m_commandsCount(0)
    {

    }

    //////////////////////////////////////////
    void RenderCommandsBuffer::executeAndClear(IterateCallback _executeCallback)
    {
        U8* MAZE_RESTRICT_ALIAS commandsBufferBase = m_commandsBufferData;
        for (S32 i = m_commandsCount; i--; )
        {
            RenderCommand* MAZE_RESTRICT_ALIAS command = reinterpret_cast<RenderCommand* MAZE_RESTRICT_ALIAS>(commandsBufferBase);

            _executeCallback(command);

            commandsBufferBase += c_renderCommandFixedSize;
        }

        m_commandsCount = 0;
    }

    //////////////////////////////////////////
    void RenderCommandsBuffer::iterate(IterateCallback _iterateCallback)
    {
        U8* MAZE_RESTRICT_ALIAS commandsBufferBase = m_commandsBufferData;
        for (S32 i = m_commandsCount; i--; )
        {
            RenderCommand* MAZE_RESTRICT_ALIAS command = reinterpret_cast<RenderCommand* MAZE_RESTRICT_ALIAS>(commandsBufferBase);

            _iterateCallback(command);

            commandsBufferBase += c_renderCommandFixedSize;
        }
    }


} // namespace Maze
//////////////////////////////////////////
