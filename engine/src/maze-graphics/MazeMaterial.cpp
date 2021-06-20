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
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Material
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Material, Object,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, name, String(), getName, setName));

    //////////////////////////////////////////
    Material::Material()
        : m_renderSystem(nullptr)
    {
    }

    //////////////////////////////////////////
    Material::~Material()
    {
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
            material->loadFromAssetFile(_assetFile);

            String name = _assetFile->getFileName();
            StringHelper::RemoveSubstring(name, String(".mzmaterial"));
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
            material->loadFromAssetFile(_assetFileName);

            String name = _assetFileName;
            StringHelper::RemoveSubstring(name, String(".mzmaterial"));
            material->setName(name);
        }

        return material;
    }

    //////////////////////////////////////////
    MaterialPtr Material::Create(MaterialPtr const& _material)
    {
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
        static RenderPassPtr nullPointer;

        Vector<RenderPassPtr>& passesByType = m_passes[_passType];
        if (passesByType.empty() || _index >= passesByType.size())
            return nullPointer;

        return passesByType[_index];
    }

    //////////////////////////////////////////
    RenderPassPtr const& Material::getFirstRenderPass(RenderPassType _passType)
    {
        static RenderPassPtr nullPointer;

        Vector<RenderPassPtr>& passesByType = m_passes[_passType];
        if (passesByType.empty())
            return nullPointer;

        return passesByType.front();
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
    void Material::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();

        MAZE_ERROR_RETURN_IF(!rootNode, "File loading error - empty root node!");

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_IF(!rootNode, "File loading error - empty root node children!");

        tinyxml2::XMLElement const* rootElement = rootNode->ToElement();
        MAZE_ERROR_RETURN_IF(!rootElement, "File loading error - root node is not XML element!");

        loadFromXMLElement(rootElement);
    }

    //////////////////////////////////////////
    void Material::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        tinyxml2::XMLDocument doc;
        MAZE_LOG("Loading %s...", _assetFile->getFileName().c_str());
        _assetFile->readToXMLDocument(doc);
        loadFromXMLDocument(doc);
    }

    //////////////////////////////////////////
    void Material::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
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
    void Material::setUniform(String const& _uniformName, S32 _value)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_value);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, F32 _value)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_value);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Texture2DPtr const& _texture2D)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_texture2D);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, TextureCubePtr const& _textureCube)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_textureCube);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec2DF const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec3DF const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec4DF const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec2DS const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec3DS const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec4DS const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec2DU const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec3DU const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Vec4DU const& _vector)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_vector);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Mat3DF const& _matrix)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_matrix);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, Mat4DF const& _matrix)
    {
        ShaderUniformVariantPtr const& uniformVariant = ensureUniform(_uniformName);
        uniformVariant->set(_matrix);
    }

    //////////////////////////////////////////
    void Material::setUniform(String const& _uniformName, ShaderUniformVariant const& _shaderUniformVariant)
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
    ShaderUniformVariantPtr const& Material::getUniform(String const& _uniformName)
    {
        static ShaderUniformVariantPtr nullPointer;

        for (ShaderUniformVariantPtr const& uniform : m_uniforms)
            if (uniform->getName() == _uniformName)
                return uniform;
        
        return nullPointer;
    }

    //////////////////////////////////////////
    ShaderUniformVariantPtr const& Material::ensureUniform(String const& _uniformName, ShaderUniformType _type)
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
        for (auto const& uniformVariantData : m_uniforms)
        {
            ShaderUniformVariantPtr const& uniformVariant = uniformVariantData;
            ShaderPtr const& shader = _renderPass->getShader();
            MAZE_DEBUG_ERROR_IF(!shader, "Render pass has no shader!");

            shader->setUniform(*uniformVariant.get());
        }
    }

    //////////////////////////////////////////
    void Material::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        clearAllRenderPasses();
        clearAllUniforms();

        DeserializeMetaInstanceFromXMLElement(getMetaClass(), getMetaInstance(), _element);

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
        tinyxml2::XMLElement* element = SerializeMetaInstanceToXMLElement(getMetaClass(), getMetaInstance(), _doc);

        for (RenderPassType passType = RenderPassType(1); passType < RenderPassType::MAX; ++passType)
            for (RenderPassPtr const& renderPass : m_passes[passType])
            {
                element->InsertEndChild(renderPass->toXMLElement(_doc));
            }

        return element;
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
    void Material::FromString(MaterialPtr& _value, CString _data, Size _count)
    {
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

            void* ptr;
            std::stringstream ss;
            ss << std::hex << data;
            ss >> ptr;

            Material* material = reinterpret_cast<Material*>(ptr);
            if (material)
            {
                _value = material->getSharedPtr();
            }
            else
            {
                _value.reset();
            }
        }
        else
        {
            _value = materialManager->getMaterial(_data);
        }
    }

    //////////////////////////////////////////
    void Material::ToString(MaterialPtr const& _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        String const& materialName = materialManager->getMaterialName(_value);
        if (!materialName.empty())
        {
            _data = materialName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value.get());            
        }
    }

} // namespace Maze
//////////////////////////////////////////
