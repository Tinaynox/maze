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
#if (!defined(_MazeDebugGridRenderer_hpp_))
#define _MazeDebugGridRenderer_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(DebugGridRenderer);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    // Class DebugGridRenderer
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API DebugGridRenderer
        : public Component
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DebugGridRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(DebugGridRenderer);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~DebugGridRenderer();

        //////////////////////////////////////////
        static DebugGridRendererPtr Create(
            Camera3DPtr const& _camera,
            RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }


    protected:

        //////////////////////////////////////////
        DebugGridRenderer();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Camera3DPtr const& _camera, RenderSystem* _renderSystem);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void updateMesh();

    protected:
        RenderSystem* m_renderSystem;
        Camera3DPtr m_camera;

        Transform3DPtr m_transform;
        MeshRendererPtr m_meshRenderer;

        Vec3DF m_prevMeshCameraPosition;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDebugGridRenderer_hpp_
//////////////////////////////////////////
