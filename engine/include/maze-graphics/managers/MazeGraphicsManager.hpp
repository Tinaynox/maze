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
#if (!defined(_MazeGraphicsManager_hpp_))
#define _MazeGraphicsManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GraphicsManager);
    MAZE_USING_SHARED_PTR(GizmosManager);
    MAZE_USING_SHARED_PTR(RenderSystem);


    //////////////////////////////////////////
    // Class GraphicsManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GraphicsManager 
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~GraphicsManager();

        //////////////////////////////////////////
        static void Initialize(GraphicsManagerPtr& _graphicsManager);


        //////////////////////////////////////////
        static inline GraphicsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline GraphicsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline StringKeyMap<RenderSystemPtr> const& getRenderSystems() const { return m_renderSystems; }

        //////////////////////////////////////////
        void addRenderSystem(RenderSystemPtr const& renderSystem);

        //////////////////////////////////////////
        void removeRenderSystem(RenderSystemPtr const& renderSystem);

        //////////////////////////////////////////
        inline RenderSystemPtr const& getDefaultRenderSystem() const { return m_defaultRenderSystem; }

        //////////////////////////////////////////
        inline RenderSystem* getDefaultRenderSystemRaw() const { return m_defaultRenderSystem.get(); }

        //////////////////////////////////////////
        void setDefaultRenderSystem(RenderSystemPtr const& renderSystem);

        //////////////////////////////////////////
        template <typename TRenderSystem>
        inline TRenderSystem* getRenderSystem()
        {
            for (auto const& renderSystemData : m_renderSystems)
            {
                if (renderSystemData.second->getMetaClass()->isInheritedFrom<TRenderSystem>())
                    return renderSystemData.second->castRaw<TRenderSystem>();
            }

            return nullptr;
        }

        //////////////////////////////////////////
        inline GizmosManagerPtr const& getGizmosManager() const { return m_gizmosManager; }

        
    public:

        //////////////////////////////////////////
        MultiDelegate<RenderSystemPtr const&> eventDefaultRenderSystemWillBeChanged;
        MultiDelegate<RenderSystemPtr const&> eventDefaultRenderSystemChanged;

    protected:

        //////////////////////////////////////////
        GraphicsManager();

        //////////////////////////////////////////
        virtual bool init();

        ////////////////////////////////////////// 
        virtual void update(F32 _dt) MAZE_OVERRIDE;
    
    protected:
        static GraphicsManager* s_instance;

        StringKeyMap<RenderSystemPtr> m_renderSystems;
        RenderSystemPtr m_defaultRenderSystem;

        GizmosManagerPtr m_gizmosManager;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGraphicsManager_hpp_
//////////////////////////////////////////
