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
#if (!defined(_MazeShaderUniformOpenGL_hpp_))
#define _MazeShaderUniformOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(ShaderUniformOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class ShaderUniformOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ShaderUniformOpenGL
        : public ShaderUniform
    {

        //////////////////////////////////////////
        friend class ShaderUniform;
        friend class ShaderOpenGL;

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformOpenGL();

        //////////////////////////////////////////
        static ShaderUniformOpenGLPtr Create(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None);


        //////////////////////////////////////////
        inline MZGLint getLocation() const { return m_location; }

        //////////////////////////////////////////
        inline MZGLint getTextureIndex() const { return m_textureIndex; }

        //////////////////////////////////////////
        void validateState();


        //////////////////////////////////////////
        virtual void upload(F32 const* _values, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec2F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec3F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec4F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat3F const* _matrices, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat4F const* _matrices, Size _count) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformOpenGL();

        //////////////////////////////////////////
        virtual bool init(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline void setLocation(MZGLint _location) { m_location = _location; }
        

        //////////////////////////////////////////
        virtual void processUniformWillBeChanged() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processSimpleUniformChanged() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw() const;

        //////////////////////////////////////////
        void setTextureIndex(MZGLint _textureIndex);

        //////////////////////////////////////////
        void setTextureIndices(MZGLint* _textureIndices, U32 _count);

    protected:
        MZGLint m_location;

        MZGLint m_textureIndex;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformOpenGL_hpp_
//////////////////////////////////////////
