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
#if (!defined(_MazeFunctionsOpenAL_hpp_))
#define _MazeFunctionsOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
#if (MAZE_DEBUG)
#    define MAZE_DEBUG_AL (1)
#else
#    define MAZE_DEBUG_AL (0)
#endif

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#   undef MAZE_DEBUG_AL
#   define MAZE_DEBUG_AL (0)
#endif


//////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalEnable)(MZALenum _capability);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDisable)(MZALenum _capability);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALboolean (MAZE_AL_FUNCPTR* mzalIsEnabled)(MZALenum _capability);
MAZE_SOUND_SYSTEM_OPENAL_API extern const MZALchar* (MAZE_AL_FUNCPTR* mzalGetString)(MZALenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetBooleanv)(MZALenum _param, MZALboolean* _data);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetIntegerv)(MZALenum _param, MZALint* _data);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetFloatv)(MZALenum _param, MZALfloat* _data);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetDoublev)(MZALenum _param, MZALdouble* _data);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALboolean (MAZE_AL_FUNCPTR* mzalGetBoolean)(MZALenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALint (MAZE_AL_FUNCPTR* mzalGetInteger)(MZALenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALfloat (MAZE_AL_FUNCPTR* mzalGetFloat)(MZALenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALdouble (MAZE_AL_FUNCPTR* mzalGetDouble)(MZALenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALenum (MAZE_AL_FUNCPTR* mzalGetError)(void);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALboolean (MAZE_AL_FUNCPTR* mzalIsExtensionPresent)(MZALchar const* _extname);
MAZE_SOUND_SYSTEM_OPENAL_API extern void* (MAZE_AL_FUNCPTR* mzalGetProcAddress)(MZALchar const* _fname);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALenum (MAZE_AL_FUNCPTR* mzalGetEnumValue)(MZALchar const* _ename);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalListenerf)(MZALenum _param, MZALfloat _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalListener3f)(MZALenum _param, MZALfloat _value1, MZALfloat _value2, MZALfloat _value3);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalListenerfv)(MZALenum _param, const MZALfloat* _values);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalListeneri)(MZALenum _param, MZALint _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetListenerf)(MZALenum _param, MZALfloat* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetListener3f)(MZALenum _param, MZALfloat* _value1, MZALfloat* _value2, MZALfloat* _value3);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetListenerfv)(MZALenum _param, MZALfloat* _values);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetListeneri)(MZALenum _param, MZALint* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGenSources)(MZALsizei _n, MZALuint* _sources);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDeleteSources)(MZALsizei _n, MZALuint const* _sources);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALboolean (MAZE_AL_FUNCPTR* mzalIsSource)(MZALuint _sid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcef)(MZALuint _sid, MZALenum _param, MZALfloat _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSource3f)(MZALuint _sid, MZALenum _param, MZALfloat _value1, MZALfloat _value2, MZALfloat _value3);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcefv)(MZALuint _sid, MZALenum _param, MZALfloat const* _values);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcei)(MZALuint _sid, MZALenum _param, MZALint _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetSourcef)(MZALuint _sid, MZALenum param, MZALfloat* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetSource3f)(MZALuint _sid, MZALenum param, MZALfloat* _value1, MZALfloat* _value2, MZALfloat* _value3);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetSourcefv)(MZALuint _sid, MZALenum _param, MZALfloat* _values);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetSourcei)(MZALuint _sid, MZALenum _param, MZALint* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcePlayv)(MZALsizei _ns, MZALuint const* _sids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceStopv)(MZALsizei _ns, MZALuint const* _sids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceRewindv)(MZALsizei _ns, MZALuint const* _sids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcePausev)(MZALsizei _ns, MZALuint const* _sids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcePlay)(MZALuint _sid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceStop)(MZALuint _sid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceRewind)(MZALuint _sid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourcePause)(MZALuint _sid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceQueueBuffers)(MZALuint _sid, MZALsizei _numEntries, MZALuint const* _bids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalSourceUnqueueBuffers)(MZALuint _sid, MZALsizei _numEntries, MZALuint* _bids);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGenBuffers)(MZALsizei _n, MZALuint* _buffers);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDeleteBuffers)(MZALsizei _n, MZALuint const* _buffers);
MAZE_SOUND_SYSTEM_OPENAL_API extern MZALboolean (MAZE_AL_FUNCPTR* mzalIsBuffer)(MZALuint _bid);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalBufferData)(MZALuint _bid, MZALenum _format, MZALvoid const* _data, MZALsizei _size, MZALsizei _freq);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetBufferf)(MZALuint _bid, MZALenum _param, MZALfloat* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalGetBufferi)(MZALuint _bid, MZALenum _param, MZALint* _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDopplerFactor)(MZALfloat _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDopplerVelocity)(MZALfloat _value);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalDistanceModel)(MZALenum _distanceModel);

