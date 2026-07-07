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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsSound.hpp"
#include "maze-sound/ecs/components/MazeAudioSource.hpp"
#include "maze-sound/ecs/components/MazeAudioListener.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // AudioSource
    //////////////////////////////////////////
    inline MonoString* AudioSourceGetClipName(Component* _component)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT_RETURN_VALUE(AudioSource, nullptr);

        SoundPtr const& clip = _component->castRaw<AudioSource>()->getClip();
        if (!clip)
            return nullptr;

        return mono_string_new(mono_domain_get(), clip->getName().c_str());
    }

    //////////////////////////////////////////
    inline void AudioSourceSetClipByName(Component* _component, MonoString* _name)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);

        Char* cstr = mono_string_to_utf8(_name);
        SoundPtr const& sound = SoundManager::GetInstancePtr()->getOrLoadSound(cstr);
        _component->castRaw<AudioSource>()->setClip(sound);
        mono_free(cstr);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetVolume(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getVolume();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetVolume(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setVolume(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetPitch(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getPitch();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetPitch(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setPitch(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetLoop(Component* _component, bool& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getLoop();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetLoop(Component* _component, bool _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setLoop(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetMute(Component* _component, bool& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getMute();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetMute(Component* _component, bool _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setMute(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetPlayOnAwake(Component* _component, bool& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getPlayOnAwake();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetPlayOnAwake(Component* _component, bool _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setPlayOnAwake(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetSpatialBlend(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getSpatialBlend();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetSpatialBlend(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setSpatialBlend(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetMinDistance(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getMinDistance();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetMinDistance(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setMinDistance(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetMaxDistance(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getMaxDistance();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetMaxDistance(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setMaxDistance(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourceGetRolloffFactor(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->getRolloffFactor();
    }

    //////////////////////////////////////////
    inline void AudioSourceSetRolloffFactor(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->setRolloffFactor(_value);
    }

    //////////////////////////////////////////
    inline void AudioSourcePlay(Component* _component)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->play();
    }

    //////////////////////////////////////////
    inline void AudioSourceStop(Component* _component)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _component->castRaw<AudioSource>()->stop();
    }

    //////////////////////////////////////////
    inline void AudioSourceIsPlaying(Component* _component, bool& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioSource);
        _outValue = _component->castRaw<AudioSource>()->isPlaying();
    }


    //////////////////////////////////////////
    // AudioListener
    //////////////////////////////////////////
    inline void AudioListenerGetActive(Component* _component, bool& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioListener);
        _outValue = _component->castRaw<AudioListener>()->getActive();
    }

    //////////////////////////////////////////
    inline void AudioListenerSetActive(Component* _component, bool _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(AudioListener);
        _component->castRaw<AudioListener>()->setActive(_value);
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsSound()
    {
        // AudioSource
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetClipName);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetClipByName);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetVolume);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetVolume);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetPitch);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetPitch);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetLoop);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetLoop);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetMute);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetMute);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetPlayOnAwake);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetPlayOnAwake);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetSpatialBlend);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetSpatialBlend);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetMinDistance);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetMinDistance);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetMaxDistance);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetMaxDistance);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceGetRolloffFactor);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceSetRolloffFactor);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourcePlay);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceStop);
        MAZE_SOUND_MONO_BIND_FUNC(AudioSourceIsPlaying);

        // AudioListener
        MAZE_SOUND_MONO_BIND_FUNC(AudioListenerGetActive);
        MAZE_SOUND_MONO_BIND_FUNC(AudioListenerSetActive);
    }

} // namespace Maze
//////////////////////////////////////////
