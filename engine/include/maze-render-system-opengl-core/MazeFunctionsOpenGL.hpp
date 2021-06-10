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
#if (!defined(_MazeFunctionsOpenGL_hpp_))
#define _MazeFunctionsOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
#if (MAZE_DEBUG)
#    define MAZE_DEBUG_GL (1)
#else
#    define MAZE_DEBUG_GL (0)
#endif


//////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    
//////////////////////////////////////////
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglEnable)(MZGLenum _cap);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDisable)(MZGLenum _cap);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglEnablei)(MZGLenum _cap, MZGLuint _index);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDisablei)(MZGLenum _cap, MZGLuint _index);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendFunc)(MZGLenum _src, MZGLenum _dst);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendFunci)(MZGLuint _buf, MZGLenum _src, MZGLenum _dst);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendFuncSeparate)(MZGLenum _sfactorRGB, MZGLenum _dfactorRGB, MZGLenum _sfactorAlpha, MZGLenum _dfactorAlpha);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendFuncSeparatei)(MZGLuint _buf, MZGLenum _srcRGB, MZGLenum _dstRGB, MZGLenum _srcAlpha, MZGLenum _dstAlpha);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendEquation)(MZGLenum _mode);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlendEquationi)(MZGLuint _buf, MZGLenum _mode);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindRenderbuffer)(MZGLenum _target, MZGLuint _renderbuffer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenFramebuffers)(MZGLsizei _n, MZGLuint* _framebuffers);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDiscardFramebuffer)(MZGLenum _target, MZGLsizei _numAttachments, const MZGLenum* _attachments);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteRenderbuffers)(MZGLsizei _n, const MZGLuint* _renderbuffers);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFramebufferRenderbuffer)(MZGLenum _target, MZGLenum _attachment, MZGLenum _renderbuffertarget, MZGLuint _renderbuffer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindFramebuffer)(MZGLenum _target, MZGLuint _framebuffer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawBuffers)(MZGLsizei _n, const MZGLenum* _bufs);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawElementsInstanced)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, const void* _indices, MZGLsizei _primcount);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawElementsInstancedBaseVertex)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, MZGLvoid* _indices, MZGLsizei _primcount, MZGLint _basevertex);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawRangeElements)(MZGLenum _mode, MZGLuint _start, MZGLuint _end, MZGLsizei _count, MZGLenum _type, const MZGLvoid* _indices);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawRangeElementsBaseVertex)(MZGLenum _mode, MZGLuint _start, MZGLuint _end, MZGLsizei _count, MZGLenum _type, MZGLvoid* _indices, MZGLint _basevertex);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawArrays)(MZGLenum _mode, MZGLint _first, MZGLsizei _count);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawArraysInstanced)(MZGLenum _mode, MZGLint _first, MZGLsizei _count, MZGLsizei _primcount);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglVertexAttribDivisor)(MZGLuint _index, MZGLuint _divisor);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void* (MAZE_GL_FUNCPTR *mzglMapBufferRange)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _length, MZGLbitfield _access);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFlushMappedBufferRange)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _length);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglPixelStorei)(MZGLenum _pname, MZGLint _param);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFramebufferTexture2D)(MZGLenum _target, MZGLenum _attachment, MZGLenum _textarget, MZGLuint _texture, MZGLint _level);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteFramebuffers)(MZGLsizei _n, const MZGLuint* _framebuffers);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFlush)();
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFinish)();
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLenum (MAZE_GL_FUNCPTR *mzglGetError)();
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetFloatv)(MZGLenum _pname, MZGLfloat* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetIntegerv)(MZGLenum _pname, MZGLint* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetDoublev)(MZGLenum _pname, MZGLdouble* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetProgramiv)(MZGLuint _program, MZGLenum _pname, MZGLint* _param);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLint (MAZE_GL_FUNCPTR *mzglGetUniformLocation)(MZGLuint _program, const MZGLchar* _name);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLint (MAZE_GL_FUNCPTR *mzglGetAttribLocation)(MZGLuint _program, const MZGLchar* _name);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetShaderiv)(MZGLuint _shader, MZGLenum _pname, MZGLint* _param);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetUniformfv)(MZGLuint _program, MZGLint _location, MZGLfloat* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetShaderInfoLog)(MZGLuint _shader, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _infoLog);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetProgramInfoLog)(MZGLuint _program, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _infoLog);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern const MZGLubyte* (MAZE_GL_FUNCPTR *mzglGetString)(MZGLenum _name);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern const MZGLubyte* (MAZE_GL_FUNCPTR *mzglGetStringi)(MZGLenum _name, MZGLuint _index);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLboolean (MAZE_GL_FUNCPTR *mzglIsEnabled)(MZGLenum _cap);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglEnableVertexAttribArray)(MZGLuint _index);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDisableVertexAttribArray)(MZGLuint _index);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglViewport)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglScissor)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglActiveTexture)(MZGLenum _texture);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindTexture)(MZGLenum _target, MZGLuint _texture);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLuint (MAZE_GL_FUNCPTR *mzglCreateProgram)();
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLuint (MAZE_GL_FUNCPTR *mzglCreateShader)(MZGLenum _type);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglShaderSource)(MZGLuint _shader, MZGLsizei _count, const MZGLchar* const* _string, const MZGLint* _length);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglCompileShader)(MZGLuint _shader);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUseProgram)(MZGLuint _program);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteProgram)(MZGLuint _program);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglAttachShader)(MZGLuint _program, MZGLuint _shader);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglLinkProgram)(MZGLuint _program);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindAttribLocation)(MZGLuint _program, MZGLuint _index, const MZGLchar* name);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDetachShader)(MZGLuint _program, MZGLuint _shader);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteShader)(MZGLuint _shader);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform1i)(MZGLint _location, MZGLint _v0);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform2i)(MZGLint _location, MZGLint _v0, MZGLint _v1);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform3i)(MZGLint _location, MZGLint _v0, MZGLint _v1, MZGLint _v2);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform4i)(MZGLint _location, MZGLint _v0, MZGLint _v1, MZGLint _v2, MZGLint _v3);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform1ui)(MZGLint _location, MZGLuint _v0);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform2ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform3ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1, MZGLuint _v2);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform4ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1, MZGLuint _v2, MZGLuint _v3);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform1f)(MZGLint _location, MZGLfloat _v0);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform2f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform3f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1, MZGLfloat _v2);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniform4f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1, MZGLfloat _v2, MZGLfloat _v3);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglUniform1fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglUniform2fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglUniform3fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglUniform4fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniformMatrix3fv)(MZGLint _location, MZGLsizei _count, MZGLboolean _transpose, const MZGLfloat* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniformMatrix4fv)(MZGLint _location, MZGLsizei _count, MZGLboolean _transpose, const MZGLfloat* _value);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetVertexAttribiv)(MZGLuint _index, MZGLenum _pname, MZGLint* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenQueries)(MZGLsizei _n, MZGLuint* _ids);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBeginQuery)(MZGLenum _target, MZGLuint _id);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglEndQuery)(MZGLenum _target);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetQueryObjectuiv)(MZGLuint _id, MZGLenum _pname, MZGLuint* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenBuffers)(MZGLsizei _n, MZGLuint* _buffers);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteBuffers)(MZGLsizei _n, const MZGLuint* _buffers);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglClear)(MZGLbitfield _mask);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglClearColor)(MZGLclampf _red, MZGLclampf _green, MZGLclampf _blue, MZGLclampf _alpha);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglClearDepth)(MZGLclampd _depth);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglVertexAttribPointer)(MZGLuint _index, MZGLint _size, MZGLenum _type, MZGLboolean _normalized, MZGLsizei _stride, const void* _pointer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindBuffer)(MZGLenum _target, MZGLuint _buffer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBufferData)(MZGLenum _target, MZGLsizeiptr _size, const void* _data, MZGLenum _usage);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBufferSubData)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _size, const void* _data);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBufferStorage)(MZGLenum _target, MZGLsizeiptr _size, const void* _data, MZGLbitfield _flags);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDrawElements)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, const void* _indices);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenTextures)(MZGLsizei _n, MZGLuint* _textures);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteTextures)(MZGLsizei _n, const MZGLuint* _textures);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLboolean (MAZE_GL_FUNCPTR *mzglIsTexture)(MZGLuint _texture);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenerateMipmap)(MZGLenum _target);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglTexParameteri)(MZGLenum _target, MZGLenum _pname, MZGLint _param);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglCompressedTexImage2D)(MZGLenum _target, MZGLint _level, MZGLenum _internalformat, MZGLsizei _width, MZGLsizei _height, MZGLint _border, MZGLsizei _imageSize, const MZGLvoid* _data);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetTexLevelParameteriv)(MZGLenum _target, MZGLint _level, MZGLenum _pname, MZGLint* _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglTexImage2D)(MZGLenum _target, MZGLint _level, MZGLint _internalformat, MZGLsizei _width, MZGLsizei _height, MZGLint _border, MZGLenum _format, MZGLenum _type, const MZGLvoid* _pixels);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetTexImage)(MZGLenum _target, MZGLint _level, MZGLenum _format, MZGLenum _type, void* _pixels);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLenum (MAZE_GL_FUNCPTR *mzglCheckFramebufferStatus)(MZGLenum _target);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBlitFramebuffer)(MZGLint _srcX0, MZGLint _srcY0, MZGLint _srcX1, MZGLint _srcY1, MZGLint _dstX0, MZGLint _dstY0, MZGLint _dstX1, MZGLint _dstY1, MZGLbitfield _mask, MZGLenum _filter);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglCopyTexImage2D)(MZGLenum _target, MZGLint _level, MZGLenum _internalFormat, MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height, MZGLint _border);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglTexSubImage2D)(MZGLenum _target, MZGLint _level, MZGLint _xoffset, MZGLint _yoffset, MZGLsizei _width, MZGLsizei _height, MZGLenum _format, MZGLenum _type, const void* _pixels);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglReadPixels)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height, MZGLenum _format, MZGLenum _type, MZGLvoid* _data);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGenVertexArrays)(MZGLsizei _n, MZGLuint* _arrays);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindVertexArray)(MZGLuint _array);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDeleteVertexArrays)(MZGLsizei _n, const MZGLuint* _arrays);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLuint (MAZE_GL_FUNCPTR *mzglGetUniformBlockIndex)(MZGLuint _program, const MZGLchar* _uniformBlockName);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglBindBufferBase)(MZGLenum _target, MZGLuint _index, MZGLuint _buffer);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglUniformBlockBinding)(MZGLuint _program, MZGLuint _uniformBlockIndex, MZGLuint _uniformBlockBinding);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void* (MAZE_GL_FUNCPTR *mzglMapBuffer)(MZGLenum _target, MZGLenum _access);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLboolean (MAZE_GL_FUNCPTR *mzglUnmapBuffer)(MZGLenum _target);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDepthFunc)(MZGLenum _func);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDepthMask)(MZGLboolean _flag);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglCullFace)(MZGLenum _mode);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglFrontFace)(MZGLenum _mode);

