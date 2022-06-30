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
#include "MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeFunctionsOpenGL3.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignOpenGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& _functionPointer, CString _functionName)
    {
        _functionPointer = reinterpret_cast<TGLFunction*>(_renderContext->getFunction(_functionName));

#if (MAZE_PLATFORM != MAZE_PLATFORM_EMSCRIPTEN)
        Debug::log << "mz" << _functionName << " = " << (_functionPointer != nullptr ? _functionName : "null") << endl;
#endif
        
        return _functionPointer != nullptr ? true
                                           : false;
    }

    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignOpenGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& _functionPointer, TGLFunction const* const _glFunctionPointer)
    {
        _functionPointer = _glFunctionPointer;
        return true;
    }

    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignOpenGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& _functionPointer, nullptr_t)
    {
        _functionPointer = nullptr;
        return true;
    }

    //////////////////////////////////////////
    bool AssignFunctionsOpenGL(ContextOpenGLPtr const& _renderContext)
    {
        return false;
    }

    //////////////////////////////////////////
    bool AssignFunctionsOpenGL3(ContextOpenGLPtr const& _renderContext)
    {        
    
//#if (!defined(MAZE_RS_OPENGL_ES))
        AssignOpenGLFunction(_renderContext, mzglEnable, "glEnable");
        AssignOpenGLFunction(_renderContext, mzglDisable, "glDisable");
        AssignOpenGLFunction(_renderContext, mzglEnablei, "glEnablei");
        AssignOpenGLFunction(_renderContext, mzglDisablei, "glDisablei");
        AssignOpenGLFunction(_renderContext, mzglBlendFunc, "glBlendFunc");
        AssignOpenGLFunction(_renderContext, mzglBlendFunci, "glBlendFunci");
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparate, "glBlendFuncSeparate");
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparatei, "glBlendFuncSeparatei");
        AssignOpenGLFunction(_renderContext, mzglBlendEquation, "glBlendEquation");
        AssignOpenGLFunction(_renderContext, mzglBlendEquationi, "glBlendEquationi");
        AssignOpenGLFunction(_renderContext, mzglBindRenderbuffer, "glBindRenderbuffer");
        AssignOpenGLFunction(_renderContext, mzglGenFramebuffers, "glGenFramebuffers");
        AssignOpenGLFunction(_renderContext, mzglDiscardFramebuffer, "glDiscardFramebufferEXT");
        AssignOpenGLFunction(_renderContext, mzglDeleteRenderbuffers, "glDeleteRenderbuffers");
        AssignOpenGLFunction(_renderContext, mzglFramebufferRenderbuffer, "glFramebufferRenderbuffer");
        AssignOpenGLFunction(_renderContext, mzglBindFramebuffer, "glBindFramebuffer");
        AssignOpenGLFunction(_renderContext, mzglDrawBuffers, "glDrawBuffers");
        AssignOpenGLFunction(_renderContext, mzglFramebufferTexture2D, "glFramebufferTexture2D");
        AssignOpenGLFunction(_renderContext, mzglDeleteFramebuffers, "glDeleteFramebuffers");
        AssignOpenGLFunction(_renderContext, mzglFlush, "glFlush");
        AssignOpenGLFunction(_renderContext, mzglFinish, "glFinish");
        AssignOpenGLFunction(_renderContext, mzglGetError, "glGetError");
        AssignOpenGLFunction(_renderContext, mzglGetFloatv, "glGetFloatv");
        AssignOpenGLFunction(_renderContext, mzglGetIntegerv, "glGetIntegerv");
        AssignOpenGLFunction(_renderContext, mzglGetDoublev, "glGetDoublev");
        AssignOpenGLFunction(_renderContext, mzglGetBufferParameteriv, "glGetBufferParameteriv");
        AssignOpenGLFunction(_renderContext, mzglGetProgramiv, "glGetProgramiv");
        AssignOpenGLFunction(_renderContext, mzglGetUniformLocation, "glGetUniformLocation");
        AssignOpenGLFunction(_renderContext, mzglGetActiveUniform, "glGetActiveUniform");
        AssignOpenGLFunction(_renderContext, mzglGetAttribLocation, "glGetAttribLocation");
        AssignOpenGLFunction(_renderContext, mzglGetShaderiv, "glGetShaderiv");
        AssignOpenGLFunction(_renderContext, mzglGetUniformfv, "glGetUniformfv");
        AssignOpenGLFunction(_renderContext, mzglGetShaderInfoLog, "glGetShaderInfoLog");
        AssignOpenGLFunction(_renderContext, mzglGetProgramInfoLog, "glGetProgramInfoLog");
        AssignOpenGLFunction(_renderContext, mzglGetString, "glGetString");
        AssignOpenGLFunction(_renderContext, mzglGetStringi, "glGetStringi");
        AssignOpenGLFunction(_renderContext, mzglIsEnabled, "glIsEnabled");
        AssignOpenGLFunction(_renderContext, mzglEnableVertexAttribArray, "glEnableVertexAttribArray");
        AssignOpenGLFunction(_renderContext, mzglDisableVertexAttribArray, "glDisableVertexAttribArray");
        AssignOpenGLFunction(_renderContext, mzglViewport, "glViewport");
        AssignOpenGLFunction(_renderContext, mzglScissor, "glScissor");
        AssignOpenGLFunction(_renderContext, mzglActiveTexture, "glActiveTexture");
        AssignOpenGLFunction(_renderContext, mzglBindTexture, "glBindTexture");
        AssignOpenGLFunction(_renderContext, mzglCreateProgram, "glCreateProgram");
        AssignOpenGLFunction(_renderContext, mzglCreateShader, "glCreateShader");
        AssignOpenGLFunction(_renderContext, mzglShaderSource, "glShaderSource");
        AssignOpenGLFunction(_renderContext, mzglCompileShader, "glCompileShader");
        AssignOpenGLFunction(_renderContext, mzglUseProgram, "glUseProgram");
        AssignOpenGLFunction(_renderContext, mzglDeleteProgram, "glDeleteProgram");
        AssignOpenGLFunction(_renderContext, mzglAttachShader, "glAttachShader");
        AssignOpenGLFunction(_renderContext, mzglLinkProgram, "glLinkProgram");
        AssignOpenGLFunction(_renderContext, mzglBindAttribLocation, "glBindAttribLocation");
        AssignOpenGLFunction(_renderContext, mzglDetachShader, "glDetachShader");
        AssignOpenGLFunction(_renderContext, mzglDeleteShader, "glDeleteShader");
        AssignOpenGLFunction(_renderContext, mzglUniform1i, "glUniform1i");
        AssignOpenGLFunction(_renderContext, mzglUniform2i, "glUniform2i");
        AssignOpenGLFunction(_renderContext, mzglUniform3i, "glUniform3i");
        AssignOpenGLFunction(_renderContext, mzglUniform4i, "glUniform4i");
        AssignOpenGLFunction(_renderContext, mzglUniform1ui, "glUniform1ui");
        AssignOpenGLFunction(_renderContext, mzglUniform2ui, "glUniform2ui");
        AssignOpenGLFunction(_renderContext, mzglUniform3ui, "glUniform3ui");
        AssignOpenGLFunction(_renderContext, mzglUniform4ui, "glUniform4ui");
        AssignOpenGLFunction(_renderContext, mzglUniform1f, "glUniform1f");
        AssignOpenGLFunction(_renderContext, mzglUniform2f, "glUniform2f");
        AssignOpenGLFunction(_renderContext, mzglUniform3f, "glUniform3f");
        AssignOpenGLFunction(_renderContext, mzglUniform4f, "glUniform4f");
        AssignOpenGLFunction(_renderContext, mzglUniform1fv, "glUniform1fv");
        AssignOpenGLFunction(_renderContext, mzglUniform2fv, "glUniform2fv");
        AssignOpenGLFunction(_renderContext, mzglUniform3fv, "glUniform3fv");
        AssignOpenGLFunction(_renderContext, mzglUniform4fv, "glUniform4fv");
        AssignOpenGLFunction(_renderContext, mzglUniform1iv, "glUniform1iv");
        AssignOpenGLFunction(_renderContext, mzglUniform2iv, "glUniform2iv");
        AssignOpenGLFunction(_renderContext, mzglUniform3iv, "glUniform3iv");
        AssignOpenGLFunction(_renderContext, mzglUniform4iv, "glUniform4iv");
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix3fv, "glUniformMatrix3fv");
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix4fv, "glUniformMatrix4fv");
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribiv, "glGetVertexAttribivARB");
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribPointerv, "glGetVertexAttribPointervARB");
        AssignOpenGLFunction(_renderContext, mzglGenQueries, "glGenQueries");
        AssignOpenGLFunction(_renderContext, mzglBeginQuery, "glBeginQuery");
        AssignOpenGLFunction(_renderContext, mzglEndQuery, "glEndQuery");
        AssignOpenGLFunction(_renderContext, mzglGetQueryObjectuiv, "glGetQueryObjectuiv");
        AssignOpenGLFunction(_renderContext, mzglGenBuffers, "glGenBuffers");
        AssignOpenGLFunction(_renderContext, mzglDeleteBuffers, "glDeleteBuffers");
        AssignOpenGLFunction(_renderContext, mzglClear, "glClear");
        AssignOpenGLFunction(_renderContext, mzglClearColor, "glClearColor");        
        AssignOpenGLFunction(_renderContext, mzglClearDepth, "glClearDepth");
        AssignOpenGLFunction(_renderContext, mzglVertexAttribPointer, "glVertexAttribPointer");
        AssignOpenGLFunction(_renderContext, mzglBindBuffer, "glBindBuffer");
        AssignOpenGLFunction(_renderContext, mzglBufferData, "glBufferData");
        AssignOpenGLFunction(_renderContext, mzglBufferSubData, "glBufferSubData");
        AssignOpenGLFunction(_renderContext, mzglBufferStorage, "glBufferStorage");
        AssignOpenGLFunction(_renderContext, mzglDrawElements, "glDrawElements");
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstanced, "glDrawElementsInstanced");
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstancedBaseVertex, "glDrawElementsInstancedBaseVertex");
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElements, "glDrawRangeElements");
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElementsBaseVertex, "glDrawRangeElementsBaseVertex");
        AssignOpenGLFunction(_renderContext, mzglDrawArrays, "glDrawArrays");
        AssignOpenGLFunction(_renderContext, mzglDrawArraysInstanced, "glDrawArraysInstanced");
        AssignOpenGLFunction(_renderContext, mzglVertexAttribDivisor, "glVertexAttribDivisor");
        AssignOpenGLFunction(_renderContext, mzglMapBufferRange, "glMapBufferRange");
        AssignOpenGLFunction(_renderContext, mzglFlushMappedBufferRange, "glFlushMappedBufferRange");
        AssignOpenGLFunction(_renderContext, mzglPixelStorei, "glPixelStorei");
        AssignOpenGLFunction(_renderContext, mzglGenTextures, "glGenTextures");
        AssignOpenGLFunction(_renderContext, mzglDeleteTextures, "glDeleteTextures");
        AssignOpenGLFunction(_renderContext, mzglIsTexture, "glIsTexture");
        AssignOpenGLFunction(_renderContext, mzglGenerateMipmap, "glGenerateMipmap");
        AssignOpenGLFunction(_renderContext, mzglTexParameteri, "glTexParameteri");
        AssignOpenGLFunction(_renderContext, mzglCompressedTexImage2D, "glCompressedTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglGetTexLevelParameteriv, "glGetTexLevelParameteriv");
        AssignOpenGLFunction(_renderContext, mzglTexImage2D, "glTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglTexImage2DMultisample, "glTexImage2DMultisample");
        AssignOpenGLFunction(_renderContext, mzglTexStorage2DMultisample, "glTexStorage2DMultisample");
        AssignOpenGLFunction(_renderContext, mzglGetTexImage, "glGetTexImage");
        AssignOpenGLFunction(_renderContext, mzglCheckFramebufferStatus, "glCheckFramebufferStatus");
        AssignOpenGLFunction(_renderContext, mzglBlitFramebuffer, "glBlitFramebuffer");
        AssignOpenGLFunction(_renderContext, mzglCopyTexImage2D, "glCopyTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglTexSubImage2D, "glTexSubImage2D");
        AssignOpenGLFunction(_renderContext, mzglReadPixels, "glReadPixels");
        AssignOpenGLFunction(_renderContext, mzglGenVertexArrays, "glGenVertexArrays");
        AssignOpenGLFunction(_renderContext, mzglBindVertexArray, "glBindVertexArray");
        AssignOpenGLFunction(_renderContext, mzglDeleteVertexArrays, "glDeleteVertexArrays");
        AssignOpenGLFunction(_renderContext, mzglGetUniformBlockIndex, "glGetUniformBlockIndex");
        AssignOpenGLFunction(_renderContext, mzglBindBufferBase, "glBindBufferBase");
        AssignOpenGLFunction(_renderContext, mzglUniformBlockBinding, "glUniformBlockBinding");
        AssignOpenGLFunction(_renderContext, mzglMapBuffer, "glMapBuffer");
        AssignOpenGLFunction(_renderContext, mzglUnmapBuffer, "glUnmapBuffer");
        AssignOpenGLFunction(_renderContext, mzglDepthFunc, "glDepthFunc");
        AssignOpenGLFunction(_renderContext, mzglDepthMask, "glDepthMask");
        AssignOpenGLFunction(_renderContext, mzglCullFace, "glCullFace");
        AssignOpenGLFunction(_renderContext, mzglFrontFace, "glFrontFace");
        AssignOpenGLFunction(_renderContext, mzglClipPlane, "glClipPlane");

        AssignOpenGLFunction(_renderContext, mzglDebugMessageControl, "glDebugMessageControl");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageCallback, "glDebugMessageCallback");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageControlKHR, "glDebugMessageControlKHR");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageInsertKHR, "glDebugMessageInsertKHR");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageCallbackKHR, "glDebugMessageCallbackKHR");
        AssignOpenGLFunction(_renderContext, mzglGetDebugMessageLogKHR, "glGetDebugMessageLogKHR");
        AssignOpenGLFunction(_renderContext, mzglPushDebugGroupKHR, "glPushDebugGroupKHR");
        AssignOpenGLFunction(_renderContext, mzglPopDebugGroupKHR, "glPopDebugGroupKHR");
        AssignOpenGLFunction(_renderContext, mzglObjectLabelKHR, "glObjectLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetObjectLabelKHR, "glGetObjectLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglObjectPtrLabelKHR, "glObjectPtrLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetObjectPtrLabelKHR, "glGetObjectPtrLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetPointervKHR, "glGetPointervKHR");
        AssignOpenGLFunction(_renderContext, mzglPolygonMode, "glPolygonMode");

        /*
#else

#   if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)

        AssignOpenGLFunction(_renderContext, mzglEnable, "glEnable");
        AssignOpenGLFunction(_renderContext, mzglDisable, "glDisable");
        AssignOpenGLFunction(_renderContext, mzglEnablei, "glEnablei");
        AssignOpenGLFunction(_renderContext, mzglDisablei, "glDisablei");
        AssignOpenGLFunction(_renderContext, mzglBlendFunc, "glBlendFunc");
        AssignOpenGLFunction(_renderContext, mzglBlendFunci, "glBlendFunci");
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparate, "glBlendFuncSeparate");
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparatei, "glBlendFuncSeparatei");
        AssignOpenGLFunction(_renderContext, mzglBlendEquation, "glBlendEquation");
        AssignOpenGLFunction(_renderContext, mzglBlendEquationi, "glBlendEquationi");
        AssignOpenGLFunction(_renderContext, mzglDiscardFramebuffer, "glDiscardFramebufferEXT");
        AssignOpenGLFunction(_renderContext, mzglDrawBuffers, "glDrawBuffers");
        AssignOpenGLFunction(_renderContext, mzglFlush, "glFlush");
        AssignOpenGLFunction(_renderContext, mzglFinish, "glFinish");
        AssignOpenGLFunction(_renderContext, mzglGetError, "glGetError");
        AssignOpenGLFunction(_renderContext, mzglGetFloatv, "glGetFloatv");
        AssignOpenGLFunction(_renderContext, mzglGetIntegerv, "glGetIntegerv");
        AssignOpenGLFunction(_renderContext, mzglGetDoublev, "glGetDoublev");
        AssignOpenGLFunction(_renderContext, mzglGetBufferParameteriv, "glGetBufferParameteriv");
        AssignOpenGLFunction(_renderContext, mzglGetProgramiv, "glGetProgramiv");
        AssignOpenGLFunction(_renderContext, mzglGetUniformLocation, "glGetUniformLocation");
        AssignOpenGLFunction(_renderContext, mzglGetActiveUniform, "glGetActiveUniform");
        AssignOpenGLFunction(_renderContext, mzglGetAttribLocation, "glGetAttribLocation");
        AssignOpenGLFunction(_renderContext, mzglGetShaderiv, "glGetShaderiv");
        AssignOpenGLFunction(_renderContext, mzglGetUniformfv, "glGetUniformfv");
        AssignOpenGLFunction(_renderContext, mzglGetShaderInfoLog, "glGetShaderInfoLog");
        AssignOpenGLFunction(_renderContext, mzglGetProgramInfoLog, "glGetProgramInfoLog");
        AssignOpenGLFunction(_renderContext, mzglGetString, "glGetString");
        AssignOpenGLFunction(_renderContext, mzglGetStringi, "glGetStringi");
        AssignOpenGLFunction(_renderContext, mzglIsEnabled, "glIsEnabled");
        AssignOpenGLFunction(_renderContext, mzglEnableVertexAttribArray, "glEnableVertexAttribArray");
        AssignOpenGLFunction(_renderContext, mzglDisableVertexAttribArray, "glDisableVertexAttribArray");
        AssignOpenGLFunction(_renderContext, mzglViewport, "glViewport");
        AssignOpenGLFunction(_renderContext, mzglScissor, "glScissor");
        AssignOpenGLFunction(_renderContext, mzglActiveTexture, "glActiveTexture");
        AssignOpenGLFunction(_renderContext, mzglBindTexture, "glBindTexture");
        AssignOpenGLFunction(_renderContext, mzglCreateProgram, "glCreateProgram");
        AssignOpenGLFunction(_renderContext, mzglCreateShader, "glCreateShader");
        AssignOpenGLFunction(_renderContext, mzglShaderSource, "glShaderSource");
        AssignOpenGLFunction(_renderContext, mzglCompileShader, "glCompileShader");
        AssignOpenGLFunction(_renderContext, mzglUseProgram, "glUseProgram");
        AssignOpenGLFunction(_renderContext, mzglDeleteProgram, "glDeleteProgram");
        AssignOpenGLFunction(_renderContext, mzglAttachShader, "glAttachShader");
        AssignOpenGLFunction(_renderContext, mzglLinkProgram, "glLinkProgram");
        AssignOpenGLFunction(_renderContext, mzglBindAttribLocation, "glBindAttribLocation");
        AssignOpenGLFunction(_renderContext, mzglDetachShader, "glDetachShader");
        AssignOpenGLFunction(_renderContext, mzglDeleteShader, "glDeleteShader");
        AssignOpenGLFunction(_renderContext, mzglUniform1i, "glUniform1i");
        AssignOpenGLFunction(_renderContext, mzglUniform2i, "glUniform2i");
        AssignOpenGLFunction(_renderContext, mzglUniform3i, "glUniform3i");
        AssignOpenGLFunction(_renderContext, mzglUniform4i, "glUniform4i");
        AssignOpenGLFunction(_renderContext, mzglUniform1ui, "glUniform1ui");
        AssignOpenGLFunction(_renderContext, mzglUniform2ui, "glUniform2ui");
        AssignOpenGLFunction(_renderContext, mzglUniform3ui, "glUniform3ui");
        AssignOpenGLFunction(_renderContext, mzglUniform4ui, "glUniform4ui");
        AssignOpenGLFunction(_renderContext, mzglUniform1f, "glUniform1f");
        AssignOpenGLFunction(_renderContext, mzglUniform2f, "glUniform2f");
        AssignOpenGLFunction(_renderContext, mzglUniform3f, "glUniform3f");
        AssignOpenGLFunction(_renderContext, mzglUniform4f, "glUniform4f");
        AssignOpenGLFunction(_renderContext, mzglUniform1fv, "glUniform1fv");
        AssignOpenGLFunction(_renderContext, mzglUniform2fv, "glUniform2fv");
        AssignOpenGLFunction(_renderContext, mzglUniform3fv, "glUniform3fv");
        AssignOpenGLFunction(_renderContext, mzglUniform4fv, "glUniform4fv");
        AssignOpenGLFunction(_renderContext, mzglUniform1iv, "glUniform1iv");
        AssignOpenGLFunction(_renderContext, mzglUniform2iv, "glUniform2iv");
        AssignOpenGLFunction(_renderContext, mzglUniform3iv, "glUniform3iv");
        AssignOpenGLFunction(_renderContext, mzglUniform4iv, "glUniform4iv");
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix3fv, "glUniformMatrix3fv");
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix4fv, "glUniformMatrix4fv");
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribiv, "glGetVertexAttribiv");
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribPointerv, "glGetVertexAttribPointerv");
        AssignOpenGLFunction(_renderContext, mzglGenQueries, "glGenQueriesEXT");
        AssignOpenGLFunction(_renderContext, mzglBeginQuery, "glBeginQuery");
        AssignOpenGLFunction(_renderContext, mzglEndQuery, "glEndQuery");
        AssignOpenGLFunction(_renderContext, mzglGetQueryObjectuiv, "glGetQueryObjectuiv");
        AssignOpenGLFunction(_renderContext, mzglGenBuffers, "glGenBuffers");
        AssignOpenGLFunction(_renderContext, mzglDeleteBuffers, "glDeleteBuffers");
        AssignOpenGLFunction(_renderContext, mzglClear, "glClear");
        AssignOpenGLFunction(_renderContext, mzglClearColor, "glClearColor");
        AssignOpenGLFunction(_renderContext, mzglClearDepth, "glClearDepth");
        AssignOpenGLFunction(_renderContext, mzglVertexAttribPointer, "glVertexAttribPointer");
        AssignOpenGLFunction(_renderContext, mzglBindBuffer, "glBindBuffer");
        AssignOpenGLFunction(_renderContext, mzglBufferData, "glBufferData");
        AssignOpenGLFunction(_renderContext, mzglBufferSubData, "glBufferSubData");
        AssignOpenGLFunction(_renderContext, mzglBufferStorage, "glBufferStorage");
        AssignOpenGLFunction(_renderContext, mzglDrawElements, "glDrawElements");
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstanced, "glDrawElementsInstanced");
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstancedBaseVertex, "glDrawElementsInstancedBaseVertex");
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElements, "glDrawRangeElements");
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElementsBaseVertex, "glDrawRangeElementsBaseVertex");
        AssignOpenGLFunction(_renderContext, mzglDrawArrays, "glDrawArrays");
        AssignOpenGLFunction(_renderContext, mzglDrawArraysInstanced, "glDrawArraysInstanced");
        AssignOpenGLFunction(_renderContext, mzglVertexAttribDivisor, "glVertexAttribDivisor");
        AssignOpenGLFunction(_renderContext, mzglMapBufferRange, "glMapBufferRange");
        AssignOpenGLFunction(_renderContext, mzglFlushMappedBufferRange, "glFlushMappedBufferRange");
        AssignOpenGLFunction(_renderContext, mzglPixelStorei, "glPixelStorei");
        AssignOpenGLFunction(_renderContext, mzglGenTextures, "glGenTextures");
        AssignOpenGLFunction(_renderContext, mzglDeleteTextures, "glDeleteTextures");
        AssignOpenGLFunction(_renderContext, mzglIsTexture, "glIsTexture");
        AssignOpenGLFunction(_renderContext, mzglGenerateMipmap, "glGenerateMipmap");
        AssignOpenGLFunction(_renderContext, mzglTexParameteri, "glTexParameteri");
        AssignOpenGLFunction(_renderContext, mzglCompressedTexImage2D, "glCompressedTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglGetTexLevelParameteriv, "glGetTexLevelParameteriv");
        AssignOpenGLFunction(_renderContext, mzglTexImage2D, "glTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglTexImage2DMultisample, "glTexImage2DMultisample");
        AssignOpenGLFunction(_renderContext, mzglTexStorage2DMultisample, "glTexStorage2DMultisample");
        AssignOpenGLFunction(_renderContext, mzglGetTexImage, "glGetTexImage");
        AssignOpenGLFunction(_renderContext, mzglCheckFramebufferStatus, "glCheckFramebufferStatus");
        AssignOpenGLFunction(_renderContext, mzglBlitFramebuffer, "glBlitFramebuffer");
        AssignOpenGLFunction(_renderContext, mzglCopyTexImage2D, "glCopyTexImage2D");
        AssignOpenGLFunction(_renderContext, mzglTexSubImage2D, "glTexSubImage2D");
        AssignOpenGLFunction(_renderContext, mzglReadPixels, "glReadPixels");
        AssignOpenGLFunction(_renderContext, mzglGenVertexArrays, "glGenVertexArrays");
        AssignOpenGLFunction(_renderContext, mzglBindVertexArray, "glBindVertexArray");
        AssignOpenGLFunction(_renderContext, mzglDeleteVertexArrays, "glDeleteVertexArrays");
        AssignOpenGLFunction(_renderContext, mzglGetUniformBlockIndex, "glGetUniformBlockIndex");
        AssignOpenGLFunction(_renderContext, mzglBindBufferBase, "glBindBufferBase");
        AssignOpenGLFunction(_renderContext, mzglUniformBlockBinding, "glUniformBlockBinding");
        AssignOpenGLFunction(_renderContext, mzglMapBuffer, "glMapBuffer");
        AssignOpenGLFunction(_renderContext, mzglUnmapBuffer, "glUnmapBuffer");
        AssignOpenGLFunction(_renderContext, mzglDepthFunc, "glDepthFunc");
        AssignOpenGLFunction(_renderContext, mzglDepthMask, "glDepthMask");
        AssignOpenGLFunction(_renderContext, mzglCullFace, "glCullFace");
        AssignOpenGLFunction(_renderContext, mzglFrontFace, "glFrontFace");
        AssignOpenGLFunction(_renderContext, mzglClipPlane, "glClipPlane");
                             _
#   if (MAZE_PLATFORM != MAZE__PLATFORM_ANDROID)
        AssignOpenGLFunction(_renderContext, mzglBindRenderbuffer, "glBindRenderbufferOES");
        AssignOpenGLFunction(_renderContext, mzglGenFramebuffers, "glGenFramebuffersOES");
        AssignOpenGLFunction(_renderContext, mzglDeleteRenderbuffers, "glDeleteRenderbuffersOES");
        AssignOpenGLFunction(_renderContext, mzglFramebufferRenderbuffer, "glFramebufferRenderbufferOES");
        AssignOpenGLFunction(_renderContext, mzglFramebufferTexture2D, "glFramebufferTexture2DOES");
        AssignOpenGLFunction(_renderContext, mzglDeleteFramebuffers, "glDeleteFramebuffersOES");
        AssignOpenGLFunction(_renderContext, mzglBindFramebuffer, "glBindFramebufferOES");
        AssignOpenGLFunction(_renderContext, mzglGenerateMipmap, "glGenerateMipmapOES");
#   else
        AssignOpenGLFunction(_renderContext, mzglBindRenderbuffer, "glBindRenderbuffer");
        AssignOpenGLFunction(_renderContext, mzglGenFramebuffers, "glGenFramebuffers");
        AssignOpenGLFunction(_renderContext, mzglDeleteRenderbuffers, "glDeleteRenderbuffers");
        AssignOpenGLFunction(_renderContext, mzglFramebufferRenderbuffer, "glFramebufferRenderbuffer");
        AssignOpenGLFunction(_renderContext, mzglFramebufferTexture2D, "glFramebufferTexture2D");
        AssignOpenGLFunction(_renderContext, mzglDeleteFramebuffers, "glDeleteFramebuffers");
        AssignOpenGLFunction(_renderContext, mzglBindFramebuffer, "glBindFramebuffer");

        AssignOpenGLFunction(_renderContext, mzglDebugMessageControl, "glDebugMessageControl");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageCallback, "glDebugMessageCallback");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageControlKHR, "glDebugMessageControlKHR");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageInsertKHR, "glDebugMessageInsertKHR");
        AssignOpenGLFunction(_renderContext, mzglDebugMessageCallbackKHR, "glDebugMessageCallbackKHR");
        AssignOpenGLFunction(_renderContext, mzglGetDebugMessageLogKHR, "glGetDebugMessageLogKHR");
        AssignOpenGLFunction(_renderContext, mzglPushDebugGroupKHR, "glPushDebugGroupKHR");
        AssignOpenGLFunction(_renderContext, mzglPopDebugGroupKHR, "glPopDebugGroupKHR");
        AssignOpenGLFunction(_renderContext, mzglObjectLabelKHR, "glObjectLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetObjectLabelKHR, "glGetObjectLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglObjectPtrLabelKHR, "glObjectPtrLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetObjectPtrLabelKHR, "glGetObjectPtrLabelKHR");
        AssignOpenGLFunction(_renderContext, mzglGetPointervKHR, "glGetPointervKHR");
        AssignOpenGLFunction(_renderContext, mzglPolygonMode, "glPolygonMode");
#   endif

#   else
        AssignOpenGLFunction(_renderContext, mzglEnable, glEnable);
        AssignOpenGLFunction(_renderContext, mzglDisable, glDisable);
        AssignOpenGLFunction(_renderContext, mzglEnablei, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglDisablei, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglBlendFunc, glBlendFunc);
        AssignOpenGLFunction(_renderContext, mzglBlendFunci, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparate, glBlendFuncSeparate);
        AssignOpenGLFunction(_renderContext, mzglBlendFuncSeparatei, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglBlendEquation, glBlendEquation);
        AssignOpenGLFunction(_renderContext, mzglBlendEquationi, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglBindRenderbuffer, glBindRenderbufferOES);
        AssignOpenGLFunction(_renderContext, mzglGenFramebuffers, glGenFramebuffersOES);
        AssignOpenGLFunction(_renderContext, mzglDiscardFramebuffer, glDiscardFramebufferEXT);
        AssignOpenGLFunction(_renderContext, mzglDeleteRenderbuffers, glDeleteRenderbuffersOES);
        AssignOpenGLFunction(_renderContext, mzglFramebufferRenderbuffer, glFramebufferRenderbufferOES);
        AssignOpenGLFunction(_renderContext, mzglBindFramebuffer, glBindFramebufferOES);
        AssignOpenGLFunction(_renderContext, mzglDrawBuffers, glDrawBuffers);
        AssignOpenGLFunction(_renderContext, mzglFramebufferTexture2D, glFramebufferTexture2DOES);
        AssignOpenGLFunction(_renderContext, mzglDeleteFramebuffers, glDeleteFramebuffersOES);
        AssignOpenGLFunction(_renderContext, mzglFlush, glFlush);
        AssignOpenGLFunction(_renderContext, mzglFinish, glFinish);
        AssignOpenGLFunction(_renderContext, mzglGetError, glGetError);
        AssignOpenGLFunction(_renderContext, mzglGetFloatv, glGetFloatv);
        AssignOpenGLFunction(_renderContext, mzglGetIntegerv, glGetIntegerv);
        AssignOpenGLFunction(_renderContext, mzglGetDoublev, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglGetBufferParameteriv, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglGetProgramiv, glGetProgramiv);
        AssignOpenGLFunction(_renderContext, mzglGetUniformLocation, glGetUniformLocation);
        AssignOpenGLFunction(_renderContext, mzglGetActiveUniform, glGetActiveUniform);
        AssignOpenGLFunction(_renderContext, mzglGetAttribLocation, glGetAttribLocation);
        AssignOpenGLFunction(_renderContext, mzglGetShaderiv, glGetShaderiv);
        AssignOpenGLFunction(_renderContext, mzglGetUniformfv, glGetUniformfv);
        AssignOpenGLFunction(_renderContext, mzglGetShaderInfoLog, glGetShaderInfoLog);
        AssignOpenGLFunction(_renderContext, mzglGetProgramInfoLog, glGetProgramInfoLog);
        AssignOpenGLFunction(_renderContext, mzglGetString, glGetString);
        AssignOpenGLFunction(_renderContext, mzglGetStringi, glGetStringi);
        AssignOpenGLFunction(_renderContext, mzglIsEnabled, glIsEnabled);
        AssignOpenGLFunction(_renderContext, mzglEnableVertexAttribArray, glEnableVertexAttribArray);
        AssignOpenGLFunction(_renderContext, mzglDisableVertexAttribArray, glDisableVertexAttribArray);
        AssignOpenGLFunction(_renderContext, mzglViewport, glViewport);
        AssignOpenGLFunction(_renderContext, mzglScissor, glScissor);
        AssignOpenGLFunction(_renderContext, mzglActiveTexture, glActiveTexture);
        AssignOpenGLFunction(_renderContext, mzglBindTexture, glBindTexture);
        AssignOpenGLFunction(_renderContext, mzglCreateProgram, glCreateProgram);
        AssignOpenGLFunction(_renderContext, mzglCreateShader, glCreateShader);
        AssignOpenGLFunction(_renderContext, mzglShaderSource, glShaderSource);
        AssignOpenGLFunction(_renderContext, mzglCompileShader, glCompileShader);
        AssignOpenGLFunction(_renderContext, mzglUseProgram, glUseProgram);
        AssignOpenGLFunction(_renderContext, mzglDeleteProgram, glDeleteProgram);
        AssignOpenGLFunction(_renderContext, mzglAttachShader, glAttachShader);
        AssignOpenGLFunction(_renderContext, mzglLinkProgram, glLinkProgram);
        AssignOpenGLFunction(_renderContext, mzglBindAttribLocation, glBindAttribLocation);
        AssignOpenGLFunction(_renderContext, mzglDetachShader, glDetachShader);
        AssignOpenGLFunction(_renderContext, mzglDeleteShader, glDeleteShader);
        AssignOpenGLFunction(_renderContext, mzglUniform1i, glUniform1i);
        AssignOpenGLFunction(_renderContext, mzglUniform2i, glUniform2i);
        AssignOpenGLFunction(_renderContext, mzglUniform3i, glUniform3i);
        AssignOpenGLFunction(_renderContext, mzglUniform4i, glUniform4i);
        AssignOpenGLFunction(_renderContext, mzglUniform1ui, glUniform1ui);
        AssignOpenGLFunction(_renderContext, mzglUniform2ui, glUniform2ui);
        AssignOpenGLFunction(_renderContext, mzglUniform3ui, glUniform3ui);
        AssignOpenGLFunction(_renderContext, mzglUniform4ui, glUniform4ui);
        AssignOpenGLFunction(_renderContext, mzglUniform1f, glUniform1f);
        AssignOpenGLFunction(_renderContext, mzglUniform2f, glUniform2f);
        AssignOpenGLFunction(_renderContext, mzglUniform3f, glUniform3f);
        AssignOpenGLFunction(_renderContext, mzglUniform4f, glUniform4f);
        AssignOpenGLFunction(_renderContext, mzglUniform1fv, glUniform1fv);
        AssignOpenGLFunction(_renderContext, mzglUniform2fv, glUniform2fv);
        AssignOpenGLFunction(_renderContext, mzglUniform3fv, glUniform3fv);
        AssignOpenGLFunction(_renderContext, mzglUniform4fv, glUniform4fv);
        AssignOpenGLFunction(_renderContext, mzglUniform1iv, glUniform1iv);
        AssignOpenGLFunction(_renderContext, mzglUniform2iv, glUniform2iv);
        AssignOpenGLFunction(_renderContext, mzglUniform3iv, glUniform3iv);
        AssignOpenGLFunction(_renderContext, mzglUniform4iv, glUniform4iv);
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix3fv, glUniformMatrix3fv);
        AssignOpenGLFunction(_renderContext, mzglUniformMatrix4fv, glUniformMatrix4fv);
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribiv, glGetVertexAttribiv);
        AssignOpenGLFunction(_renderContext, mzglGetVertexAttribPointerv, glGetVertexAttribPointerv);
        AssignOpenGLFunction(_renderContext, mzglGenQueries, glGenQueriesEXT);
        AssignOpenGLFunction(_renderContext, mzglBeginQuery, glBeginQuery);
        AssignOpenGLFunction(_renderContext, mzglEndQuery, glEndQuery);
        AssignOpenGLFunction(_renderContext, mzglGetQueryObjectuiv, glGetQueryObjectuiv);
        AssignOpenGLFunction(_renderContext, mzglGenBuffers, glGenBuffers);
        AssignOpenGLFunction(_renderContext, mzglDeleteBuffers, glDeleteBuffers);
        AssignOpenGLFunction(_renderContext, mzglClear, glClear);
        AssignOpenGLFunction(_renderContext, mzglClearColor, glClearColor);
        AssignOpenGLFunction(_renderContext, mzglClearDepth, glClearDepth);
        AssignOpenGLFunction(_renderContext, mzglVertexAttribPointer, glVertexAttribPointer);
        AssignOpenGLFunction(_renderContext, mzglBindBuffer, glBindBuffer);
        AssignOpenGLFunction(_renderContext, mzglBufferData, glBufferData);
        AssignOpenGLFunction(_renderContext, mzglBufferSubData, glBufferSubData);
        AssignOpenGLFunction(_renderContext, mzglBufferStorage, glBufferStorage);
        AssignOpenGLFunction(_renderContext, mzglDrawElements, glDrawElements);
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstanced, glDrawElementsInstanced);
        AssignOpenGLFunction(_renderContext, mzglDrawElementsInstancedBaseVertex, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElements, glDrawRangeElements);
        AssignOpenGLFunction(_renderContext, mzglDrawRangeElementsBaseVertex, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglDrawArrays, glDrawArrays);
        AssignOpenGLFunction(_renderContext, mzglDrawArraysInstanced, glDrawArraysInstanced);
        AssignOpenGLFunction(_renderContext, mzglVertexAttribDivisor, glVertexAttribDivisor);
        AssignOpenGLFunction(_renderContext, mzglMapBufferRange, glMapBufferRange);
        AssignOpenGLFunction(_renderContext, mzglFlushMappedBufferRange, glFlushMappedBufferRange);
        AssignOpenGLFunction(_renderContext, mzglPixelStorei, glPixelStorei);
        AssignOpenGLFunction(_renderContext, mzglGenTextures, glGenTextures);
        AssignOpenGLFunction(_renderContext, mzglDeleteTextures, glDeleteTextures);
        AssignOpenGLFunction(_renderContext, mzglIsTexture, glIsTexture);
        AssignOpenGLFunction(_renderContext, mzglGenerateMipmap, glGenerateMipmapOES);
        AssignOpenGLFunction(_renderContext, mzglTexParameteri, glTexParameteri);
        AssignOpenGLFunction(_renderContext, mzglCompressedTexImage2D, glCompressedTexImage2D);
        AssignOpenGLFunction(_renderContext, mzglGetTexLevelParameteriv, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglTexImage2D, glTexImage2D);
        AssignOpenGLFunction(_renderContext, mzglTexImage2DMultisample, glTexImage2DMultisample);
        AssignOpenGLFunction(_renderContext, mzglTexStorage2DMultisample, glTexStorage2DMultisample);
        AssignOpenGLFunction(_renderContext, mzglGetTexImage, MAZE_NULL);
        AssignOpenGLFunction(_renderContext, mzglCheckFramebufferStatus, glCheckFramebufferStatus);
        AssignOpenGLFunction(_renderContext, mzglBlitFramebuffer, glBlitFramebuffer);
        AssignOpenGLFunction(_renderContext, mzglCopyTexImage2D, glCopyTexImage2D);
        AssignOpenGLFunction(_renderContext, mzglTexSubImage2D, glTexSubImage2D);
        AssignOpenGLFunction(_renderContext, mzglReadPixels, glReadPixels);
        AssignOpenGLFunction(_renderContext, mzglGenVertexArrays, glGenVertexArrays);
        AssignOpenGLFunction(_renderContext, mzglBindVertexArray, glBindVertexArray);
        AssignOpenGLFunction(_renderContext, mzglDeleteVertexArrays, glDeleteVertexArrays);
        AssignOpenGLFunction(_renderContext, mzglGetUniformBlockIndex, glGetUniformBlockIndex);
        AssignOpenGLFunction(_renderContext, mzglBindBufferBase, glBindBufferBase);
        AssignOpenGLFunction(_renderContext, mzglUniformBlockBinding, glUniformBlockBinding);
        AssignOpenGLFunction(_renderContext, mzglMapBuffer, glMapBufferOES);
        AssignOpenGLFunction(_renderContext, mzglUnmapBuffer, glUnmapBuffer);
        AssignOpenGLFunction(_renderContext, mzglDepthFunc, glDepthFunc);
        AssignOpenGLFunction(_renderContext, mzglDepthMask, glDepthMask);
        AssignOpenGLFunction(_renderContext, mzglCullFace, glCullFace);
        AssignOpenGLFunction(_renderContext, mzglFrontFace, glFrontFace);
        AssignOpenGLFunction(_renderContext, mzglClipPlane, glClipPlane);

#   endif
        
#endif
     */
        return true;
    }


} // namespace Maze
//////////////////////////////////////////
