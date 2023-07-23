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
#if (!defined(_MazeShader_hpp_))
#define _MazeShader_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(ShaderUniform);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Texture2D);
    

    //////////////////////////////////////////
    // Class Shader
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Shader
        : public SharedObject<Shader>
        , public ISharedCopyable<Shader>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Shader);

    public:

        //////////////////////////////////////////
        virtual ~Shader();


        //////////////////////////////////////////
        static ShaderPtr CreateFromFile(RenderSystemPtr const& _renderSystem, AssetFilePtr const& _shaderFile);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_ABSTRACT;


        //////////////////////////////////////////
        ShaderUniformPtr const& ensureUniform(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None);

        //////////////////////////////////////////
        bool hasUniform(HashedCString _uniformName);


        ///////////////////////////f///////////////
        void setUniform(HashedCString _uniformName, ShaderUniformVariant const& _value, bool _warningIfNotExists = false);

        ///////////////////////////f///////////////
        void setUniform(HashedCString _uniformName, S32 _value, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, F32 _value, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, bool _value, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Texture2DPtr const& _texture2D, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DF const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DF const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DF const& _vector, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DS const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DS const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DS const& _vector, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DU const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DU const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DU const& _vector, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DB const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformNamee, Vec3DB const& _vector, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DB const& _vector, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat3DF const& _matrix, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat4DF const& _matrix, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(ShaderUniformVariant const& _variant, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat3DF* _matrices, Size _count, bool _warningIfNotExists = false);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat4DF* _matrices, Size _count, bool _warningIfNotExists = false);


        //////////////////////////////////////////
        AssetFilePtr const& getShaderFile() const { return m_assetFile; }


        //////////////////////////////////////////
        virtual CString getLanguage() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }

        //////////////////////////////////////////
        RenderSystemPtr getRenderSystem() const { return m_renderSystem.lock(); }

        //////////////////////////////////////////
        RenderSystem* getRenderSystemRaw() const { return m_renderSystemRaw; }


        //////////////////////////////////////////
        inline AssetFilePtr const& getAssetFile() const { return m_assetFile; }

        //////////////////////////////////////////
        Path const& getAssetFileName();



        //////////////////////////////////////////
        virtual bool loadFromAssetFile(AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        bool loadFromSourceFile(AssetFilePtr const& _sourceFile);

        //////////////////////////////////////////
        bool loadFromSourceFiles(AssetFilePtr const& _vertexShaderFile, AssetFilePtr const& _fragmentShaderFile);

        //////////////////////////////////////////
        virtual bool loadFromSource(String const& _shaderSource) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource) MAZE_ABSTRACT;


        //////////////////////////////////////////
        ShaderUniformPtr const& getClipDistance0Uniform() const { return m_clipDistance0Uniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getClipDistanceEnableUniform() const { return m_clipDistanceEnableUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getProjectionMatrixUniform() const { return m_projectionMatrixUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getProjectionParamsUniform() const { return m_projectionParamsUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getViewMatrixUniform() const { return m_viewMatrixUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getModelMatricesUniform() const { return m_modelMatricesUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getModelMatricesTextureUniform() const { return m_modelMatricesTextureUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getModelMatricesTextureSizeUniform() const { return m_modelMatricesTextureSizeUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getModelMatriciesOffsetUniform() const { return m_modelMatriciesOffsetUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getViewPositionUniform() const { return m_viewPositionUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getTimeUniform() const { return m_timeUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getMainLightColorUniform() const { return m_mainLightColorUniform; }

        //////////////////////////////////////////
        ShaderUniformPtr const& getMainLightDirectionUniform() const { return m_mainLightDirectionUniform; }

        //////////////////////////////////////////
        void addLocalFeature(CString _name, CString _value = "");

        //////////////////////////////////////////
        void removeLocalFeature(CString _name);

        //////////////////////////////////////////
        void reload();

        //////////////////////////////////////////
        virtual void recompile() MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline UnorderedMap<U32, ShaderUniformPtr> const& getUniforms() const { return m_uniformsCache; }


        //////////////////////////////////////////
        Vector<ShaderUniformVariant> getUniformVariants() const;

        //////////////////////////////////////////
        void cacheUniformVariants();

        //////////////////////////////////////////
        void applyCachedUniformVariants();

    public:

        //////////////////////////////////////////
        static void FromString(ShaderPtr& _value, CString _data, Size _count);

    protected:

        //////////////////////////////////////////
        Shader();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual bool init(ShaderPtr const& _shader);

        //////////////////////////////////////////
        virtual ShaderUniformPtr const& createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None) MAZE_ABSTRACT;
    
        //////////////////////////////////////////
        void assignDefaultUniforms();

        //////////////////////////////////////////
        void resetDefaultUniforms();

        //////////////////////////////////////////
        void clearUniformsCache();

        //////////////////////////////////////////
        void setAssetFile(AssetFilePtr const& _assetFile) { m_assetFile = _assetFile; }

    protected:
        String m_name;

        AssetFilePtr m_assetFile;

        UnorderedMap<U32, ShaderUniformPtr> m_uniformsCache;

        StringKeyMap<String> m_localFeatures;

        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        ShaderUniformPtr m_clipDistance0Uniform;
        ShaderUniformPtr m_clipDistanceEnableUniform;
        ShaderUniformPtr m_projectionMatrixUniform;
        ShaderUniformPtr m_projectionParamsUniform;
        ShaderUniformPtr m_viewMatrixUniform;
        ShaderUniformPtr m_modelMatricesUniform;
        ShaderUniformPtr m_modelMatricesTextureUniform;
        ShaderUniformPtr m_modelMatricesTextureSizeUniform;
        ShaderUniformPtr m_modelMatriciesOffsetUniform;
        ShaderUniformPtr m_viewPositionUniform;
        ShaderUniformPtr m_timeUniform;
        ShaderUniformPtr m_mainLightColorUniform;
        ShaderUniformPtr m_mainLightDirectionUniform;

        Vector<ShaderUniformVariant> m_cachedUniformVariants;
    };


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ShaderPtr>::value), void>::type
        ValueToString(ShaderPtr const& _value, String& _data)
    {
        if (_value->getAssetFile())
            _data = _value->getAssetFile()->getFileName();
        else
            _data = _value->getName();
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ShaderPtr>::value), void>::type
        ValueFromString(ShaderPtr& _value, CString _data, Size _count)
    {
        Shader::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ShaderPtr>::value), U32>::type
        GetValueSerializationSize(ShaderPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ShaderPtr>::value), void>::type
        SerializeValue(ShaderPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ShaderPtr>::value), void>::type
        DeserializeValue(ShaderPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShader_hpp_
//////////////////////////////////////////