using GLDEBUGPROCKHR = void (MAZE_GL_FUNCPTR *)(MZGLenum _source, MZGLenum _type, MZGLuint _id, MZGLenum _severity, MZGLsizei _length, const MZGLchar* _message, const void* _userParam);

MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglDebugMessageControl)(MZGLenum _source, MZGLenum _type, MZGLenum _severity, MZGLsizei _count, MZGLuint* _ids, MZGLboolean _enabled);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR* mzglDebugMessageCallback)(GLDEBUGPROCKHR _callback, const void* _userParam);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDebugMessageControlKHR)(MZGLenum _source, MZGLenum _type, MZGLenum _severity, MZGLsizei _count, const MZGLuint* _ids, MZGLboolean _enabled);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDebugMessageInsertKHR)(MZGLenum _source, MZGLenum _type, MZGLuint _id, MZGLenum _severity, MZGLsizei _length, const MZGLchar* _buf);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglDebugMessageCallbackKHR)(GLDEBUGPROCKHR _callback, const void* _userParam);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern MZGLuint (MAZE_GL_FUNCPTR *mzglGetDebugMessageLogKHR)(MZGLuint _count, MZGLsizei _bufSize, MZGLenum* _sources, MZGLenum* _types, MZGLuint* _ids, MZGLenum* _severities, MZGLsizei* _lengths, MZGLchar* _messageLog);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglPushDebugGroupKHR)(MZGLenum _source, MZGLuint _id, MZGLsizei _length, const MZGLchar* _message);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglPopDebugGroupKHR)(void);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglObjectLabelKHR)(MZGLenum _identifier, MZGLuint _name, MZGLsizei _length, const MZGLchar* _label);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetObjectLabelKHR)(MZGLenum _identifier, MZGLuint _name, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _label);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglObjectPtrLabelKHR)(const void* _ptr, MZGLsizei _length, const MZGLchar* _label);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetObjectPtrLabelKHR)(const void* _ptr, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _label);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglGetPointervKHR)(MZGLenum _pname, void** _params);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API extern void (MAZE_GL_FUNCPTR *mzglPolygonMode)(MZGLenum _face, MZGLenum _mode);


