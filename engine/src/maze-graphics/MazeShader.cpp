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
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Shader
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Shader,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, name, String(), getName, setName),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AssetFilePtr, assetFile, AssetFilePtr(), getAssetFile, setAssetFile));

    //////////////////////////////////////////
    Shader::Shader()
        : m_renderSystemRaw(nullptr)
    {
    }

    //////////////////////////////////////////
    Shader::~Shader()
    {
    }

    //////////////////////////////////////////
    ShaderPtr Shader::CreateFromFile(RenderSystemPtr const& _renderSystem, AssetFilePtr const& _shaderFile)
    {
        ShaderSystemPtr const& shaderSystem = _renderSystem->getShaderSystem();
        return shaderSystem->createShader(_shaderFile);
    }

    //////////////////////////////////////////
    bool Shader::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        return true;
    }

    //////////////////////////////////////////
    bool Shader::init(ShaderPtr const& _shader)
    {
        if (!MetaClassHelper::CopyProperties(
            getMetaInstance(),
            _shader->getMetaInstance()))
            return false;

        m_renderSystem = _shader->m_renderSystem;
        m_renderSystemRaw = _shader->m_renderSystemRaw;

        m_localFeatures = _shader->m_localFeatures;

        return true;
    }

    //////////////////////////////////////////
    ShaderUniformPtr const& Shader::ensureUniform(HashedCString _uniformName, ShaderUniformType _type)
    {
        UnorderedMap<U32, ShaderUniformPtr>::const_iterator it = m_uniformsCache.find(_uniformName.hash);
        if (it != m_uniformsCache.end())
        {
#if MAZE_DEBUG
            MAZE_ERROR_IF(it->second && it->second->getName().asHashedCString() != _uniformName, "Hash collision detected!");
#endif
            return it->second;
        }

        return createUniformFromShader(_uniformName);
    }

    //////////////////////////////////////////
    bool Shader::hasUniform(HashedCString _uniformName)
    {
        UnorderedMap<U32, ShaderUniformPtr>::const_iterator it = m_uniformsCache.find(_uniformName.hash);
        return (it != m_uniformsCache.end()) && it->second;
    }

    ///////////////////////////f///////////////
    void Shader::setUniform(HashedCString _uniformName, ShaderUniformVariant const& _value, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_value);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, S32 _value, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_value);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, F32 _value, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_value);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, bool _value, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_value);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Texture2DPtr const& _texture2D, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_texture2D);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec2DF const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec3DF const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec4DF const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }


    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec2DS const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec3DS const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec4DS const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }


    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec2DU const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec3DU const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec4DU const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec2DB const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec3DB const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Vec4DB const& _vector, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_vector);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Mat3DF const& _matrix, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_matrix);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Mat4DF const& _matrix, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_matrix);
    }

    //////////////////////////////////////////
    void Shader::setUniform(ShaderUniformVariant const& _variant, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_variant.getName());

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _variant.getName().c_str());
            return;
        }
        
        uniform->set(_variant);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Mat3DF* _matrices, Size _count, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);
        
        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_matrices, _count);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Mat4DF* _matrices, Size _count, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->set(_matrices, _count);
    }

    //////////////////////////////////////////
    String const& Shader::getAssetFileName()
    {
        static String nullValue;

        if (!m_assetFile)
            return nullValue;

        return m_assetFile->getFileName().getString();
    }


    //////////////////////////////////////////
    // Struct ShaderXMLUniformData
    //
    //////////////////////////////////////////
    struct ShaderXMLUniformData
    {
        HashedString name;
        String type;
        String value;
    };


    //////////////////////////////////////////
    bool Shader::loadFromAssetFile(AssetFilePtr const& _shaderFile)
    {
        if (!_shaderFile)
            return false;

        m_assetFile = _shaderFile;

        setName(_shaderFile->getFileName());

        tinyxml2::XMLDocument doc;
        if (!_shaderFile->readToXMLDocument(doc))
            return false;


        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", _shaderFile->getFileName().c_str());
            return false;
        }

        
        rootNode = rootNode->NextSibling();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node children!", _shaderFile->getFileName().c_str());
            return false;
        }
        

        AssetFilePtr shaderFile;
        AssetFilePtr vertexShaderFile;
        AssetFilePtr fragmentShaderFile;
        Vector<ShaderXMLUniformData> uniformsData;

        AssetManager* assetManager = AssetManager::GetInstancePtr();
        TextureManagerPtr const& textureManager = getRenderSystemRaw()->getTextureManager();

        tinyxml2::XMLNode* shaderNode = rootNode;
        while (shaderNode)
        {
            tinyxml2::XMLElement* shaderElement = shaderNode->ToElement();
            if (!shaderElement)
            {
                shaderNode = shaderNode->NextSibling();
                continue;
            }

            

            tinyxml2::XMLNode* shaderChildNode = shaderNode->FirstChild();
            while (shaderChildNode)
            {
                tinyxml2::XMLElement* shaderChildElement = shaderChildNode->ToElement();
                if (!shaderChildElement)
                {
                    shaderChildNode = shaderChildNode->NextSibling();
                    continue;
                }

                if (strcmp(shaderChildElement->Name(), "Feature") == 0)
                {
                    CString name = shaderChildElement->Attribute("name");

                    if (name)
                    {
                        CString value = shaderChildElement->Attribute("value");
                        addLocalFeature(name, value ? value : "(1)");
                    }
                }
                else
                if (strcmp(shaderChildElement->Name(), getLanguage()) == 0)
                {
                    CString shaderFileName = shaderChildElement->Attribute("shader");
                    if (shaderFileName)
                    {
                        shaderFile = assetManager->getAssetFileByFileName(shaderFileName);
                    }
                    else
                    {
                        CString vertexShaderFileName = shaderChildElement->Attribute("vertex");
                        CString fragmentShaderFileName = shaderChildElement->Attribute("fragment");

                        vertexShaderFile = vertexShaderFileName ? assetManager->getAssetFileByFileName(vertexShaderFileName) : AssetFilePtr();
                        fragmentShaderFile = fragmentShaderFileName ? assetManager->getAssetFileByFileName(fragmentShaderFileName) : AssetFilePtr();

                        MAZE_ERROR_IF(vertexShaderFileName && !vertexShaderFile, "Undefined vertex shader: %s", vertexShaderFileName);
                        MAZE_ERROR_IF(fragmentShaderFileName && !fragmentShaderFile, "Undefined vertex shader: %s", fragmentShaderFileName);
                    }
                }
                else
                if (strcmp(shaderChildElement->Name(), "Uniform") == 0)
                {
                    CString name = shaderChildElement->Attribute("name");
                    CString type = shaderChildElement->Attribute("type");
                    CString value = shaderChildElement->Attribute("value");

                    ShaderXMLUniformData uniformData;
                    uniformData.name = name ? name : "";
                    uniformData.type = type ? type : "";
                    uniformData.value = value ? value : "";
                    uniformsData.push_back(uniformData);
                }

                shaderChildNode = shaderChildNode->NextSibling();
            }


            shaderNode = shaderNode->NextSibling();
        }


        if (shaderFile)
        {
            if (!loadFromSourceFile(shaderFile))
                return false;
        }
        else
        if (vertexShaderFile && fragmentShaderFile)
        {
            if (!loadFromSourceFiles(vertexShaderFile, fragmentShaderFile))
                return false;
        }
        else
        {
            MAZE_ERROR("%s: Invalid shader file syntax!", m_assetFile->getFileName().c_str());
        }

        for (ShaderXMLUniformData const& uniformData : uniformsData)
        {
            ShaderUniformType::Enum uniformType = ShaderUniformType::FromString(uniformData.type);

            switch (uniformType)
            {
                case ShaderUniformType::UniformS32:
                {
                    setUniform(uniformData.name.asHashedCString(), StringHelper::StringToS32(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformF32:
                {
                    setUniform(uniformData.name.asHashedCString(), StringHelper::StringToF32(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformBool:
                {
                    setUniform(uniformData.name.asHashedCString(), StringHelper::StringToBool(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformTexture2D:
                {
                    Texture2DPtr const& texture2D = textureManager->getTexture2D(uniformData.value);

                    if (texture2D)
                    {
                        setUniform(uniformData.name.asHashedCString(), texture2D);
                    }
                    else
                    {
                        MAZE_ERROR("Shader: '%s'. Undefined texture for uniform '%s': '%s'", getName().c_str(), uniformData.name.c_str(), uniformData.value.c_str());
                        setUniform(uniformData.name.asHashedCString(), textureManager->getErrorTexture());
                    }
                    break;
                }

                case ShaderUniformType::UniformVec2DF:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec2DF::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec3DF:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec3DF::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec4DF:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec4DF::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec2DS:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec2DS::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec3DS:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec3DS::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec4DS:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec4DS::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec2DU:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec2DU::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec3DU:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec3DU::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec4DU:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec4DU::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec2DB:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec2DB::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec3DB:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec3DB::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformVec4DB:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec4DB::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformMat3DF:
                {
                    setUniform(uniformData.name.asHashedCString(), Mat3DF::FromString(uniformData.value));
                    break;
                }

                case ShaderUniformType::UniformMat4DF:
                {
                    setUniform(uniformData.name.asHashedCString(), Mat4DF::FromString(uniformData.value));
                    break;
                }
                
                case ShaderUniformType::UniformColorF128:
                {
                    setUniform(uniformData.name.asHashedCString(), Vec4DF::FromString(uniformData.value));
                    break;
                }

                default:
                {
                    MAZE_ERROR("Undefined uniform type - %s!", uniformData.type.c_str());
                    break;
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool Shader::loadFromSourceFile(AssetFilePtr const& _sourceFile)
    {
        if (!_sourceFile)
            return false;

        String shaderSource;
        _sourceFile->readToString(shaderSource);

        return loadFromSource(shaderSource);
    }

    //////////////////////////////////////////
    bool Shader::loadFromSourceFiles(AssetFilePtr const& _vertexShaderFile, AssetFilePtr const& _fragmentShaderFile)
    {
        if (!_vertexShaderFile || !_fragmentShaderFile)
            return false;

        String vertexShaderSource;
        String fragmentShaderSource;
        _vertexShaderFile->readToString(vertexShaderSource);
        _fragmentShaderFile->readToString(fragmentShaderSource);

        return loadFromSources(vertexShaderSource, fragmentShaderSource);
    }

    //////////////////////////////////////////
    void Shader::assignDefaultUniforms()
    {
        m_clipDistance0Uniform = ensureUniform("u_clipDistance0");
        m_clipDistanceEnableUniform = ensureUniform("u_clipDistanceEnable");
        m_projectionMatrixUniform = ensureUniform("u_projectionMatrix");
        m_projectionParamsUniform = ensureUniform("u_projectionParams");
        m_viewMatrixUniform = ensureUniform("u_viewMatrix");
        m_modelMatricesUniform = ensureUniform("u_modelMatrices");
        m_modelMatricesTextureUniform = ensureUniform("u_modelMatricesTexture");
        m_modelMatricesTextureSizeUniform = ensureUniform("u_modelMatricesTextureSize");
        m_modelMatriciesOffsetUniform = ensureUniform("u_modelMatriciesOffset");
        m_viewPositionUniform = ensureUniform("u_viewPosition");
        m_timeUniform = ensureUniform("u_time");
        m_mainLightColorUniform = ensureUniform("u_mainLightColor");
        m_mainLightDirectionUniform = ensureUniform("u_mainLightDirection");
    }

    //////////////////////////////////////////
    void Shader::resetDefaultUniforms()
    {
        m_projectionMatrixUniform.reset();
        m_viewMatrixUniform.reset();
        m_modelMatricesUniform.reset();
        m_modelMatricesTextureUniform.reset();
        m_modelMatricesTextureSizeUniform.reset();
        m_modelMatriciesOffsetUniform.reset();
        m_viewPositionUniform.reset();
        m_timeUniform.reset();
        m_mainLightColorUniform.reset();
        m_mainLightDirectionUniform.reset();
    }

    //////////////////////////////////////////
    void Shader::clearUniformsCache()
    {
        m_uniformsCache.clear();
    }

    //////////////////////////////////////////
    void Shader::addLocalFeature(CString _name, CString _value)
    {
        MAZE_ERROR_RETURN_IF(!_name, "Name is null!");

        m_localFeatures[_name] = _value;
    }

    //////////////////////////////////////////
    void Shader::removeLocalFeature(CString _name)
    {
        auto it =  m_localFeatures.find(_name);
        if (it != m_localFeatures.end())
            m_localFeatures.erase(it);
    }

    //////////////////////////////////////////
    void Shader::reload()
    {
        if (m_assetFile)
        {
            loadFromAssetFile(m_assetFile);
        }
    }


    //////////////////////////////////////////
    void Shader::FromString(ShaderPtr& _value, CString _shaderName, Size _count)
    {
        if (_count == 0)
            _count = strlen(_shaderName);

        _value = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getShaderSystem()->getShader(_shaderName);
        if (!_value)
        {
            if (_shaderName && strcmp(_shaderName, "") != 0)
            {
                MAZE_ERROR("Undefined shader - %s!", _shaderName);
            }
        }
    }

    //////////////////////////////////////////
    Vector<ShaderUniformVariant> Shader::getUniformVariants() const
    {
        Vector<ShaderUniformVariant> uniforms;

        for (auto const& uniformData : m_uniformsCache)
        {
            if (uniformData.second)
                uniforms.push_back(uniformData.second->getValue());
        }

        return uniforms;
    }

    //////////////////////////////////////////
    void Shader::cacheUniformVariants()
    {
        m_cachedUniformVariants = getUniformVariants();
    }

    //////////////////////////////////////////
    void Shader::applyCachedUniformVariants()
    {
        for (auto const& uniformData : m_cachedUniformVariants)
            setUniform(uniformData);
    }

} // namespace Maze
//////////////////////////////////////////
