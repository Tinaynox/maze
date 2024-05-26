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
#if (!defined(_MazeParticleSystem3DGizmos_hpp_))
#define _MazeParticleSystem3DGizmos_hpp_


//////////////////////////////////////////
#include "maze-plugin-particles-editor-tools/MazeParticlesEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ParticleSystem3DGizmos);


    //////////////////////////////////////////
    // Class ParticleSystem3DGizmos
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API ParticleSystem3DGizmos
        : public ComponentGizmos
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ParticleSystem3DGizmos, ComponentGizmos);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ParticleSystem3DGizmos);

    public:

        //////////////////////////////////////////
        virtual void drawGizmos(
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer* _drawer) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawGizmosSelected(
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer* _drawer) MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual ~ParticleSystem3DGizmos();

        //////////////////////////////////////////
        static ParticleSystem3DGizmosPtr Create();


    protected:

        //////////////////////////////////////////
        ParticleSystem3DGizmos();

        //////////////////////////////////////////
        bool init();

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DGizmos_hpp_
//////////////////////////////////////////
