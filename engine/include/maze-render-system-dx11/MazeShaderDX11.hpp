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
#if (!defined(_MazeShaderDX11_hpp_))
#define _MazeShaderDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderDX11);
    MAZE_USING_SHARED_PTR(RenderSystem);
    class RenderSystemDX11;


    //////////////////////////////////////////
    enum class ShaderDX11Stage : S32
    {
        Vertex = 0,
        Pixel = 1,
        MAX = 2
    };


    //////////////////////////////////////////
    // Struct ShaderDX11InputParameter
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_DX11_API ShaderDX11InputParameter
    {
        String semanticName;
        U32 semanticIndex = 0u;
        D3D_REGISTER_COMPONENT_TYPE componentType = D3D_REGISTER_COMPONENT_FLOAT32;
    };


    //////////////////////////////////////////
    // Struct ShaderDX11ConstantBuffer
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_DX11_API ShaderDX11ConstantBuffer
    {
        String name;
        U32 bindPoint = 0u;
        ID3D11Buffer* buffer = nullptr;
        Vector<U8> shadow;
        bool dirty = false;
    };


    //////////////////////////////////////////
    // Struct ShaderDX11UniformData
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_DX11_API ShaderDX11UniformData
    {
        //////////////////////////////////////////
        struct StageBinding
        {
            // Constant buffer variable
            S32 constantBufferIndex = -1;
            U32 offset = 0u;
            U32 size = 0u;

            // Texture resource
            S32 srvSlot = -1;
            S32 samplerSlot = -1;
            U32 srvCount = 0u;
        };

        String name;
        StageBinding stageBindings[(Size)ShaderDX11Stage::MAX];
        bool isTexture = false;

        //////////////////////////////////////////
        inline bool isUsed() const
        {
            for (Size i = 0; i < (Size)ShaderDX11Stage::MAX; ++i)
                if (stageBindings[i].constantBufferIndex >= 0 || stageBindings[i].srvSlot >= 0)
                    return true;
            return false;
        }
    };


    //////////////////////////////////////////
    // Class ShaderDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API ShaderDX11
        : public Shader
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderDX11, Shader);

    public:

        //////////////////////////////////////////
        virtual ~ShaderDX11();

        //////////////////////////////////////////
        static ShaderPtr Create(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static ShaderPtr CreateFromFile(
            RenderSystemPtr const& _renderSystem,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        static ShaderPtr CreateFromSource(
            RenderSystemPtr const& _renderSystem,
            String const& _shaderSource,
            CString _shaderName = nullptr);

        //////////////////////////////////////////
        static ShaderPtr Create(ShaderDX11Ptr const& _shader);

        //////////////////////////////////////////
        virtual ShaderPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual CString getLanguage() const MAZE_OVERRIDE { return "HLSL"; }


        //////////////////////////////////////////
        virtual bool loadFromSource(String const& _shaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void recompile() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        inline ID3D11VertexShader* getVertexShader() const { return m_vertexShader; }

        //////////////////////////////////////////
        inline ID3D11PixelShader* getPixelShader() const { return m_pixelShader; }

        //////////////////////////////////////////
        inline ID3DBlob* getVertexShaderCode() const { return m_vertexShaderCode; }

        //////////////////////////////////////////
        inline U32 getInputSignatureId() const { return m_inputSignatureId; }

        //////////////////////////////////////////
        inline Vector<ShaderDX11InputParameter> const& getInputParameters() const { return m_inputParameters; }


        //////////////////////////////////////////
        ShaderDX11UniformData const* getUniformData(HashedCString _uniformName) const;

        //////////////////////////////////////////
        // Writes packed bytes into the constant buffer shadow copies of every stage the uniform exists in
        void writeUniformData(
            ShaderDX11UniformData const& _uniformData,
            U8 const* _bytes,
            U32 _bytesCount);

        //////////////////////////////////////////
        // Uploads dirty constant buffers and binds them
        void flushConstantBuffers();

        //////////////////////////////////////////
        // Binds all texture uniforms to their reflected slots
        void bindTextures();

    protected:

        //////////////////////////////////////////
        ShaderDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool init(
            RenderSystemPtr const& _renderSystem,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        bool init(
            RenderSystemPtr const& _renderSystem,
            String const& _shaderSource,
            CString _shaderName);

        //////////////////////////////////////////
        bool init(ShaderDX11Ptr const& _shader);

        //////////////////////////////////////////
        virtual ShaderUniformPtr const& createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool loadDXShader(String const& _vertexShaderSource, String const& _fragmentShaderSource);

        //////////////////////////////////////////
        void unloadDXShader();

        //////////////////////////////////////////
        ID3DBlob* compileDXShader(
            String const& _source,
            ShaderDX11Stage _stage);

        //////////////////////////////////////////
        bool reflectDXShader(
            ID3DBlob* _shaderCode,
            ShaderDX11Stage _stage);

        //////////////////////////////////////////
        String buildShaderCodeHeader(ShaderDX11Stage _stage);

        //////////////////////////////////////////
        String buildLocalShaderFeatures();

        //////////////////////////////////////////
        UnorderedMap<S32, String> preProcessSource(String const& _source);

    protected:
        String m_vertexShaderSource;
        String m_fragmentShaderSource;

        ID3D11VertexShader* m_vertexShader = nullptr;
        ID3D11PixelShader* m_pixelShader = nullptr;
        ID3DBlob* m_vertexShaderCode = nullptr;

        U32 m_inputSignatureId = 0u;
        Vector<ShaderDX11InputParameter> m_inputParameters;

        Vector<ShaderDX11ConstantBuffer> m_constantBuffers[(Size)ShaderDX11Stage::MAX];
        UnorderedMap<U32, ShaderDX11UniformData> m_uniformsReflection;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderDX11_hpp_
//////////////////////////////////////////
