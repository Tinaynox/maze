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
#include "MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"
#include "maze-sound-system-openal/MazeContextOpenAL.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalEnable)(ALenum _capability) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDisable)(ALenum _capability) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALboolean (MAZE_AL_FUNCPTR* mzalIsEnabled)(ALenum _capability) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API const ALchar* (MAZE_AL_FUNCPTR* mzalGetString)(ALenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetBooleanv)(ALenum _param, ALboolean* _data) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetIntegerv)(ALenum _param, ALint* _data) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetFloatv)(ALenum _param, ALfloat* _data) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetDoublev)(ALenum _param, ALdouble* _data) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALboolean (MAZE_AL_FUNCPTR* mzalGetBoolean)(ALenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALint (MAZE_AL_FUNCPTR* mzalGetInteger)(ALenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALfloat (MAZE_AL_FUNCPTR* mzalGetFloat)(ALenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALdouble (MAZE_AL_FUNCPTR* mzalGetDouble)(ALenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALenum (MAZE_AL_FUNCPTR* mzalGetError)(void) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALboolean (MAZE_AL_FUNCPTR* mzalIsExtensionPresent)(ALchar const* _extname) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void* (MAZE_AL_FUNCPTR* mzalGetProcAddress)(ALchar const* _fname) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALenum (MAZE_AL_FUNCPTR* mzalGetEnumValue)(ALchar const* _ename) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalListenerf)(ALenum _param, ALfloat _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalListener3f)(ALenum _param, ALfloat _value1, ALfloat _value2, ALfloat _value3) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalListenerfv)(ALenum _param, const ALfloat* _values) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalListeneri)(ALenum _param, ALint _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetListenerf)(ALenum _param, ALfloat* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetListener3f)(ALenum _param, ALfloat* _value1, ALfloat* _value2, ALfloat* _value3) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetListenerfv)(ALenum _param, ALfloat* _values) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetListeneri)(ALenum _param, ALint* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGenSources)(ALsizei _n, ALuint* _sources) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDeleteSources)(ALsizei _n, ALuint const* _sources) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALboolean (MAZE_AL_FUNCPTR* mzalIsSource)(ALuint _sid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcef)(ALuint _sid, ALenum _param, ALfloat _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSource3f)(ALuint _sid, ALenum _param, ALfloat _value1, ALfloat _value2, ALfloat _value3) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcefv)(ALuint _sid, ALenum _param, ALfloat const* _values) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcei)(ALuint _sid, ALenum _param, ALint _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetSourcef)(ALuint _sid, ALenum param, ALfloat* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetSource3f)(ALuint _sid, ALenum param, ALfloat* _value1, ALfloat* _value2, ALfloat* _value3) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetSourcefv)(ALuint _sid, ALenum _param, ALfloat* _values) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetSourcei)(ALuint _sid, ALenum _param, ALint* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcePlayv)(ALsizei _ns, ALuint const* _sids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceStopv)(ALsizei _ns, ALuint const* _sids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceRewindv)(ALsizei _ns, ALuint const* _sids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcePausev)(ALsizei _ns, ALuint const* _sids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcePlay)(ALuint _sid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceStop)(ALuint _sid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceRewind)(ALuint _sid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourcePause)(ALuint _sid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceQueueBuffers)(ALuint _sid, ALsizei _numEntries, ALuint const* _bids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalSourceUnqueueBuffers)(ALuint _sid, ALsizei _numEntries, ALuint* _bids) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGenBuffers)(ALsizei _n, ALuint* _buffers) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDeleteBuffers)(ALsizei _n, ALuint const* _buffers) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALboolean (MAZE_AL_FUNCPTR* mzalIsBuffer)(ALuint _bid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalBufferData)(ALuint _bid, ALenum _format, ALvoid const* _data, ALsizei _size, ALsizei _freq) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetBufferf)(ALuint _bid, ALenum _param, ALfloat* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalGetBufferi)(ALuint _bid, ALenum _param, ALint* _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDopplerFactor)(ALfloat _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDopplerVelocity)(ALfloat _value) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalDistanceModel)(ALenum _distanceModel) = nullptr;

