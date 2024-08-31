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
#if (!defined(_MazeEditorActionTransform3DRotate_hpp_))
#define _MazeEditorActionTransform3DRotate_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionTransform3DRotate);


    //////////////////////////////////////////
    // Class EditorActionTransform3DRotate
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionTransform3DRotate
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionTransform3DRotate, EditorAction);

    public:

        //////////////////////////////////////////
        virtual ~EditorActionTransform3DRotate();

        //////////////////////////////////////////
        static EditorActionTransform3DRotatePtr Create(
            EntityPtr const& _entity,
            Quaternion const& _newRotation);


        //////////////////////////////////////////
        inline EntityPtr const& getEntity() const { return m_entity; }

        //////////////////////////////////////////
        void merge(Quaternion const& _newRotation);

    protected:

        //////////////////////////////////////////
        EditorActionTransform3DRotate();

        //////////////////////////////////////////
        bool init(
            EntityPtr const& _entity,
            Quaternion const& _newRotation);


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        EntityPtr m_entity;
        Quaternion m_prevRotation = Quaternion::c_identity;
        Quaternion m_newRotation = Quaternion::c_identity;

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionTransform3DRotate_hpp_
//////////////////////////////////////////
