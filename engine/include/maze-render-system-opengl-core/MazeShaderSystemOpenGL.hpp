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
#if (!defined(_MazeShaderSystemOpenGL_hpp_))
#define _MazeShaderSystemOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderSystemOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class ShaderSystemOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ShaderSystemOpenGL
        : public ShaderSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderSystemOpenGL();

        //////////////////////////////////////////
        static void Initialize(ShaderSystemPtr& _object, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual ShaderPtr createShader(AssetFilePtr const& _shaderFile) MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline MZGLint getGLSLVersion() const { return m_glslVersion; }

        //////////////////////////////////////////
        virtual ShaderPtr const& createBuiltinShader(BuiltinShaderType _shaderType) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderSystemOpenGL();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGL();

        //////////////////////////////////////////
        void notifyGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyGLContextSetup(ContextOpenGL* _contextOpenGL);
    
    protected:
        MZGLint m_glslVersion;

        ContextOpenGLPtr m_contextOpenGL;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystemOpenGL_hpp_
//////////////////////////////////////////
