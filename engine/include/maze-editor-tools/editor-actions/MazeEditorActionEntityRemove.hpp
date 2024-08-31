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
#if (!defined(_MazeEditorActionEntityRemove_hpp_))
#define _MazeEditorActionEntityRemove_hpp_


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
    MAZE_USING_SHARED_PTR(EditorActionEntityRemove);


    //////////////////////////////////////////
    // Class EditorActionEntityRemove
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionEntityRemove
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionEntityRemove, EditorAction);

        //////////////////////////////////////////
        struct EntityData
        {
            EntityPtr entity;
            Transform2DPtr parentTransform2D;
            Transform3DPtr parentTransform3D;
        };

    public:

        //////////////////////////////////////////
        virtual ~EditorActionEntityRemove();

        //////////////////////////////////////////
        static EditorActionEntityRemovePtr Create(
            EntityPtr const& _entity);


        //////////////////////////////////////////
        inline EntityPtr const& getEntity() const { return m_entity; }

    protected:

        //////////////////////////////////////////
        EditorActionEntityRemove();

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
        //Transform2DPtr m_parentTransform2D;
        //Transform3DPtr m_parentTransform3D;

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionEntityRemove_hpp_
//////////////////////////////////////////