MAZE_SOUND_SYSTEM_OPENAL_API ALCcontext* (MAZE_AL_FUNCPTR* mzalcCreateContext)(ALCdevice* _device, ALCint const* _attrlist) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCboolean (MAZE_AL_FUNCPTR* mzalcMakeContextCurrent)(ALCcontext* _context) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalcProcessContext)(ALCcontext* _context) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalcSuspendContext)(ALCcontext* _context) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalcDestroyContext)(ALCcontext* _context) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCcontext* (MAZE_AL_FUNCPTR* mzalcGetCurrentContext)(ALCvoid) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCdevice* (MAZE_AL_FUNCPTR* mzalcGetContextsDevice)(ALCcontext* _context) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCdevice* (MAZE_AL_FUNCPTR* mzalcOpenDevice)(ALCchar const* _devicename) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCboolean (MAZE_AL_FUNCPTR* mzalcCloseDevice)(ALCdevice* _device) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCenum (MAZE_AL_FUNCPTR* mzalcGetError)(ALCdevice* _device) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCboolean (MAZE_AL_FUNCPTR* mzalcIsExtensionPresent)(ALCdevice* _device, ALCchar const* _extname) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void* (MAZE_AL_FUNCPTR* mzalcGetProcAddress)(ALCdevice* _device, ALCchar const* _funcname) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API ALCenum (MAZE_AL_FUNCPTR* mzalcGetEnumValue)(ALCdevice* _device, ALCchar const* _enumname) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API const ALCchar* (MAZE_AL_FUNCPTR* mzalcGetString)(ALCdevice* _device, ALCenum _param) = nullptr;
MAZE_SOUND_SYSTEM_OPENAL_API void (MAZE_AL_FUNCPTR* mzalcGetIntegerv)(ALCdevice* _device, ALCenum _param, ALCsizei _size, ALCint* _dest) = nullptr;


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename TALFunction>
    inline bool AssignOpenALFunction(ContextOpenALPtr const& _soundContext, TALFunction*& _functionPointer, CString _functionName)
    {
        _functionPointer = reinterpret_cast<TALFunction*>(_soundContext->getFunction(_functionName));

#if (MAZE_PLATFORM != MAZE_PLATFORM_EMSCRIPTEN)
        Debug::log << "mz" << _functionName << " = " << (_functionPointer != nullptr ? _functionName : "null") << endl;
#endif

        return _functionPointer != nullptr ? true : false;
    }

    //////////////////////////////////////////
    template <typename TALFunction>
    inline bool AssignOpenALFunction(ContextOpenALPtr const& _soundContext, TALFunction*& _functionPointer, TALFunction const* const _alFunctionPointer)
    {
        _functionPointer = _alFunctionPointer;
        return true;
    }

    //////////////////////////////////////////
    template <typename TALFunction>
    inline bool AssignOpenALFunction(ContextOpenALPtr const& _soundContext, TALFunction*& _functionPointer, nullptr_t)
    {
        _functionPointer = nullptr;
        return true;
    }

    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API bool AssignFunctionsOpenAL(ContextOpenALPtr const& _soundContext)
    {
#if MAZE_PLATFORM == MAZE_PLATFORM_OSX
        mzalEnable = alEnable;
        mzalDisable = alDisable;
        mzalIsEnabled = alIsEnabled;
        mzalGetBoolean = alGetBoolean;
        mzalGetInteger = alGetInteger;
        mzalGetFloat = alGetFloat;
        mzalGetDouble = alGetDouble;
        mzalGetBooleanv = alGetBooleanv;
        mzalGetIntegerv = alGetIntegerv;
        mzalGetFloatv = alGetFloatv;
        mzalGetDoublev = alGetDoublev;
        mzalGetString = alGetString;
        mzalGetError = alGetError;
        mzalIsExtensionPresent = alIsExtensionPresent;
        mzalGetProcAddress = alGetProcAddress;
        mzalGetEnumValue = alGetEnumValue;
        mzalListeneri = alListeneri;
        mzalListenerf = alListenerf;
        mzalListener3f = alListener3f;
        mzalListenerfv = alListenerfv;
        mzalGetListeneri = alGetListeneri;
        mzalGetListenerf = alGetListenerf;
        mzalGetListener3f = alGetListener3f;
        mzalGetListenerfv = alGetListenerfv;
        mzalGenSources = alGenSources;
        mzalDeleteSources = alDeleteSources;
        mzalIsSource = alIsSource;
        mzalSourcei = alSourcei;
        mzalSourcef = alSourcef;
        mzalSource3f = alSource3f;
        mzalSourcefv = alSourcefv;
        mzalGetSourcei = alGetSourcei;
        mzalGetSourcef = alGetSourcef;
        mzalGetSourcefv = alGetSourcefv;
        mzalSourcePlayv = alSourcePlayv;
        mzalSourceStopv = alSourceStopv;
        mzalSourceRewindv = alSourceRewindv;
        mzalSourcePlay = alSourcePlay;
        mzalSourcePause = alSourcePause;
        mzalSourceStop = alSourceStop;
        mzalSourceRewind = alSourceRewind;
        mzalGenBuffers = alGenBuffers;
        mzalDeleteBuffers = alDeleteBuffers;
        mzalIsBuffer = alIsBuffer;
        mzalBufferData = alBufferData;
        mzalGetBufferi = alGetBufferi;
        mzalGetBufferf = alGetBufferf;
        mzalSourceQueueBuffers = alSourceQueueBuffers;
        mzalSourceUnqueueBuffers = alSourceUnqueueBuffers;
        mzalDistanceModel = alDistanceModel;
        mzalDopplerFactor = alDopplerFactor;
        mzalDopplerVelocity = alDopplerVelocity;

        mzalcGetString = alcGetString;
        mzalcGetIntegerv = alcGetIntegerv;
        mzalcOpenDevice = alcOpenDevice;
        mzalcCloseDevice = alcCloseDevice;
        mzalcCreateContext = alcCreateContext;
        mzalcMakeContextCurrent = alcMakeContextCurrent;
        mzalcProcessContext = alcProcessContext;
        mzalcGetCurrentContext = alcGetCurrentContext;
        mzalcGetContextsDevice = alcGetContextsDevice;
        mzalcSuspendContext = alcSuspendContext;
        mzalcDestroyContext = alcDestroyContext;
        mzalcGetError = alcGetError;
        mzalcIsExtensionPresent = alcIsExtensionPresent;
        mzalcGetProcAddress = alcGetProcAddress;
        mzalcGetEnumValue = alcGetEnumValue;
#else
        AssignOpenALFunction(_soundContext, mzalEnable, "alEnable");
        AssignOpenALFunction(_soundContext, mzalDisable, "alDisable");
        AssignOpenALFunction(_soundContext, mzalIsEnabled, "alIsEnabled");
        AssignOpenALFunction(_soundContext, mzalGetBoolean, "alGetBoolean");
        AssignOpenALFunction(_soundContext, mzalGetInteger, "alGetInteger");
        AssignOpenALFunction(_soundContext, mzalGetFloat, "alGetFloat");
        AssignOpenALFunction(_soundContext, mzalGetDouble, "alGetDouble");
        AssignOpenALFunction(_soundContext, mzalGetBooleanv, "alGetBooleanv");
        AssignOpenALFunction(_soundContext, mzalGetIntegerv, "alGetIntegerv");
        AssignOpenALFunction(_soundContext, mzalGetFloatv, "alGetFloatv");
        AssignOpenALFunction(_soundContext, mzalGetDoublev, "alGetDoublev");
        AssignOpenALFunction(_soundContext, mzalGetString, "alGetString");
        AssignOpenALFunction(_soundContext, mzalGetError, "alGetError");
        AssignOpenALFunction(_soundContext, mzalIsExtensionPresent, "alIsExtensionPresent");
        AssignOpenALFunction(_soundContext, mzalGetProcAddress, "alGetProcAddress");
        AssignOpenALFunction(_soundContext, mzalGetEnumValue, "alGetEnumValue");
        AssignOpenALFunction(_soundContext, mzalListeneri, "alListeneri");
        AssignOpenALFunction(_soundContext, mzalListenerf, "alListenerf");
        AssignOpenALFunction(_soundContext, mzalListener3f, "alListener3f");
        AssignOpenALFunction(_soundContext, mzalListenerfv, "alListenerfv");
        AssignOpenALFunction(_soundContext, mzalGetListeneri, "alGetListeneri");
        AssignOpenALFunction(_soundContext, mzalGetListenerf, "alGetListenerf");
        AssignOpenALFunction(_soundContext, mzalGetListener3f, "alGetListener3f");
        AssignOpenALFunction(_soundContext, mzalGetListenerfv, "alGetListenerfv");
        AssignOpenALFunction(_soundContext, mzalGenSources, "alGenSources");
        AssignOpenALFunction(_soundContext, mzalDeleteSources, "alDeleteSources");
        AssignOpenALFunction(_soundContext, mzalIsSource, "alIsSource");
        AssignOpenALFunction(_soundContext, mzalSourcei, "alSourcei");
        AssignOpenALFunction(_soundContext, mzalSourcef, "alSourcef");
        AssignOpenALFunction(_soundContext, mzalSource3f, "alSource3f");
        AssignOpenALFunction(_soundContext, mzalSourcefv, "alSourcefv");
        AssignOpenALFunction(_soundContext, mzalGetSourcei, "alGetSourcei");
        AssignOpenALFunction(_soundContext, mzalGetSourcef, "alGetSourcef");
        AssignOpenALFunction(_soundContext, mzalGetSourcefv, "alGetSourcefv");
        AssignOpenALFunction(_soundContext, mzalSourcePlayv, "alSourcePlayv");
        AssignOpenALFunction(_soundContext, mzalSourceStopv, "alSourceStopv");
        AssignOpenALFunction(_soundContext, mzalSourceRewindv, "alSourceRewindv");
        AssignOpenALFunction(_soundContext, mzalSourcePlay, "alSourcePlay");
        AssignOpenALFunction(_soundContext, mzalSourcePause, "alSourcePause");
        AssignOpenALFunction(_soundContext, mzalSourceStop, "alSourceStop");
        AssignOpenALFunction(_soundContext, mzalSourceRewind, "alSourceRewind");
        AssignOpenALFunction(_soundContext, mzalGenBuffers, "alGenBuffers");
        AssignOpenALFunction(_soundContext, mzalDeleteBuffers, "alDeleteBuffers");
        AssignOpenALFunction(_soundContext, mzalIsBuffer, "alIsBuffer");
        AssignOpenALFunction(_soundContext, mzalBufferData, "alBufferData");
        AssignOpenALFunction(_soundContext, mzalGetBufferi, "alGetBufferi");
        AssignOpenALFunction(_soundContext, mzalGetBufferf, "alGetBufferf");
        AssignOpenALFunction(_soundContext, mzalSourceQueueBuffers, "alSourceQueueBuffers");
        AssignOpenALFunction(_soundContext, mzalSourceUnqueueBuffers, "alSourceUnqueueBuffers");
        AssignOpenALFunction(_soundContext, mzalDistanceModel, "alDistanceModel");
        AssignOpenALFunction(_soundContext, mzalDopplerFactor, "alDopplerFactor");
        AssignOpenALFunction(_soundContext, mzalDopplerVelocity, "alDopplerVelocity");

        AssignOpenALFunction(_soundContext, mzalcGetString, "alcGetString");
        AssignOpenALFunction(_soundContext, mzalcGetIntegerv, "alcGetIntegerv");
        AssignOpenALFunction(_soundContext, mzalcOpenDevice, "alcOpenDevice");
        AssignOpenALFunction(_soundContext, mzalcCloseDevice, "alcCloseDevice");
        AssignOpenALFunction(_soundContext, mzalcCreateContext, "alcCreateContext");
        AssignOpenALFunction(_soundContext, mzalcMakeContextCurrent, "alcMakeContextCurrent");
        AssignOpenALFunction(_soundContext, mzalcProcessContext, "alcProcessContext");
        AssignOpenALFunction(_soundContext, mzalcGetCurrentContext, "alcGetCurrentContext");
        AssignOpenALFunction(_soundContext, mzalcGetContextsDevice, "alcGetContextsDevice");
        AssignOpenALFunction(_soundContext, mzalcSuspendContext, "alcSuspendContext");
        AssignOpenALFunction(_soundContext, mzalcDestroyContext, "alcDestroyContext");
        AssignOpenALFunction(_soundContext, mzalcGetError, "alcGetError");
        AssignOpenALFunction(_soundContext, mzalcIsExtensionPresent, "alcIsExtensionPresent");
        AssignOpenALFunction(_soundContext, mzalcGetProcAddress, "alcGetProcAddress");
        AssignOpenALFunction(_soundContext, mzalcGetEnumValue, "alcGetEnumValue");
#endif
       
        return true;
    }

    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API bool ALCheckOpenALError(CString _file, U32 _line, CString _expression)
    {       
        bool hasErrors = false;

        do 
        {
            ALenum error = alGetError();
            if (error == AL_NO_ERROR) 
                break;

            String fileString = _file;
            fileString = fileString.substr(fileString.find_last_of("\\/") + 1);

            CString errorText = "";
            switch (error) 
            {
                case AL_INVALID_ENUM:                      errorText = "AL_INVALID_ENUM";      break;
                case AL_INVALID_VALUE:                     errorText = "AL_INVALID_VALUE";     break;
                case AL_INVALID_OPERATION:                 errorText = "AL_INVALID_OPERATION"; break;
                case AL_OUT_OF_MEMORY:                     errorText = "AL_OUT_OF_MEMORY";     break;
                default:                                   errorText = "Unknown OpenAL error"; break;
            }

            if (_expression)
            {
                MAZE_ERROR("OpenAL Error!\n\tFile: %s(%u).\n\tnExpression:%s\n\tError:%s", fileString.c_str(), _line, _expression, errorText);
            }
            else
            {
                MAZE_ERROR("OpenAL Error!\n\tFile: %s(%u).\n\tError:%s", fileString.c_str(), _line, errorText);
            }

            hasErrors = true;
        } 
        while (false);
            
        return hasErrors;
    }

    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API void ALCallStarted()
    {
        ALenum error = alGetError();
        MAZE_ERROR_IF(error != AL_NO_ERROR, "AL Error occurred before MAZE_AL_CALL call!") ;
    }

    ////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API void ALCallEnded(CString _file, U32 _line, CString _expression)
    {
        
    }


} // namespace Maze
//////////////////////////////////////////
