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
#if (!defined(_MazeCanvasRenderer_hpp_))
#define _MazeCanvasRenderer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(CanvasRenderer);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(CanvasGroup);


    //////////////////////////////////////////
    // Class CanvasRenderer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API CanvasRenderer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CanvasRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(CanvasRenderer);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            AlphaDirty = MAZE_BIT(0),
            AlphaChanged = MAZE_BIT(1),
        };

    public:

        //////////////////////////////////////////
        virtual ~CanvasRenderer();

        //////////////////////////////////////////
        static CanvasRendererPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        F32 getAlpha();


        //////////////////////////////////////////
        inline void dirtyAlpha() { m_flags |= AlphaDirty; }

        //////////////////////////////////////////
        inline bool isAlphaDirty() const { return m_flags & AlphaDirty; }

        //////////////////////////////////////////
        inline bool isAlphaChanged() const { return m_flags & AlphaChanged; }

        //////////////////////////////////////////
        inline void clearEventFlags()
        {
            m_flags &= ~(
                AlphaDirty | 
                AlphaChanged);
        }


    protected:

        //////////////////////////////////////////
        CanvasRenderer();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void setAlpha(F32 _alpha);

        //////////////////////////////////////////
        CanvasGroup* getParentCanvasGroup();

        //////////////////////////////////////////
        F32 calculateAlpha();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        F32 m_alpha;
        S32 m_flags;

        Transform2DPtr m_transform;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCanvasRenderer_hpp_
//////////////////////////////////////////
