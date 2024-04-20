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
#if (!defined(_MazeUISystem2D_hpp_))
#define _MazeUISystem2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(UISystem2D);
    MAZE_USING_SHARED_PTR(TextRenderer2D);
    MAZE_USING_SHARED_PTR(RenderControlSystemModule2D);


    //////////////////////////////////////////
    // Class UISystem2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API UISystem2D
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UISystem2D, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UISystem2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~UISystem2D();

        //////////////////////////////////////////
        static UISystem2DPtr Create();



        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 49000; }

    protected:

        //////////////////////////////////////////
        UISystem2D();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processSystemRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(UpdateEvent const* _event) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void nofifyRenderControlSystemModule2DPostUpdate(F32 _dt);

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<TextRenderer2D>> m_textRenderers2DSample;
        
        RenderControlSystemModule2DPtr m_module2D;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUISystem2D_hpp_
//////////////////////////////////////////
