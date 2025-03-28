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
#if (!defined(_MazeCSharpEditorToolsManager_hpp_))
#define _MazeCSharpEditorToolsManager_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeScriptPropertyDrawer.hpp"
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeScriptFieldDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(CSharpEditorToolsManager);


    //////////////////////////////////////////
    template <typename TDrawer>
    inline PropertyDrawerPtr CreateScriptPropertyDrawerDefault(DataBlock const& _dataBlock)
    {
        return TDrawer::Create(_dataBlock);
    }


    //////////////////////////////////////////
    template <typename TDrawer>
    inline ScriptPropertyDrawerCallbacks BuildScriptPropertyDrawerCallbacks(
        std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, TDrawer*)> _processDataToUICb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, TDrawer const*)> _processDataFromUICb)
    {
        ScriptPropertyDrawerCallbacks callbacks;
        callbacks.createDrawerCb = _createDrawerCb;
        callbacks.processDataToUICb =
            [_processDataToUICb](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, PropertyDrawerPtr const& _drawer)
            {
                _processDataToUICb(_world, _instance, _prop, _drawer->castRaw<TDrawer>());
            };
        callbacks.processDataFromUICb =
            [_processDataFromUICb](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, PropertyDrawerPtr const& _drawer)
            {
                _processDataFromUICb(_world, _instance, _prop, _drawer->castRaw<TDrawer>());
            };
        return std::move(callbacks);
    }


    //////////////////////////////////////////
    template <typename TDrawer>
    inline ScriptFieldDrawerCallbacks BuildScriptFieldDrawerCallbacks(
        std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, TDrawer*)> _processDataToUICb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, TDrawer const*)> _processDataFromUICb)
    {
        ScriptFieldDrawerCallbacks callbacks;
        callbacks.createDrawerCb = _createDrawerCb;
        callbacks.processDataToUICb =
            [_processDataToUICb](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _prop, PropertyDrawerPtr const& _drawer)
            {
                _processDataToUICb(_world, _instance, _prop, _drawer->castRaw<TDrawer>());
            };
        callbacks.processDataFromUICb =
            [_processDataFromUICb](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _prop, PropertyDrawerPtr const& _drawer)
            {
                _processDataFromUICb(_world, _instance, _prop, _drawer->castRaw<TDrawer>());
            };
        return std::move(callbacks);
    }


    //////////////////////////////////////////
    // Class CSharpEditorToolsManager
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API CSharpEditorToolsManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~CSharpEditorToolsManager();

        //////////////////////////////////////////
        static void Initialize(
            CSharpEditorToolsManagerPtr& _manager);


        //////////////////////////////////////////
        static inline CSharpEditorToolsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline CSharpEditorToolsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        ScriptPropertyDrawerCallbacks const* getScriptPropertyDrawerCallbacks(ScriptPropertyPtr const& _prop);

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptPropertyDrawerCallbacks(
            HashedCString _monoTypeName,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, TDrawer*)> _processDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, TDrawer const*)> _processDataFromUICb)
        {
            m_monoPropertyDrawerCallbacksPerMonoType[_monoTypeName] = BuildScriptPropertyDrawerCallbacks<TDrawer>(
                _createDrawerCb,
                _processDataToUICb,
                _processDataFromUICb);
        }

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptPropertyDrawerSuperClassCallbacks(
            MonoClass* _superClass,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, TDrawer*)> _processDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, TDrawer const*)> _processDataFromUICb)
        {
            m_monoPropertyDrawerCallbacksPerMonoSuperClass[_superClass] = BuildScriptPropertyDrawerCallbacks<TDrawer>(
                _createDrawerCb,
                _processDataToUICb,
                _processDataFromUICb);
        }

        //////////////////////////////////////////
        ScriptPropertyDrawerCallbacks buildScriptPropertyDrawerSliderF32Callbacks(
            F32 _from,
            F32 _to);


        //////////////////////////////////////////
        ScriptFieldDrawerCallbacks const* getScriptFieldDrawerCallbacks(ScriptFieldPtr const& _field);

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptFieldDrawerCallbacks(
            HashedCString _monoTypeName,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, TDrawer*)> _processDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, TDrawer const*)> _processDataFromUICb)
        {
            m_monoFieldDrawerCallbacksPerMonoType[_monoTypeName] = BuildScriptFieldDrawerCallbacks<TDrawer>(
                _createDrawerCb,
                _processDataToUICb,
                _processDataFromUICb);
        }

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptFieldDrawerSuperClassCallbacks(
            MonoClass* _superClass,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, TDrawer*)> _processDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, TDrawer const*)> _processDataFromUICb)
        {
            m_monoFieldDrawerCallbacksPerMonoSuperClass[_superClass] = BuildScriptFieldDrawerCallbacks<TDrawer>(
                _createDrawerCb,
                _processDataToUICb,
                _processDataFromUICb);
        }

        //////////////////////////////////////////
        ScriptFieldDrawerCallbacks buildScriptFieldDrawerSliderF32Callbacks(
            F32 _from,
            F32 _to);



        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptPropertyAndFieldDrawerCallbacks(
            HashedCString _monoTypeName,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, TDrawer*)> _propertyProcessDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, TDrawer const*)> _propertyProcessDataFromUICb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, TDrawer*)> _fieldProcessDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, TDrawer const*)> _fieldProcessDataFromUICb,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb = CreateScriptPropertyDrawerDefault<TDrawer>)
        {
            registerScriptPropertyDrawerCallbacks(_monoTypeName, _createDrawerCb, _propertyProcessDataToUICb, _propertyProcessDataFromUICb);
            registerScriptFieldDrawerCallbacks(_monoTypeName, _createDrawerCb, _fieldProcessDataToUICb, _fieldProcessDataFromUICb);
        }

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerScriptPropertyAndFieldDrawerSuperClassCallbacks(
            MonoClass* _superClass,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, TDrawer*)> _propertyProcessDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, TDrawer const*)> _propertyProcessDataFromUICb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, TDrawer*)> _fieldProcessDataToUICb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, TDrawer const*)> _fieldProcessDataFromUICb,
            std::function<PropertyDrawerPtr(DataBlock const&)> _createDrawerCb = CreateScriptPropertyDrawerDefault<TDrawer>)
        {
            registerScriptPropertyDrawerSuperClassCallbacks(_superClass, _createDrawerCb, _propertyProcessDataToUICb, _propertyProcessDataFromUICb);
            registerScriptFieldDrawerSuperClassCallbacks(_superClass, _createDrawerCb, _fieldProcessDataToUICb, _fieldProcessDataFromUICb);
        }

    protected:

        //////////////////////////////////////////
        CSharpEditorToolsManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

        //////////////////////////////////////////
        void registerDrawers();

    protected:
        static CSharpEditorToolsManager* s_instance;

        StringKeyMap<ScriptPropertyDrawerCallbacks> m_monoPropertyDrawerCallbacksPerMonoType;
        StringKeyMap<ScriptFieldDrawerCallbacks> m_monoFieldDrawerCallbacksPerMonoType;

        UnorderedMap<MonoClass*, ScriptPropertyDrawerCallbacks> m_monoPropertyDrawerCallbacksPerMonoSuperClass;
        UnorderedMap<MonoClass*, ScriptFieldDrawerCallbacks> m_monoFieldDrawerCallbacksPerMonoSuperClass;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCSharpEditorToolsManager_hpp_
//////////////////////////////////////////
