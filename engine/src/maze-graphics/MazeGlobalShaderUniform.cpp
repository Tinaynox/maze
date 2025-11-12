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
#include "maze-graphics/MazeGlobalShaderUniform.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/events/MazeGraphicsEvents.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/managers/MazeEventManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class GlobalShaderUniform
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_INDEXED_RESOURCE(GlobalShaderUniform);
    
    //////////////////////////////////////////
    GlobalShaderUniform::GlobalShaderUniform()
    {
    }

    //////////////////////////////////////////
    GlobalShaderUniform::~GlobalShaderUniform()
    {
    }

    //////////////////////////////////////////
    GlobalShaderUniformPtr GlobalShaderUniform::Create(
        RenderSystem* _renderSystem,
        HashedString const& _name)
    {
        GlobalShaderUniformPtr subMesh;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(GlobalShaderUniform, subMesh, init(_renderSystem, _name));
        return subMesh;
    }

    //////////////////////////////////////////
    bool GlobalShaderUniform::init(
        RenderSystem* _renderSystem,
        HashedString const& _name)
    {
        m_renderSystem = _renderSystem;
        m_name = _name;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<GlobalShaderUniformAddedEvent>(getResourceId());

        return true;
    }

    //////////////////////////////////////////
    void GlobalShaderUniform::setVariant(ShaderUniformVariant const& _variant)
    {
        if (m_variant == _variant)
            return;

        m_variant = _variant;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<GlobalShaderUniformChangedEvent>(getResourceId());
    }

    //////////////////////////////////////////
    void GlobalShaderUniform::setVariantImmediate(ShaderUniformVariant const& _variant)
    {
        if (m_variant == _variant)
            return;

        m_variant = _variant;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEventImmediate<GlobalShaderUniformChangedEvent>(getResourceId());
    }

    //////////////////////////////////////////
    void GlobalShaderUniform::setValue(Vector<Vec3F> const& _vec)
    {
        S32 size = S32(_vec.size() * sizeof(Vec3F));
        m_heapData.resize(size);
        m_heapData.copyFrom(reinterpret_cast<U8 const*>(&_vec[0]), size);
        m_variant.set((Vec3F const*)m_heapData.getDataRO(), (U32)_vec.size());
    }

    //////////////////////////////////////////
    void GlobalShaderUniform::setValue(Vector<Vec4F> const& _vec)
    {
        S32 size = S32(_vec.size() * sizeof(Vec4F));
        m_heapData.resize(size);
        m_heapData.copyFrom(reinterpret_cast<U8 const*>(&_vec[0]), size);
        m_variant.set((Vec4F const*)m_heapData.getDataRO(), (U32)_vec.size());
    }


} // namespace Maze
//////////////////////////////////////////
