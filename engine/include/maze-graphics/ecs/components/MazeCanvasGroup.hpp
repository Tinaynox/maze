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
#if (!defined(_MazeCanvasGroup_hpp_))
#define _MazeCanvasGroup_hpp_


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
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(CanvasGroup);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    // Class CanvasGroup
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API CanvasGroup
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CanvasGroup, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(CanvasGroup);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            WorldAlphaDirty = MAZE_BIT(0),
            LocalAlphaChangedEvent = MAZE_BIT(3),
            WorldAlphaChangedEvent = MAZE_BIT(4),
        };

    public:

        //////////////////////////////////////////
        virtual ~CanvasGroup();

        //////////////////////////////////////////
        static CanvasGroupPtr Create();


        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        void setLocalAlpha(F32 _alpha);

        //////////////////////////////////////////
        inline F32 getLocalAlpha() const { return m_localAlpha; }

        //////////////////////////////////////////
        F32 getWorldAlpha();


        //////////////////////////////////////////
        inline bool isLocalAlphaChanged() const { return m_flags & Flags::LocalAlphaChangedEvent; }

        //////////////////////////////////////////
        inline bool isWorldAlphaChanged() const { return m_flags & Flags::WorldAlphaChangedEvent; }

        //////////////////////////////////////////
        inline void clearEventFlags()
        {
            m_flags &= ~(
                LocalAlphaChangedEvent | 
                WorldAlphaChangedEvent);
        }

        //////////////////////////////////////////
        inline bool getBlocksRaycasts() const { return m_blocksRaycasts; }

        //////////////////////////////////////////
        void setBlocksRaycasts(bool _value);

    protected:

        //////////////////////////////////////////
        CanvasGroup();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void dirtyWorldAlphaRecursive();

        //////////////////////////////////////////
        void _dirtyWorldAlphaRecursive(Transform2D* _transform2D);

        //////////////////////////////////////////
        F32 calculateWorldAlpha();

        //////////////////////////////////////////
        CanvasGroup* getParentCanvasGroup();

    protected:
        F32 m_localAlpha;
        F32 m_worldAlpha;

        bool m_blocksRaycasts;

        S32 m_flags;

        Transform2DPtr m_transform;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCanvasGroup_hpp_
//////////////////////////////////////////
