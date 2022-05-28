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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"


//////////////////////////////////////////
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglEnable)(MZGLenum _cap) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDisable)(MZGLenum _cap) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglEnablei)(MZGLenum _cap, MZGLuint _index) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDisablei)(MZGLenum _cap, MZGLuint _index) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendFunc)(MZGLenum _src, MZGLenum _dst) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendFunci)(MZGLuint _buf, MZGLenum _src, MZGLenum _dst) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendFuncSeparate)(MZGLenum _sfactorRGB, MZGLenum _dfactorRGB, MZGLenum _sfactorAlpha, MZGLenum _dfactorAlpha) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendFuncSeparatei)(MZGLuint _buf, MZGLenum _srcRGB, MZGLenum _dstRGB, MZGLenum _srcAlpha, MZGLenum _dstAlpha) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendEquation)(MZGLenum _mode) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlendEquationi)(MZGLuint _buf, MZGLenum _mode) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindRenderbuffer)(MZGLenum _target, MZGLuint _renderbuffer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenFramebuffers)(MZGLsizei _n, MZGLuint* _framebuffers) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDiscardFramebuffer)(MZGLenum _target, MZGLsizei _numAttachments, MZGLenum const* _attachments) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteRenderbuffers)(MZGLsizei _n, MZGLuint const* _renderbuffers) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFramebufferRenderbuffer)(MZGLenum _target, MZGLenum _attachment, MZGLenum _renderbuffertarget, MZGLuint _renderbuffer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindFramebuffer)(MZGLenum _target, MZGLuint _framebuffer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawBuffers)(MZGLsizei _n, MZGLenum const* _bufs) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFramebufferTexture2D)(MZGLenum _target, MZGLenum _attachment, MZGLenum _textarget, MZGLuint _texture, MZGLint _level) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteFramebuffers)(MZGLsizei _n, const MZGLuint* _framebuffers) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFlush)() = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFinish)() = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum (MAZE_GL_FUNCPTR *mzglGetError)() = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetFloatv)(MZGLenum _pname, MZGLfloat* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetIntegerv)(MZGLenum _pname, MZGLint* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetDoublev)(MZGLenum _pname, MZGLdouble* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetBufferParameteriv)(MZGLenum _target, MZGLenum _value, MZGLint* _data) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetProgramiv)(MZGLuint _program, MZGLenum _pname, MZGLint* _param) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLint (MAZE_GL_FUNCPTR *mzglGetUniformLocation)(MZGLuint _program, MZGLchar const* _name) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetActiveUniform)(MZGLuint _program, MZGLuint _index, MZGLsizei _bufSize, MZGLsizei* _length, MZGLint* _size, MZGLenum* _type, MZGLchar* _name) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLint (MAZE_GL_FUNCPTR *mzglGetAttribLocation)(MZGLuint _program, MZGLchar const* _name) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetShaderiv)(MZGLuint _shader, MZGLenum _pname, MZGLint* _param) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetUniformfv)(MZGLuint _program, MZGLint _location, MZGLfloat* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetShaderInfoLog)(MZGLuint _shader, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _infoLog) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetProgramInfoLog)(MZGLuint _program, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _infoLog) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API const MZGLubyte* (MAZE_GL_FUNCPTR *mzglGetString)(MZGLenum _name) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API const MZGLubyte* (MAZE_GL_FUNCPTR *mzglGetStringi)(MZGLenum _name, MZGLuint _index) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLboolean (MAZE_GL_FUNCPTR *mzglIsEnabled)(MZGLenum _cap) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglEnableVertexAttribArray)(MZGLuint _index) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDisableVertexAttribArray)(MZGLuint _index) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglViewport)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglScissor)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglActiveTexture)(MZGLenum _texture) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindTexture)(MZGLenum _target, MZGLuint _texture) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint (MAZE_GL_FUNCPTR *mzglCreateProgram)() = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint (MAZE_GL_FUNCPTR *mzglCreateShader)(MZGLenum _type) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglShaderSource)(MZGLuint _shader, MZGLsizei _count, MZGLchar const* const* _string, MZGLint const* _length) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglCompileShader)(MZGLuint _shader) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUseProgram)(MZGLuint _program) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteProgram)(MZGLuint _program) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglAttachShader)(MZGLuint _program, MZGLuint _shader) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglLinkProgram)(MZGLuint _program) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindAttribLocation)(MZGLuint _program, MZGLuint _index, MZGLchar const* _name) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDetachShader)(MZGLuint _program, MZGLuint _shader) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteShader)(MZGLuint _shader) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform1i)(MZGLint _location, MZGLint _v0) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform2i)(MZGLint _location, MZGLint _v0, MZGLint _v1) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform3i)(MZGLint _location, MZGLint _v0, MZGLint _v1, MZGLint _v2) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform4i)(MZGLint _location, MZGLint _v0, MZGLint _v1, MZGLint _v2, MZGLint _v3) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform1ui)(MZGLint _location, MZGLuint _v0) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform2ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform3ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1, MZGLuint _v2) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform4ui)(MZGLint _location, MZGLuint _v0, MZGLuint _v1, MZGLuint _v2, MZGLuint _v3) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform1f)(MZGLint _location, MZGLfloat _v0) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform2f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform3f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1, MZGLfloat _v2) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniform4f)(MZGLint _location, MZGLfloat _v0, MZGLfloat _v1, MZGLfloat _v2, MZGLfloat _v3) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglUniform1fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglUniform2fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglUniform3fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglUniform4fv)(MZGLint _location, MZGLsizei _count, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniformMatrix3fv)(MZGLint _location, MZGLsizei _count, MZGLboolean _transpose, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniformMatrix4fv)(MZGLint _location, MZGLsizei _count, MZGLboolean _transpose, MZGLfloat const* _value) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetVertexAttribiv)(MZGLuint _index, MZGLenum _pname, MZGLint* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetVertexAttribPointerv)(MZGLuint _index, MZGLenum _pname, void** _pointer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenQueries)(MZGLsizei _n, MZGLuint* _ids) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBeginQuery)(MZGLenum _target, MZGLuint _id) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglEndQuery)(MZGLenum _target) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetQueryObjectuiv)(MZGLuint _id, MZGLenum _pname, MZGLuint* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenBuffers)(MZGLsizei _n, MZGLuint* _buffers) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteBuffers)(MZGLsizei _n, const MZGLuint* _buffers) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglClear)(MZGLbitfield _mask) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglClearColor)(MZGLclampf _red, MZGLclampf _green, MZGLclampf _blue, MZGLclampf _alpha) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglClearDepth)(MZGLclampd _depth);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglVertexAttribPointer)(MZGLuint _index, MZGLint _size, MZGLenum _type, MZGLboolean _normalized, MZGLsizei _stride, void const* _pointer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindBuffer)(MZGLenum _target, MZGLuint _buffer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBufferData)(MZGLenum _target, MZGLsizeiptr _size, void const* _data, MZGLenum _usage) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBufferSubData)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _size, void const* _data) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBufferStorage)(MZGLenum _target, MZGLsizeiptr _size, void const* _data, MZGLbitfield _flags);
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawElements)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, void const* _indices) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawElementsInstanced)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, void const* _indices, MZGLsizei _primcount) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawElementsInstancedBaseVertex)(MZGLenum _mode, MZGLsizei _count, MZGLenum _type, MZGLvoid* _indices, MZGLsizei _primcount, MZGLint _basevertex) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawRangeElements)(MZGLenum _mode, MZGLuint _start, MZGLuint _end, MZGLsizei _count, MZGLenum _type, MZGLvoid const* _indices) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawRangeElementsBaseVertex)(MZGLenum _mode, MZGLuint _start, MZGLuint _end, MZGLsizei _count, MZGLenum _type, MZGLvoid* _indices, MZGLint _basevertex) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawArrays)(MZGLenum _mode, MZGLint _first, MZGLsizei _count) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDrawArraysInstanced)(MZGLenum _mode, MZGLint _first, MZGLsizei _count, MZGLsizei _primcount) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglVertexAttribDivisor)(MZGLuint _index, MZGLuint _divisor) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void* (MAZE_GL_FUNCPTR *mzglMapBufferRange)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _length, MZGLbitfield _access) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFlushMappedBufferRange)(MZGLenum _target, MZGLintptr _offset, MZGLsizeiptr _length) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglPixelStorei)(MZGLenum _pname, MZGLint _param) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenTextures)(MZGLsizei _n, MZGLuint* _textures) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteTextures)(MZGLsizei _n, MZGLuint const* _textures) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLboolean (MAZE_GL_FUNCPTR *mzglIsTexture)(MZGLuint _texture) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenerateMipmap)(MZGLenum _target) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglTexParameteri)(MZGLenum _target, MZGLenum _pname, MZGLint _param) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglCompressedTexImage2D)(MZGLenum _target, MZGLint _level, MZGLenum _internalformat, MZGLsizei _width, MZGLsizei _height, MZGLint _border, MZGLsizei _imageSize, MZGLvoid const* _data) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetTexLevelParameteriv)(MZGLenum _target, MZGLint _level, MZGLenum _pname, MZGLint* _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglTexImage2D)(MZGLenum _target, MZGLint _level, MZGLint _internalformat, MZGLsizei _width, MZGLsizei _height, MZGLint _border, MZGLenum _format, MZGLenum _type, MZGLvoid const* _pixels) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglTexImage2DMultisample)(MZGLenum _target, MZGLsizei _samples, MZGLenum _internalformat, MZGLsizei _width, MZGLsizei _height, MZGLboolean _fixedsamplelocations) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetTexImage)(MZGLenum _target, MZGLint _level, MZGLenum _format, MZGLenum _type, void* _pixels) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum (MAZE_GL_FUNCPTR *mzglCheckFramebufferStatus)(MZGLenum _target) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBlitFramebuffer)(MZGLint _srcX0, MZGLint _srcY0, MZGLint _srcX1, MZGLint _srcY1, MZGLint _dstX0, MZGLint _dstY0, MZGLint _dstX1, MZGLint _dstY1, MZGLbitfield _mask, MZGLenum _filter) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglCopyTexImage2D)(MZGLenum _target, MZGLint _level, MZGLenum internalFormat, MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height, MZGLint _border) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglTexSubImage2D)(MZGLenum _target, MZGLint _level, MZGLint _xoffset, MZGLint _yoffset, MZGLsizei _width, MZGLsizei _height, MZGLenum _format, MZGLenum _type, const void* _pixels) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglReadPixels)(MZGLint _x, MZGLint _y, MZGLsizei _width, MZGLsizei _height, MZGLenum _format, MZGLenum _type, MZGLvoid* _data) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGenVertexArrays)(MZGLsizei _n, MZGLuint* _arrays) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindVertexArray)(MZGLuint _array) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDeleteVertexArrays)(MZGLsizei _n, MZGLuint const* _arrays) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint (MAZE_GL_FUNCPTR *mzglGetUniformBlockIndex)(MZGLuint _program, MZGLchar const* _uniformBlockName) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglBindBufferBase)(MZGLenum _target, MZGLuint _index, MZGLuint _buffer) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglUniformBlockBinding)(MZGLuint _program, MZGLuint _uniformBlockIndex, MZGLuint _uniformBlockBinding) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void* (MAZE_GL_FUNCPTR *mzglMapBuffer)(MZGLenum _target, MZGLenum _access) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLboolean (MAZE_GL_FUNCPTR *mzglUnmapBuffer)(MZGLenum _target) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDepthFunc)(MZGLenum _func) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDepthMask)(MZGLboolean _flag) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglCullFace)(MZGLenum _mode) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglFrontFace)(MZGLenum _mode) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglClipPlane)(MZGLenum _plane, MZGLdouble const* _equation) = nullptr;

MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglDebugMessageControl)(MZGLenum _source, MZGLenum _type, MZGLenum _severity, MZGLsizei _count, MZGLuint* _ids, MZGLboolean _enabled) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR* mzglDebugMessageCallback)(GLDEBUGPROCKHR _callback, void const* _userParam) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDebugMessageControlKHR)(MZGLenum _source, MZGLenum _type, MZGLenum _severity, MZGLsizei _count, MZGLuint const* _ids, MZGLboolean _enabled) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDebugMessageInsertKHR)(MZGLenum _source, MZGLenum _type, MZGLuint _id, MZGLenum _severity, MZGLsizei _length, MZGLchar const* _buf) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglDebugMessageCallbackKHR)(GLDEBUGPROCKHR _callback, void const* _userParam) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint (MAZE_GL_FUNCPTR *mzglGetDebugMessageLogKHR)(MZGLuint _count, MZGLsizei _bufSize, MZGLenum* _sources, MZGLenum* _types, MZGLuint* _ids, MZGLenum* _severities, MZGLsizei* _lengths, MZGLchar* _messageLog) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglPushDebugGroupKHR)(MZGLenum _source, MZGLuint _id, MZGLsizei _length, MZGLchar const* _message) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglPopDebugGroupKHR)(void) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglObjectLabelKHR)(MZGLenum _identifier, MZGLuint _name, MZGLsizei _length, MZGLchar const* _label) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetObjectLabelKHR)(MZGLenum _identifier, MZGLuint _name, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _label) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglObjectPtrLabelKHR)(void const* _ptr, MZGLsizei _length, MZGLchar const* _label) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetObjectPtrLabelKHR)(void const* _ptr, MZGLsizei _bufSize, MZGLsizei* _length, MZGLchar* _label) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglGetPointervKHR)(MZGLenum _pname, void** _params) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL_CORE_API void (MAZE_GL_FUNCPTR *mzglPolygonMode)(MZGLenum _face, MZGLenum _mode);


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API bool GLCheckOpenGLError(CString _file, U32 _line, CString _expression)
    {
        if (!mzglGetError)
            return false;
        
        bool hasErrors = false;

        do 
        {
            MZGLenum error = mzglGetError();
            if (error == MAZE_GL_NO_ERROR) 
                break;

            String fileString = _file;
            fileString = fileString.substr(fileString.find_last_of("\\/") + 1);

            CString errorText = "";
            switch (error) 
            {
                case MAZE_GL_INVALID_ENUM:                      errorText = "GL_INVALID_ENUM";      break;
                case MAZE_GL_INVALID_VALUE:                     errorText = "GL_INVALID_VALUE";     break;
                case MAZE_GL_INVALID_OPERATION:                 errorText = "GL_INVALID_OPERATION"; break;
                case MAZE_GL_OUT_OF_MEMORY:                     errorText = "GL_OUT_OF_MEMORY";     break;
                case MAZE_GL_STACK_OVERFLOW:                    errorText = "GL_STACK_OVERFLOW";    break;
                case MAZE_GL_STACK_UNDERFLOW:                   errorText = "GL_STACK_UNDERFLOW";   break;
                case MAZE_GL_INVALID_FRAMEBUFFER_OPERATION:     errorText = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                default:                                        errorText = "Unknown OpenGL error"; break;
            }

            if (_expression)
            {
                MAZE_ERROR("OpenGL Error!\n\tFile: %s(%u).\n\tnExpression:%s\n\tError:%s", fileString.c_str(), _line, _expression, errorText);
            }
            else
            {
                MAZE_ERROR("OpenGL Error!\n\tFile: %s(%u).\n\tError:%s", fileString.c_str(), _line, errorText);
            }

            switch (error) 
            {
                case MAZE_GL_INVALID_FRAMEBUFFER_OPERATION:
                {
                    MZGLint frameBuffer;
                    MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_DRAW_FRAMEBUFFER_BINDING, &frameBuffer));

                    Debug::logerr << "Current Framebuffer: " << frameBuffer << endl;

                    break;
                }
            }

            hasErrors = true;
        } 
        while (false);
            
        return hasErrors;
    }

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API void GLCallStarted()
    {
        if (mzglGetError)
        {
            MZGLenum error = mzglGetError();
            MAZE_ERROR_IF(error != MAZE_GL_NO_ERROR, "GL Error occurred before MAZE_DEBUG_GL call!") ;
        }
    }

    ////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API void GLCallEnded(CString _file, U32 _line, CString _expression)
    {
        
    }


} // namespace Maze
//////////////////////////////////////////
