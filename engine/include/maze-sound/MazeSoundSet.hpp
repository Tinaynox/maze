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
#if (!defined(_MazeSoundSet_hpp_))
#define _MazeSoundSet_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-sound/MazeSound.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSet);


    //////////////////////////////////////////
    // Class SoundSet
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundSet
        : public SharedObject<SoundSet>
        , public IJSONValueSerializable
        , public IDataBlockSerializable
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(SoundSet);

    public:

        //////////////////////////////////////////
        SoundSet(
            Vector<SoundPtr> const& _sounds = Vector<SoundPtr>(),
            F32 _volume = 1.0f);

        //////////////////////////////////////////
        SoundSet(
            SoundPtr const& _sound,
            F32 _volume = 1.0f);

        //////////////////////////////////////////
        SoundSet(
            CString _soundName,
            F32 _volume = 1.0f);

        //////////////////////////////////////////
        SoundSet(
            CString _soundName00,
            CString _soundName01,
            F32 _volume = 1.0f);


        //////////////////////////////////////////
        inline void setSounds(Vector<SoundPtr> const& _value) { m_sounds = _value; }

        //////////////////////////////////////////
        inline Vector<SoundPtr> const& getSounds() const { return m_sounds; }

        //////////////////////////////////////////
        void addSound(SoundPtr const& _sound);

        //////////////////////////////////////////
        void setSounds(Vector<String> _value);

        //////////////////////////////////////////
        Vector<String> getSoundNames() const;


        //////////////////////////////////////////
        inline void setVolume(F32 _value) { m_volume = _value; }

        //////////////////////////////////////////
        inline F32 getVolume() const { return m_volume; }


        //////////////////////////////////////////
        inline void setNoRepeats(bool _value) { m_noRepeats = _value; }

        //////////////////////////////////////////
        inline bool getNoRepeats() const { return m_noRepeats; }


        //////////////////////////////////////////
        inline void setMinPitch(F32 _value) { m_minPitch = _value; }

        //////////////////////////////////////////
        inline F32 getMinPitch() const { return m_minPitch; }

        //////////////////////////////////////////
        inline void setMaxPitch(F32 _value) { m_maxPitch = _value; }

        //////////////////////////////////////////
        inline F32 getMaxPitch() const { return m_maxPitch; }


        //////////////////////////////////////////
        SoundPtr const& fetch();

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
        {
            JSONHelper::DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
        }

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE
        {
            return JSONHelper::SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
        }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
        {
            DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            return true;
        }

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
        {
            DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
        }

    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        static void FromString(SoundSetPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(SoundSet const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        inline void setPrevFetchIndex(S32 _value) { m_prevFetchIndex = _value; }

        //////////////////////////////////////////
        inline S32 getPrevFetchIndex() const { return m_prevFetchIndex; }

    protected:
        Vector<SoundPtr> m_sounds;
        F32 m_volume = 1.0f;
        S32 m_prevFetchIndex = -1;
        bool m_noRepeats = false;
        F32 m_minPitch = 1.0f;
        F32 m_maxPitch = 1.0f;
    };


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SoundSetPtr>::value), void>::type
        ValueToString(SoundSetPtr const& _value, String& _data)
    {
        SoundSet::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SoundSetPtr>::value), void>::type
        ValueFromString(SoundSetPtr& _value, CString _data, Size _count)
    {
        SoundSet::FromString(_value, _data, _count);
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSet_hpp_
//////////////////////////////////////////
