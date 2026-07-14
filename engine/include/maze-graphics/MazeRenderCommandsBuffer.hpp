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
#pragma once
#if (!defined(_MazeRenderCommandsBuffer_hpp_))
#define _MazeRenderCommandsBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderCommand.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct RenderCommandsBuffer;


    //////////////////////////////////////////
    // Class RenderCommandsBuffer
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandsBuffer
    {
    public:

        //////////////////////////////////////////
        static Size const c_renderCommandFixedSize = 32;
        static Size const c_renderCommandsCountMax = 65536;

        ////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderCommandsBuffer);

        ////////////////////////////////////
        using IterateCallback = std::function<void(RenderCommand*)>;

    public:

        //////////////////////////////////////////
        RenderCommandsBuffer();

        //////////////////////////////////////////
        void executeAndClear(IterateCallback _executeCallback);

        //////////////////////////////////////////
        void iterate(IterateCallback _executeCallback);

        //////////////////////////////////////////
        template <typename TCommand, typename ...TArgs>
        inline TCommand* createCommand(TArgs... _args)
        {
            static_assert(
                sizeof(TCommand) <= c_renderCommandFixedSize,
                "RenderCommand size exceeds c_renderCommandFixedSize!");

            if (m_commandsCount >= (S32)c_renderCommandsCountMax)
            {
                // Drop the command instead of writing past the buffer end.
                // The scratch slot keeps the returned pointer safe to write through
                MAZE_ERROR("Render commands count exceeded (%d)! Command dropped.", (S32)c_renderCommandsCountMax);
                return new (m_overflowScratch) TCommand(_args...);
            }

            U8* pointer = m_commandsBufferData + m_commandsCount * c_renderCommandFixedSize;
            ++m_commandsCount;
            TCommand* command = new (pointer) TCommand(_args...);
            return command;
        }

        //////////////////////////////////////////
        inline void appendCommand(RenderCommand const& _command)
        {
            if (m_commandsCount >= (S32)c_renderCommandsCountMax)
            {
                MAZE_ERROR("Render commands count exceeded (%d)! Command dropped.", (S32)c_renderCommandsCountMax);
                return;
            }

            Size pointerOffset = m_commandsCount * c_renderCommandFixedSize;

            ++m_commandsCount;
            memcpy(
                &m_commandsBufferData[pointerOffset],
                &_command,
                c_renderCommandFixedSize);
        }

        //////////////////////////////////////////
        inline void appendCommands(RenderCommandsBuffer const& _buffer)
        {
            S32 count = _buffer.getCommandsCount();
            S32 freeCount = (S32)c_renderCommandsCountMax - m_commandsCount;
            if (count > freeCount)
            {
                MAZE_ERROR("Render commands count exceeded (%d)! %d command(s) dropped.", (S32)c_renderCommandsCountMax, count - freeCount);
                count = freeCount;
                if (count <= 0)
                    return;
            }

            Size pointerOffset = m_commandsCount * c_renderCommandFixedSize;

            m_commandsCount += count;
            memcpy(
                &m_commandsBufferData[pointerOffset],
                _buffer.getCommandsBufferData(),
                c_renderCommandFixedSize * count);
        }

        //////////////////////////////////////////
        inline RenderCommand* MAZE_RESTRICT_ALIAS_RETURN getCommand(Size _index)
        {
            return (RenderCommand* MAZE_RESTRICT_ALIAS)(m_commandsBufferData + _index * c_renderCommandFixedSize);
        }

        //////////////////////////////////////////
        inline void clear()
        {
            m_commandsCount = 0;
        }

        //////////////////////////////////////////
        inline S32 getCommandsCount() const { return m_commandsCount; }

        //////////////////////////////////////////
        inline U8* MAZE_RESTRICT_ALIAS_RETURN getCommandsBufferData() const { return (U8*)m_commandsBufferData; }

        //////////////////////////////////////////
        inline bool empty() const { return m_commandsCount == 0; }


    public:
        alignas(16) U8 m_commandsBufferData[c_renderCommandFixedSize * c_renderCommandsCountMax];
        alignas(16) U8 m_overflowScratch[c_renderCommandFixedSize];
        S32 m_commandsCount;
    };
       

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderCommandsBuffer_hpp_
//////////////////////////////////////////
