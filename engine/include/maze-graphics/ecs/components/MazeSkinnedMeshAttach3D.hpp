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
#if (!defined(_MazeSkinnedMeshAttach3D_hpp_))
#define _MazeSkinnedMeshAttach3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/ecs/components/MazeSkinnedMeshSkeleton.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SkinnedMeshAttach3D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimator);


    //////////////////////////////////////////
    // Class SkinnedMeshAttach3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SkinnedMeshAttach3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SkinnedMeshAttach3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SkinnedMeshAttach3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SkinnedMeshAttach3D();

        //////////////////////////////////////////
        static SkinnedMeshAttach3DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(SkinnedMeshSkeleton, skeleton, Skeleton);


        //////////////////////////////////////////
        inline String const& getBoneName() const { return m_boneName; }

        //////////////////////////////////////////
        inline void setBoneName(String const& _value) { m_boneName = _value; updateBoneIndex(); }

        //////////////////////////////////////////
        inline S32 getBoneIndex() const { return m_boneIndex; }



        //////////////////////////////////////////
        void assignSkeletonIfRequired();

    protected:

        //////////////////////////////////////////
        SkinnedMeshAttach3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void updateBoneIndex();

    protected:
        SkinnedMeshSkeletonPtr m_skeleton;

        String m_boneName;
        S32 m_boneIndex = -1;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSkinnedMeshAttach3D_hpp_
//////////////////////////////////////////
