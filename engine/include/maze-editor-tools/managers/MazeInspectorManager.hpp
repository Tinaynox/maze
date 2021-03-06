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
#if (!defined(_MazeInspectorManager_hpp_))
#define _MazeInspectorManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-editor-tools/property-drawers/MazeMaterial.hpp"
#include "maze-editor-tools/property-drawers/MazeRenderMesh.hpp"
#include "maze-editor-tools/property-drawers/MazeRadians.hpp"
#include "maze-editor-tools/property-drawers/MazeVec2DU.hpp"
#include "maze-editor-tools/property-drawers/MazeTexture2D.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeDefault.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InspectorManager);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(PropertyDrawer);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ComponentEditor);


    //////////////////////////////////////////
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<MaterialPtr, PropertyDrawerMaterial>;
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<RenderMeshPtr, PropertyDrawerRenderMesh>;
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<F32, PropertyDrawerRadians>;
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<Vec2DS, PropertyDrawerVec2DU>;
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<Vec2DU, PropertyDrawerVec2DU>;
    template class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerDefault<Texture2DPtr, PropertyDrawerTexture2D>;


    //////////////////////////////////////////
    // Class InspectorManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API InspectorManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~InspectorManager();

        //////////////////////////////////////////
        static void Initialize(InspectorManagerPtr& _inspectorManager);


        //////////////////////////////////////////
        static inline InspectorManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline InspectorManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        template <typename TComponent, typename TComponentEditor>
        inline void registerComponentEditor()
        {
            ClassUID componentUID = ClassInfo<TComponent>::UID();
            registerComponentEditor(
                componentUID,
                []()
                {
                    return TComponentEditor::Create();
                });
        }

        //////////////////////////////////////////
        inline void registerComponentEditor(ClassUID _componentUID, std::function<ComponentEditorPtr()> _func)
        {
            m_componentEditors.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_componentUID),
                std::forward_as_tuple(_func));
        }

        //////////////////////////////////////////
        template <typename TProperty, typename TPropertyDrawer>
        inline void registerPropertyDrawer()
        {
            ClassUID propertyUID = ClassInfo<TProperty>::UID();
            registerPropertyDrawer(
                propertyUID,
                [](String const& _label)
                {
                    return TPropertyDrawer::Create(_label);
                });
        }

        //////////////////////////////////////////
        inline void registerPropertyDrawer(ClassUID _propertyUID, std::function<PropertyDrawerPtr(String const&)> _func)
        {
            m_propertyDrawers.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_propertyUID),
                std::forward_as_tuple(_func));
        }

        //////////////////////////////////////////
        template <typename TProperty, typename TPropertyDrawer>
        inline void registerMetaPropertyDrawer()
        {
            ClassUID propertyUID = ClassInfo<TProperty>::UID();
            registerMetaPropertyDrawer(
                propertyUID,
                [](MetaProperty* _metaProperty)
                {
                    return TPropertyDrawer::Create(_metaProperty);
                });
        }

        //////////////////////////////////////////
        inline void registerMetaPropertyDrawer(ClassUID _propertyUID, std::function<MetaPropertyDrawerPtr(MetaProperty*)> _func)
        {
            m_metaPropertyDrawers.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_propertyUID),
                std::forward_as_tuple(_func));
        }

        //////////////////////////////////////////
        ComponentEditorPtr createComponentEditor(MetaClass* _metaClass);

        //////////////////////////////////////////
        PropertyDrawerPtr createPropertyDrawer(ClassUID _propertyClassUID, String const& _label);

        //////////////////////////////////////////
        MetaPropertyDrawerPtr createMetaPropertyDrawer(MetaProperty* _metaProperty);

        //////////////////////////////////////////
        void addComponentContextMenuOption(
            ClassUID _componentUID,
            String const& _title,
            std::function<void(Entity*, Component*)> _callback);

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOption(
            String const& _title,
            std::function<void(Entity*, TComponent*)> _callback)
        {
            this->addComponentContextMenuOption(
                ClassInfo<TComponent>::UID(),
                _title,
                [_callback](Entity* _entity, Component* _component)
                {
                    _callback(_entity, _component->castRaw<TComponent>());
                });
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1)
        {
            this->addComponentContextMenuOption<TComponent>(_title0, _callback0);
            this->addComponentContextMenuOption<TComponent>(_title1, _callback1);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2)
        {
            this->addComponentContextMenuOption<TComponent>(_title0, _callback0);
            this->addComponentContextMenuOption<TComponent>(_title1, _callback1);
            this->addComponentContextMenuOption<TComponent>(_title2, _callback2);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2,
            String const& _title3, std::function<void(Entity*, TComponent*)> _callback3)
        {
            this->addComponentContextMenuOptions<TComponent>(_title0, _callback0, _title1, _callback1);
            this->addComponentContextMenuOptions<TComponent>(_title2, _callback2, _title3, _callback3);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2,
            String const& _title3, std::function<void(Entity*, TComponent*)> _callback3,
            String const& _title4, std::function<void(Entity*, TComponent*)> _callback4)
        {
            this->addComponentContextMenuOptions<TComponent>(_title0, _callback0, _title1, _callback1);
            this->addComponentContextMenuOptions<TComponent>(_title2, _callback2, _title3, _callback3, _title4, _callback4);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2,
            String const& _title3, std::function<void(Entity*, TComponent*)> _callback3,
            String const& _title4, std::function<void(Entity*, TComponent*)> _callback4,
            String const& _title5, std::function<void(Entity*, TComponent*)> _callback5)
        {
            this->addComponentContextMenuOptions<TComponent>(_title0, _callback0, _title1, _callback1, _title2, _callback2);
            this->addComponentContextMenuOptions<TComponent>(_title3, _callback3, _title4, _callback4, _title5, _callback5);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2,
            String const& _title3, std::function<void(Entity*, TComponent*)> _callback3,
            String const& _title4, std::function<void(Entity*, TComponent*)> _callback4,
            String const& _title5, std::function<void(Entity*, TComponent*)> _callback5,
            String const& _title6, std::function<void(Entity*, TComponent*)> _callback6)
        {
            this->addComponentContextMenuOptions<TComponent>(_title0, _callback0, _title1, _callback1);
            this->addComponentContextMenuOptions<TComponent>(_title2, _callback2, _title3, _callback3);
            this->addComponentContextMenuOptions<TComponent>(_title4, _callback4, _title5, _callback5, _title6, _callback6);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline void addComponentContextMenuOptions(
            String const& _title0, std::function<void(Entity*, TComponent*)> _callback0,
            String const& _title1, std::function<void(Entity*, TComponent*)> _callback1,
            String const& _title2, std::function<void(Entity*, TComponent*)> _callback2,
            String const& _title3, std::function<void(Entity*, TComponent*)> _callback3,
            String const& _title4, std::function<void(Entity*, TComponent*)> _callback4,
            String const& _title5, std::function<void(Entity*, TComponent*)> _callback5,
            String const& _title6, std::function<void(Entity*, TComponent*)> _callback6,
            String const& _title7, std::function<void(Entity*, TComponent*)> _callback7)
        {
            this->addComponentContextMenuOptions<TComponent>(_title0, _callback0, _title1, _callback1);
            this->addComponentContextMenuOptions<TComponent>(_title2, _callback2, _title3, _callback3);
            this->addComponentContextMenuOptions<TComponent>(_title4, _callback4, _title5, _callback5);
            this->addComponentContextMenuOptions<TComponent>(_title6, _callback6, _title7, _callback7);
        }

        //////////////////////////////////////////
        Vector<std::pair<String, std::function<void(Entity*, Component*)>>> const& getInspectorComponentContextMenuOptions(ClassUID _classUID) const;


        //////////////////////////////////////////
        inline bool getSaveAssetButtonEnabled() const { return m_saveAssetButtonEnabled; }

        //////////////////////////////////////////
        inline void setSaveAssetButtonEnabled(bool _value) { m_saveAssetButtonEnabled = _value; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventInspectorChanged;

    protected:

        //////////////////////////////////////////
        InspectorManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static InspectorManager* s_instance;

        Map<ClassUID, std::function<ComponentEditorPtr()>> m_componentEditors;
        Map<ClassUID, std::function<PropertyDrawerPtr(String const&)>> m_propertyDrawers;
        Map<ClassUID, std::function<MetaPropertyDrawerPtr(MetaProperty*)>> m_metaPropertyDrawers;
        Map<ClassUID, Vector<std::pair<String, std::function<void(Entity*, Component*)>>>> m_inspectorComponentContextMenuOptions;

        bool m_saveAssetButtonEnabled = true;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInspectorManager_hpp_
//////////////////////////////////////////
