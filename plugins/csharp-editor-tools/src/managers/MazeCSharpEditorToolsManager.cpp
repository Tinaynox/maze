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
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


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


        // System  
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerString>(MAZE_HCS("System.String"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerString* _drawer)
            {
                String value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerString const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerString* _drawer)
            {
                String value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerString const* _drawer)
            {
                _instance.setFieldValue(_field, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Char"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U16 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerBool>(MAZE_HCS("System.Boolean"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerBool* _drawer)
            {
                Bool value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerBool const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerBool* _drawer)
            {
                Bool value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerBool const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.SByte"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S8 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S8 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int16"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S16 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int32"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S32 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S32 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int64"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                S64 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Byte"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U8 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U8 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt16"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U16 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U16 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt32"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U32 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U32 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt64"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32* _drawer)
            {
                U64 value; _instance.getFieldValue(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Single"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32* _drawer)
            {
                F32 value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerF32* _drawer)
            {
                F32 value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Double"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32* _drawer)
            {
                F64 value; _instance.getPropertyValue(_property, value); _drawer->setValue((F32)value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setPropertyValue(_property, (F64)_drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerF32* _drawer)
            {
                F64 value; _instance.getFieldValue(_property, value); _drawer->setValue((F32)value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setFieldValue(_property, (F64)_drawer->getValue());
            });

        // Maze Core
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerEntityPtr>(MAZE_HCS("Maze.Core.Entity"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerEntityPtr* _drawer)
            {
                Entity* value; _instance.getPropertyValue(_property, value);
                _drawer->setValue(value ? value->getSharedPtr() : nullptr);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerEntityPtr const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue().get());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerEntityPtr* _drawer)
            {
                Entity* value; _instance.getFieldValue(_property, value);
                _drawer->setValue(value ? value->getSharedPtr() : nullptr);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerEntityPtr const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue().get());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2S32>(MAZE_HCS("Maze.Core.Vec2S"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2S32* _drawer)
            {
                Vec2S value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2S32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2S32* _drawer)
            {
                Vec2S value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2S32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2U32>(MAZE_HCS("Maze.Core.Vec2U"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2U32* _drawer)
            {
                Vec2U value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2U32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2U32* _drawer)
            {
                Vec2U value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2U32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec2F32>(MAZE_HCS("Maze.Core.Vec2F"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2F32* _drawer)
            {
                Vec2F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec2F32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2F32* _drawer)
            {
                Vec2F value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec2F32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec3F32>(MAZE_HCS("Maze.Core.Vec3F"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec3F32* _drawer)
            {
                Vec3F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec3F32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec3F32* _drawer)
            {
                Vec3F value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec3F32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerVec4F32>(MAZE_HCS("Maze.Core.Vec4F"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec4F32* _drawer)
            {
                Vec4F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerVec4F32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec4F32* _drawer)
            {
                Vec4F value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerVec4F32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });

        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerRect2F>(MAZE_HCS("Maze.Core.Rect2F"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerRect2F* _drawer)
            {
                Rect2F value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerRect2F const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerRect2F* _drawer)
            {
                Rect2F value; _instance.getFieldValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerRect2F const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue());
            });
        

        // Maze Graphics
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerColorU32>(MAZE_HCS("Maze.Graphics.ColorU32"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorU32* _drawer)
            {
                Vec4U8 value; _instance.getPropertyValue(_property, value); _drawer->setValue(ColorU32::FromVec4U8(value));
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorU32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue().toVec4U8());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerColorU32* _drawer)
            {
                Vec4U8 value; _instance.getFieldValue(_property, value); _drawer->setValue(ColorU32::FromVec4U8(value));
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerColorU32 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue().toVec4U8());
            });
        registerScriptPropertyAndFieldDrawerCallbacks<PropertyDrawerColorF128>(MAZE_HCS("Maze.Graphics.ColorF128"),
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorF128* _drawer)
            {
                Vec4F value; _instance.getPropertyValue(_property, value); _drawer->setValue(ColorF128(value));
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerColorF128 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue().toVec4F32());
            },
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _property, PropertyDrawerColorF128* _drawer)
            {
                Vec4F value; _instance.getFieldValue(_property, value); _drawer->setValue(ColorF128(value));
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _property, PropertyDrawerColorF128 const* _drawer)
            {
                _instance.setFieldValue(_property, _drawer->getValue().toVec4F32());
            });

        return true;
    }

    //////////////////////////////////////////
    ScriptPropertyDrawerCallbacks* CSharpEditorToolsManager::getScriptPropertyDrawerCallbacksPerMonoType(HashedCString _monoTypeName)
    {
        auto it = m_monoPropertyDrawerCallbacksPerMonoType.find(_monoTypeName);
        if (it != m_monoPropertyDrawerCallbacksPerMonoType.end())
            return &it->second;

        return nullptr;
    }

    //////////////////////////////////////////
    ScriptPropertyDrawerCallbacks CSharpEditorToolsManager::buildScriptPropertyDrawerSliderF32Callbacks(
        F32 _from,
        F32 _to)
    {
        ScriptPropertyDrawerCallbacks callbacks = BuildScriptPropertyDrawerCallbacks<PropertyDrawerSliderF32>(
            CreateScriptPropertyDrawerDefault<PropertyDrawerSliderF32>,
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _property, PropertyDrawerSliderF32* _drawer)
            {
                F32 value; _instance.getPropertyValue(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _property, PropertyDrawerSliderF32 const* _drawer)
            {
                _instance.setPropertyValue(_property, _drawer->getValue());
            });
        callbacks.createDrawerCb =
            [_from, _to]()
            {
                PropertyDrawerSliderF32Ptr drawer = PropertyDrawerSliderF32::Create("");
                drawer->setFromValue(_from);
                drawer->setToValue(_to);
                return drawer;
            };

        return callbacks;
    }

    //////////////////////////////////////////
    ScriptFieldDrawerCallbacks* CSharpEditorToolsManager::getScriptFieldDrawerCallbacksPerMonoType(HashedCString _monoTypeName)
    {
        auto it = m_monoFieldDrawerCallbacksPerMonoType.find(_monoTypeName);
        if (it != m_monoFieldDrawerCallbacksPerMonoType.end())
            return &it->second;

        return nullptr;
    }

    //////////////////////////////////////////
    ScriptFieldDrawerCallbacks CSharpEditorToolsManager::buildScriptFieldDrawerSliderF32Callbacks(
        F32 _from,
        F32 _to)
    {
        ScriptFieldDrawerCallbacks callbacks = BuildScriptFieldDrawerCallbacks<PropertyDrawerSliderF32>(
            CreateScriptPropertyDrawerDefault<PropertyDrawerSliderF32>,
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _field, PropertyDrawerSliderF32* _drawer)
            {
                F32 value; _instance.getFieldValue(_field, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, ScriptFieldPtr const& _field, PropertyDrawerSliderF32 const* _drawer)
            {
                _instance.setFieldValue(_field, _drawer->getValue());
            });
        callbacks.createDrawerCb =
            [_from, _to]()
            {
                PropertyDrawerSliderF32Ptr drawer = PropertyDrawerSliderF32::Create("");
                drawer->setFromValue(_from);
                drawer->setToValue(_to);
                return drawer;
            };

        return callbacks;
    }

} // namespace Maze
//////////////////////////////////////////
