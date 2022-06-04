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
#if (!defined(_MazePropertyDrawerTexture2D_hpp_))
#define _MazePropertyDrawerTexture2D_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerTexture2D);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);


    //////////////////////////////////////////
    // Class PropertyDrawerTexture2D
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerTexture2D
        : public GenericPropertyDrawer<Texture2DPtr>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerTexture2D, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerTexture2D);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerTexture2D();

        //////////////////////////////////////////
        static PropertyDrawerTexture2DPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(Texture2DPtr const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DPtr getValue() const MAZE_OVERRIDE;


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
        PropertyDrawerTexture2D();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event);

    protected:
        Texture2DPtr m_texture;
        SpritePtr m_sprite;
        SpriteRenderer2DPtr m_textureRenderer;

        ClickButton2DPtr m_selectAssetButton;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerTexture2D_hpp_
//////////////////////////////////////////