#ifdef __cplusplus
}
#endif // __cplusplus


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API bool GLCheckOpenGLError(CString _file, U32 _line, CString _expression);
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API void GLCallStarted();
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API void GLCallEnded(CString _file, U32 _line, CString _expression);


    //////////////////////////////////////////
    #define MAZE_GL_CALL_FORCE(x)   do\
                                    {\
                                        Maze::GLCallStarted();\
                                        x;\
                                        Maze::GLCallEnded(__FILE__, __LINE__, #x);\
                                        Maze::GLCheckOpenGLError(__FILE__, __LINE__, #x);\
                                    }\
                                    while (false)

    //////////////////////////////////////////
    #define MAZE_GL_CHECK_ERROR()   do\
                                    {\
                                        Maze::GLCallStarted();\
                                        Maze::GLCallEnded(__FILE__, __LINE__, nullptr);\
                                        Maze::GLCheckOpenGLError(__FILE__, __LINE__, nullptr);\
                                    }\
                                    while (false)


    //////////////////////////////////////////
    #if (MAZE_DEBUG_GL)
    #    define MAZE_GL_CALL(x) MAZE_GL_CALL_FORCE(x)
    #else
    #    define MAZE_GL_CALL(x) x
    #endif

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFunctionsOpenGL_hpp_
//////////////////////////////////////////
