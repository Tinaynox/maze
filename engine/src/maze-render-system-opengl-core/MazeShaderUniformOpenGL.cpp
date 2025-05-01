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
                // Will be uploaded in bindTextures
                break;
            }

            case ShaderUniformType::UniformVec2F32:
            {
                MAZE_GL_CALL(mzglUniform2f((MZGLint)m_location, getVec4F32().x, getVec4F32().y));
                break;
            }

            case ShaderUniformType::UniformVec3F32:
            {
                MAZE_GL_CALL(mzglUniform3f((MZGLint)m_location, getVec4F32().x, getVec4F32().y, getVec4F32().z));
                break;
            }

            case ShaderUniformType::UniformVec4F32:
            {
                MAZE_GL_CALL(mzglUniform4f((MZGLint)m_location, getVec4F32().x, getVec4F32().y, getVec4F32().z, getVec4F32().w));
                break;
            }


            case ShaderUniformType::UniformVec2S32:
            {
                MAZE_GL_CALL(mzglUniform2i((MZGLint)m_location, getVecS().x, getVecS().y));
                break;
            }

            case ShaderUniformType::UniformVec3S32:
            {
                MAZE_GL_CALL(mzglUniform3i((MZGLint)m_location, getVecS().x, getVecS().y, getVecS().z));
                break;
            }

            case ShaderUniformType::UniformVec4S32:
            {
                MAZE_GL_CALL(mzglUniform4i((MZGLint)m_location, getVecS().x, getVecS().y, getVecS().z, getVecS().w));
                break;
            }


            case ShaderUniformType::UniformVec2U32:
            {
                MAZE_GL_CALL(mzglUniform2ui((MZGLint)m_location, getVecU().x, getVecU().y));
                break;
            }

            case ShaderUniformType::UniformVec3U32:
            {
                MAZE_GL_CALL(mzglUniform3ui((MZGLint)m_location, getVecU().x, getVecU().y, getVecU().z));
                break;
            }

            case ShaderUniformType::UniformVec4U32:
            {
                MAZE_GL_CALL(mzglUniform4ui((MZGLint)m_location, getVecU().x, getVecU().y, getVecU().z, getVecU().w));
                break;
            }


            case ShaderUniformType::UniformVec2B:
            {
                MAZE_GL_CALL(mzglUniform2i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y));
                break;
            }

            case ShaderUniformType::UniformVec3B:
            {
                MAZE_GL_CALL(mzglUniform3i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y, (MZGLint)getVecB().z));
                break;
            }

            case ShaderUniformType::UniformVec4B:
            {
                MAZE_GL_CALL(mzglUniform4i((MZGLint)m_location, (MZGLint)getVecB().x, (MZGLint)getVecB().y, (MZGLint)getVecB().z, (MZGLint)getVecB().w));
                break;
            }


            case ShaderUniformType::UniformMat3F32:
            {
                MAZE_GL_CALL(mzglUniformMatrix3fv((MZGLint)m_location, 1, MAZE_GL_FALSE, getMat3F32().getPlaneMatrix()));
                break;
            }

            case ShaderUniformType::UniformMat4F32:
            {
                MAZE_GL_CALL(mzglUniformMatrix4fv((MZGLint)m_location, 1, MAZE_GL_FALSE, getMat4F32().getPlaneMatrix()));
                break;
            }

            case ShaderUniformType::UniformTMat:
            {
                // 4 * Vec3F
                MAZE_GL_CALL(mzglUniform3fv((MZGLint)m_location, (MZGLsizei)4, (MZGLfloat const*) getTMat().getPlaneMatrix()));
                break;
            }

            case ShaderUniformType::UniformColorF128:
            {
                MAZE_GL_CALL(mzglUniform4f((MZGLint)m_location, getVec4F32().x, getVec4F32().y, getVec4F32().z, getVec4F32().w));
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
        
        // #TODO: Clean this mess
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

            case ShaderUniformType::UniformVec2F32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3F32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4F32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2S32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3S32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4S32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2U32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec3U32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec4U32:
            {
                
                break;
            }

            case ShaderUniformType::UniformVec2B:
            {

                break;
            }

            case ShaderUniformType::UniformVec3B:
            {

                break;
            }

            case ShaderUniformType::UniformVec4B:
            {

                break;
            }

            case ShaderUniformType::UniformMat3F32:
            {
                
                break;
            }

            case ShaderUniformType::UniformMat4F32:
            {
                
                break;
            }

            case ShaderUniformType::UniformTMat:
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
    void ShaderUniformOpenGL::upload(Vec2F const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform2fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Vec3F const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform3fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Vec4F const* _vectors, Size _count)
    {
        MAZE_GL_CALL(mzglUniform4fv((MZGLint)m_location, (MZGLsizei)_count, (MZGLfloat const*)_vectors));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Mat3F const* _matrices, Size _count)
    {
        MAZE_GL_CALL(mzglUniformMatrix3fv((MZGLint)m_location, (MZGLsizei)_count, MAZE_GL_FALSE, _matrices[0].getPlaneMatrix()));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(Mat4F const* _matrices, Size _count)
    {
        MAZE_GL_CALL(mzglUniformMatrix4fv((MZGLint)m_location, (MZGLsizei)_count, MAZE_GL_FALSE, _matrices[0].getPlaneMatrix()));
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::upload(TMat const* _matrices, Size _count)
    {
        // 4 * Vec3F
        MAZE_GL_CALL(mzglUniform3fv((MZGLint)m_location, (MZGLsizei)_count * 4, (MZGLfloat const*)_matrices));
    }


    //////////////////////////////////////////
    void ShaderUniformOpenGL::setTextureIndex(MZGLint _textureIndex)
    {
        if (m_textureIndex == _textureIndex)
            return;

        m_textureIndex = _textureIndex;

        if (m_textureIndex != -1)
        {

            if (getType() == ShaderUniformType::UniformTexture2D ||
                getType() == ShaderUniformType::UniformTextureCube)
            {
                ShaderOpenGLScopeBind scopeBind(m_shaderRaw);

                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

                MAZE_GL_CALL(mzglUniform1i((MZGLint)m_location, m_textureIndex));
            }
        }
    }

    //////////////////////////////////////////
    void ShaderUniformOpenGL::setTextureIndices(MZGLint* _textureIndices, U32 _count)
    {
        m_textureIndex = -1;

        if (getType() == ShaderUniformType::UniformTexture2DArray)
        {
            ShaderOpenGLScopeBind scopeBind(m_shaderRaw);

            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

            MAZE_GL_CALL(mzglUniform1iv((MZGLint)m_location, (MZGLsizei)_count, (MZGLint const*)_textureIndices));
        }
    }

} // namespace Maze
//////////////////////////////////////////
