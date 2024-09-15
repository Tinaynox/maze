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
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/events/MazeGraphicsEvents.hpp"
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
    Shader* Shader::s_instancesList = nullptr;

    //////////////////////////////////////////
    Shader::Shader()
        : m_renderSystemRaw(nullptr)
    {
        if (s_instancesList)
            s_instancesList->m_instancesListNext = this;
        m_instancesListPrev = s_instancesList;
        s_instancesList = this;
    }

    //////////////////////////////////////////
    Shader::~Shader()
    {
        if (m_instancesListPrev)
            m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        if (m_instancesListNext)
            m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        else
        if (s_instancesList == this)
            s_instancesList = m_instancesListPrev;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEventImmediate<ShaderDestroyedEvent>(this);
    }

    //////////////////////////////////////////
    ShaderPtr Shader::Create(RenderSystemPtr const& _renderSystem)
    {
        ShaderSystemPtr const& shaderSystem = _renderSystem ? _renderSystem->getShaderSystem() : ShaderSystem::GetCurrentInstancePtr();
        if (shaderSystem)
            return shaderSystem->createShader();
        
        return nullptr;
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
    void Shader::setUniform(HashedCString _uniformName, Vec2F const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec3F const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec4F const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec2S const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec3S const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec4S const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec2U const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec3U const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec4U const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec2B const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec3B const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Vec4B const& _vector, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Mat3F const& _matrix, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Mat4F const& _matrix, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, TMat const& _matrix, bool _warningIfNotExists)
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
    void Shader::setUniform(HashedCString _uniformName, Mat3F* _matrices, Size _count, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);
        
        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->upload(_matrices, _count);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, Mat4F* _matrices, Size _count, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->upload(_matrices, _count);
    }

    //////////////////////////////////////////
    void Shader::setUniform(HashedCString _uniformName, TMat* _matrices, Size _count, bool _warningIfNotExists)
    {
        const ShaderUniformPtr& uniform = ensureUniform(_uniformName);

        if (!uniform)
        {
            MAZE_WARNING_IF(_warningIfNotExists, "Undefined Shader Uniform with name '%s'!", _uniformName.str);
            return;
        }

        uniform->upload(_matrices, _count);
    }

    //////////////////////////////////////////
    Path const& Shader::getAssetFileName()
    {
        static Path nullValue;

        if (!m_assetFile)
            return nullValue;

        return m_assetFile->getFileName();
    }



    //////////////////////////////////////////
    bool Shader::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("Shader::loadFromAssetFile");

        if (!_assetFile)
            return false;

        m_assetFile = _assetFile;
        setName(_assetFile->getFileName());

        ByteBufferPtr assetFileHeader = _assetFile->readHeaderAsByteBuffer(6);
        assetFileHeader->setByte(5, 0);

        if (strstr((CString)assetFileHeader->getDataRO(), "xml") != nullptr)
        {
            Debug::LogWarning("Obsolete Shader format - %s", _assetFile->getFileName().toUTF8().c_str());
            tinyxml2::XMLDocument doc;
            _assetFile->readToXMLDocument(doc);
            loadFromXMLDocument(doc);
        }
        else
        {
            DataBlock dataBlock;
            ByteBufferPtr byteBuffer = _assetFile->readAsByteBuffer();
            dataBlock.loadFromByteBuffer(*byteBuffer.get());
            loadFromDataBlock(dataBlock);
        }
        

        return true;
    }

    //////////////////////////////////////////
    bool Shader::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", getName().c_str());
            return false;
        }


        rootNode = rootNode->NextSibling();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node children!", getName().c_str());
            return false;
        }


        AssetFilePtr shaderFile;
        AssetFilePtr vertexShaderFile;
        AssetFilePtr fragmentShaderFile;
        Vector<ShaderUniformVariant> uniformsData;

        AssetManager* assetManager = AssetManager::GetInstancePtr();

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
                        shaderFile = assetManager->getAssetFileByFileName(Path(shaderFileName));
                    }
                    else
                    {
                        CString vertexShaderFileName = shaderChildElement->Attribute("vertex");
                        CString fragmentShaderFileName = shaderChildElement->Attribute("fragment");

                        vertexShaderFile = vertexShaderFileName ? assetManager->getAssetFileByFileName(Path(vertexShaderFileName)) : AssetFilePtr();
                        fragmentShaderFile = fragmentShaderFileName ? assetManager->getAssetFileByFileName(Path(fragmentShaderFileName)) : AssetFilePtr();

                        MAZE_ERROR_IF(vertexShaderFileName && !vertexShaderFile, "Undefined vertex shader: %s", vertexShaderFileName);
                        MAZE_ERROR_IF(fragmentShaderFileName && !fragmentShaderFile, "Undefined vertex shader: %s", fragmentShaderFileName);
                    }
                }
                else
                if (strcmp(shaderChildElement->Name(), "Uniform") == 0)
                {
                    ShaderUniformVariant uniformData(m_renderSystemRaw);
                    uniformData.loadFromXMLElement(shaderChildElement);
                    uniformsData.push_back(uniformData);
                }

                shaderChildNode = shaderChildNode->NextSibling();
            }


            shaderNode = shaderNode->NextSibling();
        }

        return processLoadShader(
            shaderFile,
            vertexShaderFile,
            fragmentShaderFile,
            uniformsData);

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
        m_clipDistance0Uniform = ensureUniform(MAZE_HCS("u_clipDistance0"));
        m_clipDistanceEnableUniform = ensureUniform(MAZE_HCS("u_clipDistanceEnable"));
        m_projectionMatrixUniform = ensureUniform(MAZE_HCS("u_projectionMatrix"));
        m_projectionParamsUniform = ensureUniform(MAZE_HCS("u_projectionParams"));
        m_viewMatrixUniform = ensureUniform(MAZE_HCS("u_viewMatrix"));
        m_modelMatricesUniform = ensureUniform(MAZE_HCS("u_modelMatrices"));
        m_modelMatricesTextureUniform = ensureUniform(MAZE_HCS("u_modelMatricesTexture"));
        m_modelMatricesTextureSizeUniform = ensureUniform(MAZE_HCS("u_modelMatricesTextureSize"));
        m_modelMatriciesOffsetUniform = ensureUniform(MAZE_HCS("u_modelMatriciesOffset"));
        m_viewPositionUniform = ensureUniform(MAZE_HCS("u_viewPosition"));
        m_timeUniform = ensureUniform(MAZE_HCS("u_time"));
        m_mainLightColorUniform = ensureUniform(MAZE_HCS("u_mainLightColor"));
        m_mainLightDirectionUniform = ensureUniform(MAZE_HCS("u_mainLightDirection"));
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
    bool Shader::processLoadShader(
        AssetFilePtr const& _shaderFile,
        AssetFilePtr const& _vertexShaderFile,
        AssetFilePtr const& _fragmentShaderFile,
        Vector<ShaderUniformVariant> const& _uniformsData)
    {
        if (_shaderFile)
        {
            if (!loadFromSourceFile(_shaderFile))
                return false;
        }
        else
        if (_vertexShaderFile && _fragmentShaderFile)
        {
            if (!loadFromSourceFiles(_vertexShaderFile, _fragmentShaderFile))
                return false;
        }
        else
        {
            MAZE_ERROR("%s: Invalid shader file syntax!", m_assetFile->getFileName().toUTF8().c_str());
        }


        for (ShaderUniformVariant const& uniformData : _uniformsData)
            setUniform(uniformData);
        

        return true;
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
    bool Shader::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        AssetFilePtr shaderFile;
        AssetFilePtr vertexShaderFile;
        AssetFilePtr fragmentShaderFile;
        Vector<ShaderUniformVariant> uniformsData;

        AssetManager* assetManager = AssetManager::GetInstancePtr();

        for (DataBlock::DataBlockIndex i = 0; i < _dataBlock.getDataBlocksCount(); ++i)
        {
            DataBlock const* subBlock = _dataBlock.getDataBlock(i);

            if (subBlock->getName() == MAZE_HCS("feature"))
            {
                CString name = subBlock->getCString(MAZE_HCS("name"));

                if (name)
                {
                    CString value = subBlock->getCString(MAZE_HCS("value"));
                    addLocalFeature(name, value ? value : "(1)");
                }
            }
            else
            if (strcmp(subBlock->getName(), getLanguage()) == 0)
            {
                CString shaderFileName = subBlock->getCString(MAZE_HCS("shader"));
                if (shaderFileName)
                {
                    shaderFile = assetManager->getAssetFileByFileName(Path(shaderFileName));
                }
                else
                {
                    CString vertexShaderFileName = subBlock->getCString(MAZE_HCS("vertex"));
                    CString fragmentShaderFileName = subBlock->getCString(MAZE_HCS("fragment"));

                    vertexShaderFile = vertexShaderFileName ? assetManager->getAssetFileByFileName(Path(vertexShaderFileName)) : AssetFilePtr();
                    fragmentShaderFile = fragmentShaderFileName ? assetManager->getAssetFileByFileName(Path(fragmentShaderFileName)) : AssetFilePtr();

                    MAZE_ERROR_IF(vertexShaderFileName && !vertexShaderFile, "Undefined vertex shader: %s", vertexShaderFileName);
                    MAZE_ERROR_IF(fragmentShaderFileName && !fragmentShaderFile, "Undefined vertex shader: %s", fragmentShaderFileName);
                }
            }
            else
            if (subBlock->getName() == MAZE_HCS("uniform"))
            {
                ShaderUniformVariant uniformData(m_renderSystemRaw);
                uniformData.loadFromDataBlock(*subBlock);
                uniformsData.push_back(uniformData);
            }
        }

        return processLoadShader(
            shaderFile,
            vertexShaderFile,
            fragmentShaderFile,
            uniformsData);
    }

    //////////////////////////////////////////
    void Shader::toDataBlock(DataBlock& _dataBlock) const
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    String Shader::toString() const
    {
        if (getAssetFile())
            return getAssetFile()->getFileName();
        else
            return getName();
    }

    //////////////////////////////////////////
    void Shader::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    void Shader::FromString(ShaderPtr& _value, CString _shaderName, Size _count)
    {
        if (_count == 0)
            _count = strlen(_shaderName);

        _value = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getShaderSystem()->getOrLoadShader(_shaderName);
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
            if (uniformData.second && uniformData.second->getType() != ShaderUniformType::None)
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

    //////////////////////////////////////////
    void Shader::IterateShaders(std::function<bool(Shader*)> _cb)
    {
        Shader* instance = s_instancesList;
        while (instance)
        {
            if (!_cb(instance))
                break;

            instance = instance->m_instancesListPrev;
        }
    }


    //////////////////////////////////////////
    // Class ShaderAssetRef
    //
    //////////////////////////////////////////
    String ShaderAssetRef::toString() const
    {
        if (!m_shader)
            return String();

        return m_shader->getAssetFile() ? static_cast<String>(m_shader->getAssetFileName()) : m_shader->getName();
    }

    //////////////////////////////////////////
    void ShaderAssetRef::setString(CString _data, Size _count)
    {
        ShaderPtr const& shader = RenderSystem::GetCurrentInstancePtr()->getShaderSystem()->getOrLoadShader(_data);
        setShader(shader);
    }

} // namespace Maze
//////////////////////////////////////////
