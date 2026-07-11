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
#if (!defined(_MazeShaderSystemVulkan_hpp_))
#define _MazeShaderSystemVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderSystemVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class ShaderSystemVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API ShaderSystemVulkan
        : public ShaderSystem
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderSystemVulkan();

        //////////////////////////////////////////
        static void Initialize(ShaderSystemPtr& _object, RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkan();


        //////////////////////////////////////////
        virtual ShaderPtr createShader() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ShaderPtr createShader(AssetFilePtr const& _shaderFile) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ShaderPtr const& createBuiltinShader(BuiltinShaderType _shaderType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void reloadShaders() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderSystemVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem) MAZE_OVERRIDE;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystemVulkan_hpp_
//////////////////////////////////////////
