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
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeMonoPropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(CSharpEditorToolsManager);


    //////////////////////////////////////////
    template <typename TDrawer>
    inline MonoPropertyDrawerCallbacks BuildMonoPropertyDrawerCallbacks(
        std::function<void(ScriptInstance const&, MonoProperty*, TDrawer*)> _processDataToUICb,
        std::function<void(ScriptInstance&, MonoProperty*, TDrawer const*)> _processDataFromUICb)
    {
        MonoPropertyDrawerCallbacks callbacks;
        callbacks.createDrawerCb =
            []()
            {
                return TDrawer::Create("");
            };
        callbacks.processDataToUICb =
            [_processDataToUICb](ScriptInstance const& _instance, MonoProperty* _monoProperty, PropertyDrawerPtr const& _drawer)
            {
                _processDataToUICb(_instance, _monoProperty, _drawer->castRaw<TDrawer>());
            };
        callbacks.processDataFromUICb =
            [_processDataFromUICb](ScriptInstance& _instance, MonoProperty* _monoProperty, PropertyDrawerPtr const& _drawer)
            {
                _processDataFromUICb(_instance, _monoProperty, _drawer->castRaw<TDrawer>());
            };
        return std::move(callbacks);
    }


    //////////////////////////////////////////
    // Class CSharpEditorToolsManager
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API CSharpEditorToolsManager
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
        MonoPropertyDrawerCallbacks* getMonoPropertyDrawerCallbacksPerMonoType(HashedCString _monoTypeName);

        //////////////////////////////////////////
        template <typename TDrawer>
        inline void registerMonoPropertyDrawerCallbacks(
            HashedCString _monoTypeName,
            std::function<void(ScriptInstance const&, MonoProperty*, TDrawer*)> _processDataToUICb,
            std::function<void(ScriptInstance&, MonoProperty*, TDrawer const*)> _processDataFromUICb)
        {
            m_monoPropertyDrawerCallbacksPerMonoType[_monoTypeName] = BuildMonoPropertyDrawerCallbacks<TDrawer>(
                _processDataToUICb,
                _processDataFromUICb);
        }

        //////////////////////////////////////////
        MonoPropertyDrawerCallbacks buildMonoPropertyDrawerSliderF32Callbacks(
            F32 _from,
            F32 _to);


    protected:

        //////////////////////////////////////////
        CSharpEditorToolsManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static CSharpEditorToolsManager* s_instance;

        StringKeyMap<MonoPropertyDrawerCallbacks> m_monoPropertyDrawerCallbacksPerMonoType;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCSharpEditorToolsManager_hpp_
//////////////////////////////////////////
