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
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-graphics/assets/MazeAssetUnitTexture2D.hpp"
#include "maze-graphics/assets/MazeAssetUnitTextureCube.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ShaderUniformType);


    //////////////////////////////////////////
    // Class ShaderUniformVariant
    //
    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(
        RenderSystem* _renderSystem,
        ShaderUniformType _type,
        HashedCString _name)
        : m_renderSystem(_renderSystem)
        , m_type(_type)
        , m_name(_name)
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, S32 _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformS32)
        , m_S32(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, F32 _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformF32)
        , m_F32(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, F64 _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformF64)
        , m_F64(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, bool _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformBool)
        , m_bool(_value)
    {

    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Texture2D const* _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTexture2D)
        , m_texture(_value ? const_cast<Texture2D*>(_value)->cast<Texture2D>() : nullptr)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Texture2DPtr const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTexture2D)
        , m_texture(_value)
    {
    
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Texture2D const** _value, U32 _count)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTexture2DArray)
        , m_ptr(_value)
        , m_count(_count)
    {

    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, TextureCube const* _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTextureCube)
        , m_texture(_value ? const_cast<TextureCube*>(_value)->cast<TextureCube>() : nullptr)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, TextureCubePtr const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTextureCube)
        , m_texture(_value)
    {

    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2F const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2F32)
        , m_vectorF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3F const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3F32)
        , m_vectorF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4F const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4F32)
        , m_vectorF(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2S const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2S32)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3S const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3S32)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4S const& _value)
        : m_type(ShaderUniformType::UniformVec4S32)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2U const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2U32)
        , m_vectorU(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3U const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3U32)
        , m_vectorU(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4U const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4U32)
        , m_vectorU(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2B const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2B)
        , m_vectorB(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3B const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3B)
        , m_vectorB(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4B const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4B)
        , m_vectorB(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Mat3F const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformMat3F32)
        , m_matrix3DF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Mat4F const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformMat4F32)
        , m_matrix4DF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, TMat const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTMat)
        , m_matrixT(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, ColorF128 const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformColorF128)
        , m_vectorF(_value.toVec4F32())
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(ShaderUniformVariant const& _variant)
        : m_renderSystem(_variant.m_renderSystem)
    {
        this->operator=(_variant);
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, ShaderUniformVariant const& _variant)
        : m_renderSystem(_renderSystem)
    {
        this->operator=(_variant);
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, ShaderUniformVariant&& _variant)
        : m_renderSystem(_renderSystem)
    {
        this->operator=(_variant);
    }

    //////////////////////////////////////////
    ShaderUniformVariant::~ShaderUniformVariant()
    {

    }
    
    //////////////////////////////////////////
    void ShaderUniformVariant::setTexture2D(AssetFilePtr const& _assetFile)
    {
        Texture2DPtr const& texture2D = m_renderSystem->getTextureManager()->getOrLoadTexture2D(_assetFile);
        if (texture2D)
            set(texture2D);
        else
            set(m_renderSystem->getTextureManager()->getErrorTexture());
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTexture2D(String const& _textureName)
    {
        if (!_textureName.empty())
        {
            Texture2DPtr const& texture = m_renderSystem->getTextureManager()->getOrLoadTexture2D(_textureName);
            if (texture)
                set(texture);
            else
                set(m_renderSystem->getTextureManager()->getErrorTexture());
        }
        else
        {
            set(Texture2DPtr());
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTextureCube(AssetFilePtr const& _assetFile)
    {
        TextureCubePtr const& textureCube = m_renderSystem->getTextureManager()->getOrLoadTextureCube(_assetFile);
        set(textureCube);
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTextureCube(String const& _textureName)
    {
        if (!_textureName.empty())
        {
            TextureCubePtr const& texture = m_renderSystem->getTextureManager()->getOrLoadTextureCube(_textureName);
            set(texture);
        }
        else
        {
            set(TextureCubePtr());
        }
    }

    //////////////////////////////////////////
    U32 ShaderUniformVariant::calculateCRC32(U32 _seed) const
    {
        switch (m_type)
        {
            case ShaderUniformType::UniformS32:
            {
                return Hash::CalculateCRC32((const S8*)&m_S32, sizeof(S32), _seed);
                break;
            }
            case ShaderUniformType::UniformF32:
            {
                return Hash::CalculateCRC32((const S8*)&m_F32, sizeof(F32), _seed);
                break;
            }
            case ShaderUniformType::UniformF64:
            {
                return Hash::CalculateCRC32((const S8*)&m_F64, sizeof(F64), _seed);
                break;
            }
            case ShaderUniformType::UniformBool:
            {
                return Hash::CalculateCRC32((const S8*)&m_bool, sizeof(bool), _seed);
                break;
            }
            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            {
                Texture const* textureRaw = m_texture.get();
                U32 hash = Hash::CalculateCRC32((const S8*)&textureRaw, sizeof(Texture*), _seed);
                return hash;
                break;
            }
            case ShaderUniformType::UniformVec2S32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec2S), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3S32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec3S), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4S32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec4S), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2U32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec2U), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3U32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec3U), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4U32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec4U), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2B:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec2B), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3B:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec3B), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4B:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec4B), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2F32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec2F), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3F32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec3F), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4F32:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec4F), _seed);
                break;
            }
            case ShaderUniformType::UniformMat3F32:
            {
                return Hash::CalculateCRC32((const S8*)&m_matrix3DF, sizeof(Mat3F), _seed);
                break;
            }
            case ShaderUniformType::UniformMat4F32:
            {
                return Hash::CalculateCRC32((const S8*)&m_matrix4DF, sizeof(Mat4F), _seed);
                break;
            }
            case ShaderUniformType::UniformTMat:
            {
                return Hash::CalculateCRC32((const S8*)&m_matrixT, sizeof(TMat), _seed);
                break;
            }
            case ShaderUniformType::UniformColorF128:
            {
                return Hash::CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec4F), _seed);
                break;
            }
            default:
            {
                MAZE_ERROR("Unsupported ShaderUniformType!");
            }
        }

        return 0;
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setString(ShaderUniformType _type, CString _data, Size _count)
    {
        switch (_type)
        {
            case ShaderUniformType::UniformS32:
            {
                S32 value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformF32:
            {
                F32 value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformF64:
            {
                F64 value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformBool:
            {
                bool value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformTexture2D:
            {
                String value;
                ValueFromString(value, _data, _count);
                setTexture2D(value);
                return;
            }
            case ShaderUniformType::UniformTextureCube:
            {
                String value;
                ValueFromString(value, _data, _count);
                setTextureCube(value);
                return;
            }
            case ShaderUniformType::UniformVec2S32:
            {
                Vec2S value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3S32:
            {
                Vec3S value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4S32:
            {
                Vec4S value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2U32:
            {
                Vec2U value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3U32:
            {
                Vec3U value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4U32:
            {
                Vec4U value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2B:
            {
                Vec2B value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3B:
            {
                Vec3B value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4B:
            {
                Vec4B value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2F32:
            {
                Vec2F value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3F32:
            {
                Vec3F value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4F32:
            {
                Vec4F value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformMat3F32:
            {
                Mat3F value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformMat4F32:
            {
                Mat4F value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformTMat:
            {
                TMat value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformColorF128:
            {
                Vec4F value;
                ValueFromString(value, _data, _count);
                setColor(value);
                return;
            }
            default:
            {
                MAZE_ERROR("Unsupported ShaderUniformType!");
            }
        }
    }

    //////////////////////////////////////////
    String ShaderUniformVariant::toStringValue() const
    {
        String data;

        switch (m_type)
        {
            case ShaderUniformType::UniformS32:
            {
                ValueToString(m_S32, data);
                return data;
            }
            case ShaderUniformType::UniformF32:
            {
                ValueToString(m_F32, data);
                return data;
            }
            case ShaderUniformType::UniformF64:
            {
                ValueToString(m_F64, data);
                return data;
            }
            case ShaderUniformType::UniformBool:
            {
                ValueToString(m_bool, data);
                return data;
            }
            case ShaderUniformType::UniformTexture2D:
            {
                if (m_texture)
                    data = m_texture->castRaw<Texture2D>()->toString();
                return data;
            }
            case ShaderUniformType::UniformTextureCube:
            {
                if (m_texture)
                    data = m_texture->castRaw<TextureCube>()->getAssetFileName();
                return data;
            }
            case ShaderUniformType::UniformVec2S32:
            {
                ValueToString(getVec2S32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3S32:
            {
                ValueToString(getVec3S32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4S32:
            {
                ValueToString(getVec4S32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2U32:
            {
                ValueToString(getVec2U32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3U32:
            {
                ValueToString(getVec3U32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4U32:
            {
                ValueToString(getVec4U32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2B:
            {
                ValueToString(getVec2B(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3B:
            {
                ValueToString(getVec3B(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4B:
            {
                ValueToString(getVec4B(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2F32:
            {
                ValueToString(getVec2F32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3F32:
            {
                ValueToString(getVec3F32(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4F32:
            {
                ValueToString(getVec4F32(), data);
                break;
            }
            case ShaderUniformType::UniformMat3F32:
            {
                ValueToString(getMat3F32(), data);
                return data;
            }
            case ShaderUniformType::UniformMat4F32:
            {
                ValueToString(getMat4F32(), data);
                return data;
            }
            case ShaderUniformType::UniformTMat:
            {
                ValueToString(getTMat(), data);
                return data;
            }
            case ShaderUniformType::UniformColorF128:
            {
                ValueToString(getVec4F32(), data);
                break;
            }
            default:
            {
                MAZE_ERROR("Unsupported ShaderUniformType!");
            }
        }

        return data;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariant::operator==(ShaderUniformVariant const& _variant) const
    {
        if (m_type != _variant.m_type)
            return false;

        switch (m_type)
        {
            case ShaderUniformType::UniformS32:
            {
                return m_S32 == _variant.m_S32;
                break;
            }
            case ShaderUniformType::UniformF32:
            {
                return m_F32 == _variant.m_F32;
                break;
            }
            case ShaderUniformType::UniformF64:
            {
                return m_F64 == _variant.m_F64;
                break;
            }
            case ShaderUniformType::UniformBool:
            {
                return m_bool == _variant.m_bool;
                break;
            }
            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            {
                return m_texture == _variant.m_texture;
                break;
            }
            case ShaderUniformType::UniformTexture2DArray:
            case ShaderUniformType::UniformVec2F32Array:
            case ShaderUniformType::UniformVec3F32Array:
            case ShaderUniformType::UniformVec4F32Array:
            {
                return m_ptr == _variant.getPtr();
                break;
            }
            case ShaderUniformType::UniformVec2S32:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y);
                break;
            }
            case ShaderUniformType::UniformVec3S32:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y)
                        &&    (m_vectorS.z == _variant.m_vectorS.z);
                break;
            }
            case ShaderUniformType::UniformVec4S32:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y)
                        &&    (m_vectorS.z == _variant.m_vectorS.z)
                        &&    (m_vectorS.w == _variant.m_vectorS.w);
                break;
            }
            case ShaderUniformType::UniformVec2U32:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y);
                break;
            }
            case ShaderUniformType::UniformVec3U32:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y)
                        &&    (m_vectorU.z == _variant.m_vectorU.z);
                break;
            }
            case ShaderUniformType::UniformVec4U32:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y)
                        &&    (m_vectorU.z == _variant.m_vectorU.z)
                        &&    (m_vectorU.w == _variant.m_vectorU.w);
                break;
            }
            case ShaderUniformType::UniformVec2F32:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y);
                break;
            }
            case ShaderUniformType::UniformVec3F32:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y)
                        &&    (m_vectorF.z == _variant.m_vectorF.z);
                break;
            }
            case ShaderUniformType::UniformVec4F32:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y)
                        &&    (m_vectorF.z == _variant.m_vectorF.z)
                        &&    (m_vectorF.w == _variant.m_vectorF.w);
                break;
            }
            case ShaderUniformType::UniformVec2B:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y);
                break;
            }
            case ShaderUniformType::UniformVec3B:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y)
                    &&        (m_vectorB.z == _variant.m_vectorB.z);
                break;
            }
            case ShaderUniformType::UniformVec4B:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y)
                    &&        (m_vectorB.z == _variant.m_vectorB.z)
                    &&        (m_vectorB.w == _variant.m_vectorB.w);
                break;
            }
            case ShaderUniformType::UniformMat3F32:
            {
                return m_matrix3DF == _variant.m_matrix3DF;
                break;
            }
            case ShaderUniformType::UniformMat4F32:
            {
                return m_matrix4DF == _variant.m_matrix4DF;
                break;
            }
            case ShaderUniformType::UniformTMat:
            {
                return m_matrixT == _variant.m_matrixT;
                break;
            }
            case ShaderUniformType::UniformColorF128:
            {
                return    (m_vectorF.x == _variant.m_vectorF.x)
                    &&    (m_vectorF.y == _variant.m_vectorF.y)
                    &&    (m_vectorF.z == _variant.m_vectorF.z)
                    &&    (m_vectorF.w == _variant.m_vectorF.w);
                break;
            }
            default:
            {
                MAZE_ERROR("Unsupported ShaderUniformType!");
            }
        }

        return false;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariant::operator!=(ShaderUniformVariant const& _variant) const
    {
        return !operator==(_variant);
    }

    //////////////////////////////////////////
    ShaderUniformVariant& ShaderUniformVariant::operator=(ShaderUniformVariant const& _variant)
    {
        setName(_variant.getName());

        switch (_variant.getType())
        {
            case ShaderUniformType::None:                   m_type = ShaderUniformType::None; break;
            case ShaderUniformType::UniformS32:             set(_variant.getS32()); break;
            case ShaderUniformType::UniformF32:             set(_variant.getF32()); break;
            case ShaderUniformType::UniformF64:             set(_variant.getF64()); break;
            case ShaderUniformType::UniformBool:            set(_variant.getBool()); break;
            case ShaderUniformType::UniformTexture2D:       set(Maze::static_pointer_cast<Texture2D>(_variant.getTexture())); break;
            case ShaderUniformType::UniformTextureCube:     set(Maze::static_pointer_cast<TextureCube>(_variant.getTexture())); break;
            case ShaderUniformType::UniformTexture2DArray:  set((Texture2D const**)_variant.getPtr(), _variant.getCount()); break;
            case ShaderUniformType::UniformVec2S32:         set(_variant.getVec2S32()); break;
            case ShaderUniformType::UniformVec3S32:         set(_variant.getVec3S32()); break;
            case ShaderUniformType::UniformVec4S32:         set(_variant.getVec4S32()); break;
            case ShaderUniformType::UniformVec2U32:         set(_variant.getVec2U32()); break;
            case ShaderUniformType::UniformVec3U32:         set(_variant.getVec3U32()); break;
            case ShaderUniformType::UniformVec4U32:         set(_variant.getVec4U32()); break;
            case ShaderUniformType::UniformVec2F32:         set(_variant.getVec2F32()); break;
            case ShaderUniformType::UniformVec3F32:         set(_variant.getVec3F32()); break;
            case ShaderUniformType::UniformVec4F32:         set(_variant.getVec4F32()); break;
            case ShaderUniformType::UniformVec2F32Array:    set((Vec2F const*)_variant.getPtr(), _variant.getCount()); break;
            case ShaderUniformType::UniformVec3F32Array:    set((Vec3F const*)_variant.getPtr(), _variant.getCount()); break;
            case ShaderUniformType::UniformVec4F32Array:    set((Vec4F const*)_variant.getPtr(), _variant.getCount()); break;
            case ShaderUniformType::UniformVec2B:           set(_variant.getVec2B()); break;
            case ShaderUniformType::UniformVec3B:           set(_variant.getVec3B()); break;
            case ShaderUniformType::UniformVec4B:           set(_variant.getVec4B()); break;
            case ShaderUniformType::UniformMat3F32:         set(_variant.getMat3F32()); break;
            case ShaderUniformType::UniformMat4F32:         set(_variant.getMat4F32()); break;
            case ShaderUniformType::UniformTMat:         set(_variant.getTMat()); break;
            case ShaderUniformType::UniformColorF128:      set(_variant.getColorF128()); break;
            default:
            {
                MAZE_ERROR("%s: Unsupported ShaderUniformType - %s(%d), value=%s",
                    getName().c_str(), _variant.getType().toCString(), (S32)_variant.getType(), _variant.toStringValue().c_str());
            }
        }

        return *this;
    }
        
    //////////////////////////////////////////
    ShaderUniformVariant& ShaderUniformVariant::operator=(ShaderUniformVariant&& _variant) noexcept
    {
        return operator=(static_cast<ShaderUniformVariant const&>(_variant));
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        CString name = _element->Attribute("name");
        CString type = _element->Attribute("type");
        CString value = _element->Attribute("value");

        ShaderUniformType shaderUniformType = ShaderUniformType::FromString(type);

        setString(shaderUniformType, value, strlen(value));
        setName(HashedString(name ? name : ""));
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* ShaderUniformVariant::toXMLElement(tinyxml2::XMLDocument& _doc) const
    {
        CString className = ClassInfo<ShaderUniformVariant>::Name();

        tinyxml2::XMLElement* element = _doc.NewElement(className);

        String value = toStringValue();

        element->SetAttribute("name", m_name.c_str());
        element->SetAttribute("type", m_type.toCString());
        element->SetAttribute("value", value.c_str());

        return element;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariant::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        CString name = _dataBlock.getCString(MAZE_HCS("name"));
        setName(HashedString(name ? name : ""));

        DataBlock::ParamIndex valueParamIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
        MAZE_ERROR_RETURN_VALUE_IF(valueParamIndex < 0, false, "Value param missed!");

        DataBlockParamType valueParamType = _dataBlock.getParamType(valueParamIndex);
        switch (valueParamType)
        {
            case DataBlockParamType::ParamS32:          set(_dataBlock.getS32(valueParamIndex)); break;
            case DataBlockParamType::ParamF32:          set(_dataBlock.getF32(valueParamIndex)); break;
            case DataBlockParamType::ParamF64:          set(_dataBlock.getF64(valueParamIndex)); break;
            case DataBlockParamType::ParamBool:         set(_dataBlock.getBool(valueParamIndex)); break;
            case DataBlockParamType::ParamVec4S8:       set((Vec4S32)_dataBlock.getVec4S8(valueParamIndex)); break;
            case DataBlockParamType::ParamVec4U8:       set((Vec4U32)_dataBlock.getVec4U8(valueParamIndex)); break;
            case DataBlockParamType::ParamVec2S32:      set(_dataBlock.getVec2S32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec3S32:      set(_dataBlock.getVec3S32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec4S32:      set(_dataBlock.getVec4S32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec2U32:      set(_dataBlock.getVec2U32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec3U32:      set(_dataBlock.getVec3U32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec4U32:      set(_dataBlock.getVec4U32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec2F32:      set(_dataBlock.getVec2F32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec3F32:      set(_dataBlock.getVec3F32(valueParamIndex)); break;
            case DataBlockParamType::ParamVec2B:        set(_dataBlock.getVec2B(valueParamIndex)); break;
            case DataBlockParamType::ParamVec3B:        set(_dataBlock.getVec3B(valueParamIndex)); break;
            case DataBlockParamType::ParamVec4B:        set(_dataBlock.getVec4B(valueParamIndex)); break;
            case DataBlockParamType::ParamMat3F32:      set(_dataBlock.getMat3F32(valueParamIndex)); break;
            case DataBlockParamType::ParamMat4F32:      set(_dataBlock.getMat4F32(valueParamIndex)); break;
            case DataBlockParamType::ParamTMat:         set(_dataBlock.getTMat(valueParamIndex)); break;
            default:
            {
                CString type = _dataBlock.getCString(MAZE_HCS("type"));
                ShaderUniformType shaderUniformType = type ? ShaderUniformType::FromString(type) : ShaderUniformType::None;

                switch (shaderUniformType)
                {
                    case ShaderUniformType::UniformTexture2D:
                    {
                        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
                        if (paramIndex >= 0)
                        {
                            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
                            switch (paramType)
                            {
                                // by AUID
                                case DataBlockParamType::ParamU32:
                                {
                                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTexture2D>::UID())
                                    {
                                        set(assetUnit->castRaw<AssetUnitTexture2D>()->loadTexture(true));
                                        return true;
                                    }

                                    break;
                                }
                                // by name
                                case DataBlockParamType::ParamString:
                                {
                                    setTexture2D(_dataBlock.getString(paramIndex));
                                    return true;
                                }
                                default:
                                {
                                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                                    break;
                                }
                            }
                        }
                        set(Texture2DPtr());
                        break;
                    }
                    case ShaderUniformType::UniformTextureCube:
                    {
                        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
                        if (paramIndex >= 0)
                        {
                            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
                            switch (paramType)
                            {
                                // by AUID
                                case DataBlockParamType::ParamU32:
                                {
                                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTextureCube>::UID())
                                    {
                                        set(assetUnit->castRaw<AssetUnitTextureCube>()->loadTexture(true));
                                        return true;
                                    }

                                    break;
                                }
                                // by name
                                case DataBlockParamType::ParamString:
                                {
                                    setTextureCube(_dataBlock.getString(paramIndex));
                                    return true;
                                }
                                default:
                                {
                                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                                    break;
                                }
                            }
                        }
                        set(TextureCubePtr());
                        break;
                    }
                    case ShaderUniformType::UniformVec4F32:             set(_dataBlock.getVec4F32(MAZE_HCS("value"))); break;
                    case ShaderUniformType::UniformColorF128:           setColor(_dataBlock.getVec4F32(MAZE_HCS("value"))); break;
                    default:
                        Debug::LogError("Unsupported ShaderUniformType - %s", shaderUniformType.toCString());
                        return false;
                }

                break;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::toDataBlock(DataBlock& _dataBlock) const
    {
        _dataBlock.setString(MAZE_HCS("name"), m_name);
        _dataBlock.setCString(MAZE_HCS("type"), m_type.toCString());

        switch (m_type)
        {
            case ShaderUniformType::UniformS32:                 _dataBlock.setS32(MAZE_HCS("value"), getS32()); break;
            case ShaderUniformType::UniformF32:                 _dataBlock.setF32(MAZE_HCS("value"), getF32()); break;
            case ShaderUniformType::UniformF64:                 _dataBlock.setF64(MAZE_HCS("value"), getF64()); break;
            case ShaderUniformType::UniformBool:                _dataBlock.setBool(MAZE_HCS("value"), getBool()); break;
            case ShaderUniformType::UniformTexture2D:
            {
                // Save as AUID
                if (AssetUnitManager::GetInstancePtr())
                {
                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_texture->getName());
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTexture2D>::UID())
                    {
                        Texture2DPtr const& assetUnitTexture = assetUnit->castRaw<AssetUnitTexture2D>()->getTexture();
                        if (assetUnitTexture == m_texture)
                        {
                            _dataBlock.setU32(MAZE_HCS("value"), assetUnit->getAssetUnitId());
                            break;
                        }
                    }
                }

                _dataBlock.setString(MAZE_HCS("value"), toStringValue());
                break;
            }
            case ShaderUniformType::UniformTextureCube:
            {
                // Save as AUID
                if (AssetUnitManager::GetInstancePtr())
                {
                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_texture->getName());
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTextureCube>::UID())
                    {
                        TextureCubePtr const& assetUnitTexture = assetUnit->castRaw<AssetUnitTextureCube>()->getTexture();
                        if (assetUnitTexture == m_texture)
                        {
                            _dataBlock.setU32(MAZE_HCS("value"), assetUnit->getAssetUnitId());
                            break;
                        }
                    }
                }

                _dataBlock.setString(MAZE_HCS("value"), toStringValue());
                break;
            }
            case ShaderUniformType::UniformVec2S32:             _dataBlock.setVec2S32(MAZE_HCS("value"), getVec2S32()); break;
            case ShaderUniformType::UniformVec3S32:             _dataBlock.setVec3S32(MAZE_HCS("value"), getVec3S32()); break;
            case ShaderUniformType::UniformVec4S32:             _dataBlock.setVec4S32(MAZE_HCS("value"), getVec4S32()); break;
            case ShaderUniformType::UniformVec2U32:             _dataBlock.setVec2U32(MAZE_HCS("value"), getVec2U32()); break;
            case ShaderUniformType::UniformVec3U32:             _dataBlock.setVec3U32(MAZE_HCS("value"), getVec3U32()); break;
            case ShaderUniformType::UniformVec4U32:             _dataBlock.setVec4U32(MAZE_HCS("value"), getVec4U32()); break;
            case ShaderUniformType::UniformVec2F32:             _dataBlock.setVec2F32(MAZE_HCS("value"), getVec2F32()); break;
            case ShaderUniformType::UniformVec3F32:             _dataBlock.setVec3F32(MAZE_HCS("value"), getVec3F32()); break;
            case ShaderUniformType::UniformVec4F32:             _dataBlock.setVec4F32(MAZE_HCS("value"), getVec4F32()); break;
            case ShaderUniformType::UniformVec2B:               _dataBlock.setVec2B(MAZE_HCS("value"), getVec2B()); break;
            case ShaderUniformType::UniformVec3B:               _dataBlock.setVec3B(MAZE_HCS("value"), getVec3B()); break;
            case ShaderUniformType::UniformVec4B:               _dataBlock.setVec4B(MAZE_HCS("value"), getVec4B()); break;
            case ShaderUniformType::UniformMat3F32:             _dataBlock.setMat3F32(MAZE_HCS("value"), getMat3F32()); break;
            case ShaderUniformType::UniformMat4F32:             _dataBlock.setMat4F32(MAZE_HCS("value"), getMat4F32()); break;
            case ShaderUniformType::UniformTMat:                _dataBlock.setTMat(MAZE_HCS("value"), getTMat()); break;
            case ShaderUniformType::UniformColorF128:           _dataBlock.setVec4F32(MAZE_HCS("value"), getVec4F32()); break;
            default:
                Debug::LogError("Unsupported ShaderUniformType - %s", m_type.toCString());
        }
    }

    //////////////////////////////////////////
    std::ostream& operator<<(
        std::ostream& _o,
        ShaderUniformVariant const& _variant)
    {
        _o << _variant.getName().c_str() << "(" << _variant.getType().toCString() << ")=" << _variant.toStringValue();

        switch (_variant.getType())
        {
            case ShaderUniformType::UniformTexture2D:
            {
                if (_variant.getTexture())
                    _o << " " << (_variant.getTexture()->castRaw<Texture2D>()->isValid() ? "[Valid]" : "[Invalid]");
                else
                    _o << " " << "[None]";
                break;
            }
            default:
            {
                break;
            }
        }

        return _o;
    }

} // namespace Maze
//////////////////////////////////////////
