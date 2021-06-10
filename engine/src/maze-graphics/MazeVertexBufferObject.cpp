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
#include "maze-graphics/MazeVertexBufferObject.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class VertexBufferObject
    //
    //////////////////////////////////////////
    VertexBufferObject::VertexBufferObject()
        : m_renderSystem(nullptr)
        , m_accessType(GPUByteBufferAccessType::None)
    {
    }

    //////////////////////////////////////////
    VertexBufferObject::~VertexBufferObject()
    {
    }

    //////////////////////////////////////////
    VertexBufferObjectPtr VertexBufferObject::Create(
        RenderSystem* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createVertexBufferObject(_accessType, _singleMapping);
    }

    //////////////////////////////////////////
    bool VertexBufferObject::init(
        RenderSystem* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        m_renderSystem = _renderSystem;
        m_accessType = _accessType;

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
