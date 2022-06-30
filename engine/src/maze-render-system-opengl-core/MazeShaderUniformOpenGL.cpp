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
#include "maze-graphics/MazeTexture.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ShaderUniformOpenGL
    //
    //////////////////////////////////////////
    ShaderUniformOpenGL::ShaderUniformOpenGL()
        : m_location(0)
        , m_textureIndex(-1)
    {

    }

    //////////////////////////////////////////
    ShaderUniformOpenGL::~ShaderUniformOpenGL()
    {
    }

    //////////////////////////////////////////
    ShaderUniformOpenGLPtr ShaderUniformOpenGL::Create(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        ShaderUniformOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformOpenGL, object, init(_shader, _type));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformOpenGL::init(ShaderPtr const& _shader, ShaderUniformType _type)
    {
        if (!ShaderUniform::init(_shader, _type))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::processUniformWillBeChanged()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL* ShaderUniformOpenGL::getRenderSystemOpenGLRaw() const
    {
        if (!m_shaderRaw)
            return nullptr;

        ShaderOpenGL* shaderOpenGL = m_shaderRaw->castRaw<ShaderOpenGL>();
        return shaderOpenGL->getRenderSystemOpenGLRaw();
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::processSimpleUniformChanged()
    {
        ShaderOpenGLScopeBind scopeBind(m_shaderRaw);
        
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        
        switch (getType())
        {
            case ShaderUniformType::UniformS32:
            {
                MAZE_GL_CALL(mzglUniform1i((MZGLint)m_location, getS32()));
                break;
            }

            case ShaderUniformType::UniformF32:
            {
                MAZE_GL_CALL(mzglUniform1f((MZGLint)m_location, getF32()));
                break;
            }

            case ShaderUniformType::UniformF64:
            {
                MAZE_GL_CALL(mzglUniform1f((MZGLint)m_location, (F32)getF64()));
                break;
            }

            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            case ShaderUniformType::UniformTexture2DArray:
            {
                m_textureIndex = -1;
                m_shaderRaw->castRaw<ShaderOpenGL>()->assignUniformTextureIndexes();

                break;
            }

            case ShaderUniformType::UniformVec2DF:
            {
                MAZE_GL_CALL(mzglUniform2f((MZGLint)m_location, getVecF().x, getVecF().y));
                break;
            }

            case ShaderUniformType::UniformVec3DF:
            {
                MAZE_GL_CALL(mzglUniform3f((MZGLint)m_location, getVecF().x, getVecF().y, getVecF().z));
                break;
            }

            case ShaderUniformType::UniformVec4DF:
            {
                MAZE_GL_CALL(mzglUniform4f((MZGLint)m_location, getVecF().x, getVecF().y, getVecF().z, getVecF().w));
                break;
            }


            case ShaderUniformType::UniformVec2DS:
            {
                MAZE_GL_CALL(mzglUniform2i((MZGLint)m_location, getVecS().x, getVecS().y));
                break;
            }

            case ShaderUniformType::UniformVec3DS:
            {
                MAZE_GL_CALL(mzglUniform3i((MZGLint)m_location, getVecS().x, getVecS().y, getVecS().z));
                break;
            }

            case ShaderUniformType::UniformVec4DS:
            {
                MAZE_GL_CALL(mzglUniform4i((MZGLint)m_location, getVecS().x, getVecS().y, getVecS().z, getVecS().w));
                break;
            }


            case ShaderUniformType::UniformVec2DU:
            {
                MAZE_GL_CALL(mzglUniform2ui((MZGLint)m_location, getVecU().x, getVecU().y));
                break;
            }

            case ShaderUniformType::UniformVec3DU:
            {
                MAZE_GL_CALL(mzglUniform3ui((MZGLint)m_location, getVecU().x, getVecU().y, getVecU().z));
                break;
            }

            case ShaderUniformType::UniformVec4DU:
            {
                MAZE_GL_CALL(mzglUniform4ui((MZGLint)m_location, getVecU().x, getVecU().y, getVecU().z, getVecU().w));
                break;
            }


            case ShaderUniformType::UniformVec2DB:
            {
                MAZE_GL_CALL(mzglUniform2i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y));
                break;
            }

            case ShaderUniformType::UniformVec3DB:
            {
                MAZE_GL_CALL(mzglUniform3i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y, (MZGLint)getVecB().z));
                break;
            }

            case ShaderUniformType::UniformVec4DB:
            {
                MAZE_GL_CALL(mzglUniform4i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y, (MZGLint)getVecB().z, (MZGLint)getVecB().w));
                break;
            }


            case ShaderUniformType::UniformMat3DF:
            {
                MAZE_GL_CALL(mzglUniformMatrix3fv((MZGLint)m_location, 1, MAZE_GL_FALSE, getMat3DF().getPlaneMatrix()));
                break;
            }

            case ShaderUniformType::UniformMat4DF:
            {
                MAZE_GL_CALL(mzglUniformMatrix4fv((MZGLint)m_location, 1, MAZE_GL_FALSE, getMat4DF().getPlaneMatrix()));
                break;
            }


            case ShaderUniformType::UniformColorF128:
            {
                MAZE_GL_CALL(mzglUniform4f((MZGLint)m_location, getVecF().x, getVecF().y, getVecF().z, getVecF().w));
                break;
            }

            default:
            {
                MAZE_DEBUG_BP;
            }
        }

    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::validateState()
    {
        ShaderOpenGLScopeBind scopeBind(m_shaderRaw);
        
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        
        switch (getType())
        {
            case ShaderUniformType::UniformS32:
            {
                
                break;
            }

            case ShaderUniformType::UniformF32:
            {
                MZGLfloat value;
                MAZE_GL_CALL(
                    mzglGetUniformfv(
                        m_shaderRaw->castRaw<ShaderOpenGL>()->getProgramId(),
                        (MZGLint)m_location,
                        &value));

                MAZE_ERROR_IF(Math::Abs(value - getF32()) >= 0.00001, "GPU value is invalid GPU=%.3f CPU=%.3f", value, getF32());

                break;
            }

            case ShaderUniformType::UniformF64:
            {
                
                break;
            }

            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            case ShaderUniformType::UniformTexture2DArray:
            {

                break;
            }

            case ShaderUniformType::UniformVec2DF:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3DF:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4DF:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2DS:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3DS:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4DS:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2DU:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3DU:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4DU:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2DB:
            {

                break;
            }

            case ShaderUniformType::UniformVec3DB:
            {

                break;
            }

            case ShaderUniformType::UniformVec4DB:
            {

                break;
            }

            case ShaderUniformType::UniformMat3DF:
            {
                
                break;
            }

            case ShaderUniformType::UniformMat4DF:
            {
                
                break;
            }

            case ShaderUniformType::UniformColorF128:
            {

                break;
            }

            default:
            {
                MAZE_DEBUG_BP;
            }
        }
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(F32 const* _value, Size _count)
    {
        MAZE_GL_CALL(mzglUniform1fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_value));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Vec2DF const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform2fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Vec3DF const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform3fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Vec4DF const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform4fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Mat3DF const* _matrices, Size _count)
    {
        MAZE_GL_CALL(mzglUniformMatrix3fv((MZGLint)m_location, (MZGLsizei)_count, MAZE_GL_FALSE, _matrices[0].getPlaneMatrix()));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Mat4DF const* _matrices, Size _count)
    {
        MAZE_GL_CALL(mzglUniformMatrix4fv((MZGLint)m_location, (MZGLsizei)_count, MAZE_GL_FALSE, _matrices[0].getPlaneMatrix()));
    }


    //////////////////////////////////////////
    void ShaderUniformOpenGL::setTextureIndex(MZGLint _textureIndex)
    {
        if (m_textureIndex == _textureIndex)
            return;

        m_textureIndex = _textureIndex;

        if (getType() == ShaderUniformType::UniformTexture2D ||
            getType() == ShaderUniformType::UniformTextureCube)
        {
            ShaderOpenGLScopeBind scopeBind(m_shaderRaw);

            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

            MAZE_GL_CALL(mzglUniform1i((MZGLint)m_location, m_textureIndex));
        }
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::setTextureIndices(MZGLint* _textureIndices, U32 _count)
    {
        if (m_textureIndex == *_textureIndices)
            return;

        m_textureIndex = *_textureIndices;

        if (getType() == ShaderUniformType::UniformTexture2DArray)
        {
            ShaderOpenGLScopeBind scopeBind(m_shaderRaw);

            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

            MAZE_GL_CALL(mzglUniform1iv((MZGLint)m_location, (MZGLsizei)_count, (MZGLint const*)_textureIndices));
        }
    }

} // namespace Maze
//////////////////////////////////////////
