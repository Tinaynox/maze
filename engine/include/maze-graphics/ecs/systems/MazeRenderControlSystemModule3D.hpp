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
#if (!defined(_MazeRenderControlSystemModule3D_hpp_))
#define _MazeRenderControlSystemModule3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderControlSystemModule3D);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Light3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);
    MAZE_USING_SHARED_PTR(RenderPass);
    MAZE_USING_SHARED_PTR(VertexArrayObject);


    //////////////////////////////////////////
    // Struct
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderUnit
    {
        //////////////////////////////////////////
        RenderUnit()
        {
        }

        //////////////////////////////////////////
        RenderUnit(
            RenderPassPtr const& _renderPass,
            VertexArrayObjectPtr const& _vao,
            Vec3DF const& _worldPosition,
            S32 _count,
            Mat4DF const* _modelMatricies,
            Vec4DF const* _colorStream = nullptr,
            Vec4DF const* _uvStream = nullptr)
            : renderPass(_renderPass)
            , vao(_vao)
            , worldPosition(_worldPosition)
            , count(_count)
            , modelMatricies(_modelMatricies)
            , colorStream(_colorStream)
            , uvStream(_uvStream)
        {
        }

        RenderPassPtr renderPass;
        VertexArrayObjectPtr vao;
        Vec3DF worldPosition;
        S32 count = 0;
        Mat4DF const* modelMatricies = nullptr;
        Vec4DF const* colorStream = nullptr;
        Vec4DF const* uvStream = nullptr;

        F32 sqrDistanceToCamera;
    };


    //////////////////////////////////////////
    // Class RenderControlSystemModule3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderControlSystemModule3D
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderControlSystemModule3D();

        //////////////////////////////////////////
        static RenderControlSystemModule3DPtr Create(ECSWorldPtr const& _world);


        //////////////////////////////////////////
        void draw(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processUpdate(F32 _dt);

        //////////////////////////////////////////
        void processPostUpdate(F32 _dt);

    public:

        //////////////////////////////////////////
        MultiDelegate<Camera3D*, Vector<RenderUnit>&> eventGatherRenderUnits;

    protected:

        //////////////////////////////////////////
        RenderControlSystemModule3D();

        //////////////////////////////////////////
        bool init(ECSWorldPtr const& _world);


    protected:
        ECSWorld* m_world;

        SharedPtr<GenericInclusiveEntitiesSample<MeshRenderer, Transform3D>> m_meshRenderers;
        SharedPtr<GenericInclusiveEntitiesSample<TrailRenderer3D, Transform3D>> m_trailRenderers3DSample;
        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
        SharedPtr<GenericInclusiveEntitiesSample<Light3D>> m_lights3DSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderControlSystemModule3D_hpp_
//////////////////////////////////////////
