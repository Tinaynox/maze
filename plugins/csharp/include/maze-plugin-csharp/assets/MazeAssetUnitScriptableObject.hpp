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
#if (!defined(_MazeAssetUnitScriptableObject_hpp_))
#define _MazeAssetUnitScriptableObject_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetUnitScriptableObject);   
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(ScriptableObject);


    //////////////////////////////////////////
    // Class AssetUnitScriptableObject
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API AssetUnitScriptableObject
        : public AssetUnit
        , public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitScriptableObject, AssetUnit);
        
        //////////////////////////////////////////
        MAZE_FORCEINLINE static HashedCString GetDataBlockId() { return MAZE_HCS("scriptableObject"); }

    public:

        //////////////////////////////////////////
        virtual ~AssetUnitScriptableObject();

        //////////////////////////////////////////
        virtual HashedCString getDataBlockId() const MAZE_OVERRIDE { return GetDataBlockId(); }

        //////////////////////////////////////////
        static AssetUnitScriptableObjectPtr Create(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data = DataBlock::c_empty);


        //////////////////////////////////////////
        inline ScriptableObjectPtr const& getScriptableObject() const { return m_scriptableObject; }

        //////////////////////////////////////////
        ScriptableObjectPtr const& loadScriptableObject(bool _syncLoad = false);


        //////////////////////////////////////////
        ScriptableObjectPtr const& initScriptableObject();

    protected:

        //////////////////////////////////////////
        AssetUnitScriptableObject();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_OVERRIDE;
    
        
        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    protected:
        ScriptableObjectPtr m_scriptableObject;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitScriptableObject_hpp_
//////////////////////////////////////////
