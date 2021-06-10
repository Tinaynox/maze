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
#if (!defined(_MazePreviewController_hpp_))
#define _MazePreviewController_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-debugger/scenes/SceneDebugPreview.hpp"
#include "maze-debugger/preview-inspectors/MazePreviewInspector.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(PreviewController);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(PreviewInspector);


    //////////////////////////////////////////
    // Class PreviewController
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API PreviewController
        : public Component
        , public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PreviewController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PreviewController);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~PreviewController();

        //////////////////////////////////////////
        static PreviewControllerPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        template <typename TInspector>
        inline SharedPtr<TInspector> setupEditor()
        {
            if (!m_previewInspector || m_previewInspector->getMetaClass() != TInspector::GetMetaClass())
            {
                clearEditor();

                m_previewInspector = TInspector::Create(
                    m_layout->getTransform(),
                    m_scene);

                if (m_previewInspector)
                {
                    set3DSceneVisible(m_previewInspector->get3DSceneVisible());
                }
            }

            return std::static_pointer_cast<TInspector>(m_previewInspector);
        }

        //////////////////////////////////////////
        void clearEditor();

    protected:

        //////////////////////////////////////////
        PreviewController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void processEntityAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityRemoved() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void set3DSceneVisible(bool _value);

        //////////////////////////////////////////
        void notifySelectionChanged();


        //////////////////////////////////////////
        void notifyBodyBackgroundElementCursorPress(
            Vec2DF const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyBodyBackgroundElementCursorDrag(
            Vec2DF const& _positionOS, CursorInputEvent const& _event);

    protected:
        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpritePtr m_bodySprite;
        SpriteRenderer2DPtr m_bodyBackground;
        UIElement2DPtr m_bodyBackgroundElement;
        VerticalLayout2DPtr m_layout;

        SceneDebugPreviewPtr m_scene;
        RenderBufferPtr m_renderBuffer;

        PreviewInspectorPtr m_previewInspector;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePreviewController_hpp_
//////////////////////////////////////////
