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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeMaterialDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MaterialDX11
    //
    //////////////////////////////////////////
    MaterialDX11::MaterialDX11()
    {
    }

    //////////////////////////////////////////
    MaterialDX11::~MaterialDX11()
    {
    }

    //////////////////////////////////////////
    MaterialDX11Ptr MaterialDX11::Create(
        RenderSystem* _renderSystem,
        MaterialDeleter const& _deleter)
    {
        MaterialDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialDX11, object, _deleter, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    MaterialDX11Ptr MaterialDX11::Create(
        MaterialDX11Ptr const& _material,
        MaterialDeleter const& _deleter)
    {
        MaterialDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialDX11, object, _deleter, init(MaterialPtr(_material)));
        return object;
    }

    //////////////////////////////////////////
    bool MaterialDX11::init(RenderSystem* _renderSystem)
    {
        if (!Material::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool MaterialDX11::init(MaterialPtr const& _material)
    {
        if (!Material::init(_material))
            return false;

        return true;
    }

    //////////////////////////////////////////
    MaterialPtr MaterialDX11::createCopy()
    {
        return MaterialDX11::Create(cast<MaterialDX11>());
    }

    //////////////////////////////////////////
    void MaterialDX11::set(MaterialPtr const& _material)
    {
        Material::set(_material);
    }


} // namespace Maze
//////////////////////////////////////////
