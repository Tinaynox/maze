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
#include "maze-plugin-csharp-editor-tools/managers/MazeCSharpEditorToolsManager.hpp"
#include "maze-editor-tools/property-drawers/MazePDString.hpp"
#include "maze-editor-tools/property-drawers/MazePDS32.hpp"
#include "maze-editor-tools/property-drawers/MazePDBool.hpp"
#include "maze-editor-tools/property-drawers/MazePDF32.hpp"
#include "maze-editor-tools/property-drawers/MazePDSliderF32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec3F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec4F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2S32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2U32.hpp"
#include "maze-editor-tools/property-drawers/MazePDRect2F.hpp"
#include "maze-editor-tools/property-drawers/MazePDColorU32.hpp"
#include "maze-editor-tools/property-drawers/MazePDColorF128.hpp"
#include "maze-editor-tools/property-drawers/MazePDEntityPtr.hpp"
#include "maze-editor-tools/property-drawers/MazePDComponentPtr.hpp"
#include "maze-editor-tools/property-drawers/MazePDAssetUnitId.hpp"
#include "maze-editor-tools/property-drawers/MazePDAssetFileId.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CSharpEditorToolsManager
    //
    //////////////////////////////////////////
    CSharpEditorToolsManager* CSharpEditorToolsManager::s_instance = nullptr;

    //////////////////////////////////////////
    CSharpEditorToolsManager::CSharpEditorToolsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    CSharpEditorToolsManager::~CSharpEditorToolsManager()
    {
        s_instance = nullptr;

        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<CSharpCoreAssemblyLoadedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
        }
    }

    //////////////////////////////////////////
    void CSharpEditorToolsManager::Initialize(
        CSharpEditorToolsManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(CSharpEditorToolsManager, _manager, init());
    }

    //////////////////////////////////////////
    bool CSharpEditorToolsManager::init()
    {
        EventManager::GetInstancePtr()->subscribeEvent<CSharpCoreAssemblyLoadedEvent>(this, &CSharpEditorToolsManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &CSharpEditorToolsManager::notifyEvent);

        AssetEditorToolsManager::GetInstancePtr()->registerIconCallbackForAssetFileExtension(
            "cs",
            [](AssetFilePtr const&)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TextFile),
                        ColorU32(75, 216, 19)
                    };
            });

        registerDrawers();

        return true;
    }

    //////////////////////////////////////////
    ScriptPropertyDrawerCallbacks const* CSharpEditorToolsManager::getScriptPropertyDrawerCallbacks(ScriptPropertyPtr const& _prop)
    {
        auto it = m_monoPropertyDrawerCallbacksPerMonoType.find(_prop->getTypeName());
        if (it != m_monoPropertyDrawerCallbacksPerMonoType.end())
            return &it->second;
        else
        {
            MonoClass* monoClass = mono_class_from_mono_type(_prop->getMonoType());
            if (monoClass)
            {
                for (auto const& data : m_monoPropertyDrawerCallbacksPerMonoSuperClass)
                {
                    MonoClass* baseClass = data.first;
                    if (mono_class_is_subclass_of(monoClass, baseClass, false))
                    {
                        return &data.second;
                    }
                }
            }
        }

        return nullptr;
    }

    //////////////////////////////////////////
    ScriptPropertyDrawerCallbacks CSharpEditorToolsManager::buildScriptPropertyDrawerSliderF32Callbacks(
        F32 _from,
        F32 _to)
    {
        ScriptPropertyDrawerCallbacks callbacks = BuildScriptPropertyDrawerCallbacks<PropertyDrawerSliderF32>(
            CreateScriptPropertyDrawerDefault<PropertyDrawerSliderF32>,
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerSliderF32* _drawer)
            {
                F32 value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerSliderF32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            });
        callbacks.createDrawerCb =
            [_from, _to](DataBlock const& _data)
            {
                PropertyDrawerSliderF32Ptr drawer = PropertyDrawerSliderF32::Create(_data);
                drawer->setFromValue(_from);
                drawer->setToValue(_to);
                return drawer;
            };

        return callbacks;
    }

    //////////////////////////////////////////
    ScriptFieldDrawerCallbacks const* CSharpEditorToolsManager::getScriptFieldDrawerCallbacks(ScriptFieldPtr const& _field)
    {
        auto it = m_monoFieldDrawerCallbacksPerMonoType.find(_field->getTypeName());
        if (it != m_monoFieldDrawerCallbacksPerMonoType.end())
            return &it->second;
        else
        {
            MonoClass* monoClass = mono_class_from_mono_type(_field->getMonoType());
            if (monoClass)
            {
                for (auto const& data : m_monoFieldDrawerCallbacksPerMonoSuperClass)
                {
                    MonoClass* baseClass = data.first;
                    if (mono_class_is_subclass_of(monoClass, baseClass, false))
                    {
                        return &data.second;
                    }
                }
            }
        }

        return nullptr;
    }

    //////////////////////////////////////////
    ScriptFieldDrawerCallbacks CSharpEditorToolsManager::buildScriptFieldDrawerSliderF32Callbacks(
        F32 _from,
        F32 _to)
    {
        ScriptFieldDrawerCallbacks callbacks = BuildScriptFieldDrawerCallbacks<PropertyDrawerSliderF32>(
            CreateScriptPropertyDrawerDefault<PropertyDrawerSliderF32>,
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerSliderF32* _drawer)
            {
                F32 value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
            },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerSliderF32 const* _drawer)
            {
                _instance.setFieldValue(_field, _drawer->getValue());
            });
        callbacks.createDrawerCb =
            [_from, _to](DataBlock const& _dataBlock)
            {
                PropertyDrawerSliderF32Ptr drawer = PropertyDrawerSliderF32::Create(_dataBlock);
                drawer->setFromValue(_from);
                drawer->setToValue(_to);
                return drawer;
            };

        return callbacks;
    }

    //////////////////////////////////////////
    void CSharpEditorToolsManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<CSharpCoreAssemblyLoadedEvent>::UID())
        {
            registerDrawers();
        }
        else
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_monoPropertyDrawerCallbacksPerMonoType.clear();
            m_monoFieldDrawerCallbacksPerMonoType.clear();
            m_monoPropertyDrawerCallbacksPerMonoSuperClass.clear();
            m_monoFieldDrawerCallbacksPerMonoSuperClass.clear();
        }
    }

    //////////////////////////////////////////
    void CSharpEditorToolsManager::registerDrawers()
    {
        // System  
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerString>(MAZE_HCS("System.String"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerString* _drawer)
        {
            String value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerString const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerString* _drawer)
        {
            String value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerString const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Char"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            U16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            U16 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerBool>(MAZE_HCS("System.Boolean"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerBool* _drawer)
        {
            Bool value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerBool const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerBool* _drawer)
        {
            Bool value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerBool const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.SByte"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            S8 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            S8 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int16"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            S16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            S16 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int32"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            S32 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            S32 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int64"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            S64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            S64 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Byte"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            U8 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            U8 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt16"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            U16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            U16 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt32"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            U32 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            U32 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt64"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
        {
            U64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32* _drawer)
        {
            U64 value; _instance.getFieldValue(_field, value); _drawer->setValue((S32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerS32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Single"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32* _drawer)
        {
            F32 value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerF32* _drawer)
        {
            F32 value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerF32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Double"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32* _drawer)
        {
            F64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((F32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32 const* _drawer)
        {
            _instance.setPropertyValue(_property, (F64)_drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerF32* _drawer)
        {
            F64 value; _instance.getFieldValue(_field, value); _drawer->setValue((F32)value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerF32 const* _drawer)
        {
            _instance.setFieldValue(_field, (F64)_drawer->getValue());
        });

        // Maze Core
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerEntityPtr>(MAZE_HCS("Maze.Core.Entity"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerEntityPtr* _drawer)
        {
            Entity* value; _instance.getPropertyValue(_property, value);
            _drawer->setValue(value ? value->getSharedPtr() : nullptr);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerEntityPtr const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue().get());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerEntityPtr* _drawer)
        {
            Entity* value; _instance.getFieldValue(_field, value);
            _drawer->setValue(value ? value->getSharedPtr() : nullptr);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerEntityPtr const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue().get());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2S32>(MAZE_HCS("Maze.Core.Vec2S"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2S32* _drawer)
        {
            Vec2S value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2S32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2S32* _drawer)
        {
            Vec2S value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2S32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2U32>(MAZE_HCS("Maze.Core.Vec2U"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2U32* _drawer)
        {
            Vec2U value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2U32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2U32* _drawer)
        {
            Vec2U value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2U32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2F32>(MAZE_HCS("Maze.Core.Vec2F"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2F32* _drawer)
        {
            Vec2F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2F32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2F32* _drawer)
        {
            Vec2F value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec2F32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec3F32>(MAZE_HCS("Maze.Core.Vec3F"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec3F32* _drawer)
        {
            Vec3F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec3F32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec3F32* _drawer)
        {
            Vec3F value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec3F32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec4F32>(MAZE_HCS("Maze.Core.Vec4F"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec4F32* _drawer)
        {
            Vec4F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec4F32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec4F32* _drawer)
        {
            Vec4F value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerVec4F32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });

        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerRect2F>(MAZE_HCS("Maze.Core.Rect2F"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerRect2F* _drawer)
        {
            Rect2F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerRect2F const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerRect2F* _drawer)
        {
            Rect2F value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerRect2F const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });

        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerAssetUnitId>(MAZE_HCS("Maze.Core.AssetUnitId"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerAssetUnitId* _drawer)
        {
            AssetUnitId value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerAssetUnitId const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerAssetUnitId* _drawer)
        {
            AssetUnitId value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerAssetUnitId const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });

        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerAssetFileId>(MAZE_HCS("Maze.Core.AssetFileId"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerAssetFileId* _drawer)
        {
            AssetFileId value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerAssetFileId const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerAssetFileId* _drawer)
        {
            AssetFileId value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerAssetFileId const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue());
        });


        // Maze Graphics
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerColorU32>(MAZE_HCS("Maze.Graphics.ColorU32"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorU32* _drawer)
        {
            Vec4U8 value; _instance.getPropertyValue(_property, value); _drawer->setValue(ColorU32::FromVec4U8(value));
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorU32 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue().toVec4U8());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerColorU32* _drawer)
        {
            Vec4U8 value; _instance.getFieldValue(_field, value); _drawer->setValue(ColorU32::FromVec4U8(value));
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerColorU32 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue().toVec4U8());
        });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerColorF128>(MAZE_HCS("Maze.Graphics.ColorF128"),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorF128* _drawer)
        {
            Vec4F value; _instance.getPropertyValue(_property, value); _drawer->setValue(ColorF128(value));
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorF128 const* _drawer)
        {
            _instance.setPropertyValue(_property, _drawer->getValue().toVec4F32());
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerColorF128* _drawer)
        {
            Vec4F value; _instance.getFieldValue(_field, value); _drawer->setValue(ColorF128(value));
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerColorF128 const* _drawer)
        {
            _instance.setFieldValue(_field, _drawer->getValue().toVec4F32());
        });

        ScriptClassPtr const& componentClass = MonoEngine::GetComponentClass();

        registerScriptPropertyAndFieldDrawerSuperClassCallbacks<PropertyDrawerComponentPtr>(
            componentClass->getMonoClass(),
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerComponentPtr* _drawer)
        {
            MonoObject* componentInstance = nullptr;
            _instance.getPropertyValue(_property, componentInstance);

            if (!componentInstance)
            {
                _drawer->setValue(nullptr);
                return;
            }

            MonoProperty* componentPtrProperty = MonoEngine::GetNativeComponentPtrProperty()->getMonoProperty();

            MonoObject* result = mono_property_get_value(componentPtrProperty, componentInstance, nullptr, nullptr);
            if (result)
            {
                Component* component = *(Component**)mono_object_unbox(result);
                _drawer->setValue(component ? component->getSharedPtr() : nullptr);
            }
            else
                _drawer->setValue(nullptr);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerComponentPtr const* _drawer)
        {
            ComponentPtr component = _drawer->getValue();
            if (component)
            {
                MonoObject* result = MonoHelper::GetComponentByType(component.get(), _property->getMonoType());
                if (result)
                    _instance.setPropertyValue(_property, *result);
                else
                    _instance.resetPropertyValue(_property);
            }
            else
                _instance.resetPropertyValue(_property);
        },
            [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerComponentPtr* _drawer)
        {
            MonoObject* componentInstance = nullptr;
            _instance.getFieldValue(_field, componentInstance);

            if (!componentInstance)
            {
                _drawer->setValue(nullptr);
                return;
            }

            MonoProperty* componentPtrProperty = MonoEngine::GetNativeComponentPtrProperty()->getMonoProperty();

            MonoObject* result = mono_property_get_value(componentPtrProperty, componentInstance, nullptr, nullptr);
            if (result)
            {
                Component* component = *(Component**)mono_object_unbox(result);
                _drawer->setValue(component ? component->getSharedPtr() : nullptr);
            }
            else
                _drawer->setValue(nullptr);
        },
            [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerComponentPtr const* _drawer)
        {
            ComponentPtr component = _drawer->getValue();
            if (component)
            {
                MonoObject* result = MonoHelper::GetComponentByType(component.get(), _field->getMonoType());
                if (result)
                    _instance.setFieldValue(_field, *result);
                else
                    _instance.resetFieldValue(_field);
            }
            else
                _instance.resetFieldValue(_field);
        });
    }

} // namespace Maze
//////////////////////////////////////////
