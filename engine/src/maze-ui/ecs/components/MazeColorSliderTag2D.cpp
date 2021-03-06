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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeColorSliderTag2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorSliderTag2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ColorSliderTag2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, colorRenderer, ComponentPtr(), getColorRendererComponent, setColorRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, toggleButton, ComponentPtr(), getToggleButtonComponent, setToggleButton));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ColorSliderTag2D);

    //////////////////////////////////////////
    ColorSliderTag2D::ColorSliderTag2D()
    {
    }

    //////////////////////////////////////////
    ColorSliderTag2D::~ColorSliderTag2D()
    {

    }

    //////////////////////////////////////////
    ColorSliderTag2DPtr ColorSliderTag2D::Create()
    {
        ColorSliderTag2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorSliderTag2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ColorSliderTag2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ColorSliderTag2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
    }        


} // namespace Maze
//////////////////////////////////////////
