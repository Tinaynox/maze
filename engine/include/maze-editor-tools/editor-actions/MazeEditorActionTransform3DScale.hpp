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
#if (!defined(_MazeEditorActionTransform3DScale_hpp_))
#define _MazeEditorActionTransform3DScale_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionTransform3DScale);


    //////////////////////////////////////////
    // Class EditorActionTransform3DScale
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionTransform3DScale
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionTransform3DScale, EditorAction);

    public:

        //////////////////////////////////////////
        virtual ~EditorActionTransform3DScale();

        //////////////////////////////////////////
        static EditorActionTransform3DScalePtr Create(
            EntityPtr const& _entity,
            Vec3F const& _newScale);


        //////////////////////////////////////////
        inline EntityPtr const& getEntity() const { return m_entity; }

        //////////////////////////////////////////
        void merge(Vec3F const& _newScale);

    protected:

        //////////////////////////////////////////
        EditorActionTransform3DScale();

        //////////////////////////////////////////
        bool init(
            EntityPtr const& _entity,
            Vec3F const& _newScale);


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        EntityPtr m_entity;
        Vec3F m_prevScale = Vec3F::c_zero;
        Vec3F m_newScale = Vec3F::c_zero;

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionTransform3DScale_hpp_
//////////////////////////////////////////