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
#if (!defined(_MazePropertyDrawerRenderMesh_hpp_))
#define _MazePropertyDrawerRenderMesh_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-editor-tools/ecs/components/MazeColorEdit2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerRenderMesh);
    MAZE_USING_SHARED_PTR(PropertyDrawerRenderMeshAssetRef);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerRenderMesh
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerRenderMesh
        : public GenericPropertyDrawer<RenderMeshPtr>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerRenderMesh, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerRenderMesh);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerRenderMesh();

        //////////////////////////////////////////
        static PropertyDrawerRenderMeshPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(RenderMeshPtr const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderMeshPtr getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerRenderMesh();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyRenderMeshButtonClick(Button2D* _button, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event);

    protected:
        ClickButton2DPtr m_renderMeshButton;
        SpriteRenderer2DPtr m_renderMeshIcon;
        AbstractTextRenderer2DPtr m_renderMeshNameDrawer;
        RenderMeshPtr m_renderMesh;

        ClickButton2DPtr m_selectAssetButton;
    };


    //////////////////////////////////////////
    // Class PropertyDrawerRenderMeshAssetRef
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerRenderMeshAssetRef
        : public GenericPropertyDrawer<RenderMeshAssetRef>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerRenderMeshAssetRef, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerRenderMeshAssetRef);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerRenderMeshAssetRef();

        //////////////////////////////////////////
        static PropertyDrawerRenderMeshAssetRefPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(RenderMeshAssetRef const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderMeshAssetRef getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerRenderMeshAssetRef();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processDataFromUI();

    protected:
        PropertyDrawerRenderMeshPtr m_drawer;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerRenderMesh_hpp_
//////////////////////////////////////////
