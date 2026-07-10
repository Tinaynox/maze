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
#if (!defined(_MazeShaderSystemDX11_hpp_))
#define _MazeShaderSystemDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderSystemDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class ShaderSystemDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API ShaderSystemDX11
        : public ShaderSystem
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderSystemDX11();

        //////////////////////////////////////////
        static void Initialize(ShaderSystemPtr& _object, RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11();


        //////////////////////////////////////////
        virtual ShaderPtr createShader() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ShaderPtr createShader(AssetFilePtr const& _shaderFile) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ShaderPtr const& createBuiltinShader(BuiltinShaderType _shaderType) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderSystemDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem) MAZE_OVERRIDE;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystemDX11_hpp_
//////////////////////////////////////////
