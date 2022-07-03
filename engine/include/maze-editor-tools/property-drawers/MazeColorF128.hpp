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
#if (!defined(_MazePropertyDrawerColorF128_hpp_))
#define _MazePropertyDrawerColorF128_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-ui/ecs/components/MazeColorHDREdit2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerColorF128);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerColorF128
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerColorF128
        : public GenericPropertyDrawer<ColorF128>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerColorF128, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerColorF128);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerColorF128();

        //////////////////////////////////////////
        static PropertyDrawerColorF128Ptr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(ColorF128 const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ColorF128 getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(String const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String getString() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerColorF128();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        void notifyColorChanged(ColorHDREdit2D* _colorEdit, ColorF128 const& _color);

    protected:
        ColorHDREdit2DPtr m_colorEdit;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerColorF128_hpp_
//////////////////////////////////////////
