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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-editor-tools/property-drawers/MazePDBool.hpp"
#include "maze-editor-tools/property-drawers/MazePDS32.hpp"
#include "maze-editor-tools/property-drawers/MazePDF32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2S32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2U32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec3F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDRect2F.hpp"
#include "maze-editor-tools/property-drawers/MazePDString.hpp"
#include "maze-editor-tools/property-drawers/MazePDQuaternion.hpp"
#include "maze-editor-tools/property-drawers/MazePDEnumClass.hpp"
#include "maze-editor-tools/property-drawers/MazePDColorU32.hpp"
#include "maze-editor-tools/property-drawers/MazePDColorF128.hpp"
#include "maze-editor-tools/property-drawers/MazePDColorGradient.hpp"
#include "maze-editor-tools/property-drawers/MazePDAnimationCurve.hpp"
#include "maze-editor-tools/property-drawers/MazePDMaterial.hpp"
#include "maze-editor-tools/property-drawers/MazePDRenderMesh.hpp"
#include "maze-editor-tools/property-drawers/MazePDRadians.hpp"
#include "maze-editor-tools/property-drawers/MazePDTexture2D.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDBool.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDS32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDF32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDVec2F32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDVec3F32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDRect2F.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDString.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDQuaternion.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDEnumClass.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDVector.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDColorU32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDColorF128.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDColorGradient.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDAnimationCurve.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDDefault.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"
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
    using MetaPropertyDrawerMaterialAssetRef =
        MetaPropertyDrawerDefault<
        MaterialAssetRef,
        PropertyDrawerMaterialAssetRef>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerMaterialAssetRef, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerRenderMesh =
        MetaPropertyDrawerDefault<
            RenderMeshPtr,
            PropertyDrawerRenderMesh>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerRenderMesh, MetaPropertyDrawer);

    //////////////////////////////////////////
    using MetaPropertyDrawerRenderMeshAssetRef =
        MetaPropertyDrawerDefault<
        RenderMeshAssetRef,
        PropertyDrawerRenderMeshAssetRef>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerRenderMeshAssetRef, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerF32Radians =
        MetaPropertyDrawerDefault<
        F32,
        PropertyDrawerRadians>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerF32Radians, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerVec2S32 =
        MetaPropertyDrawerDefault<
        Vec2S,
        PropertyDrawerVec2U32>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerVec2S32, MetaPropertyDrawer);


    //////////////////////////////////////////
    using MetaPropertyDrawerVec2U32 =
        MetaPropertyDrawerDefault<
        Vec2U,
        PropertyDrawerVec2U32>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerVec2U32, MetaPropertyDrawer);


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
        registerPropertyDrawer<U32, PropertyDrawerS32>();
        registerPropertyDrawer<F32, PropertyDrawerF32>();
        registerPropertyDrawer<Vec2F, PropertyDrawerVec2F32>();
        registerPropertyDrawer<Vec2S, PropertyDrawerVec2S32>();
        registerPropertyDrawer<Vec2U, PropertyDrawerVec2U32>();
        registerPropertyDrawer<Vec3F, PropertyDrawerVec3F32>();
        registerPropertyDrawer<Rect2F, PropertyDrawerRect2F>();
        registerPropertyDrawer<String, PropertyDrawerString>();
        registerPropertyDrawer<Quaternion, PropertyDrawerQuaternion>();
        registerPropertyDrawer<ColorU32, PropertyDrawerColorU32>();
        registerPropertyDrawer<ColorF128, PropertyDrawerColorF128>();
        registerPropertyDrawer<ColorGradient, PropertyDrawerColorGradient>();
        registerPropertyDrawer<AnimationCurve, PropertyDrawerAnimationCurve>();
        registerPropertyDrawer<MaterialPtr, PropertyDrawerMaterial>();
        registerPropertyDrawer<MaterialAssetRef, PropertyDrawerMaterialAssetRef>();
        registerPropertyDrawer<RenderMeshPtr, PropertyDrawerRenderMesh>();
        registerPropertyDrawer<RenderMeshAssetRef, PropertyDrawerRenderMeshAssetRef>();
        registerPropertyDrawer<Texture2DPtr, PropertyDrawerTexture2D>();

        registerMetaPropertyDrawer<bool, MetaPropertyDrawerBool>();
        registerMetaPropertyDrawer<S32, MetaPropertyDrawerS32>();
        registerMetaPropertyDrawer<U32, MetaPropertyDrawerS32>();
        registerMetaPropertyDrawer<F32, MetaPropertyDrawerF32>();
        registerMetaPropertyDrawer<Vec2F, MetaPropertyDrawerVec2F32>();
        registerMetaPropertyDrawer<Vec2S, MetaPropertyDrawerVec2S32>();
        registerMetaPropertyDrawer<Vec2U, MetaPropertyDrawerVec2U32>();
        registerMetaPropertyDrawer<Vec3F, MetaPropertyDrawerVec3F32>();
        registerMetaPropertyDrawer<Rect2F, MetaPropertyDrawerRect2F>();
        registerMetaPropertyDrawer<String, MetaPropertyDrawerString>();
        registerMetaPropertyDrawer<Quaternion, MetaPropertyDrawerQuaternion>();
        registerMetaPropertyDrawer<ColorU32, MetaPropertyDrawerColorU32>();
        registerMetaPropertyDrawer<ColorF128, MetaPropertyDrawerColorF128>();
        registerMetaPropertyDrawer<ColorGradient, MetaPropertyDrawerColorGradient>();
        registerMetaPropertyDrawer<AnimationCurve, MetaPropertyDrawerAnimationCurve>();
        registerMetaPropertyDrawer<MaterialPtr, MetaPropertyDrawerMaterial>();
        registerMetaPropertyDrawer<MaterialAssetRef, MetaPropertyDrawerMaterialAssetRef>();
        registerMetaPropertyDrawer<RenderMeshPtr, MetaPropertyDrawerRenderMesh>();
        registerMetaPropertyDrawer<RenderMeshAssetRef, MetaPropertyDrawerRenderMeshAssetRef>();
        registerMetaPropertyDrawer<Texture2DPtr, MetaPropertyDrawerTexture2D>();

        registerComponentEditor<Transform3D, ComponentEditorTransform3D>();
        registerComponentEditor<Camera3D, ComponentEditorCamera3D>();


        addComponentContextMenuOption<Transform3D>(
            "Reset Transform",
            [](Entity* _entity, Transform3D* _component)
            {
                if (EditorToolsActionManager::GetInstancePtr())
                {
                    Transform3DPtr component = _component->cast<Transform3D>();
                    EditorToolsActionManager::GetInstancePtr()->applyAction(
                        EditorActionCustom::Create(
                            [component]() { component->resetTransform(); },
                            [component, oldValue = component->getLocalTransform()]() { component->setLocalTransform(oldValue); }));
                }
                else
                    _component->resetTransform();
            });

        addComponentContextMenuOption<Transform3D>(
            "Reset Rotation and Scale",
            [](Entity* _entity, Transform3D* _component)
            {
                TMat newValue = TMat::CreateTranslation(_component->getLocalPosition());
                if (EditorToolsActionManager::GetInstancePtr())
                {
                    Transform3DPtr component = _component->cast<Transform3D>();
                    EditorToolsActionManager::GetInstancePtr()->applyAction(
                        EditorActionCustom::Create(
                            [component]() { component->resetTransform(); },
                            [component, oldValue = component->getLocalTransform()]() { component->setLocalTransform(oldValue); }));
                }
                else
                    _component->resetTransform();
            });

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
                EditorActionHelper::Translate(
                    _entity->getSharedPtr(),
                    Vec3F::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
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
                EditorActionHelper::Rotate(
                    _entity->getSharedPtr(),
                    Quaternion::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
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
                EditorActionHelper::Scale(
                    _entity->getSharedPtr(),
                    Vec3F::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
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
                EditorActionHelper::Transform(
                    _entity->getSharedPtr(),
                    TMat::FromString(SystemManager::GetInstancePtr()->getClipboardAsString()));
            });

        return true;
    }

    //////////////////////////////////////////
    void InspectorManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void InspectorManager::addAddComponentCallback(
        String const& _menuName,
        std::function<void(EntityPtr const&)> const& _callback,
        MenuListTree2D::ItemValidateCallback const& _validate)
    {
        m_extraAddComponentCallbacks.emplace_back(_menuName, _callback, _validate);
    }

    //////////////////////////////////////////
    ComponentEditorPtr InspectorManager::createComponentEditor(
        ComponentId _componentId,
        MetaClass* _metaClass)
    {
        auto it = m_componentEditors.find(_metaClass->getClassUID());
        if (it == m_componentEditors.end())
        {
            return ComponentEditorDefault::Create(_componentId, _metaClass);
        }
        else
        {
            return it->second(_componentId, _metaClass);
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
