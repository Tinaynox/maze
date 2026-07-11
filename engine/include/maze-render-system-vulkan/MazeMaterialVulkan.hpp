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
#if (!defined(_MazeMaterialVulkan_hpp_))
#define _MazeMaterialVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(MaterialVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class MaterialVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API MaterialVulkan
        : public Material
    {
    public:

        //////////////////////////////////////////
        using MaterialDeleter = std::function<void(MaterialVulkan* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~MaterialVulkan();

        //////////////////////////////////////////
        static MaterialVulkanPtr Create(
            RenderSystem* _renderSystem,
            MaterialDeleter const& _deleter = DefaultDelete<MaterialVulkan>());

        //////////////////////////////////////////
        static MaterialVulkanPtr Create(
            MaterialVulkanPtr const& _material,
            MaterialDeleter const& _deleter = DefaultDelete<MaterialVulkan>());

        //////////////////////////////////////////
        virtual MaterialPtr createCopy() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void set(MaterialPtr const& _material) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MaterialVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool init(MaterialPtr const& _material) MAZE_OVERRIDE;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialVulkan_hpp_
//////////////////////////////////////////
