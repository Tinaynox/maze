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
#if (!defined(_MazeMonoSerializationManager_hpp_))
#define _MazeMonoSerializationManager_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MonoSerializationManager);


    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API CSharpPropertyDataBlockSerializationData
    {
        std::function<void(ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> propToDataBlockCb;
        std::function<void(ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> propFromDataBlockCb;
    };


    //////////////////////////////////////////
    // Class MonoSerializationManager
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API MonoSerializationManager
    {
    public:

        //////////////////////////////////////////
        virtual ~MonoSerializationManager();

        //////////////////////////////////////////
        static void Initialize(
            MonoSerializationManagerPtr& _manager);


        //////////////////////////////////////////
        static inline MonoSerializationManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline MonoSerializationManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        void registerPropertyDataBlockSerialization(
            HashedCString _typeName,
            std::function<void(ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> _propToDataBlockCb,
            std::function<void(ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> _propFromDataBlockCb);

        //////////////////////////////////////////
        bool savePropertyToDataBlock(ScriptInstance const& _instance, ScriptPropertyPtr const& _property, DataBlock& _dataBlock);

        //////////////////////////////////////////
        bool loadPropertyFromDataBlock(ScriptInstance& _instance, ScriptPropertyPtr const& _property, DataBlock const& _dataBlock);

    protected:

        //////////////////////////////////////////
        MonoSerializationManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static MonoSerializationManager* s_instance;

        StringKeyMap<CSharpPropertyDataBlockSerializationData> m_propertyDataBlockSerializationData;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoSerializationManager_hpp_
//////////////////////////////////////////
