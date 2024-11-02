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
#include "maze-editor-tools/property-drawers/MazePDVec2F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec3F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec4F32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2S32.hpp"
#include "maze-editor-tools/property-drawers/MazePDVec2U32.hpp"
#include "maze-editor-tools/property-drawers/MazePDRect2F.hpp"


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

        registerMonoPropertyDrawerCallbacks<PropertyDrawerString>(MAZE_HCS("System.String"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerString* _drawer)
            {
                String value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerString const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerBool>(MAZE_HCS("System.Boolean"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerBool* _drawer)
            {
                Bool value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerBool const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Char"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                S8 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int16"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                S16 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int32"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                S32 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Int64"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                S64 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.Byte"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                U8 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt16"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                U16 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt32"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                U32 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerS32>(MAZE_HCS("System.UInt64"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerS32* _drawer)
            {
                U64 value; _instance.getProperty(_property, value); _drawer->setValue((S32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerS32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Single"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerF32* _drawer)
            {
                F32 value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerF32>(MAZE_HCS("System.Double"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerF32* _drawer)
            {
                F64 value; _instance.getProperty(_property, value); _drawer->setValue((F32)value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerF32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });

        registerMonoPropertyDrawerCallbacks<PropertyDrawerVec2S32>(MAZE_HCS("Maze.Vec2S"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerVec2S32* _drawer)
            {
                Vec2S value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerVec2S32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerVec2U32>(MAZE_HCS("Maze.Vec2U"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerVec2U32* _drawer)
            {
                Vec2U value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerVec2U32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerVec2F32>(MAZE_HCS("Maze.Vec2F"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerVec2F32* _drawer)
            {
                Vec2F value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerVec2F32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerVec3F32>(MAZE_HCS("Maze.Vec3F"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerVec3F32* _drawer)
            {
                Vec3F value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerVec3F32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });
        registerMonoPropertyDrawerCallbacks<PropertyDrawerVec4F32>(MAZE_HCS("Maze.Vec4F"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerVec4F32* _drawer)
            {
                Vec4F value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerVec4F32 const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });

        registerMonoPropertyDrawerCallbacks<PropertyDrawerRect2F>(MAZE_HCS("Maze.Rect2F"),
            [](ScriptInstance const& _instance, MonoProperty* _property, PropertyDrawerRect2F* _drawer)
            {
                Rect2F value; _instance.getProperty(_property, value); _drawer->setValue(value);
            },
            [](ScriptInstance& _instance, MonoProperty* _property, PropertyDrawerRect2F const* _drawer)
            {
                _instance.setProperty(_property, _drawer->getValue());
            });

        return true;
    }

    //////////////////////////////////////////
    MonoPropertyDrawerCallbacks* CSharpEditorToolsManager::getMonoPropertyDrawerCallbacksPerMonoType(HashedCString _monoTypeName)
    {
        auto it = m_monoPropertyDrawerCallbacksPerMonoType.find(_monoTypeName);
        if (it != m_monoPropertyDrawerCallbacksPerMonoType.end())
            return &it->second;

        return nullptr;
    }

} // namespace Maze
//////////////////////////////////////////
