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
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/assets/MazeAssetUnitMaterial.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockBinarySerialization.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static const S32 c_materialDataBlockVersion = 1;


    //////////////////////////////////////////
    // Class Material
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Material, Object,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, name, String(), getName, setName));

    //////////////////////////////////////////
    Material* Material::s_instancesList = nullptr;

    //////////////////////////////////////////
    Material::Material()
        : m_renderSystem(nullptr)
    {
        if (s_instancesList)
            s_instancesList->m_instancesListNext = this;
        m_instancesListPrev = s_instancesList;
        s_instancesList = this;
    }

    //////////////////////////////////////////
    Material::~Material()
    {
        if (m_instancesListPrev)
            m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        if (m_instancesListNext)
            m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        else
            if (s_instancesList == this)
                s_instancesList = m_instancesListPrev;
    }

    //////////////////////////////////////////
    MaterialPtr Material::Create(RenderSystem* _renderSystem)
    {
        if (_renderSystem == nullptr)
        {
            MAZE_ERROR_RETURN_VALUE_IF(!Maze::GraphicsManager::GetInstancePtr(), MaterialPtr(), "GraphicsManager is null!");
            _renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem().get();
            MAZE_ERROR_RETURN_VALUE_IF(!_renderSystem, MaterialPtr(), "RenderSystem is null!");
        }

        return _renderSystem->createMaterial();
    }

    //////////////////////////////////////////
    MaterialPtr Material::Create(AssetFilePtr const& _assetFile)
    {
        MaterialPtr material = Material::Create();

        if (material)
        {
            if (!material->loadFromAssetFile(_assetFile))
                return nullptr;

            String name = _assetFile->getFileName();
            StringHelper::RemoveSubstring(name, ".mzmaterial");
            material->setName(name);
            
        }
        
        return material;
    }

    //////////////////////////////////////////
    MaterialPtr Material::Create(String const& _assetFileName)
    {
        MaterialPtr material = Material::Create();

        if (material)
        {
            if (!material->loadFromAssetFile(_assetFileName))
                return nullptr;

            String name = _assetFileName;
            StringHelper::RemoveSubstring(name, ".mzmaterial");
            material->setName(name);
        }

        return material;
    }

    //////////////////////////////////////////
    MaterialPtr Material::Create(MaterialPtr const& _material)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_material, nullptr, "Null material!");
        return _material->createCopy();
    }

    //////////////////////////////////////////
    bool Material::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool Material::init(MaterialPtr const& _material)
    {
        setName(_material->getName());

        m_renderSystem = _material->m_renderSystem;

        m_uniforms = _material->m_uniforms;

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
        {
            for (RenderPassPtr const& renderPass : _material->m_passes[passType])
            {
                RenderPassPtr renderPassCopy = renderPass->createCopy();
                addRenderPass(renderPassCopy);
            }
        }

        return true;
    }

    //////////////////////////////////////////
    RenderPassPtr Material::createRenderPass(RenderPassType _passType)
    {
        RenderPassPtr renderPass = m_renderSystem->createRenderPass(_passType);

        if (renderPass)
            addRenderPass(renderPass);

        return renderPass;
    }

    //////////////////////////////////////////
    void Material::addRenderPass(RenderPassPtr const& _renderPass)
    {
        RenderPassType renderPassType = _renderPass->getPassType();
        Vector<RenderPassPtr>& passesByType = m_passes[renderPassType];
        passesByType.push_back(_renderPass);
        _renderPass->setMaterial(this);
    }

    //////////////////////////////////////////
    RenderPassPtr const& Material::getRenderPass(RenderPassType _passType, Size _index)
    {
        static RenderPassPtr const nullPointer;

        Vector<RenderPassPtr>& passesByType = m_passes[_passType];
        if (passesByType.empty() || _index >= passesByType.size())
            return nullPointer;

        return passesByType[_index];
    }

    //////////////////////////////////////////
    RenderPassPtr const& Material::getFirstRenderPass(RenderPassType _passType) const
    {
        static RenderPassPtr const nullPointer;

        Vector<RenderPassPtr> const& passesByType = m_passes[_passType];
        if (passesByType.empty())
            return nullPointer;

        return passesByType.front();
    }

    //////////////////////////////////////////
    void Material::clear()
    {
        clearAllRenderPasses();
        clearAllUniforms();
    }

    //////////////////////////////////////////
    void Material::clearAllRenderPasses()
    {
        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
            m_passes[passType].clear();
    }

    //////////////////////////////////////////
    void Material::clearAllUniforms()
    {
        m_uniforms.clear();
    }

    //////////////////////////////////////////
    bool Material::isEqual(MaterialPtr const& _material) const
    {
        if (!_material)
            return false;

        if (getName() != _material->getName())
            return false;

        if (m_uniforms.size() != _material->m_uniforms.size())
            return false;

        for (Size i = 0, in = _material->m_uniforms.size(); i < in; ++i)
        {
            if (*m_uniforms[i].get() != *_material->m_uniforms[i].get())
                return false;
        }

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
        {
            Size passesCount = _material->m_passes[passType].size();
            if (m_passes[passType].size() != passesCount)
                return false;

            for (Size i = 0; i < passesCount; ++i)
            {
                RenderPassPtr const& otherRenderPass = _material->m_passes[passType][i];
                RenderPassPtr const& renderPass = m_passes[passType][i];

                if (!renderPass->isEqual(otherRenderPass))
                    return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool Material::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        MAZE_PROFILE_EVENT("Material::loadFromXMLDocument");

        tinyxml2::XMLNode* rootNode = _doc.FirstChild();

        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File loading error - empty root node!");

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File loading error - empty root node children!");

        tinyxml2::XMLElement const* rootElement = rootNode->ToElement();
        MAZE_ERROR_RETURN_VALUE_IF(!rootElement, false, "File loading error - root node is not XML element!");

        loadFromXMLElement(rootElement);
        return true;
    }

    //////////////////////////////////////////
    bool Material::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("Material::loadFromAssetFile");

        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Asset File is null!");

        MAZE_LOG("Loading Material: %s...", _assetFile->getFileName().toUTF8().c_str());
        Timer timer;

        ByteBufferPtr assetFileHeader = _assetFile->readHeaderAsByteBuffer(6);
        assetFileHeader->setByte(5, 0);

        bool result = false;
        if (strstr((CString)assetFileHeader->getDataRO(), "xml") != nullptr)
        {
            Debug::LogWarning("Obsolete Material format - %s", _assetFile->getFileName().toUTF8().c_str());
            tinyxml2::XMLDocument doc;
            _assetFile->readToXMLDocument(doc);
            result = loadFromXMLDocument(doc);
        }
        else
        {
            DataBlock dataBlock;
            ByteBufferPtr byteBuffer = _assetFile->readAsByteBuffer();
            dataBlock.loadFromByteBuffer(*byteBuffer.get());
            result = loadFromDataBlock(dataBlock);
        }
        
        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        MAZE_LOG(result ? "Material %s loaded for %.1fms." : "%s material failed to load for %.1fms.", _assetFile->getFileName().toUTF8().c_str(), msTime);

        return result;
    }

    //////////////////////////////////////////
    bool Material::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        return loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    void Material::set(MaterialPtr const& _material)
    {
        setName(_material->getName());

        m_renderSystem = _material->m_renderSystem;

        m_uniforms.resize(_material->m_uniforms.size());
        for (Size i = 0, in = _material->m_uniforms.size(); i < in; ++i)
        {
            if (m_uniforms[i])
                *m_uniforms[i] = *_material->m_uniforms[i].get();
            else
                m_uniforms[i] = std::make_shared<ShaderUniformVariant>(*_material->m_uniforms[i].get());
        }

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
        {
            Size passesCount = _material->m_passes[passType].size();
            m_passes[passType].resize(passesCount);
            for (Size i = 0; i < passesCount; ++i)
            {
                RenderPassPtr const& otherRenderPass = _material->m_passes[passType][i];
                RenderPassPtr& renderPass = m_passes[passType][i];
                
                if (!renderPass)
                {
                    renderPass = otherRenderPass->createCopy();
                    renderPass->setMaterial(this);
                }
                else
                {
                    renderPass->set(otherRenderPass);
                }
            }
        }
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, S32 _value)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_value);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, F32 _value)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_value);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, TexturePtr const& _texture)
    {
        MAZE_ERROR_RETURN_IF(!_texture, "Null texture");

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                setUniform(_uniformName, _texture->cast<Texture2D>());
                break;
            }
            case TextureType::Cube:
            {
                setUniform(_uniformName, _texture->cast<TextureCube>());
                break;
            }
        default:
            MAZE_NOT_IMPLEMENTED;
        }
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Texture2DPtr const& _texture2D)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_texture2D);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, TextureCubePtr const& _textureCube)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_textureCube);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec2F const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec3F const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec4F const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec2S const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec3S const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec4S const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec2U const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec3U const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Vec4U const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Mat3F const& _matrix)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_matrix);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, Mat4F const& _matrix)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_matrix);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, TMat const& _matrix)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_matrix);
    }

    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, ColorF128 const& _color)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_color);
    }


    //////////////////////////////////////////
    void Material::setUniform(HashedCString _uniformName, ShaderUniformVariant const& _shaderUniformVariant)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        *uniformVariant = _shaderUniformVariant;
    }
        
    //////////////////////////////////////////
    void Material::setUniform(ShaderUniformVariant const& _uniformVariant)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformVariant.getName());
        *uniformVariant = _uniformVariant;
    }

    //////////////////////////////////////////
    ShaderUniformVariantPtr const& Material::getUniform(HashedCString _uniformName)
    {
        static ShaderUniformVariantPtr const nullPointer;

        for (ShaderUniformVariantPtr const& uniform : m_uniforms)
            if (uniform->getName() == _uniformName)
                return uniform;
        
        return nullPointer;
    }

    //////////////////////////////////////////
    ShaderUniformVariantPtr const& Material::ensureUniform(HashedCString _uniformName, ShaderUniformType _type)
    {
        ShaderUniformVariantPtr const& uniform = getUniform(_uniformName);
        if (uniform)
            return uniform;

        m_uniforms.push_back(std::make_shared<ShaderUniformVariant>(m_renderSystem, _type, _uniformName));

        return m_uniforms.back();
    }

    //////////////////////////////////////////
    void Material::applyRenderPassUniforms(RenderPass* _renderPass)
    {
        ShaderPtr const& shader = _renderPass->getShader();
        MAZE_DEBUG_ERROR_RETURN_IF(!shader, "Render pass has no shader!");

        for (auto const& uniformVariantData : m_uniforms)
            shader->setUniform(*uniformVariantData.get());
    }

    //////////////////////////////////////////
    void Material::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        clear();

        XMLHelper::DeserializeMetaInstanceFromXMLElement(getMetaClass(), getMetaInstance(), _element);

        tinyxml2::XMLNode const* node = _element->FirstChild();
        while (node)
        {
            tinyxml2::XMLElement const* childElement = node->ToElement();
            if (!childElement)
            {
                node = node->NextSibling();
                continue;
            }

            if (strcmp(childElement->Name(), ClassInfo<RenderPass>::Name()) == 0)
            {
                RenderPassType renderPassType = RenderPassType::FromString(childElement->Attribute("passType"));
                RenderPassPtr renderPass = createRenderPass(renderPassType);
                renderPass->loadFromXMLElement(childElement);
            }
            else
            if (strcmp(childElement->Name(), ClassInfo<ShaderUniformVariant>::Name()) == 0)
            {
                ShaderUniformVariant shaderUniformVariant(m_renderSystem);
                shaderUniformVariant.loadFromXMLElement(childElement);

                setUniform(shaderUniformVariant);
            }

            node = node->NextSibling();
        }
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* Material::toXMLElement(tinyxml2::XMLDocument& _doc) const
    {
        tinyxml2::XMLElement* element = XMLHelper::SerializeMetaInstanceToXMLElement(getMetaClass(), getMetaInstance(), _doc);

        for (ShaderUniformVariantPtr const& uniformVariant : m_uniforms)
            element->InsertEndChild(uniformVariant->toXMLElement(_doc));

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
            for (RenderPassPtr const& renderPass : m_passes[passType])
            {
                element->InsertEndChild(renderPass->toXMLElement(_doc));
            }

        return element;
    }

    //////////////////////////////////////////
    bool Material::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        clear();

        DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);

        for (DataBlock::DataBlockIndex i = 0; i < _dataBlock.getDataBlocksCount(); ++i)
        {
            DataBlock const* subBlock = _dataBlock.getDataBlock(i);

            if (subBlock->getName() == MAZE_HCS("renderPass"))
            {
                RenderPassType renderPassType = RenderPassType::FromString(subBlock->getCString(MAZE_HCS("passType")));
                RenderPassPtr renderPass = createRenderPass(renderPassType);
                renderPass->loadFromDataBlock(*subBlock);
            }
            else
            if (subBlock->getName() == MAZE_HCS("uniform"))
            {
                ShaderUniformVariant shaderUniformVariant(m_renderSystem);
                shaderUniformVariant.loadFromDataBlock(*subBlock);
                setUniform(shaderUniformVariant);
            }
        }
        
        return true;
    }

    //////////////////////////////////////////
    void Material::toDataBlock(DataBlock& _dataBlock) const
    {
        _dataBlock.setS32(MAZE_HCS("_version"), c_materialDataBlockVersion);

        for (Size i = 0, in = m_uniforms.size(); i < in; ++i)
        {
            ShaderUniformVariantPtr const& uniformVariant = m_uniforms[i];
            if (uniformVariant)
                uniformVariant->toDataBlock(*_dataBlock.addNewDataBlock(MAZE_HCS("uniform")));
        }

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
        {
            for (RenderPassPtr const& renderPass : m_passes[passType])
            {
                if (renderPass)
                    renderPass->toDataBlock(*_dataBlock.addNewDataBlock(MAZE_HCS("renderPass")));
            }
        }
    }

    //////////////////////////////////////////
    String Material::toString() const
    {
        String result;
        ToString(this, result);
        return result;
    }

    //////////////////////////////////////////
    void Material::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }


    //////////////////////////////////////////
    bool Material::saveToFile(Path const& _path)
    {
        DataBlock dataBlock;
        toDataBlock(dataBlock);

        return DataBlockTextSerialization::SaveTextFile(dataBlock, _path);
    }

    //////////////////////////////////////////
    void Material::addUniform(ShaderUniformVariant const& _shaderUniformVariant)
    {
        setUniform(_shaderUniformVariant.getName(), _shaderUniformVariant);
    }

    //////////////////////////////////////////
    void Material::logUniforms()
    {
        Debug::log << "Uniforms:" << endl;
        for (ShaderUniformVariantPtr uniform : m_uniforms)
        {
            Debug::log << "- " << uniform << endl;
        }
    }

    //////////////////////////////////////////
    U32 Material::calculateUniformsCRC32(U32 _seed) const
    {
        U32 crc32 = _seed;

        for (ShaderUniformVariantPtr const& uniform : m_uniforms)
            crc32 = uniform->calculateCRC32(crc32);

        return crc32;            
    }

    //////////////////////////////////////////
    bool Material::hasUniform(HashedCString _uniformName)
    {
        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
            for (RenderPassPtr const& renderPass : m_passes[passType])
                if (renderPass && renderPass->hasUniform(_uniformName))
                    return true;

        return false;
    }

    //////////////////////////////////////////
    void Material::FromString(MaterialPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("Material::FromString");

        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = materialManager->getOrLoadMaterial(MAZE_HCS(_data));
        }
    }

    //////////////////////////////////////////
    void Material::ToString(Material const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("Material::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HashedCString materialName = materialManager->getMaterialName(_value);
        if (!materialName.empty())
        {
            _data = materialName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);            
        }
    }

    //////////////////////////////////////////
    void Material::IterateMaterials(std::function<bool(Material*)> _cb)
    {
        Material* instance = s_instancesList;
        while (instance)
        {
            if (!_cb(instance))
                break;

            instance = instance->m_instancesListPrev;
        }
    }


    //////////////////////////////////////////
    // Class MaterialAssetRef
    //
    //////////////////////////////////////////
    String MaterialAssetRef::toString() const
    {
        if (!m_material)
            return String();

        HashedCString name = RenderSystem::GetCurrentInstancePtr()->getMaterialManager()->getMaterialName(m_material.get());
        return !name.empty() ? name.str : m_material->getName();
    }

    //////////////////////////////////////////
    void MaterialAssetRef::setString(CString _data, Size _count)
    {
        MaterialPtr const& material = RenderSystem::GetCurrentInstancePtr()->getMaterialManager()->getOrLoadMaterial(_data);
        setMaterial(material);
    }

    //////////////////////////////////////////
    bool MaterialAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
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
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitMaterial>::UID())
                    {
                        setMaterial(assetUnit->castRaw<AssetUnitMaterial>()->loadMaterial(true));
                        return true;
                    }

                    break;
                }
                // by name
                case DataBlockParamType::ParamString:
                {
                    String const& name = _dataBlock.getString(paramIndex);
                    MaterialPtr const& material = RenderSystem::GetCurrentInstancePtr()->getMaterialManager()->getOrLoadMaterial(name);
                    setMaterial(material);
                    return true;
                }
                default:
                {
                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                    break;
                }
            }
        }

        setMaterial(MaterialPtr());
        return true;
    }

    //////////////////////////////////////////
    void MaterialAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_material)
        {
            _dataBlock.clearData();
            return;
        }

        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(HashedCString(m_material->getName().c_str()));
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitMaterial>::UID())
            {
                MaterialPtr const& assetUnitMaterial = assetUnit->castRaw<AssetUnitMaterial>()->getMaterial();
                if (assetUnitMaterial == m_material)
                {
                    // Save as AUID
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Save as string
        HashedCString name = RenderSystem::GetCurrentInstancePtr()->getMaterialManager()->getMaterialName(m_material.get());
        ValueToDataBlock(name.str, _dataBlock);
    }

} // namespace Maze
//////////////////////////////////////////
