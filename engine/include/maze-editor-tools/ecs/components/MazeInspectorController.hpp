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
#if (!defined(_MazeInspectorController_hpp_))
#define _MazeInspectorController_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-editor-tools/inspectors/MazeInspector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(InspectorController);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(VerticalLayout2D);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(Inspector);


    //////////////////////////////////////////
    // Class InspectorController
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API InspectorController
        : public Component
        , public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InspectorController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(InspectorController);

        //////////////////////////////////////////
        friend class Entity;


        //////////////////////////////////////////
        using SetupEditorFunc = std::function<InspectorPtr(InspectorController*)>;

    public:

        //////////////////////////////////////////
        virtual ~InspectorController();

        //////////////////////////////////////////
        static InspectorControllerPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        template <typename TInspector>
        inline SharedPtr<TInspector> setupEditor()
        {
            if (!m_inspector || m_inspector->getMetaClass() != TInspector::GetMetaClass())
            {
                clearEditor();

                m_inspector = TInspector::Create(m_layout->getTransform());
            }

            return std::static_pointer_cast<TInspector>(m_inspector);
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline SharedPtr<TInspector> setupEditor(Set<AssetFilePtr> const& _assetFiles)
        {
            SharedPtr<TInspector> inspector = setupEditor<TInspector>();
            if (inspector)
            {
                if (inspector->setAssetFiles(_assetFiles))
                    m_layout->alignChildren();
            }
            return inspector;
        }

        //////////////////////////////////////////
        void clearEditor();


        //////////////////////////////////////////
        template <typename TInspector>
        static InspectorPtr ProcessSetupInspector(InspectorController* _controller)
        {
            return std::static_pointer_cast<Inspector>(_controller->setupEditor<TInspector>());
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline void registerInspectorByExtension(HashedCString _extension)
        {
            m_editorByExtension[_extension] = ProcessSetupInspector<TInspector>;
        }

        //////////////////////////////////////////
        template <typename TInspector>
        inline void registerInspectorByClassUID(ClassUID _objectUID)
        {
            m_editorByClassUID[_objectUID] = ProcessSetupInspector<TInspector>;
        }

        //////////////////////////////////////////
        template <typename TInspector, typename TObject>
        inline void registerInspectorByClassUID()
        {
            registerInspectorByClassUID<TInspector>(ClassInfo<TObject>::UID());
        }

    protected:

        //////////////////////////////////////////
        InspectorController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void notifySelectionChanged();


        //////////////////////////////////////////
        void notifyAssetFileRemoved(AssetFilePtr const& _assetFile);
        

        //////////////////////////////////////////
        void updateEditors();


        //////////////////////////////////////////
        void notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _loader);

    protected:
        bool m_editorsDirty = false;

        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;

        VerticalLayout2DPtr m_layout;

        InspectorPtr m_inspector;

        StringKeyMap<SetupEditorFunc> m_editorByExtension;
        Map<ClassUID, SetupEditorFunc> m_editorByClassUID;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInspectorController_hpp_
//////////////////////////////////////////
