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
#if (!defined(_MazeShaderUniformVariant_hpp_))
#define _MazeShaderUniformVariant_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeVec2B.hpp"
#include "maze-core/math/MazeVec3B.hpp"
#include "maze-core/math/MazeVec4B.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(ShaderUniform);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(ShaderUniformVariant);
    

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_22_API(MAZE_GRAPHICS_API, ShaderUniformType,
        UniformS32,
        UniformF32,
        UniformF64,
        UniformBool,
        UniformTexture2D,
        UniformTextureCube,
        UniformTexture2DArray,
        UniformVec2S32,
        UniformVec3S32,
        UniformVec4S32,
        UniformVec2U32,
        UniformVec3U32,
        UniformVec4U32,
        UniformVec2F32,
        UniformVec3F32,
        UniformVec4F32,
        UniformVec2B,
        UniformVec3B,
        UniformVec4B,
        UniformMat3F32,
        UniformMat4F32,
        UniformColorF128);


    //////////////////////////////////////////
    // Class ShaderUniformVariant
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ShaderUniformVariant
        : public IXMLElementSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        friend class ShaderUniform;

    public:

        //////////////////////////////////////////
        ShaderUniformVariant(
            RenderSystem* _renderSystem = nullptr,
            ShaderUniformType _type = ShaderUniformType::None,
            HashedCString _name = HashedCString());

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, S32 _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, F32 _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, F64 _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, bool _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Texture2D* _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Texture2DPtr const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Texture2D** _value, U32 _count);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, TextureCube* _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, TextureCubePtr const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec2F const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec3F const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec4F const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec2S const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec3S const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec4S const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec2U const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec3U const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec4U const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec2B const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec3B const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Vec4B const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Mat3F const& _value);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, Mat4F const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, ColorF128 const& _value);


        //////////////////////////////////////////
        ShaderUniformVariant(ShaderUniformVariant const& _variant);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, ShaderUniformVariant const& _variant);

        //////////////////////////////////////////
        ShaderUniformVariant(RenderSystem* _renderSystem, ShaderUniformVariant&& _variant);
        
        //////////////////////////////////////////
        virtual ~ShaderUniformVariant();


        //////////////////////////////////////////
        inline ShaderUniformType const& getType() const { return m_type; }


        //////////////////////////////////////////
        inline void* getPtr() const { return m_ptr; }

        //////////////////////////////////////////
        inline U32 getCount() const { return m_count; }


        //////////////////////////////////////////
        inline S32 getS32() const { return m_S32; }

        //////////////////////////////////////////
        inline F32 getF32() const { return m_F32; }

        //////////////////////////////////////////
        inline F64 getF64() const { return m_F64; }

        //////////////////////////////////////////
        inline bool getBool() const { return m_bool; }

        //////////////////////////////////////////
        inline TexturePtr const& getTexture() const { return m_texture; }

        //////////////////////////////////////////
        inline Texture2DPtr getTexture2D() const { return std::static_pointer_cast<Texture2D>(m_texture); }

        //////////////////////////////////////////
        inline TextureCubePtr getTextureCube() const { return std::static_pointer_cast<TextureCube>(m_texture); }

        //////////////////////////////////////////
        inline Vec4F const& getVecF() const { return m_vectorF; }

        //////////////////////////////////////////
        inline Vec2F getVec2F32() const { return { m_vectorF.x, m_vectorF.y }; }

        //////////////////////////////////////////
        inline Vec3F getVec3F32() const { return { m_vectorF.x, m_vectorF.y, m_vectorF.z }; }

        //////////////////////////////////////////
        inline Vec4F getVec4F32() const { return m_vectorF; }


        //////////////////////////////////////////
        inline Vec4S const& getVecS() const { return m_vectorS; }

        //////////////////////////////////////////
        inline Vec2S getVec2S32() const { return { m_vectorS.x, m_vectorS.y }; }

        //////////////////////////////////////////
        inline Vec3S getVec3S32() const { return { m_vectorS.x, m_vectorS.y, m_vectorS.z }; }

        //////////////////////////////////////////
        inline Vec4S getVec4S32() const { return m_vectorS; }


        //////////////////////////////////////////
        inline Vec4U const& getVecU() const { return m_vectorU; }

        //////////////////////////////////////////
        inline Vec2U getVec2U32() const { return { m_vectorU.x, m_vectorU.y }; }

        //////////////////////////////////////////
        inline Vec3U getVec3U32() const { return { m_vectorU.x, m_vectorU.y, m_vectorU.z }; }

        //////////////////////////////////////////
        inline Vec4U getVec4U32() const { return m_vectorU; }


        //////////////////////////////////////////
        inline Vec4B const& getVecB() const { return m_vectorB; }

        //////////////////////////////////////////
        inline Vec2B getVec2B() const { return { m_vectorB.x, m_vectorB.y }; }

        //////////////////////////////////////////
        inline Vec3B getVec3B() const { return { m_vectorB.x, m_vectorB.y, m_vectorB.z }; }

        //////////////////////////////////////////
        inline Vec4B getVec4B() const { return m_vectorB; }

        
        //////////////////////////////////////////
        inline Mat3F const& getMat3F32() const { return m_matrix3DF; }
        
        //////////////////////////////////////////
        inline Mat4F const& getMat4F32() const { return m_matrix4DF; }


        //////////////////////////////////////////
        inline ColorF128 getColorF128() const { return m_vectorF; }


        //////////////////////////////////////////
        inline void reset() { m_type = ShaderUniformType::None; }

        //////////////////////////////////////////
        inline void set(S32 _value) { m_S32 = _value; m_type = ShaderUniformType::UniformS32; }
        
        //////////////////////////////////////////
        inline void set(F32 _value) { m_F32 = _value; m_type = ShaderUniformType::UniformF32; }
        
        //////////////////////////////////////////
        inline void set(F64 _value) { m_F64 = _value; m_type = ShaderUniformType::UniformF64; }

        //////////////////////////////////////////
        inline void set(bool _value) { m_bool = _value; m_type = ShaderUniformType::UniformBool; }
        
        //////////////////////////////////////////
        inline void set(Texture2DPtr const& _texture) { m_texture = _texture; m_type = ShaderUniformType::UniformTexture2D; }

        //////////////////////////////////////////
        inline void set(TextureCubePtr const& _texture) { m_texture = _texture; m_type = ShaderUniformType::UniformTextureCube; }

        //////////////////////////////////////////
        inline void set(Texture2D** _textures, U32 _count) { m_ptr = _textures; m_count = _count; m_type = ShaderUniformType::UniformTexture2DArray; }

        //////////////////////////////////////////
        inline void set(Texture2D* _texture2D) { return set(_texture2D ? _texture2D->cast<Texture2D>() : nullptr); }

        //////////////////////////////////////////
        inline void set(TextureCube* _textureCube) { return set(_textureCube ? _textureCube->cast<TextureCube>() : nullptr); }
        
        //////////////////////////////////////////
        inline void set(Vec2F const& _vector) { m_vectorF = _vector; m_type = ShaderUniformType::UniformVec2F32; }
        
        //////////////////////////////////////////
        inline void set(Vec3F const& _vector) { m_vectorF = _vector; m_type = ShaderUniformType::UniformVec3F32; }
        
        //////////////////////////////////////////
        inline void set(Vec4F const& _vector) { m_vectorF = _vector; m_type = ShaderUniformType::UniformVec4F32; }


        //////////////////////////////////////////
        inline void set(Vec2S const& _vector) { m_vectorS = _vector; m_type = ShaderUniformType::UniformVec2S32; }
        
        //////////////////////////////////////////
        inline void set(Vec3S const& _vector) { m_vectorS = _vector; m_type = ShaderUniformType::UniformVec3S32; }
        
        //////////////////////////////////////////
        inline void set(Vec4S const& _vector) { m_vectorS = _vector; m_type = ShaderUniformType::UniformVec4S32; }


        //////////////////////////////////////////
        inline void set(Vec2U const& _vector) { m_vectorU = _vector; m_type = ShaderUniformType::UniformVec2U32; }
        
        //////////////////////////////////////////
        inline void set(Vec3U const& _vector) { m_vectorU = _vector; m_type = ShaderUniformType::UniformVec3U32; }
        
        //////////////////////////////////////////
        inline void set(Vec4U const& _vector) { m_vectorU = _vector; m_type = ShaderUniformType::UniformVec4U32; }


        //////////////////////////////////////////
        inline void set(Vec2B const& _vector) { m_vectorB = _vector; m_type = ShaderUniformType::UniformVec2B; }

        //////////////////////////////////////////
        inline void set(Vec3B const& _vector) { m_vectorB = _vector; m_type = ShaderUniformType::UniformVec3B; }

        //////////////////////////////////////////
        inline void set(Vec4B const& _vector) { m_vectorB = _vector; m_type = ShaderUniformType::UniformVec4B; }
        
        
        //////////////////////////////////////////
        inline void set(Mat3F const& _matrix) { m_matrix3DF = _matrix; m_type = ShaderUniformType::UniformMat3F32; }
                
        //////////////////////////////////////////
        inline void set(Mat4F const& _matrix) { m_matrix4DF = _matrix; m_type = ShaderUniformType::UniformMat4F32; }
        

        //////////////////////////////////////////
        inline void set(ColorF128 const& _color) { m_vectorF = _color.toVec4F32(); m_type = ShaderUniformType::UniformColorF128; }

        //////////////////////////////////////////
        inline void setColor(Vec4F const& _vector) { m_vectorF = _vector; m_type = ShaderUniformType::UniformColorF128; }


        //////////////////////////////////////////
        void setTexture2D(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void setTexture2D(String const& _textureName);

        //////////////////////////////////////////
        void setTextureCube(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void setTextureCube(String const& _textureName);

        //////////////////////////////////////////
        U32 calculateCRC32(U32 _seed = 0) const;

        //////////////////////////////////////////
        void setString(ShaderUniformType _type, CString _data, Size _count);

        //////////////////////////////////////////
        String toStringValue() const;

        //////////////////////////////////////////
        bool operator==(ShaderUniformVariant const& _variant) const;

        //////////////////////////////////////////
        bool operator!=(ShaderUniformVariant const& _variant) const;

        //////////////////////////////////////////
        ShaderUniformVariant& operator=(ShaderUniformVariant const& _variant);
        
        //////////////////////////////////////////
        ShaderUniformVariant& operator=(ShaderUniformVariant&& _variant) noexcept;


        //////////////////////////////////////////
        inline void setName(HashedString const& _name)
        {
            if (m_name == _name)
                return;

            m_name = _name;
        }

        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

    public:

        //////////////////////////////////////////
        virtual void loadFromXMLElement(tinyxml2::XMLElement const* _element) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual tinyxml2::XMLElement* toXMLElement(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        inline void setType(ShaderUniformType _type) { m_type = _type; }

    protected:
        RenderSystem* m_renderSystem;

        ShaderUniformType m_type = ShaderUniformType::None;

        TexturePtr m_texture;

        union
        {
            struct
            {
                void* m_ptr;
                U32 m_count;
            };

            S32 m_S32;
            F32 m_F32;
            F64 m_F64;
            bool m_bool;
        
            Vec4F m_vectorF;
            Vec4S m_vectorS;
            Vec4U m_vectorU;
            Vec4B m_vectorB;
            Mat3F m_matrix3DF;
            Mat4F m_matrix4DF;
        };
        
        HashedString m_name;
    };


    //////////////////////////////////////////
    std::ostream& operator<<(
        std::ostream& _o,
        ShaderUniformVariant const& _variant);


    //////////////////////////////////////////
    inline std::ostream& operator<<(
        std::ostream& _o,
        ShaderUniformVariantPtr const& _variant)
    {
        return _o << *_variant.get();
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVariant_hpp_
//////////////////////////////////////////
