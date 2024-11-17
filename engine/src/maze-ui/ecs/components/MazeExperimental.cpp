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
#include "maze-ui/ecs/components/MazeExperimental.hpp"
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
    // Class Experimental
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Experimental, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(int, test, 42, getTest, setTest),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, colorGradient, ColorGradient(), getColorGradient, setColorGradient),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AnimationCurve, animationCurve, AnimationCurve(), getAnimationCurve, setAnimationCurve),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AnimationCurve, animationCurve2, AnimationCurve(), getAnimationCurve2, setAnimationCurve2),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(int, test2, 12, getTest2, setTest2));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Experimental);

    //////////////////////////////////////////
    Experimental::Experimental()
        : m_test(42)
        , m_test2(12)
    {
    }

    //////////////////////////////////////////
    Experimental::~Experimental()
    {

    }

    //////////////////////////////////////////
    ExperimentalPtr Experimental::Create()
    {
        ExperimentalPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Experimental, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Experimental::init()
    {

        return true;
    }
   

} // namespace Maze
//////////////////////////////////////////
