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
#if (!defined(_MazeGizmosManager_hpp_))
#define _MazeGizmosManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmosManager);
    MAZE_USING_SHARED_PTR(GizmoToolsManager);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(Texture2D);


    //////////////////////////////////////////
    enum class DefaultGizmosSprite
    {
        LightGizmo = 0,
        CameraGizmo,

        MAX
    };


    //////////////////////////////////////////
    // Class GizmosManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmosManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~GizmosManager();

        //////////////////////////////////////////
        static void Initialize(GizmosManagerPtr& _gizmosManager);


        //////////////////////////////////////////
        static inline GizmosManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline GizmosManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        template <typename TComponentClass, typename TComponentGizmosClass>
        inline void registerGizmos()
        {
            registerGizmos<TComponentClass>(TComponentGizmosClass::Create());
        }

        //////////////////////////////////////////
        template <typename TComponentClass>
        inline void registerGizmos(ComponentGizmosPtr const& _gizmos)
        {
            registerGizmos(ClassInfo<TComponentClass>::UID(), _gizmos);
        }

        //////////////////////////////////////////
        inline void registerGizmos(ClassUID _componentClassUID, ComponentGizmosPtr const& _gizmos)
        {
            m_gizmosPerComponentClass.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_componentClassUID),
                std::forward_as_tuple(_gizmos));

            eventGizmosPerComponentClassChanged();
        }

        //////////////////////////////////////////
        UnorderedMap<ClassUID, ComponentGizmosPtr> const& getGizmosPerComponentClass() const
        {
            return m_gizmosPerComponentClass;
        }


        //////////////////////////////////////////
        inline Camera3DPtr const& getCamera() const { return m_camera; }

        //////////////////////////////////////////
        void setCamera(Camera3DPtr const& _camera) { m_camera = _camera; }


        //////////////////////////////////////////
        inline SpritePtr const& getDefaultGizmosSprite(DefaultGizmosSprite uiSprite) const { return m_defaultGizmosSprites[(Size)uiSprite]; }

        //////////////////////////////////////////
        void createGizmosElements();

    public:
        //////////////////////////////////////////
        MultiDelegate<> eventGizmosPerComponentClassChanged;

    protected:

        //////////////////////////////////////////
        GizmosManager();

        //////////////////////////////////////////
        virtual bool init();

        ////////////////////////////////////////// 
        virtual void update(F32 _dt) MAZE_OVERRIDE;
    

    protected:
        static GizmosManager* s_instance;

        GizmoToolsManagerPtr m_gizmoToolsManager;

        UnorderedMap<ClassUID, ComponentGizmosPtr> m_gizmosPerComponentClass;

        Camera3DPtr m_camera;

        Texture2DPtr m_gizmosElementsTexture;
        SpritePtr m_defaultGizmosSprites[(Size)DefaultGizmosSprite::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmosManager_hpp_
//////////////////////////////////////////
