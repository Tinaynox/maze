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
#if (!defined(_MazeDragAndDropZone_hpp_))
#define _MazeDragAndDropZone_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class DragAndDropZone
    //
    //////////////////////////////////////////
    class MAZE_UI_API DragAndDropZone
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DragAndDropZone, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(DragAndDropZone);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~DragAndDropZone();

        //////////////////////////////////////////
        static DragAndDropZonePtr Create();


        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_element; }

    protected:

        //////////////////////////////////////////
        DragAndDropZone();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();
        
        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        UIElement2DPtr m_element;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDragAndDropZone_hpp_
//////////////////////////////////////////
