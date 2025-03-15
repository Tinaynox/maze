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
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-editor-tools/scenes/SceneDebugPreview.hpp"
#include "maze-editor-tools/preview-inspectors/MazePreviewInspector.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
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
    class MAZE_EDITOR_TOOLS_API PreviewController
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


        //////////////////////////////////////////
        using SetupEditorFunc = std::function<PreviewInspectorPtr(PreviewController*)>;

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
        void processInit();

        //////////////////////////////////////////
        void processEntityRemoved();


        //////////////////////////////////////////
        template <typename TInspector>
        inline SharedPtr<TInspector> setupInspector()
        {
            if (!m_previewInspector || m_previewInspector->getMetaClass() != TInspector::GetMetaClass())
            {
                clearInspector();

                m_previewInspector = TInspector::Create(
                    m_layout->getTransform(),
                    m_scene.lock());

                if (m_previewInspector)
                {
                    setSceneVisibleSettings(
                        m_previewInspector->getCameraActive(),
                        m_previewInspector->getCanvasActive());
                }
            }

            return std::static_pointer_cast<TInspector>(m_previewInspector);
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline SharedPtr<TInspector> setupInspector(Set<AssetFilePtr> const& _assetFiles)
        {
            SharedPtr<TInspector> inspector = setupInspector<TInspector>();
            if (inspector)
            {
                if (inspector->setAssetFiles(_assetFiles))
                    m_layout->alignChildren();
            }
            return inspector;
        }

        //////////////////////////////////////////
        void clearInspector();

        //////////////////////////////////////////
        inline void dirtyInspectors() { m_inspectorsDirty = true; }


        //////////////////////////////////////////
        template <typename TInspector>
        static PreviewInspectorPtr ProcessSetupInspector(PreviewController* _controller)
        {
            return std::static_pointer_cast<PreviewInspector>(_controller->setupInspector<TInspector>());
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline void registerPreviewInspectorByExtension(HashedCString _extension)
        {
            m_editorByExtension[_extension] = ProcessSetupInspector<TInspector>;
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline void registerPreviewInspectorByClassUID(ClassUID _objectUID)
        {
            m_editorByClassUID[_objectUID] = ProcessSetupInspector<TInspector>;
        }

        //////////////////////////////////////////
        template <typename TInspector, typename TObject>
        inline void registerPreviewInspectorByClassUID()
        {
            registerPreviewInspectorByClassUID<TInspector>(ClassInfo<TObject>::UID());
        }

    protected:

        //////////////////////////////////////////
        PreviewController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);


        //////////////////////////////////////////
        void udpateInspectors();
                

        //////////////////////////////////////////
        void setSceneVisibleSettings(bool _camera, bool _canvas);

        //////////////////////////////////////////
        void notifySelectionChanged();


        //////////////////////////////////////////
        void notifyBodyBackgroundElementCursorPress(
            Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyBodyBackgroundElementCursorDrag(
            Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyBodyBackgroundElementCursorWheel(
            CursorWheelInputEvent const& _event);

        //////////////////////////////////////////
        void notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _loader);


        //////////////////////////////////////////
        void notifyAssetFileRemoved(AssetFilePtr const& _file);

    protected:
        bool m_inspectorsDirty = true;

        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpritePtr m_bodySprite;
        SpriteRenderer2DPtr m_bodyBackground;
        UIElement2DPtr m_bodyBackgroundElement;
        VerticalLayout2DPtr m_layout;

        SceneDebugPreviewWPtr m_scene;
        RenderBufferPtr m_renderBuffer;

        PreviewInspectorPtr m_previewInspector;

        StringKeyMap<SetupEditorFunc> m_editorByExtension;
        Map<ClassUID, SetupEditorFunc> m_editorByClassUID;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePreviewController_hpp_
//////////////////////////////////////////
