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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-editor-tools/property-drawers/MazeBool.hpp"
#include "maze-editor-tools/property-drawers/MazeS32.hpp"
#include "maze-editor-tools/property-drawers/MazeF32.hpp"
#include "maze-editor-tools/property-drawers/MazeVec2DF.hpp"
#include "maze-editor-tools/property-drawers/MazeVec2DS.hpp"
#include "maze-editor-tools/property-drawers/MazeVec2DU.hpp"
#include "maze-editor-tools/property-drawers/MazeVec3DF.hpp"
#include "maze-editor-tools/property-drawers/MazeRect2DF.hpp"
#include "maze-editor-tools/property-drawers/MazeString.hpp"
#include "maze-editor-tools/property-drawers/MazeQuaternion.hpp"
#include "maze-editor-tools/property-drawers/MazeEnumClass.hpp"
#include "maze-editor-tools/property-drawers/MazeColorU32.hpp"
#include "maze-editor-tools/property-drawers/MazeColorF128.hpp"
#include "maze-editor-tools/property-drawers/MazeColorGradient.hpp"
#include "maze-editor-tools/property-drawers/MazeAnimationCurve.hpp"
#include "maze-editor-tools/property-drawers/MazeMaterial.hpp"
#include "maze-editor-tools/property-drawers/MazeRenderMesh.hpp"
#include "maze-editor-tools/property-drawers/MazeRadians.hpp"
#include "maze-editor-tools/property-drawers/MazeTexture2D.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeBool.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeS32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeF32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeVec2DF.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeVec3DF.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeRect2DF.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeString.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeQuaternion.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeEnumClass.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeVector.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeColorU32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeColorF128.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeColorGradient.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeAnimationCurve.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeDefault.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditorDefault.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditorTransform3D.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditorCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using MetaPropertyDrawerMaterial =
        MetaPropertyDrawerDefault<
            MaterialPtr,
            PropertyDrawerMaterial>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerMaterial, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerRenderMesh =
        MetaPropertyDrawerDefault<
            RenderMeshPtr,
            PropertyDrawerRenderMesh>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerRenderMesh, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerF32Radians =
        MetaPropertyDrawerDefault<
        F32,
        PropertyDrawerRadians>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerF32Radians, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerVec2DS =
        MetaPropertyDrawerDefault<
        Vec2DS,
        PropertyDrawerVec2DU>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerVec2DS, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerVec2DU =
        MetaPropertyDrawerDefault<
        Vec2DU,
        PropertyDrawerVec2DU>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerVec2DU, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerTexture2D =
        MetaPropertyDrawerDefault<
        Texture2DPtr,
        PropertyDrawerTexture2D>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerTexture2D, MetaPropertyDrawer);


    //////////////////////////////////////////
    // Class InspectorManager
    //
    //////////////////////////////////////////
    InspectorManager* InspectorManager::s_instance = nullptr;

    //////////////////////////////////////////
    InspectorManager::InspectorManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    InspectorManager::~InspectorManager()
    {
        
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void InspectorManager::Initialize(InspectorManagerPtr& _inspectorManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(InspectorManager, _inspectorManager, init());
    }

    //////////////////////////////////////////
    bool InspectorManager::init()
    {
        registerPropertyDrawer<bool, PropertyDrawerBool>();
        registerPropertyDrawer<S32, PropertyDrawerS32>();
        registerPropertyDrawer<F32, PropertyDrawerF32>();
        registerPropertyDrawer<Vec2DF, PropertyDrawerVec2DF>();
        registerPropertyDrawer<Vec2DS, PropertyDrawerVec2DS>();
        registerPropertyDrawer<Vec2DU, PropertyDrawerVec2DU>();
        registerPropertyDrawer<Vec3DF, PropertyDrawerVec3DF>();
        registerPropertyDrawer<Rect2DF, PropertyDrawerRect2DF>();
        registerPropertyDrawer<String, PropertyDrawerString>();
        registerPropertyDrawer<Quaternion, PropertyDrawerQuaternion>();
        registerPropertyDrawer<ColorU32, PropertyDrawerColorU32>();
        registerPropertyDrawer<ColorF128, PropertyDrawerColorF128>();
        registerPropertyDrawer<ColorGradient, PropertyDrawerColorGradient>();
        registerPropertyDrawer<AnimationCurve, PropertyDrawerAnimationCurve>();
        registerPropertyDrawer<MaterialPtr, PropertyDrawerMaterial>();
        registerPropertyDrawer<RenderMeshPtr, PropertyDrawerRenderMesh>();
        registerPropertyDrawer<Texture2DPtr, PropertyDrawerTexture2D>();

        registerMetaPropertyDrawer<bool, MetaPropertyDrawerBool>();
        registerMetaPropertyDrawer<S32, MetaPropertyDrawerS32>();
        registerMetaPropertyDrawer<F32, MetaPropertyDrawerF32>();
        registerMetaPropertyDrawer<Vec2DF, MetaPropertyDrawerVec2DF>();
        registerMetaPropertyDrawer<Vec2DS, MetaPropertyDrawerVec2DS>();
        registerMetaPropertyDrawer<Vec2DU, MetaPropertyDrawerVec2DU>();
        registerMetaPropertyDrawer<Vec3DF, MetaPropertyDrawerVec3DF>();
        registerMetaPropertyDrawer<Rect2DF, MetaPropertyDrawerRect2DF>();
        registerMetaPropertyDrawer<String, MetaPropertyDrawerString>();
        registerMetaPropertyDrawer<Quaternion, MetaPropertyDrawerQuaternion>();
        registerMetaPropertyDrawer<ColorU32, MetaPropertyDrawerColorU32>();
        registerMetaPropertyDrawer<ColorF128, MetaPropertyDrawerColorF128>();
        registerMetaPropertyDrawer<ColorGradient, MetaPropertyDrawerColorGradient>();
        registerMetaPropertyDrawer<AnimationCurve, MetaPropertyDrawerAnimationCurve>();
        registerMetaPropertyDrawer<MaterialPtr, MetaPropertyDrawerMaterial>();
        registerMetaPropertyDrawer<RenderMeshPtr, MetaPropertyDrawerRenderMesh>();
        registerMetaPropertyDrawer<Texture2DPtr, MetaPropertyDrawerTexture2D>();

        registerComponentEditor<Transform3D, ComponentEditorTransform3D>();
        registerComponentEditor<Camera3D, ComponentEditorCamera3D>();


        addComponentContextMenuOption<Transform3D>(
            "Reset Transform",
            [](Entity* _entity, Transform3D* _component) { _component->resetTransform(); });

        addComponentContextMenuOption<Transform3D>(
            "Copy Position",
            [](Entity* _entity, Transform3D* _component)
            {
                SystemManager::GetInstancePtr()->setClipboardString(_component->getLocalPosition().toString());
            });
        addComponentContextMenuOption<Transform3D>(
            "Paste Position",
            [](Entity* _entity, Transform3D* _component)
            {
                _component->setLocalPosition(Vec3DF::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
            });
        addComponentContextMenuOption<Transform3D>(
            "Copy Rotation",
            [](Entity* _entity, Transform3D* _component)
            {
                SystemManager::GetInstancePtr()->setClipboardString(_component->getLocalRotation().toString());
            });
        addComponentContextMenuOption<Transform3D>(
            "Paste Rotation",
            [](Entity* _entity, Transform3D* _component)
            {
                _component->setLocalRotation(Quaternion::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
            });
        addComponentContextMenuOption<Transform3D>(
            "Copy Scale",
            [](Entity* _entity, Transform3D* _component)
            {
                SystemManager::GetInstancePtr()->setClipboardString(_component->getLocalScale().toString());
            });
        addComponentContextMenuOption<Transform3D>(
            "Paste Scale",
            [](Entity* _entity, Transform3D* _component)
            {
                _component->setLocalScale(Vec3DF::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
            });
        addComponentContextMenuOption<Transform3D>(
            "Copy Transform",
            [](Entity* _entity, Transform3D* _component)
            {
                SystemManager::GetInstancePtr()->setClipboardString(_component->getLocalTransform().toString());
            });
        addComponentContextMenuOption<Transform3D>(
            "Paste Transform",
            [](Entity* _entity, Transform3D* _component)
            {
                _component->setLocalTransform(Mat4DF::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
            });

        return true;
    }

    //////////////////////////////////////////
    void InspectorManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    ComponentEditorPtr InspectorManager::createComponentEditor(MetaClass* _metaClass)
    {
        auto it = m_componentEditors.find(_metaClass->getClassUID());
        if (it == m_componentEditors.end())
        {
            return ComponentEditorDefault::Create(_metaClass);
        }
        else
        {
            return it->second();
        }
    }

    //////////////////////////////////////////
    PropertyDrawerPtr InspectorManager::createPropertyDrawer(ClassUID _propertyClassUID, String const& _label)
    {
        auto it = m_propertyDrawers.find(_propertyClassUID);
        if (it == m_propertyDrawers.end())
        {
            return nullptr;
        }
        else
        {
            return it->second(_label);
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerPtr InspectorManager::createMetaPropertyDrawer(MetaProperty* _metaProperty)
    {
        // EnumClass
        if (_metaProperty->isEnumClass())
        {
            return MetaPropertyDrawerEnumClass::Create(_metaProperty, _metaProperty->getEnumClassValues());
        }
        else
        // Vector
        if (_metaProperty->isVector())
        {
            return MetaPropertyDrawerVector::Create(_metaProperty);
        }

        ClassUID metaPropertyClassUID = _metaProperty->getValueClassUID();

        auto it = m_metaPropertyDrawers.find(metaPropertyClassUID);
        if (it == m_metaPropertyDrawers.end())
        {
            return nullptr;
        }
        else
        {
            return it->second(_metaProperty);
        }
    }

    //////////////////////////////////////////
    void InspectorManager::addComponentContextMenuOption(
        ClassUID _componentUID,
        String const& _title,
        std::function<void(Entity*, Component*)> _callback)
    {
        m_inspectorComponentContextMenuOptions[_componentUID].emplace_back(std::make_pair(_title, _callback));
    }

    //////////////////////////////////////////
    Vector<std::pair<String, std::function<void(Entity*, Component*)>>> const& InspectorManager::getInspectorComponentContextMenuOptions(ClassUID _classUID) const
    {
        static Vector<std::pair<String, std::function<void(Entity*, Component*)>>> const nullValue;

        auto it = m_inspectorComponentContextMenuOptions.find(_classUID);
        if (it == m_inspectorComponentContextMenuOptions.end())
            return nullValue;

        return it->second;
    }
    
} // namespace Maze
//////////////////////////////////////////
