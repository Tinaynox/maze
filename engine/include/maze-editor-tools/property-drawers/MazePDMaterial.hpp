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
#if (!defined(_MazePropertyDrawerMaterial_hpp_))
#define _MazePropertyDrawerMaterial_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerMaterial);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(PropertyDrawerMaterialAssetRef);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerMaterial
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerMaterial
        : public GenericPropertyDrawer<MaterialPtr>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerMaterial, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerMaterial);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerMaterial();

        //////////////////////////////////////////
        static PropertyDrawerMaterialPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(MaterialPtr const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MaterialPtr getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerMaterial();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMaterialButtonClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event);

    protected:
        ClickButton2DPtr m_materialButton;
        SpriteRenderer2DPtr m_dragAndDropFrame;
        DragAndDropZonePtr m_dragAndDropZone;
        SpriteRenderer2DPtr m_materialIcon;
        AbstractTextRenderer2DPtr m_materialNameDrawer;
        MaterialPtr m_material;

        ClickButton2DPtr m_selectAssetButton;
    };


    //////////////////////////////////////////
    // Class PropertyDrawerMaterialAssetRef
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerMaterialAssetRef
        : public GenericPropertyDrawer<MaterialAssetRef>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerMaterialAssetRef, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerMaterialAssetRef);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerMaterialAssetRef();

        //////////////////////////////////////////
        static PropertyDrawerMaterialAssetRefPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(MaterialAssetRef const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MaterialAssetRef getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerMaterialAssetRef();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processDataFromUI();

    protected:
        PropertyDrawerMaterialPtr m_drawer;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerMaterial_hpp_
//////////////////////////////////////////
