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
    bool ShaderUniform::set(Vec2F const& _vector)
    {
        if (getVec2F32() == _vector &&
            getType() == ShaderUniformType::UniformVec2F32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3F const& _vector)
    {
        if (getVec3F32() == _vector &&
            getType() == ShaderUniformType::UniformVec3F32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4F const& _vector)
    {
        if (getVec4F32() == _vector &&
            getType() == ShaderUniformType::UniformVec4F32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();

        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2S const& _vector)
    {
        if (getVec2S32() == _vector &&
            getType() == ShaderUniformType::UniformVec2S32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3S const& _vector)
    {
        if (getVec3S32() == _vector &&
            getType() == ShaderUniformType::UniformVec3S32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4S const& _vector)
    {
        if (getVec4S32() == _vector &&
            getType() == ShaderUniformType::UniformVec4S32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();

        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2U const& _vector)
    {
        if (getVec2U32() == _vector &&
            getType() == ShaderUniformType::UniformVec2U32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3U const& _vector)
    {
        if (getVec3U32() == _vector &&
            getType() == ShaderUniformType::UniformVec3U32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector); 
        
        processSimpleUniformChanged();

        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4U const& _vector)
    {
        if (getVec4U32() == _vector &&
            getType() == ShaderUniformType::UniformVec4U32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_vector);
        
        processSimpleUniformChanged();
        
        return true;
    }


    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2B const& _vector)
    {
        if (getVec2B() == _vector &&
            getType() == ShaderUniformType::UniformVec2B)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3B const& _vector)
    {
        if (getVec3B() == _vector &&
            getType() == ShaderUniformType::UniformVec3B)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4B const& _vector)
    {
        if (getVec4B() == _vector &&
            getType() == ShaderUniformType::UniformVec4B)
            return false;

        processUniformWillBeChanged();

        m_value.set(_vector);

        processSimpleUniformChanged();

        return true;
    }
        

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat3F const& _matrix)
    {
        if (getMat3F32() == _matrix &&
            getType() == ShaderUniformType::UniformMat3F32)
        {
            return false;
        }

        processUniformWillBeChanged();
        
        m_value.set(_matrix);
        
        processSimpleUniformChanged();
        
        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::setAffineMat(Mat3F const& _matrix)
    {
        if (getMat3F32().compareAffine(_matrix) &&
            getType() == ShaderUniformType::UniformMat3F32)
        {
            return false;
        }
        
        processUniformWillBeChanged();
        
        m_value.set(_matrix); 
        
        processSimpleUniformChanged();
        
        return true;
    }
        
    //////////////////////////////////////////
    bool ShaderUniform::set(Mat4F const& _matrix)
    {
        if (getMat4F32() == _matrix &&
            getType() == ShaderUniformType::UniformMat4F32)
            return false;
        
        processUniformWillBeChanged();
        
        m_value.set(_matrix); 
        
        processSimpleUniformChanged();

        return true;
    }

    /*
    //////////////////////////////////////////
    bool ShaderUniform::set(F32 const* _values, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_values, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec2F const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec3F const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Vec4F const* _vectors, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_vectors, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat3F const* _matrices, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_matrices, _count);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderUniform::set(Mat4F const* _matrices, Size _count)
    {
        m_value.reset();
        uploadArrayUniform(_matrices, _count);
        return true;
    }
    */

    //////////////////////////////////////////
    bool ShaderUniform::set(ColorF128 const& _value)
    {
        if (   getVec4F32() == _value
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
            case ShaderUniformType::UniformVec2S32:         return set(_variant.getVec2S32());
            case ShaderUniformType::UniformVec3S32:         return set(_variant.getVec3S32());
            case ShaderUniformType::UniformVec4S32:         return set(_variant.getVec4S32());
            case ShaderUniformType::UniformVec2U32:         return set(_variant.getVec2U32());
            case ShaderUniformType::UniformVec3U32:         return set(_variant.getVec3U32());
            case ShaderUniformType::UniformVec4U32:         return set(_variant.getVec4U32());
            case ShaderUniformType::UniformVec2F32:         return set(_variant.getVec2F32());
            case ShaderUniformType::UniformVec3F32:         return set(_variant.getVec3F32());
            case ShaderUniformType::UniformVec4F32:         return set(_variant.getVec4F32());
            case ShaderUniformType::UniformVec2B:         return set(_variant.getVec2B());
            case ShaderUniformType::UniformVec3B:         return set(_variant.getVec3B());
            case ShaderUniformType::UniformVec4B:         return set(_variant.getVec4B());
            case ShaderUniformType::UniformMat3F32:         return set(_variant.getMat3F32());
            case ShaderUniformType::UniformMat4F32:         return set(_variant.getMat4F32());
            case ShaderUniformType::UniformColorF128:      return set(_variant.getColorF128());
            default:
            {
                MAZE_ERROR("%s: Unsupported ShaderUniformType - %s(%d), value=%s",
                    getName().c_str(), _variant.getType().toCString(), (S32)_variant.getType(), _variant.toStringValue().c_str());
            }
        }

        return false;
    }


} // namespace Maze
//////////////////////////////////////////
