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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeMaterialVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MaterialVulkan
    //
    //////////////////////////////////////////
    MaterialVulkan::MaterialVulkan()
    {
    }

    //////////////////////////////////////////
    MaterialVulkan::~MaterialVulkan()
    {
    }

    //////////////////////////////////////////
    MaterialVulkanPtr MaterialVulkan::Create(
        RenderSystem* _renderSystem,
        MaterialDeleter const& _deleter)
    {
        MaterialVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialVulkan, object, _deleter, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    MaterialVulkanPtr MaterialVulkan::Create(
        MaterialVulkanPtr const& _material,
        MaterialDeleter const& _deleter)
    {
        MaterialVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialVulkan, object, _deleter, init(MaterialPtr(_material)));
        return object;
    }

    //////////////////////////////////////////
    bool MaterialVulkan::init(RenderSystem* _renderSystem)
    {
        if (!Material::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool MaterialVulkan::init(MaterialPtr const& _material)
    {
        // Material::init(MaterialPtr const&) already deep-copies m_uniforms
        // (fixed for the DX11 backend, see project memory
        // project_dx11_render_system.md item 4 - a prior shallow-copy bug
        // here caused every copy of a material to share one texture variant)
        if (!Material::init(_material))
            return false;

        return true;
    }

    //////////////////////////////////////////
    MaterialPtr MaterialVulkan::createCopy()
    {
        return MaterialVulkan::Create(cast<MaterialVulkan>());
    }

    //////////////////////////////////////////
    void MaterialVulkan::set(MaterialPtr const& _material)
    {
        Material::set(_material);
    }


} // namespace Maze
//////////////////////////////////////////
