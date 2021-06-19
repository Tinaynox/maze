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
        String const& _name)
        : m_renderSystem(_renderSystem)
        , m_type(_type)
        , m_name(_name)
    {
        generateNameHash();
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
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Texture2D* _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTexture2D)
        , m_texture(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Texture2DPtr const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTexture2D)
        , m_texture(_value.get())
    {
    
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, TextureCube* _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTextureCube)
        , m_texture(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, TextureCubePtr const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformTextureCube)
        , m_texture(_value.get())
    {

    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2DF const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2DF)
        , m_vectorF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3DF const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3DF)
        , m_vectorF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4DF const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4DF)
        , m_vectorF(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2DS const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2DS)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3DS const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3DS)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4DS const& _value)
        : m_type(ShaderUniformType::UniformVec4DS)
        , m_vectorS(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2DU const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2DU)
        , m_vectorU(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3DU const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3DU)
        , m_vectorU(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4DU const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4DU)
        , m_vectorU(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec2DB const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec2DB)
        , m_vectorB(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec3DB const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec3DB)
        , m_vectorB(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Vec4DB const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformVec4DB)
        , m_vectorB(_value)
    {
    }


    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Mat3DF const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformMat3DF)
        , m_matrix3DF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, Mat4DF const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformMat4DF)
        , m_matrix4DF(_value)
    {
    }

    //////////////////////////////////////////
    ShaderUniformVariant::ShaderUniformVariant(RenderSystem* _renderSystem, ColorF128 const& _value)
        : m_renderSystem(_renderSystem)
        , m_type(ShaderUniformType::UniformColorF128)
        , m_vectorF(_value.toVec4DF())
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
        Texture2DPtr const& texture2D = m_renderSystem->getTextureManager()->getTexture2D(_assetFile);
        set(texture2D);
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTexture2D(String const& _textureName)
    {
        if (!_textureName.empty())
        {
            Texture2DPtr const& texture = m_renderSystem->getTextureManager()->getTexture2D(_textureName);
            set(texture);
        }
        else
        {
            set(Texture2DPtr());
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTextureCube(AssetFilePtr const& _assetFile)
    {
        TextureCubePtr const& textureCube = m_renderSystem->getTextureManager()->getTextureCube(_assetFile);
        set(textureCube);
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::setTextureCube(String const& _textureName)
    {
        if (!_textureName.empty())
        {
            TextureCubePtr const& texture = m_renderSystem->getTextureManager()->getTextureCube(_textureName);
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
                return CalculateCRC32((const S8*)&m_S32, sizeof(S32), _seed);
                break;
            }
            case ShaderUniformType::UniformF32:
            {
                return CalculateCRC32((const S8*)&m_F32, sizeof(F32), _seed);
                break;
            }
            case ShaderUniformType::UniformF64:
            {
                return CalculateCRC32((const S8*)&m_F64, sizeof(F64), _seed);
                break;
            }
            case ShaderUniformType::UniformBool:
            {
                return CalculateCRC32((const S8*)&m_bool, sizeof(bool), _seed);
                break;
            }
            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            {
                Texture const* textureRaw = m_texture.get();
                U32 hash = CalculateCRC32((const S8*)&textureRaw, sizeof(Texture*), _seed);
                return hash;
                break;
            }
            case ShaderUniformType::UniformVec2DS:
            {
                return CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec2DS), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3DS:
            {
                return CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec3DS), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4DS:
            {
                return CalculateCRC32((const S8*)&m_vectorS, sizeof(Vec4DS), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2DU:
            {
                return CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec2DU), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3DU:
            {
                return CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec3DU), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4DU:
            {
                return CalculateCRC32((const S8*)&m_vectorU, sizeof(Vec4DU), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2DB:
            {
                return CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec2DB), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3DB:
            {
                return CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec3DB), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4DB:
            {
                return CalculateCRC32((const S8*)&m_vectorB, sizeof(Vec4DB), _seed);
                break;
            }
            case ShaderUniformType::UniformVec2DF:
            {
                return CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec2DF), _seed);
                break;
            }
            case ShaderUniformType::UniformVec3DF:
            {
                return CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec3DF), _seed);
                break;
            }
            case ShaderUniformType::UniformVec4DF:
            {
                return CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec4DF), _seed);
                break;
            }
            case ShaderUniformType::UniformMat3DF:
            {
                return CalculateCRC32((const S8*)&m_matrix3DF, sizeof(Mat3DF), _seed);
                break;
            }
            case ShaderUniformType::UniformMat4DF:
            {
                return CalculateCRC32((const S8*)&m_matrix4DF, sizeof(Mat4DF), _seed);
                break;
            }
            case ShaderUniformType::UniformColorF128:
            {
                return CalculateCRC32((const S8*)&m_vectorF, sizeof(Vec4DF), _seed);
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
            case ShaderUniformType::UniformVec2DS:
            {
                Vec2DS value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3DS:
            {
                Vec3DS value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4DS:
            {
                Vec4DS value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2DU:
            {
                Vec2DU value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3DU:
            {
                Vec3DU value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4DU:
            {
                Vec4DU value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2DB:
            {
                Vec2DB value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3DB:
            {
                Vec3DB value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4DB:
            {
                Vec4DB value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec2DF:
            {
                Vec2DF value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec3DF:
            {
                Vec3DF value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformVec4DF:
            {
                Vec4DF value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformMat3DF:
            {
                Mat3DF value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformMat4DF:
            {
                Mat4DF value;
                ValueFromString(value, _data, _count);
                set(value);
                return;
            }
            case ShaderUniformType::UniformColorF128:
            {
                Vec4DF value;
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
                    data = m_texture->castRaw<Texture2D>()->getAssetFileName();
                return data;
            }
            case ShaderUniformType::UniformTextureCube:
            {
                if (m_texture)
                    data = m_texture->castRaw<TextureCube>()->getAssetFileName();
                return data;
            }
            case ShaderUniformType::UniformVec2DS:
            {
                ValueToString(getVec2DS(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3DS:
            {
                ValueToString(getVec3DS(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4DS:
            {
                ValueToString(getVec4DS(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2DU:
            {
                ValueToString(getVec2DU(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3DU:
            {
                ValueToString(getVec3DU(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4DU:
            {
                ValueToString(getVec4DU(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2DB:
            {
                ValueToString(getVec2DB(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3DB:
            {
                ValueToString(getVec3DB(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4DB:
            {
                ValueToString(getVec4DB(), data);
                return data;
            }
            case ShaderUniformType::UniformVec2DF:
            {
                ValueToString(getVec2DF(), data);
                return data;
            }
            case ShaderUniformType::UniformVec3DF:
            {
                ValueToString(getVec3DF(), data);
                return data;
            }
            case ShaderUniformType::UniformVec4DF:
            {
                ValueToString(getVec4DF(), data);
                break;
            }
            case ShaderUniformType::UniformMat3DF:
            {
                ValueToString(getMat3DF(), data);
                return data;
            }
            case ShaderUniformType::UniformMat4DF:
            {
                ValueToString(getMat4DF(), data);
                return data;
            }
            case ShaderUniformType::UniformColorF128:
            {
                ValueToString(getVec4DF(), data);
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
            case ShaderUniformType::UniformVec2DS:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y);
                break;
            }
            case ShaderUniformType::UniformVec3DS:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y)
                        &&    (m_vectorS.z == _variant.m_vectorS.z);
                break;
            }
            case ShaderUniformType::UniformVec4DS:
            {
                return        (m_vectorS.x == _variant.m_vectorS.x)
                        &&    (m_vectorS.y == _variant.m_vectorS.y)
                        &&    (m_vectorS.z == _variant.m_vectorS.z)
                        &&    (m_vectorS.w == _variant.m_vectorS.w);
                break;
            }
            case ShaderUniformType::UniformVec2DU:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y);
                break;
            }
            case ShaderUniformType::UniformVec3DU:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y)
                        &&    (m_vectorU.z == _variant.m_vectorU.z);
                break;
            }
            case ShaderUniformType::UniformVec4DU:
            {
                return        (m_vectorU.x == _variant.m_vectorU.x)
                        &&    (m_vectorU.y == _variant.m_vectorU.y)
                        &&    (m_vectorU.z == _variant.m_vectorU.z)
                        &&    (m_vectorU.w == _variant.m_vectorU.w);
                break;
            }
            case ShaderUniformType::UniformVec2DF:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y);
                break;
            }
            case ShaderUniformType::UniformVec3DF:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y)
                        &&    (m_vectorF.z == _variant.m_vectorF.z);
                break;
            }
            case ShaderUniformType::UniformVec4DF:
            {
                return        (m_vectorF.x == _variant.m_vectorF.x)
                        &&    (m_vectorF.y == _variant.m_vectorF.y)
                        &&    (m_vectorF.z == _variant.m_vectorF.z)
                        &&    (m_vectorF.w == _variant.m_vectorF.w);
                break;
            }
            case ShaderUniformType::UniformVec2DB:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y);
                break;
            }
            case ShaderUniformType::UniformVec3DB:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y)
                    &&        (m_vectorB.z == _variant.m_vectorB.z);
                break;
            }
            case ShaderUniformType::UniformVec4DB:
            {
                return        (m_vectorB.x == _variant.m_vectorB.x)
                    &&        (m_vectorB.y == _variant.m_vectorB.y)
                    &&        (m_vectorB.z == _variant.m_vectorB.z)
                    &&        (m_vectorB.w == _variant.m_vectorB.w);
                break;
            }
            case ShaderUniformType::UniformMat3DF:
            {
                return m_matrix3DF == _variant.m_matrix3DF;
                break;
            }
            case ShaderUniformType::UniformMat4DF:
            {
                return m_matrix4DF == _variant.m_matrix4DF;
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
            case ShaderUniformType::None:                 m_type = ShaderUniformType::None; break;
            case ShaderUniformType::UniformS32:           set(_variant.getS32()); break;
            case ShaderUniformType::UniformF32:           set(_variant.getF32()); break;
            case ShaderUniformType::UniformF64:           set(_variant.getF64()); break;
            case ShaderUniformType::UniformBool:          set(_variant.getBool()); break;
            case ShaderUniformType::UniformTexture2D:     set(std::static_pointer_cast<Texture2D>(_variant.getTexture())); break;
            case ShaderUniformType::UniformTextureCube:   set(std::static_pointer_cast<TextureCube>(_variant.getTexture())); break;
            case ShaderUniformType::UniformVec2DS:        set(_variant.getVec2DS()); break;
            case ShaderUniformType::UniformVec3DS:        set(_variant.getVec3DS()); break;
            case ShaderUniformType::UniformVec4DS:        set(_variant.getVec4DS()); break;
            case ShaderUniformType::UniformVec2DU:        set(_variant.getVec2DU()); break;
            case ShaderUniformType::UniformVec3DU:        set(_variant.getVec3DU()); break;
            case ShaderUniformType::UniformVec4DU:        set(_variant.getVec4DU()); break;
            case ShaderUniformType::UniformVec2DF:        set(_variant.getVec2DF()); break;
            case ShaderUniformType::UniformVec3DF:        set(_variant.getVec3DF()); break;
            case ShaderUniformType::UniformVec4DF:        set(_variant.getVec4DF()); break;
            case ShaderUniformType::UniformVec2DB:        set(_variant.getVec2DB()); break;
            case ShaderUniformType::UniformVec3DB:        set(_variant.getVec3DB()); break;
            case ShaderUniformType::UniformVec4DB:        set(_variant.getVec4DB()); break;
            case ShaderUniformType::UniformMat3DF:        set(_variant.getMat3DF()); break;
            case ShaderUniformType::UniformMat4DF:        set(_variant.getMat4DF()); break;
            case ShaderUniformType::UniformColorF128:     set(_variant.getColorF128()); break;
            default:
            {
                MAZE_ERROR("Unsupported uniform variant - %d", (S32)_variant.getType());    
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
    void ShaderUniformVariant::generateNameHash()
    {
        if (!m_renderSystem)
            return;

        m_nameHash = m_renderSystem->getShaderSystem()->stringToHash(m_name);
    }

    //////////////////////////////////////////
    void ShaderUniformVariant::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        CString type = _element->Attribute("type");
        CString value = _element->Attribute("value");
        CString name = _element->Attribute("name");

        ShaderUniformType shaderUniformType = ShaderUniformType::FromString(type);

        setString(shaderUniformType, value, strlen(value));
        setName(name ? name : "");
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* ShaderUniformVariant::toXMLElement(tinyxml2::XMLDocument& _doc) const
    {
        CString className = ClassInfo<ShaderUniformVariant>::Name();

        tinyxml2::XMLElement* element = _doc.NewElement(className);

        String value = toStringValue();

        element->SetAttribute("type", m_type.toCString());
        element->SetAttribute("value", value.c_str());

        return element;
    }

    //////////////////////////////////////////
    std::ostream& operator<<(
        std::ostream& _o,
        ShaderUniformVariant const& _variant)
    {
        _o << _variant.getName() << "(" << _variant.getType().toCString() << ")=" << _variant.toStringValue();

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
