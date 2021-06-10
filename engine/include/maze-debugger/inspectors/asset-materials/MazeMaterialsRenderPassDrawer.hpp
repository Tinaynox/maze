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
#if (!defined(_MazeMaterialsRenderPassDrawer_hpp_))
#define _MazeMaterialsRenderPassDrawer_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-debugger/inspectors/MazeInspector.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialsRenderPassDrawer);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    

    //////////////////////////////////////////
    // Class MaterialsRenderPassDrawer
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API MaterialsRenderPassDrawer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MaterialsRenderPassDrawer, Inspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MaterialsRenderPassDrawer);

    public:

        //////////////////////////////////////////
        virtual ~MaterialsRenderPassDrawer();

        //////////////////////////////////////////
        static MaterialsRenderPassDrawerPtr Create(RenderPassType _renderPassType, S32 _renderPassIndex);


        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        void setMaterials(Set<MaterialPtr> const& _materials);

        //////////////////////////////////////////
        void updateValues();

    protected:

        //////////////////////////////////////////
        MaterialsRenderPassDrawer();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        virtual bool init(RenderPassType _renderPassType, S32 _renderPassIndex);


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void setShader(ShaderPtr const& _shader);

    protected:
        Transform2DPtr m_transform;

        RenderPassType m_renderPassType;
        S32 m_renderPassIndex;

        Transform2DPtr m_topBlock;
        SystemTextRenderer2DPtr m_topBlockShaderNameText;
        SystemTextRenderer2DPtr m_topBlockRenderQueueIndexText;

        Set<MaterialPtr> m_materials;

        Vector<MetaPropertyDrawerPtr> m_propertyDrawers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialsRenderPassDrawer_hpp_
//////////////////////////////////////////
