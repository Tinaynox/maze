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
#if (!defined(_MazeShaderUniform_hpp_))
#define _MazeShaderUniform_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderUniform);
    

    //////////////////////////////////////////
    // Class ShaderUniform
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ShaderUniform
        : public SharedObject<ShaderUniform>
    {
    private:

        //////////////////////////////////////////
        friend class Shader;

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniform();

        //////////////////////////////////////////
        static ShaderUniformPtr Create(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None);


        //////////////////////////////////////////
        inline String const& getName() const { return m_value.getName(); }

        //////////////////////////////////////////
        inline ShaderUniformVariant const& getValue() const { return m_value; }

        //////////////////////////////////////////
        inline ShaderUniformType getType() const { return m_value.getType(); }

        //////////////////////////////////////////
        inline S32 getS32() const { return m_value.getS32(); }

        //////////////////////////////////////////
        inline F32 getF32() const { return m_value.getF32(); }

        //////////////////////////////////////////
        inline F64 getF64() const { return m_value.getF64(); }

        //////////////////////////////////////////
        inline Texture2DPtr const& getTexture2D() const { return m_value.getTexture2D(); }

        //////////////////////////////////////////
        inline Vec4DF const& getVecF() const { return m_value.getVecF(); }

        //////////////////////////////////////////
        inline Vec2DF getVec2DF() const { return m_value.getVec2DF(); }

        //////////////////////////////////////////
        inline Vec3DF getVec3DF() const { return m_value.getVec3DF(); }

        //////////////////////////////////////////
        inline Vec4DF getVec4DF() const { return m_value.getVec4DF(); }


        //////////////////////////////////////////
        inline Vec4DS const& getVecS() const { return m_value.getVecS(); }

        //////////////////////////////////////////
        inline Vec2DS getVec2DS() const { return m_value.getVec2DS(); }

        //////////////////////////////////////////
        inline Vec3DS getVec3DS() const { return m_value.getVec3DS(); }

        //////////////////////////////////////////
        inline Vec4DS getVec4DS() const { return m_value.getVec4DS(); }


        //////////////////////////////////////////
        inline Vec4DU const& getVecU() const { return m_value.getVecU(); }

        //////////////////////////////////////////
        inline Vec2DU getVec2DU() const { return m_value.getVec2DU(); }

        //////////////////////////////////////////
        inline Vec3DU getVec3DU() const { return m_value.getVec3DU(); }

        //////////////////////////////////////////
        inline Vec4DU getVec4DU() const { return m_value.getVec4DU(); }
        

        
        //////////////////////////////////////////
        inline Mat3DF const& getMat3DF() const { return m_value.getMat3DF(); }
        
        //////////////////////////////////////////
        inline Mat4DF const& getMat4DF() const { return m_value.getMat4DF(); }

        
        
        
        //////////////////////////////////////////
        bool set(S32 _value);
        
        //////////////////////////////////////////
        bool set(F32 _value);
        
        //////////////////////////////////////////
        bool set(F64 _value);
        
        //////////////////////////////////////////
        bool set(Texture2DPtr const& _texture2D);
        
        //////////////////////////////////////////
        inline bool set(Texture2D* _texture2D)
        {
            return set(_texture2D->cast<Texture2D>());
        }
        
        //////////////////////////////////////////
        bool set(Vec2DF const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3DF const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec4DF const& _vector);


        //////////////////////////////////////////
        bool set(Vec2DS const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3DS const& _vector);
        
        ////////////////////////////////////
        bool set(Vec4DS const& _vector);


        //////////////////////////////////////////
        bool set(Vec2DU const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3DU const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec4DU const& _vector);
        

        //////////////////////////////////////////
        bool set(Mat3DF const& _matrix);
        
        //////////////////////////////////////////
        bool setAffineMat(Mat3DF const& _matrix);
        
        //////////////////////////////////////////
        bool set(Mat4DF const& _matrix);
       
        //////////////////////////////////////////
        bool set(ShaderUniformVariant const& _variant);
        

        //////////////////////////////////////////
        bool set(F32 const* _values, Size _count);

        //////////////////////////////////////////
        bool set(Vec2DF const* _vectors, Size _count);

        //////////////////////////////////////////
        bool set(Vec3DF const* _vectors, Size _count);

        //////////////////////////////////////////
        bool set(Vec4DF const* _vectors, Size _count);

        //////////////////////////////////////////
        bool set(Mat3DF const* _matrices, Size _count);

        //////////////////////////////////////////
        bool set(Mat4DF const* _matrices, Size _count);
        
        
        //////////////////////////////////////////
        bool set(ColorF128 const& _value);


        //////////////////////////////////////////
        inline Shader* getShaderRaw() const { return m_shaderRaw; }

    protected:

        //////////////////////////////////////////
        ShaderUniform();

        //////////////////////////////////////////
        virtual bool init(ShaderPtr const& _shader, ShaderUniformType _type = ShaderUniformType::None);

        //////////////////////////////////////////
        void setName(String const& _name) { m_value.setName(_name); }

        
        //////////////////////////////////////////
        virtual void processUniformWillBeChanged() {};

        //////////////////////////////////////////
        virtual void processSimpleUniformChanged() MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void uploadArrayUniform(F32 const* _value, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void uploadArrayUniform(Vec2DF const* _vectors, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void uploadArrayUniform(Vec3DF const* _vectors, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void uploadArrayUniform(Vec4DF const* _vectors, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void uploadArrayUniform(Mat3DF const* _matrices, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void uploadArrayUniform(Mat4DF const* _matrices, Size _count) MAZE_ABSTRACT;
    
    protected:
        ShaderWPtr m_shader;
        Shader* m_shaderRaw;

        ShaderUniformVariant m_value;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniform_hpp_
//////////////////////////////////////////
