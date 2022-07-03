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
#if (!defined(_MazeMetaPropertyDrawerVec2DF_hpp_))
#define _MazeMetaPropertyDrawerVec2DF_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazeVec2DF.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerVec2DF);
    MAZE_USING_SHARED_PTR(EditBox2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerVec2DF
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerVec2DF
        : public GenericMetaPropertyDrawer<Vec2DF>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerVec2DF, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerVec2DF);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerVec2DF();

        //////////////////////////////////////////
        static MetaPropertyDrawerVec2DFPtr Create(MetaProperty* _metaProperty);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerVec2DF();

        //////////////////////////////////////////
        virtual bool init(MetaProperty* _metaProperty) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        PropertyDrawerVec2DFPtr m_drawer;

        bool m_processingDataToUI = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerVec2DF_hpp_
//////////////////////////////////////////
