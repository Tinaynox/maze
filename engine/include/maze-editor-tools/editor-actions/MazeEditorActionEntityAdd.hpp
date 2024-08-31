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
#if (!defined(_MazeEditorActionEntityAdd_hpp_))
#define _MazeEditorActionEntityAdd_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionEntityAdd);


    //////////////////////////////////////////
    // Class EditorActionEntityAdd
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionEntityAdd
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionEntityAdd, EditorAction);

        //////////////////////////////////////////
        struct EntityData
        {
            EntityPtr entity;
            Transform2DPtr parentTransform2D;
            Transform3DPtr parentTransform3D;
        };

    public:

        //////////////////////////////////////////
        virtual ~EditorActionEntityAdd();

        //////////////////////////////////////////
        static EditorActionEntityAddPtr Create(
            EntityPtr const& _entity);


        //////////////////////////////////////////
        inline EntityPtr const& getEntity() const { return m_entity; }

    protected:

        //////////////////////////////////////////
        EditorActionEntityAdd();

        //////////////////////////////////////////
        bool init(
            EntityPtr const& _entity);


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        EntityPtr m_entity;
        EcsWorldPtr m_world;
        EcsSceneId m_sceneId;

        Vector<EntityData> m_entityData;

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionEntityAdd_hpp_
//////////////////////////////////////////
