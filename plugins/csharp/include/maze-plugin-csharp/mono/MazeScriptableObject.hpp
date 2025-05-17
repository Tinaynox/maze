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
#if (!defined(_MazeScriptableObject_hpp_))
#define _MazeScriptableObject_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeIndexedResource.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptInstance);
    MAZE_USING_MANAGED_SHARED_PTR(ScriptableObject);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class ScriptableObject
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptableObject
        : public IndexedResource<ScriptableObject>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ScriptableObject();

        //////////////////////////////////////////
        ~ScriptableObject();

        //////////////////////////////////////////
        static ScriptableObjectPtr Create();

        //////////////////////////////////////////
        ScriptableObject(ScriptableObject const&) = delete;

        //////////////////////////////////////////
        ScriptableObject(ScriptableObject&&) = delete;

        //////////////////////////////////////////
        ScriptableObject& operator=(ScriptableObject const&) = default;

        //////////////////////////////////////////
        ScriptableObject& operator=(ScriptableObject&&) = default;


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(HashedString const& _name) { m_name = _name; }


        //////////////////////////////////////////
        bool loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool loadFromDataBlock(DataBlock const& _dataBlock);

        //////////////////////////////////////////
        void setData(DataBlock const& _dataBlock);

    private:

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    private:
        HashedString m_name;
        ScriptInstancePtr m_scriptInstance;
        
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptableObject_hpp_
//////////////////////////////////////////
