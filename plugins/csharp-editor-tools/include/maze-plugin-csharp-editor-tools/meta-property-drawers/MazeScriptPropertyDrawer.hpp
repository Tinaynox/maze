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
#if (!defined(_MazeScriptPropertyDrawer_hpp_))
#define _MazeScriptPropertyDrawer_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptPropertyDrawer);


    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API ScriptPropertyDrawerCallbacks
    {
        std::function<PropertyDrawerPtr()> createDrawerCb;
        std::function<void(ScriptInstance const&, ScriptPropertyPtr const&, PropertyDrawerPtr const&)> processDataToUICb;
        std::function<void(ScriptInstance&, ScriptPropertyPtr const&, PropertyDrawerPtr const&)> processDataFromUICb;
    };


    //////////////////////////////////////////
    // Class ScriptPropertyDrawer
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API ScriptPropertyDrawer
        : public MetaPropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ScriptPropertyDrawer, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ScriptPropertyDrawer);

    public:

        //////////////////////////////////////////
        virtual ~ScriptPropertyDrawer();

        //////////////////////////////////////////
        static ScriptPropertyDrawerPtr Create(
            ScriptPropertyPtr const& _scriptProperty,
            ScriptPropertyDrawerCallbacks _callbacks);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ScriptPropertyDrawer();

        //////////////////////////////////////////
        virtual bool init(
            ScriptPropertyPtr const& _scriptProperty,
            ScriptPropertyDrawerCallbacks _callbacks);

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        ScriptPropertyPtr m_scriptProperty;

        PropertyDrawerPtr m_drawer;
        ScriptPropertyDrawerCallbacks m_callbacks;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptPropertyDrawer_hpp_
//////////////////////////////////////////
