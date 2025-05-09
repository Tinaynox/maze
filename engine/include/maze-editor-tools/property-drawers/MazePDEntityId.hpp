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
#if (!defined(_MazePropertyDrawerEntityId_hpp_))
#define _MazePropertyDrawerEntityId_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerEntityId);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);


    //////////////////////////////////////////
    // Class PropertyDrawerEntityId
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerEntityId
        : public GenericPropertyDrawer<EntityId>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerEntityId, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerEntityId);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerEntityId();

        //////////////////////////////////////////
        static PropertyDrawerEntityIdPtr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(EntityId const& _entity) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual EntityId getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

        ////////////////////////////////////////////
        virtual void unselectUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerEntityId();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;

    protected:
        AbstractTextRenderer2DPtr m_text;

        EntityId m_entityId = c_invalidEntityId;

        SpriteRenderer2DPtr m_panelRenderer;
        SpriteRenderer2DPtr m_dragAndDropFrame;
        DragAndDropZonePtr m_dragAndDropZone;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerEntityId_hpp_
//////////////////////////////////////////
