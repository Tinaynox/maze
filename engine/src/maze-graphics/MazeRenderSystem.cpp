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
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(RenderSystem);

    //////////////////////////////////////////
    RenderSystem::RenderSystem()
        : m_systemInited(false)
        , m_currentRenderTarget(nullptr)
        , m_drawCalls(0)
        , m_drawCallsLimit(0)
    {
    }

    //////////////////////////////////////////
    RenderSystem::~RenderSystem()
    {
        m_systemFontManager.reset();
        m_materialManager.reset();
    }

    //////////////////////////////////////////
    bool RenderSystem::init()
    {
        TextureManager::Initialize(m_textureManager, getSharedPtr());
        SpriteManager::Initialize(m_imageManager, getSharedPtr());
        RenderMeshManager::Initialize(m_renderMeshManager, getSharedPtr());
        MaterialManager::Initialize(m_materialManager, getSharedPtr());
        SystemFontManager::Initialize(m_systemFontManager, getSharedPtr());

        return true;
    }

    //////////////////////////////////////////
    RenderSystem* RenderSystem::GetCurrentInstancePtr()
    {
        return GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();
    }

    //////////////////////////////////////////
    bool RenderSystem::processRenderWindowCreated(RenderWindowPtr const& _renderWindow)
    {
        m_renderWindows.insert(_renderWindow.get());
        eventRenderWindowsChanged();
        return true;
    }

    //////////////////////////////////////////
    bool RenderSystem::processRenderWindowDestroyed(RenderWindow* _renderWindow)
    {
        m_renderWindows.erase(_renderWindow);
        eventRenderWindowsChanged();
        return true;
    }

    //////////////////////////////////////////
    void RenderSystem::processRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        if (m_currentRenderTarget == _renderTarget)
        {
            m_currentRenderTarget = nullptr;
        }
    }

    //////////////////////////////////////////
    void RenderSystem::processSystemInited()
    {
        MAZE_ERROR_RETURN_IF(m_systemInited, "Double initialization detected!");

        m_systemInited = true;

        eventSystemInited();
    }


} // namespace Maze
//////////////////////////////////////////
