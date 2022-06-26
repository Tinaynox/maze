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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ShaderUniform
    //
    //////////////////////////////////////////
    ShaderUniform::ShaderUniform()
        : m_shaderRaw(nullptr)
    {
    }

    //////////////////////////////////////////
    ShaderUniform::~ShaderUniform()
    {
    }

    //////////////////////////////////////////
    ShaderUniformPtr ShaderUniform::Create(ShaderPtr const& _shader, ShaderUniformType _type)
    {
        return _shader->getRenderSystem()->createShaderUniform(_shader, _type);
    }

    //////////////////////////////////////////
    bool ShaderUniform::init(ShaderPtr const& _shader, ShaderUniformType _type)
    {
        if (!_shader)
            return false;

        m_shader = _shader;
        m_shaderRaw = _shader.get();

        m_value = ShaderUniformVariant(m_shaderRaw->getRenderSystemRaw(), _type);
        m_value.setType(_type);

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(S32 _value)
    {
        if (getS32() == _value &&
            getType() == ShaderUniformType::UniformS32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_value);
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(F32 _value)
    {
        if (getF32() == _value &&
            getType() == ShaderUniformType::UniformF32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_value);
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(F64 _value)
    {
        if (getF64() == _value &&
            getType() == ShaderUniformType::UniformF64)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_value); 
        
        processSimpleUniformChanged();
        
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(bool _value)
    {
        if (getBool() == _value &&
            getType() == ShaderUniformType::UniformBool)
            return false;

        processUniformWillBeChanged();

        m_value.set(_value);

        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Texture2DPtr const& _texture2D)
    {
        if (getTexture2D() == _texture2D &&
            getType() == ShaderUniformType::UniformTexture2D)
            return false;

        processUniformWillBeChanged();
        
        m_value.set(_texture2D); 
        
        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Texture2D** _textures, U32 _count)
    {
        if (getPtr() == _textures &&
            getType() == ShaderUniformType::UniformTexture2DArray)
            return false;

        processUniformWillBeChanged();

        m_value.set(_textures, _count);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(TextureCubePtr const& _textureCube)
    {
        if (getTextureCube() == _textureCube &&
            getType() == ShaderUniformType::UniformTextureCube)
            return false;

        processUniformWillBeChanged();

        m_value.set(_textureCube);

        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2DF const& _vector)
    {
        if (getVec2DF() == _vector &&
            getType() == ShaderUniformType::UniformVec2DF)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3DF const& _vector)
    {
        if (getVec3DF() == _vector &&
            getType() == ShaderUniformType::UniformVec3DF)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4DF const& _vector)
    {
        if (getVec4DF() == _vector &&
            getType() == ShaderUniformType::UniformVec4DF)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();

        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2DS const& _vector)
    {
        if (getVec2DS() == _vector &&
            getType() == ShaderUniformType::UniformVec2DS)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3DS const& _vector)
    {
        if (getVec3DS() == _vector &&
            getType() == ShaderUniformType::UniformVec3DS)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4DS const& _vector)
    {
        if (getVec4DS() == _vector &&
            getType() == ShaderUniformType::UniformVec4DS)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();

        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2DU const& _vector)
    {
        if (getVec2DU() == _vector &&
            getType() == ShaderUniformType::UniformVec2DU)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3DU const& _vector)
    {
        if (getVec3DU() == _vector &&
            getType() == ShaderUniformType::UniformVec3DU)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4DU const& _vector)
    {
        if (getVec4DU() == _vector &&
            getType() == ShaderUniformType::UniformVec4DU)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();
        
        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2DB const& _vector)
    {
        if (getVec2DB() == _vector &&
            getType() == ShaderUniformType::UniformVec2DB)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3DB const& _vector)
    {
        if (getVec3DB() == _vector &&
            getType() == ShaderUniformType::UniformVec3DB)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4DB const& _vector)
    {
        if (getVec4DB() == _vector &&
            getType() == ShaderUniformType::UniformVec4DB)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }
        

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat3DF const& _matrix)
    {
        if (getMat3DF() == _matrix &&
            getType() == ShaderUniformType::UniformMat3DF)
        {
            return false;
        }

        processUniformWillBeChanged();
        
        m_value.set(_matrix);
        
        processSimpleUniformChanged();
        
        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::setAffineMat(Mat3DF const& _matrix)
    {
        if (getMat3DF().compareAffine(_matrix) &&
            getType() == ShaderUniformType::UniformMat3DF)
        {
            return false;
        }
        
        processUniformWillBeChanged();
        
        m_value.set(_matrix); 
        
        processSimpleUniformChanged();
        
        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Mat4DF const& _matrix)
    {
        if (getMat4DF() == _matrix &&
            getType() == ShaderUniformType::UniformMat4DF)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_matrix); 
        
        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(F32 const* _values, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_values, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2DF const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3DF const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4DF const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat3DF const* _matrices, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_matrices, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat4DF const* _matrices, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_matrices, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(ColorF128 const& _value)
    {
        if (   getVec4DF() == _value
            && getType() == ShaderUniformType::UniformColorF128)
            return false;

        processUniformWillBeChanged();

        m_value.set(_value);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(ShaderUniformVariant const& _variant)
    {
        switch (_variant.getType())
        {
            case ShaderUniformType::None:                  return false;
            case ShaderUniformType::UniformS32:            return set(_variant.getS32());
            case ShaderUniformType::UniformF32:            return set(_variant.getF32());
            case ShaderUniformType::UniformF64:            return set(_variant.getF64());
            case ShaderUniformType::UniformTexture2D:      return set(std::static_pointer_cast<Texture2D>(_variant.getTexture()));
            case ShaderUniformType::UniformTextureCube:    return set(std::static_pointer_cast<TextureCube>(_variant.getTexture()));
            case ShaderUniformType::UniformTexture2DArray: return set(static_cast<Texture2D**>(_variant.getPtr()), _variant.getCount());
            case ShaderUniformType::UniformVec2DS:         return set(_variant.getVec2DS());
            case ShaderUniformType::UniformVec3DS:         return set(_variant.getVec3DS());
            case ShaderUniformType::UniformVec4DS:         return set(_variant.getVec4DS());
            case ShaderUniformType::UniformVec2DU:         return set(_variant.getVec2DU());
            case ShaderUniformType::UniformVec3DU:         return set(_variant.getVec3DU());
            case ShaderUniformType::UniformVec4DU:         return set(_variant.getVec4DU());
            case ShaderUniformType::UniformVec2DF:         return set(_variant.getVec2DF());
            case ShaderUniformType::UniformVec3DF:         return set(_variant.getVec3DF());
            case ShaderUniformType::UniformVec4DF:         return set(_variant.getVec4DF());
            case ShaderUniformType::UniformMat3DF:         return set(_variant.getMat3DF());
            case ShaderUniformType::UniformMat4DF:         return set(_variant.getMat4DF());
            case ShaderUniformType::UniformColorF128:      return set(_variant.getColorF128());
            default:
            {
                MAZE_ERROR("Unsupported uniform variant - %d", (S32)_variant.getType());    
            }
        }

        return false;
    }


} // namespace Maze
//////////////////////////////////////////
