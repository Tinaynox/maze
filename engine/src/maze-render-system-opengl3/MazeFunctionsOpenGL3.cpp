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

    //////////////////////////////////////////OpenGL KHR_debug log callback enabled
    template <typename TGLFunction, typename TGLDirectFunction>
    inline bool AssignOpenGLFunctionDirect(ContextOpenGLPtr const& _renderContext, TGLFunction& _functionPointer, TGLDirectFunction _glFunctionPointer)
    {
        _functionPointer = static_cast<TGLFunction>(_glFunctionPointer);
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
        Debug::Log("AssignFunctionsOpenGL3 started...");

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        AssignOpenGLFunctionDirect(_renderContext, mzglEnable, glEnable);
        AssignOpenGLFunctionDirect(_renderContext, mzglDisable, glDisable);
        AssignOpenGLFunctionDirect(_renderContext, mzglEnablei, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglDisablei, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendFunc, glBlendFunc);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendFunci, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendFuncSeparate, glBlendFuncSeparate);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendFuncSeparatei, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendEquation, glBlendEquation);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlendEquationi, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindRenderbuffer, glBindRenderbuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenFramebuffers, glGenFramebuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglDiscardFramebuffer, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteRenderbuffers, glDeleteRenderbuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglFramebufferRenderbuffer, glFramebufferRenderbuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindFramebuffer, glBindFramebuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawBuffers, glDrawBuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglFramebufferTexture2D, glFramebufferTexture2D);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteFramebuffers, glDeleteFramebuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglFlush, glFlush);
        AssignOpenGLFunctionDirect(_renderContext, mzglFinish, glFinish);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetError, glGetError);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetFloatv, glGetFloatv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetIntegerv, glGetIntegerv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetDoublev, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetBufferParameteriv, glGetBufferParameteriv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetProgramiv, glGetProgramiv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetUniformLocation, glGetUniformLocation);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetActiveUniform, glGetActiveUniform);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetAttribLocation, glGetAttribLocation);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetShaderiv, glGetShaderiv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetUniformfv, glGetUniformfv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetShaderInfoLog, glGetShaderInfoLog);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetProgramInfoLog, glGetProgramInfoLog);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetString, glGetString);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetStringi, glGetStringi);
        AssignOpenGLFunctionDirect(_renderContext, mzglIsEnabled, glIsEnabled);
        AssignOpenGLFunctionDirect(_renderContext, mzglEnableVertexAttribArray, glEnableVertexAttribArray);
        AssignOpenGLFunctionDirect(_renderContext, mzglDisableVertexAttribArray, glDisableVertexAttribArray);
        AssignOpenGLFunctionDirect(_renderContext, mzglViewport, glViewport);
        AssignOpenGLFunctionDirect(_renderContext, mzglScissor, glScissor);
        AssignOpenGLFunctionDirect(_renderContext, mzglActiveTexture, glActiveTexture);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindTexture, glBindTexture);
        AssignOpenGLFunctionDirect(_renderContext, mzglCreateProgram, glCreateProgram);
        AssignOpenGLFunctionDirect(_renderContext, mzglCreateShader, glCreateShader);
        AssignOpenGLFunctionDirect(_renderContext, mzglShaderSource, glShaderSource);
        AssignOpenGLFunctionDirect(_renderContext, mzglCompileShader, glCompileShader);
        AssignOpenGLFunctionDirect(_renderContext, mzglUseProgram, glUseProgram);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteProgram, glDeleteProgram);
        AssignOpenGLFunctionDirect(_renderContext, mzglAttachShader, glAttachShader);
        AssignOpenGLFunctionDirect(_renderContext, mzglLinkProgram, glLinkProgram);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindAttribLocation, glBindAttribLocation);
        AssignOpenGLFunctionDirect(_renderContext, mzglDetachShader, glDetachShader);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteShader, glDeleteShader);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform1i, glUniform1i);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform2i, glUniform2i);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform3i, glUniform3i);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform4i, glUniform4i);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform1ui, glUniform1ui);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform2ui, glUniform2ui);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform3ui, glUniform3ui);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform4ui, glUniform4ui);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform1f, glUniform1f);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform2f, glUniform2f);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform3f, glUniform3f);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform4f, glUniform4f);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform1fv, glUniform1fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform2fv, glUniform2fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform3fv, glUniform3fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform4fv, glUniform4fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform1iv, glUniform1iv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform2iv, glUniform2iv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform3iv, glUniform3iv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniform4iv, glUniform4iv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniformMatrix3fv, glUniformMatrix3fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniformMatrix4fv, glUniformMatrix4fv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetVertexAttribiv, glGetVertexAttribiv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetVertexAttribPointerv, glGetVertexAttribPointerv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenQueries, glGenQueries);
        AssignOpenGLFunctionDirect(_renderContext, mzglBeginQuery, glBeginQuery);
        AssignOpenGLFunctionDirect(_renderContext, mzglEndQuery, glEndQuery);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetQueryObjectuiv, glGetQueryObjectuiv);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenBuffers, glGenBuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteBuffers, glDeleteBuffers);
        AssignOpenGLFunctionDirect(_renderContext, mzglClear, glClear);
        AssignOpenGLFunctionDirect(_renderContext, mzglClearColor, glClearColor);
        AssignOpenGLFunctionDirect(_renderContext, mzglClearDepth, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglVertexAttribPointer, glVertexAttribPointer);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindBuffer, glBindBuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglBufferData, glBufferData);
        AssignOpenGLFunctionDirect(_renderContext, mzglBufferSubData, glBufferSubData);
        AssignOpenGLFunctionDirect(_renderContext, mzglBufferStorage, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawElements, glDrawElements);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawElementsInstanced, glDrawElementsInstanced);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawElementsInstancedBaseVertex, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawRangeElements, glDrawRangeElements);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawRangeElementsBaseVertex, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawArrays, glDrawArrays);
        AssignOpenGLFunctionDirect(_renderContext, mzglDrawArraysInstanced, glDrawArraysInstanced);
        AssignOpenGLFunctionDirect(_renderContext, mzglVertexAttribDivisor, glVertexAttribDivisor);
        AssignOpenGLFunctionDirect(_renderContext, mzglMapBufferRange, glMapBufferRange);
        AssignOpenGLFunctionDirect(_renderContext, mzglFlushMappedBufferRange, glFlushMappedBufferRange);
        AssignOpenGLFunctionDirect(_renderContext, mzglPixelStorei, glPixelStorei);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenTextures, glGenTextures);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteTextures, glDeleteTextures);
        AssignOpenGLFunctionDirect(_renderContext, mzglIsTexture, glIsTexture);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenerateMipmap, glGenerateMipmap);
        AssignOpenGLFunctionDirect(_renderContext, mzglTexParameteri, glTexParameteri);
        AssignOpenGLFunctionDirect(_renderContext, mzglCompressedTexImage2D, glCompressedTexImage2D);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetTexLevelParameteriv, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglTexImage2D, glTexImage2D);
        AssignOpenGLFunctionDirect(_renderContext, mzglTexImage2DMultisample, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglTexStorage2DMultisample, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetTexImage, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglCheckFramebufferStatus, glCheckFramebufferStatus);
        AssignOpenGLFunctionDirect(_renderContext, mzglBlitFramebuffer, glBlitFramebuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglCopyTexImage2D, glCopyTexImage2D);
        AssignOpenGLFunctionDirect(_renderContext, mzglTexSubImage2D, glTexSubImage2D);
        AssignOpenGLFunctionDirect(_renderContext, mzglReadPixels, glReadPixels);
        AssignOpenGLFunctionDirect(_renderContext, mzglGenVertexArrays, glGenVertexArrays);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindVertexArray, glBindVertexArray);
        AssignOpenGLFunctionDirect(_renderContext, mzglDeleteVertexArrays, glDeleteVertexArrays);
        AssignOpenGLFunctionDirect(_renderContext, mzglGetUniformBlockIndex, glGetUniformBlockIndex);
        AssignOpenGLFunctionDirect(_renderContext, mzglBindBufferBase, glBindBufferBase);
        AssignOpenGLFunctionDirect(_renderContext, mzglUniformBlockBinding, glUniformBlockBinding);
        AssignOpenGLFunctionDirect(_renderContext, mzglMapBuffer, nullptr);
        AssignOpenGLFunctionDirect(_renderContext, mzglUnmapBuffer, glUnmapBuffer);
        AssignOpenGLFunctionDirect(_renderContext, mzglDepthFunc, glDepthFunc);
        AssignOpenGLFunctionDirect(_renderContext, mzglDepthMask, glDepthMask);
        AssignOpenGLFunctionDirect(_renderContext, mzglCullFace, glCullFace);
        AssignOpenGLFunctionDirect(_renderContext, mzglFrontFace, glFrontFace);
        AssignOpenGLFunctionDirect(_renderContext, mzglClipPlane, nullptr);

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
#else
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
#endif

        /*
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

        Debug::Log("AssignFunctionsOpenGL3 finished.");

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
