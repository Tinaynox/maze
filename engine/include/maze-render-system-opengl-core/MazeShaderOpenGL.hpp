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
#if (!defined(_MazeShaderOpenGL_hpp_))
#define _MazeShaderOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class ShaderOpenGLScopeBind
    //
    //////////////////////////////////////////
    class ShaderOpenGLScopeBind
    {
    public:

        ////////////////////////////////////
        ShaderOpenGLScopeBind(Shader* _newShader);

        ////////////////////////////////////
        ~ShaderOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;
        Shader* m_prevShader;
    };


    //////////////////////////////////////////
    // Class ShaderOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ShaderOpenGL
        : public Shader
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderOpenGL, Shader);

    public:

        //////////////////////////////////////////
        virtual ~ShaderOpenGL();

        //////////////////////////////////////////
        static ShaderPtr CreateFromFile(
            RenderSystemPtr const& _renderSystem,
            ContextOpenGL* _contextOpenGL,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        static ShaderPtr CreateFromSource(
            RenderSystemPtr const& _renderSystem,
            ContextOpenGL* _contextOpenGL,
            String const& _shaderSource,
            CString _shaderName = nullptr);

        //////////////////////////////////////////
        static ShaderPtr Create(ShaderOpenGLPtr const& _shader);

        //////////////////////////////////////////
        virtual ShaderPtr createCopy() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline MZGLuint getProgramId() const { return m_programId; }
        
        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw();

        //////////////////////////////////////////
        virtual bool loadFromSource(String const& _shaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void assignUniformTextureIndexes();

        //////////////////////////////////////////
        void bindTextures();

        //////////////////////////////////////////
        virtual CString getLanguage() const MAZE_OVERRIDE { return "GLSL"; }


        //////////////////////////////////////////
        ContextOpenGL* getContextOpenGL() const { return m_context; }


        //////////////////////////////////////////
        virtual void recompile() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderOpenGL();

        //////////////////////////////////////////
        using Shader::init;
        
        //////////////////////////////////////////
        virtual bool init(
            RenderSystemPtr const& _renderSystem,
            ContextOpenGL* _contextOpenGL,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemPtr const& _renderSystem,
            ContextOpenGL* _contextOpenGL,
            String const& _shaderSource,
            CString _shaderName = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            ShaderOpenGLPtr const& _shader);
        
        //////////////////////////////////////////
        virtual ShaderUniformPtr const& createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;
    

        //////////////////////////////////////////
        bool loadGLShader(String const& _vertexShaderSource, String const& _fragmentShaderSource);

        //////////////////////////////////////////
        bool unloadGLShader();


        //////////////////////////////////////////
        String makeInternalShaderPreprocessing(String _shader);

        //////////////////////////////////////////
        bool compileGLShader(MZGLuint& _shader, MZGLenum _type, MZGLchar const* _source);

        //////////////////////////////////////////
        String logForOpenGLShader(MZGLuint _shaderId);

        //////////////////////////////////////////
        String logForOpenGLProgram(MZGLuint _programId);


        //////////////////////////////////////////
        void notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        void setContextOpenGL(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        UnorderedMap<MZGLenum, String> preProcessSource(String const& _source);

        //////////////////////////////////////////
        String buildLocalShaderFeatures();


        //////////////////////////////////////////
        void assignUniforms();

    protected:
        ContextOpenGL* m_context;

        MZGLuint m_programId;

        String m_vertexShaderSource;
        String m_fragmentShaderSource;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderOpenGL_hpp_
//////////////////////////////////////////