MAZE_SOUND_SYSTEM_OPENAL_API extern ALCcontext* (MAZE_AL_FUNCPTR* mzalcCreateContext)(ALCdevice* _device, ALCint const* _attrlist);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCboolean (MAZE_AL_FUNCPTR* mzalcMakeContextCurrent)(ALCcontext* _context);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalcProcessContext)(ALCcontext* _context);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalcSuspendContext)(ALCcontext* _context);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalcDestroyContext)(ALCcontext* _context);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCcontext* (MAZE_AL_FUNCPTR* mzalcGetCurrentContext)(ALCvoid);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCdevice* (MAZE_AL_FUNCPTR* mzalcGetContextsDevice)(ALCcontext* _context);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCdevice* (MAZE_AL_FUNCPTR* mzalcOpenDevice)(ALCchar const* _devicename);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCboolean (MAZE_AL_FUNCPTR* mzalcCloseDevice)(ALCdevice* _device);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCdevice* (MAZE_AL_FUNCPTR* mzalcCaptureOpenDevice)(ALCchar const* _devicename, ALCuint _frequency, ALCenum _format, ALCsizei _buffersize);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalcCaptureStart)(ALCdevice* _device);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCenum (MAZE_AL_FUNCPTR* mzalcGetError)(ALCdevice* _device);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCboolean (MAZE_AL_FUNCPTR* mzalcIsExtensionPresent)(ALCdevice* _device, ALCchar const* _extname);
MAZE_SOUND_SYSTEM_OPENAL_API extern void* (MAZE_AL_FUNCPTR* mzalcGetProcAddress)(ALCdevice* _device, ALCchar const* _funcname);
MAZE_SOUND_SYSTEM_OPENAL_API extern ALCenum (MAZE_AL_FUNCPTR* mzalcGetEnumValue)(ALCdevice* _device, ALCchar const* _enumname);
MAZE_SOUND_SYSTEM_OPENAL_API extern const ALCchar* (MAZE_AL_FUNCPTR* mzalcGetString)(ALCdevice* _device, ALCenum _param);
MAZE_SOUND_SYSTEM_OPENAL_API extern void (MAZE_AL_FUNCPTR* mzalcGetIntegerv)(ALCdevice* _device, ALCenum _param, ALCsizei _size, ALCint* _dest);

#ifdef __cplusplus
}
#endif // __cplusplus


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenAL);


    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API bool AssignFunctionsOpenAL(ContextOpenALPtr const& _soundContext);


    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API bool ALCheckOpenALError(CString _file, U32 _line, CString _expression);
    MAZE_SOUND_SYSTEM_OPENAL_API void ALCallStarted();
    MAZE_SOUND_SYSTEM_OPENAL_API void ALCallEnded(CString _file, U32 _line, CString _expression);


    //////////////////////////////////////////
    #define MAZE_AL_CALL_FORCE(x)   do\
                                    {\
                                        Maze::ALCallStarted();\
                                        x;\
                                        Maze::ALCallEnded(__FILE__, __LINE__, #x);\
                                        Maze::ALCheckOpenALError(__FILE__, __LINE__, #x);\
                                    }\
                                    while (false)

    //////////////////////////////////////////
    #define MAZE_AL_CHECK_ERROR()   do\
                                    {\
                                        Maze::ALCallStarted();\
                                        Maze::ALCallEnded(__FILE__, __LINE__, nullptr);\
                                        Maze::ALCheckOpenGLError(__FILE__, __LINE__, nullptr);\
                                    }\
                                    while (false)


    //////////////////////////////////////////
    #if (MAZE_DEBUG_AL)
    #    define MAZE_AL_CALL(x) MAZE_AL_CALL_FORCE(x)
    #else
    #    define MAZE_AL_CALL(x) x
    #endif

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFunctionsOpenGL_hpp_
//////////////////////////////////////////
