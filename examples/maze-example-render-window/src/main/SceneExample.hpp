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
#if (!defined(_SceneExample_hpp_))
#define _SceneExample_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


//////////////////////////////////////////
MAZE_USING_SHARED_PTR(SceneExample);


//////////////////////////////////////////
// Class SceneExample
//
//////////////////////////////////////////
class SceneExample
    : public Maze::EcsRenderScene
{
public:

    //////////////////////////////////////////
    MAZE_DECLARE_METACLASS_WITH_PARENT(SceneExample, Maze::EcsRenderScene);

public:

    //////////////////////////////////////////
    static SceneExamplePtr Create();
    
    //////////////////////////////////////////
    virtual ~SceneExample();

    //////////////////////////////////////////
    virtual void update(Maze::F32 _dt) MAZE_OVERRIDE;

    //////////////////////////////////////////
    void notifyMouse(Maze::InputEventMouseData const& _data);

    //////////////////////////////////////////
    void testRender();

protected:

    //////////////////////////////////////////
    SceneExample();

    //////////////////////////////////////////
    virtual bool init() MAZE_OVERRIDE;

protected:
    Maze::MeshRendererPtr m_axesMeshRenderer;
    Maze::MeshRendererPtr m_meshRenderer;

    Maze::F32 m_timer;

    Maze::TMat m_modelMatrix;
    Maze::TMat m_modelMatrix2;
    Maze::TMat m_modelMatrix3;

    Maze::Transform3DPtr m_cameraTransform3D;

    Maze::F32 m_yawAngle;
    Maze::F32 m_pitchAngle;

    Maze::Vec2F32 m_cursorPositionLastFrame;
    bool m_cursorDrag;
};


#endif // _SceneExample_hpp_
//////////////////////////////////////////
