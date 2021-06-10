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
#if (!defined(_MazeVertexBuffer_hpp_))
#define _MazeVertexBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexBufferObject);
    MAZE_USING_SHARED_PTR(RenderSystem);
    

    //////////////////////////////////////////
    // Class VertexBufferObject
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API VertexBufferObject
        : public SharedObject<VertexBufferObject>
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexBufferObject();

        //////////////////////////////////////////
        static VertexBufferObjectPtr Create(
            RenderSystem* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false);

        //////////////////////////////////////////
        GPUByteBufferAccessType::Enum getAccessType() const { return m_accessType; }

        //////////////////////////////////////////
        virtual void resize(Size _bytes) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(
            void const* _data,
            Size _bytes) MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        VertexBufferObject();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false);
    
    protected:
        RenderSystem* m_renderSystem;

        GPUByteBufferAccessType::Enum m_accessType;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexBuffer_hpp_
//////////////////////////////////////////
