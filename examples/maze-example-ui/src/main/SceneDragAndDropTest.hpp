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
#if (!defined(_SceneDragAndDropTest_hpp_))
#define _SceneDragAndDropTest_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDragAndDropTest);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(TextRenderer2D);
    MAZE_USING_SHARED_PTR(InventorySlot);
    MAZE_USING_SHARED_PTR(InventoryItem);
    

    //////////////////////////////////////////
    // Class SceneDragAndDropTest
    //
    //////////////////////////////////////////
    class SceneDragAndDropTest
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDragAndDropTest, EcsRenderScene);

    public:

        //////////////////////////////////////////
        static SceneDragAndDropTestPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneDragAndDropTest();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneDragAndDropTest();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();


        //////////////////////////////////////////
        InventorySlotPtr createSlot(Vec2F const& _pos);

        //////////////////////////////////////////
        InventoryItemPtr createItem();

    protected:        
        CanvasPtr m_canvas;

        SystemTextRenderer2DPtr m_systemText;
        TextRenderer2DPtr m_text;

        InventoryItemPtr m_item;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDragAndDropTest_hpp_
//////////////////////////////////////////
