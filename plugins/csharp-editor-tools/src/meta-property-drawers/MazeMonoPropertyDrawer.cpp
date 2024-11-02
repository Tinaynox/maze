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
#include "MazeCSharpEditorToolsHeader.hpp"
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeMonoPropertyDrawer.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/property-drawers/MazePDString.hpp"
#include "maze-editor-tools/property-drawers/MazePDS32.hpp"
#include "maze-editor-tools/property-drawers/MazePDBool.hpp"
#include "maze-editor-tools/property-drawers/MazePDF32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec3F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec4F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2S32.hpp"
#include "maze-editor-tools/property-drawers/MazePDRect2F.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MonoPropertyDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MonoPropertyDrawer, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MonoPropertyDrawer);

    //////////////////////////////////////////
    MonoPropertyDrawer::MonoPropertyDrawer()
    {
        
    }

    //////////////////////////////////////////
    MonoPropertyDrawer::~MonoPropertyDrawer()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    MonoPropertyDrawerPtr MonoPropertyDrawer::Create(
        MonoProperty* _monoProperty,
        MonoFieldType _monoFieldType)
    {
        MonoPropertyDrawerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MonoPropertyDrawer, object, init(_monoProperty, _monoFieldType));
        return object;
    }

    //////////////////////////////////////////
    bool MonoPropertyDrawer::init(
        MonoProperty* _monoProperty,
        MonoFieldType _monoFieldType)
    {
        if (!MetaPropertyDrawer::init(nullptr))
            return false;

        m_monoProperty = _monoProperty;
        m_monoFieldType = _monoFieldType;

        switch (m_monoFieldType)
        {
            case MonoFieldType::TypeString: m_drawer = PropertyDrawerString::Create(""); break;
            case MonoFieldType::TypeBool: m_drawer = PropertyDrawerBool::Create(""); break;
            case MonoFieldType::TypeS8: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeS16: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeS32: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeS64: m_drawer = PropertyDrawerS32::Create(""); break; // #TODO: PropertyDrawerS64
            case MonoFieldType::TypeU8: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeU16: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeU32: m_drawer = PropertyDrawerS32::Create(""); break;
            case MonoFieldType::TypeU64: m_drawer = PropertyDrawerS32::Create(""); break; // #TODO: PropertyDrawerU64
            case MonoFieldType::TypeF32: m_drawer = PropertyDrawerF32::Create(""); break;
            case MonoFieldType::TypeF64: m_drawer = PropertyDrawerF32::Create(""); break; // #TODO: PropertyDrawerF64
            case MonoFieldType::TypeVec2S: m_drawer = PropertyDrawerVec2S32::Create(""); break;
            // case MonoFieldType::TypeVec3S: m_drawer = PropertyDrawerVec3S32::Create(""); break;
            // case MonoFieldType::TypeVec4S: m_drawer = PropertyDrawerVec4S32::Create(""); break;
            // case MonoFieldType::TypeVec2U: m_drawer = PropertyDrawerVec2U32::Create(""); break;
            // case MonoFieldType::TypeVec3U: m_drawer = PropertyDrawerVec3U32::Create(""); break;
            // case MonoFieldType::TypeVec4U: m_drawer = PropertyDrawerVec4U32::Create(""); break;
            case MonoFieldType::TypeVec2F: m_drawer = PropertyDrawerVec2F32::Create(""); break;
            case MonoFieldType::TypeVec3F: m_drawer = PropertyDrawerVec3F32::Create(""); break;
            case MonoFieldType::TypeVec4F: m_drawer = PropertyDrawerVec4F32::Create(""); break;
            // case MonoFieldType::TypeMat3F: m_drawer = PropertyDrawerMat3F::Create(""); break;
            // case MonoFieldType::TypeMat4F: m_drawer = PropertyDrawerMat4F::Create(""); break;
            // case MonoFieldType::TypeTMat: m_drawer = PropertyDrawerTMat::Create(""); break;
            case MonoFieldType::TypeRect2F: m_drawer = PropertyDrawerRect2F::Create(""); break;
            default:
                return false;
        }
        
        m_drawer->eventUIData.subscribe(this, &MonoPropertyDrawer::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MonoPropertyDrawer::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        if (m_drawer)
            m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void MonoPropertyDrawer::processDataToUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();

        switch (m_monoFieldType)
        {
            case MonoFieldType::TypeString:
            {
                String value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerString>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeBool:
            {
                Bool value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerBool>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeS8:
            {
                S8 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeS16:
            {
                S16 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeS32:
            {
                S32 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeS64:
            {
                S64 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeU8:
            {
                U8 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeU16:
            {
                U16 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeU32:
            {
                U32 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeU64:
            {
                U64 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerS32>()->setValue((S32)value);
                break;
            }
            case MonoFieldType::TypeF32:
            {
                F32 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerF32>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeF64:
            {
                F64 value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerF32>()->setValue((F32)value);
                break;
            }
            case MonoFieldType::TypeVec2S:
            {
                break;
            }
            case MonoFieldType::TypeVec3S:
            {
                break;
            }
            case MonoFieldType::TypeVec4S:
            {
                break;
            }
            case MonoFieldType::TypeVec2U:
            {
                break;
            }
            case MonoFieldType::TypeVec3U:
            {
                break;
            }
            case MonoFieldType::TypeVec4U:
            {
                break;
            }
            case MonoFieldType::TypeVec2F:
            {
                Vec2F value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerVec2F32>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeVec3F:
            {
                Vec3F value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerVec3F32>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeVec4F:
            {
                Vec4F value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerVec4F32>()->setValue(value);
                break;
            }
            case MonoFieldType::TypeMat3F:
            {
                break;
            }
            case MonoFieldType::TypeMat4F:
            {
                break;
            }
            case MonoFieldType::TypeTMat:
            {
                break;
            }
            case MonoFieldType::TypeRect2F:
            {
                Rect2F value; monoBehaviour->getMonoInstance().getProperty(m_monoProperty, value);
                m_drawer->castRaw<PropertyDrawerRect2F>()->setValue(value);
                break;
            }
        }
    }

    //////////////////////////////////////////
    void MonoPropertyDrawer::processDataFromUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();

        switch (m_monoFieldType)
        {
            case MonoFieldType::TypeString:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerString>()->getValue());
                break;
            }
            case MonoFieldType::TypeBool:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerBool>()->getValue());
                break;
            }
            case MonoFieldType::TypeS8:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (S8)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeS16:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (S16)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeS32:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (S32)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeS64:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (S64)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeU8:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (U8)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeU16:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (U16)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeU32:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (U32)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeU64:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (U64)m_drawer->castRaw<PropertyDrawerS32>()->getValue());
                break;
            }
            case MonoFieldType::TypeF32:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerF32>()->getValue());
                break;
            }
            case MonoFieldType::TypeF64:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    (F64)m_drawer->castRaw<PropertyDrawerF32>()->getValue());
                break;
            }
            case MonoFieldType::TypeVec2S:
            {
                break;
            }
            case MonoFieldType::TypeVec3S:
            {
                break;
            }
            case MonoFieldType::TypeVec4S:
            {
                break;
            }
            case MonoFieldType::TypeVec2U:
            {
                break;
            }
            case MonoFieldType::TypeVec3U:
            {
                break;
            }
            case MonoFieldType::TypeVec4U:
            {
                break;
            }
            case MonoFieldType::TypeVec2F:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerVec2F32>()->getValue());
                break;
            }
            case MonoFieldType::TypeVec3F:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerVec3F32>()->getValue());
                break;
            }
            case MonoFieldType::TypeVec4F:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerVec4F32>()->getValue());
                break;
            }
            case MonoFieldType::TypeMat3F:
            {
                break;
            }
            case MonoFieldType::TypeMat4F:
            {
                break;
            }
            case MonoFieldType::TypeTMat:
            {
                break;
            }
            case MonoFieldType::TypeRect2F:
            {
                monoBehaviour->getMonoInstance().setProperty(m_monoProperty,
                    m_drawer->castRaw<PropertyDrawerRect2F>()->getValue());
                break;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
