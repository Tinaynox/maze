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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-debugger/scenes/SceneDebugEditor.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerBool.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerS32.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerF32.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerVec2DF.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerVec2DS.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerVec3DF.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerRect2DF.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerString.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerQuaternion.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerEnumClass.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerColorU32.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerColorF128.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerColorGradient.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerAnimationCurve.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerMaterial.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerRenderMesh.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerRadians.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerBool.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerS32.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerF32.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerVec2DF.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerVec3DF.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerRect2DF.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerString.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerQuaternion.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerEnumClass.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerVector.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerColorU32.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerColorF128.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerColorGradient.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerAnimationCurve.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerDefault.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditorDefault.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditorTransform3D.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditorCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using MetaPropertyDrawerMaterial =
        MetaPropertyDrawerDefault<
            MaterialPtr,
            PropertyDrawerMaterial>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerMaterial, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDrawerMaterial);
    template class MAZE_DEBUGGER_API MetaPropertyDrawerDefault<MaterialPtr, PropertyDrawerMaterial>;


    //////////////////////////////////////////
    using MetaPropertyDrawerRenderMesh =
        MetaPropertyDrawerDefault<
            RenderMeshPtr,
            PropertyDrawerRenderMesh>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerRenderMesh, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDrawerRenderMesh);
    template class MAZE_DEBUGGER_API MetaPropertyDrawerDefault<RenderMeshPtr, PropertyDrawerRenderMesh>;


    //////////////////////////////////////////
    using MetaPropertyDrawerRadians =
        MetaPropertyDrawerDefault<
            F32,
            PropertyDrawerRadians>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDrawerRadians, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDrawerRadians);
    template class MAZE_DEBUGGER_API MetaPropertyDrawerDefault<F32, PropertyDrawerRadians>;


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

        registerMetaPropertyDrawer<bool, MetaPropertyDrawerBool>();
        registerMetaPropertyDrawer<S32, MetaPropertyDrawerS32>();
        registerMetaPropertyDrawer<F32, MetaPropertyDrawerF32>();
        registerMetaPropertyDrawer<Vec2DF, MetaPropertyDrawerVec2DF>();
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

        registerComponentEditor<Transform3D, ComponentEditorTransform3D>();
        registerComponentEditor<Camera3D, ComponentEditorCamera3D>();

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
        MetaClass* metaClass = MetaClass::GetMetaClass(_propertyClassUID);

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

    
} // namespace Maze
//////////////////////////////////////////
