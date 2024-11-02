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
#if (!defined(_MazeMonoPropertyDrawer_hpp_))
#define _MazeMonoPropertyDrawer_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeMonoFieldType.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MonoPropertyDrawer);


    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API MonoPropertyDrawerCallbacks
    {
        std::function<PropertyDrawerPtr()> createDrawerCb;
        std::function<void(ScriptInstance const&, MonoProperty*, PropertyDrawerPtr const&)> processDataToUICb;
        std::function<void(ScriptInstance&, MonoProperty*, PropertyDrawerPtr const&)> processDataFromUICb;
    };


    //////////////////////////////////////////
    // Class MonoPropertyDrawer
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API MonoPropertyDrawer
        : public MetaPropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MonoPropertyDrawer, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MonoPropertyDrawer);

    public:

        //////////////////////////////////////////
        virtual ~MonoPropertyDrawer();

        //////////////////////////////////////////
        static MonoPropertyDrawerPtr Create(
            MonoProperty* _monoProperty,
            MonoPropertyDrawerCallbacks _callbacks);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MonoPropertyDrawer();

        //////////////////////////////////////////
        virtual bool init(
            MonoProperty* _monoProperty,
            MonoPropertyDrawerCallbacks _callbacks);

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        MonoProperty* m_monoProperty = nullptr;

        PropertyDrawerPtr m_drawer;
        MonoPropertyDrawerCallbacks m_callbacks;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoPropertyDrawer_hpp_
//////////////////////////////////////////
