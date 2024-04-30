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
#if (!defined(_ExampleHelper_hpp_))
#define _ExampleHelper_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-engine/MazeEngine.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace ExampleHelper
    {
        //////////////////////////////////////////
        struct SimpleLevelConfig
        {
            MaterialPtr floorMaterial;
            F32 floorTextureScale = 5.0f;
            MaterialPtr wallMaterial;
            F32 wallTextureScale = 5.0f;
        };


        //////////////////////////////////////////
        EntityPtr CreateBox(
            ECSRenderScene* _scene,
            Vec3F32 const& _position,
            Vec3F32 const& _scale,
            MaterialPtr const& _material = nullptr,
            Vec2F32 textureScale = Vec2F32::c_one);

        //////////////////////////////////////////
        void BuildSimpleLevel(
            ECSRenderScene* _scene,
            Vec2F32 const& _levelSize = Vec2F32(50.0f, 50.0f),
            SimpleLevelConfig config = SimpleLevelConfig());

    } // namespace ExampleHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _ExampleHelper_hpp_
//////////////////////////////////////////
