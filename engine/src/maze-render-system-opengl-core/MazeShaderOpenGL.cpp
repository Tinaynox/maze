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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformVariantOpenGL.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static String const c_commonShaderHeader =
#include "shaders/MazeCommonShaderHeader.mzglsl"
    ;

    //////////////////////////////////////////
    inline static MZGLenum ShaderTypeFromString(String const& _type)
    {
        if (_type == "vertex")
            return MAZE_GL_VERTEX_SHADER;
        if (_type == "fragment" || _type == "pixel")
            return MAZE_GL_FRAGMENT_SHADER;

        MAZE_ERROR("Unknown shader type!");
        return 0;
    }


    //////////////////////////////////////////
    // Class ShaderOpenGLScopeBind
    //
    //////////////////////////////////////////
    ShaderOpenGLScopeBind::ShaderOpenGLScopeBind(Shader* newShader)
        : m_context(nullptr)
    {
        ShaderOpenGL* shaderOpenGL = newShader->castRaw<ShaderOpenGL>();
        ContextOpenGL* context = shaderOpenGL->getContextOpenGL();
        if (context)
        {
            m_prevShader = context->getCurrentShader();

            if (m_prevShader != newShader)
            {
                context->setCurrentShader(newShader);
                m_context = context;
            }
        }
        else
        {
            MAZE_ERROR("Null context!");
        }
    }

    //////////////////////////////////////////
    ShaderOpenGLScopeBind::~ShaderOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->setCurrentShader(m_prevShader);
        }
    }


    //////////////////////////////////////////
    // Class ShaderOpenGL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderOpenGL, Shader);

    //////////////////////////////////////////
    ShaderOpenGL::ShaderOpenGL()
        : m_context(nullptr)
        , m_programId(0)
    {

    }

    //////////////////////////////////////////
    ShaderOpenGL::~ShaderOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    ShaderPtr ShaderOpenGL::CreateFromFile(
        RenderSystemPtr const& _renderSystem,
        ContextOpenGL* _contextOpenGL,
        AssetFilePtr const& _shaderFile)
    {
        ShaderOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderOpenGL, object, init(_renderSystem, _contextOpenGL, _shaderFile));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderOpenGL::CreateFromSource(
        RenderSystemPtr const& _renderSystem,
        ContextOpenGL* _contextOpenGL,
        String const& _shaderSource,
        CString _shaderName)
    {
        ShaderOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderOpenGL, object, init(_renderSystem, _contextOpenGL, _shaderSource, _shaderName));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderOpenGL::Create(ShaderOpenGLPtr const& _shader)
    {
        ShaderOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderOpenGL, object, init(_shader));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::init(
        RenderSystemPtr const& _renderSystem,
        ContextOpenGL* _contextOpenGL,
        AssetFilePtr const& _shaderFile)
    {
        if (!Shader::init(_renderSystem))
            return false;

        if (!_contextOpenGL)
            return false;

        if (!_contextOpenGL->isValid())
            return false;

        setContextOpenGL(_contextOpenGL);

        if (!loadFromAssetFile(_shaderFile))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::init(
        RenderSystemPtr const& _renderSystem,
        ContextOpenGL* _contextOpenGL,
        String const& _shaderSource,
        CString _shaderName)
    {
        if (!Shader::init(_renderSystem))
            return false;

        if (!_contextOpenGL)
            return false;

        if (!_contextOpenGL->isValid())
            return false;

        setContextOpenGL(_contextOpenGL);

        if (_shaderName)
            setName(_shaderName);

        if (!loadFromSource(_shaderSource))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::init(
        ShaderOpenGLPtr const& _shader)
    {
        if (!Shader::init(_shader))
            return false;

        setContextOpenGL(_shader->m_context);

        if (m_assetFile)
        {
            if (!loadFromAssetFile(m_assetFile))
                return false;
        }
        else
        if (!_shader->m_vertexShaderSource.empty() && !_shader->m_fragmentShaderSource.empty())
        {
            if (!loadFromSources(
                _shader->m_vertexShaderSource,
                _shader->m_fragmentShaderSource))
                return false;
        }

        for (auto const& uniformData : _shader->m_uniformsCache)
        {
            if (uniformData.second)
                setUniform(uniformData.second->getName(), uniformData.second->getValue());
        }

        return true;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderOpenGL::createCopy()
    {
        return ShaderOpenGL::Create(cast<ShaderOpenGL>());
    }

    //////////////////////////////////////////
    ShaderUniformPtr const& ShaderOpenGL::createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type)
    {
        static ShaderUniformPtr nullPointer;

        UnorderedMap<U32, ShaderUniformPtr>::const_iterator it = m_uniformsCache.find(_uniformName.hash);
        if (it != m_uniformsCache.end())
            return it->second;

        if (!m_programId)
        {
            return nullPointer;
        }

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        
        ShaderOpenGLScopeBind scopeBind(this);
        MZGLint uniformLocation = 0;
        MAZE_GL_CALL(uniformLocation = mzglGetUniformLocation(m_programId, _uniformName.str));

        if (uniformLocation < 0)
        {
            // Mark with empty pointer
            m_uniformsCache.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_uniformName.hash),
                std::forward_as_tuple(nullptr));
            return nullPointer;
        }

        ShaderUniformOpenGLPtr newUniform = std::static_pointer_cast<ShaderUniformOpenGL>(ShaderUniform::Create(getSharedPtr(), _type));
        MAZE_ERROR_RETURN_VALUE_IF(!newUniform, nullPointer, "Shader Uniform creation error!");
        newUniform->setLocation(uniformLocation);
        newUniform->setName(_uniformName);
        auto at = m_uniformsCache.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_uniformName.hash),
            std::forward_as_tuple(newUniform));
        if (at.second)
            return at.first->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::isValid()
    {
        return (m_programId != 0);
    }

    //////////////////////////////////////////
    RenderSystemOpenGL* ShaderOpenGL::getRenderSystemOpenGLRaw()
    {
        return m_renderSystemRaw->castRaw<RenderSystemOpenGL>();
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::loadFromSource(String const& _shaderSource)
    {
        auto shaderSources = preProcessSource(_shaderSource);

        return loadFromSources(shaderSources[MAZE_GL_VERTEX_SHADER], shaderSources[MAZE_GL_FRAGMENT_SHADER]);
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        if (loadGLShader(_vertexShaderSource, _fragmentShaderSource))
            return true;

        return false;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::loadGLShader(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_vertexShaderSource.empty(), false, "Vertex Shader Source is empty!");
        MAZE_ERROR_RETURN_VALUE_IF(_fragmentShaderSource.empty(), false, "Fragment Shader Source is empty!");

        m_vertexShaderSource = _vertexShaderSource;
        m_fragmentShaderSource = _fragmentShaderSource;

        RenderSystemOpenGL* renderSystem = getRenderSystemOpenGLRaw();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

        Debug::Log("Shader %s loading...", getName().c_str());

        ContextOpenGL* currentContext = renderSystem->ensureCurrentContext();
        MAZE_ERROR_RETURN_VALUE_IF(!currentContext, false, "Context is not valid!");

        unloadGLShader();
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        MAZE_GL_CALL(m_programId = mzglCreateProgram());

        MZGLuint vertexShaderId = 0;
        MZGLuint fragmentShaderId = 0;



        String version;
        
        /*
        if (currentContext->getConfig().flags & ContextOpenGLFlags::EmbeddedSystems)
        {
            version = StringHelper::ToString(300); // <- #TODO:
        }
        else
        {
            version = StringHelper::ToString(330); // <- #TODO:
        }
        */
        version = StringHelper::ToString(shaderSystem->castRaw<ShaderSystemOpenGL>()->getGLSLVersion());

        String shaderVersion = "#version " + version;
        
        if (currentContext->getConfig().flags & ContextOpenGLFlags::EmbeddedSystems)
        {
            if (version != "100")
                shaderVersion += " es";
        }

        // Defines
        shaderVersion += "\n#define MAZE_GLSL_VERSION " + version;

        shaderVersion += "\n#define MAZE_GL_CORE_PROFILE ";
        shaderVersion += currentContext->getConfig().flags & ContextOpenGLFlags::Core ? "(1)" : "(0)";

        shaderVersion += "\n#define MAZE_MODEL_MATRICES_VIA_ARRAY ";
        shaderVersion += (currentContext->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformArray) ? "(1)" : "(0)";

        shaderVersion += "\n#define MAZE_MODEL_MATRICES_VIA_TEXTURE ";
        shaderVersion += (currentContext->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformTexture) ? "(1)" : "(0)";

        shaderVersion += "\n#define MAZE_INSTANCE_STREAM_VIA_ARRAY ";
        shaderVersion += (currentContext->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformArray) ? "(1)" : "(0)";

        shaderVersion += "\n#define MAZE_INSTANCE_STREAM_VIA_TEXTURE ";
        shaderVersion += (currentContext->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformTexture) ? "(1)" : "(0)";


        String localShaderFeatures = buildLocalShaderFeatures();


        String vertexExtensionFeatures;
        if (currentContext->getExtensionsRaw()->getSupportClipDistance())
        {
            vertexExtensionFeatures += "#define MAZE_CLIP_DISTANCE_VERTEX(_pos) gl_ClipDistance[0] = dot(_pos, u_clipDistance0); \n";
        }
        else
        {
            vertexExtensionFeatures += "#define MAZE_CLIP_DISTANCE_VERTEX(_pos) v_clipDistance0 = dot(positionWS, u_clipDistance0); \n";
        }

        String vertexShaderBody = makeInternalShaderPreprocessing(_vertexShaderSource);
        String completeVertexShader =
            shaderVersion + '\n' +
            c_commonShaderHeader + '\n' +
            vertexExtensionFeatures + '\n' +
            localShaderFeatures + '\n' +
            buildMissingShaderDefines(vertexShaderBody) + '\n' +
            vertexShaderBody;

        
        


        if (!compileGLShader(vertexShaderId, MAZE_GL_VERTEX_SHADER, completeVertexShader.c_str()))
        {
            Debug::LogError("Vertex shader compilation error!");
            
            if (m_assetFile)
            {
                Debug::LogError("File: %s", m_assetFile->getFileName().toUTF8().c_str());
            }

            Vector<String> words;
            StringHelper::SplitWords(completeVertexShader, words, '\n');
            for (Size i = 0; i < words.size(); ++i)
                Debug::LogError("[%d]%s", i, words[i].c_str());

            MAZE_FATAL("Failed to compile the vertex shader");

            vertexShaderId = 0;
            return false;
        }


        String fragmentExtensionFeatures;
        if (currentContext->getExtensionsRaw()->getSupportClipDistance())
        {
            fragmentExtensionFeatures += "#define MAZE_CLIP_DISTANCE_FRAGMENT \n";
        }
        else
        {
            fragmentExtensionFeatures += "#define MAZE_CLIP_DISTANCE_FRAGMENT if (u_clipDistanceEnable[0] && v_clipDistance0 < 0.0) discard; \n";
        }

        String fragmentShaderBody = makeInternalShaderPreprocessing(_fragmentShaderSource);
        String completeFragmentShader = 
            shaderVersion + '\n' +
            c_commonShaderHeader + '\n' +
            fragmentExtensionFeatures + '\n' +
            localShaderFeatures + '\n' +
            buildMissingShaderDefines(fragmentShaderBody) + '\n' +
            fragmentShaderBody;

         if (!compileGLShader(fragmentShaderId, MAZE_GL_FRAGMENT_SHADER, completeFragmentShader.c_str())) 
        {
             Debug::LogError("Fragment shader compilation error!");
            if (m_assetFile)
            {
                Debug::LogError("File: %s", m_assetFile->getFileName().toUTF8().c_str());
            }

            Vector<String> words;
            StringHelper::SplitWords(completeFragmentShader, words, '\n');
            for (Size i = 0; i < words.size(); ++i)
                Debug::LogError("[%d]%s", i, words[i].c_str());

            MAZE_FATAL("Failed to compile the fragment shader");

            fragmentShaderId = 0;
            return false;
        }

        if (vertexShaderId && fragmentShaderId) 
        {
            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
            MAZE_GL_CALL(mzglAttachShader(m_programId, vertexShaderId));
            MAZE_GL_CALL(mzglAttachShader(m_programId, fragmentShaderId));
        }
        else
        {
            MAZE_ERROR("Cannot create shader!");
            return false;
        }
        
        
        // Pre Linking (For attribute locations check)
        MAZE_GL_CALL(mzglLinkProgram(m_programId));

        for (VertexAttributeSemantic attribSemantic = VertexAttributeSemantic(0);
                                     attribSemantic < VertexAttributeSemantic::MAX;
                                     attribSemantic = VertexAttributeSemantic((S32)attribSemantic + 1))
        {
            CString attribName = GetVertexAttributeName(attribSemantic); 
            
            MZGLint attribLocation;
            MAZE_GL_CALL(attribLocation = mzglGetAttribLocation(m_programId, attribName));
            if (attribLocation == -1)
                continue;
            
            MAZE_GL_CALL(mzglBindAttribLocation(m_programId,
                (MZGLuint)attribSemantic,
                attribName));
        }
        
        // Final Linking
        MAZE_GL_CALL(mzglLinkProgram(m_programId));
                

        MZGLint status;
        MAZE_GL_CALL(mzglGetProgramiv(m_programId, MAZE_GL_LINK_STATUS, &status));
        if (status == MAZE_GL_FALSE) 
        {
            String log = logForOpenGLProgram(m_programId);
            
            {
                if (m_assetFile)
                {
                    Debug::LogError("File: %s", m_assetFile->getFileName().toUTF8().c_str());
                }
                else
                {
                    Debug::LogError("Vertex Shader:");
                }
                
                Vector<String> words;
                StringHelper::SplitWords(completeVertexShader, words, '\n');
                for (Size i = 0; i < words.size(); ++i)
                    Debug::LogError("[%d]%s", i, words[i].c_str());
            }
            {
                if (m_assetFile)
                    Debug::LogError("File: %s", m_assetFile->getFileName().toUTF8().c_str());
                else
                    Debug::LogError("Fragment Shader:");
                
                Vector<String> words;
                StringHelper::SplitWords(completeFragmentShader, words, '\n');
                for (Size i = 0; i < words.size(); ++i)
                {
                    Debug::LogError("[%d]%s", i, words[i].c_str());
                }
            }
            
            MAZE_FATAL("Failed to link program: %i\n%s", m_programId, log.c_str());

            unloadGLShader();
            return false;
        }
        else
        {
            String log = logForOpenGLProgram(m_programId);
            static String const noErrors = "No errors.";
            
            if (log.size() > 1 && log.find(noErrors) == String::npos)
            {
                Vector<String> logStrings;
                StringHelper::SplitWords(log, logStrings, '\n');
                log.clear();
                for (String const& logString : logStrings)
                {   
                    if (logString.empty())
                        continue;
                    
                    if (logString.find("WARNING:") != String::npos)
                    {
                        static CString const notReadWarningText = "not read by fragment shader";
                        if (logString.find(notReadWarningText) != String::npos)
                        {
                            continue;
                        }
                    }
                    
                    log += logString;
                }
                
                if (!log.empty())
                {
                    MAZE_WARNING("Warning while link program %i:\n%s", m_programId, log.c_str());
                }
            }
        }
        
        if (vertexShaderId)
        {
            MAZE_GL_CALL(mzglDeleteShader(vertexShaderId));
            vertexShaderId = 0;
        }

        if (fragmentShaderId)
        {
            MAZE_GL_CALL(mzglDeleteShader(fragmentShaderId));
            fragmentShaderId = 0;
        }

        assignUniforms();
        assignDefaultUniforms();

        Debug::Log("Shader %s loaded.", getName().c_str());

        return true;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::unloadGLShader()
    {
        RenderSystemOpenGL* renderSystem = getRenderSystemOpenGLRaw();
        if (!renderSystem)
            return false;

        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        if (!shaderSystem) 
            return false;

        ContextOpenGL* currentContext = renderSystem->ensureCurrentContext();
        MAZE_ERROR_RETURN_VALUE_IF(!currentContext, false, "Context is not valid!");

        if (m_programId)
        {
            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
            MAZE_GL_CALL(mzglDeleteProgram(m_programId));
            m_programId = 0;
        }

        clearUniformsCache();

        return true;
    }

    //////////////////////////////////////////
    String ShaderOpenGL::makeInternalShaderPreprocessing(String _shader)
    {
        Size index = 0;
        do
        {
            index = _shader.find("#include");
            if (index != String::npos)
            {
                Size endOfLineIndex = _shader.find_first_of('\n', index);
                MAZE_DEBUG_BP_IF(endOfLineIndex == String::npos);
                Size symbols = endOfLineIndex - index;
                String line = _shader.substr(index, symbols);

                String fileName = line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - line.find_first_of('"') - 1);
                Size fileNameLastSlashIndex = fileName.find_last_of('/');
                if (fileNameLastSlashIndex != String::npos)
                    fileName = fileName.substr(fileNameLastSlashIndex + 1, fileName.size() - fileNameLastSlashIndex - 1);
                
                _shader.erase(index, symbols);

                String fileText;

                AssetFilePtr const& shaderFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(fileName);

                if (shaderFile)
                    shaderFile->readToString(fileText);

                if (fileText.size())
                    _shader.insert(index, fileText);
                else
                {
                    MAZE_ERROR("Undefined shader file include: %s!", fileName.c_str());
                }
            }
            
        }
        while (index != String::npos);

        return _shader;
    }

    //////////////////////////////////////////
    bool ShaderOpenGL::compileGLShader(MZGLuint& _shader, MZGLenum _type, MZGLchar const* _source)
    {
        MZGLint status;
 
        if (!_source)
            return false;

        
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

        MAZE_GL_CALL(_shader = mzglCreateShader(_type));
        MAZE_GL_CALL(mzglShaderSource(_shader, 1, &_source, nullptr));
        if (!_shader)
            return false;

        MAZE_GL_CALL(mzglCompileShader(_shader));
        MAZE_GL_CALL(mzglGetShaderiv(_shader, MAZE_GL_COMPILE_STATUS, &status));
    

        if (!status) 
        {
            if (_type == MAZE_GL_VERTEX_SHADER)
            {
                Debug::LogError("Vertex Shader: %s", logForOpenGLShader(_shader).c_str());
                MAZE_DEBUG_BP;
            }
            else
            {
                Debug::LogError("Fragment Shader: %s", logForOpenGLShader(_shader).c_str());
                MAZE_DEBUG_BP;
            }
        }

        return(status == MAZE_GL_TRUE);
    }

    //////////////////////////////////////////
    String ShaderOpenGL::logForOpenGLShader(MZGLuint _shaderId)
    {
        MZGLsizei logLength = 0;
        MZGLsizei charsWritten = 0;

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        MAZE_GL_CALL(mzglGetShaderiv(_shaderId, MAZE_GL_INFO_LOG_LENGTH, &logLength));
        if (logLength < 1)
            return String();

        MZGLchar* logBytes = (MZGLchar*)malloc(logLength);
        MAZE_GL_CALL(mzglGetShaderInfoLog(_shaderId, logLength, &charsWritten, logBytes));

        String text = logBytes;

        free(logBytes);

        return text;
    }

    //////////////////////////////////////////
    String ShaderOpenGL::logForOpenGLProgram(MZGLuint _programId)
    {
        MZGLsizei logLength = 0;
        MZGLsizei charsWritten = 0;

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        MAZE_GL_CALL(mzglGetProgramiv(_programId, MAZE_GL_INFO_LOG_LENGTH, &logLength));
        if (logLength < 1)
        {
            return String();
        }

        MZGLchar* logBytes = (MZGLchar*)malloc(logLength);
        MAZE_GL_CALL(mzglGetProgramInfoLog(_programId, logLength, &charsWritten, logBytes));

        String text = logBytes;

        free(logBytes);

        return text;
    }

    //////////////////////////////////////////
    void ShaderOpenGL::assignUniformTextureIndexes()
    {
        S32 textureIndex = 0;

        for (auto& uniformCache : m_uniformsCache)
        {
            ShaderUniformPtr const& uniform = uniformCache.second;

            if (!uniform)
                continue;

            if (uniform->getType() == ShaderUniformType::UniformTexture2D ||
                uniform->getType() == ShaderUniformType::UniformTextureCube)
            {
                uniform->castRaw<ShaderUniformOpenGL>()->setTextureIndex(textureIndex);
                ++textureIndex;
            }
            else
            if (uniform->getType() == ShaderUniformType::UniformTexture2DArray)
            {
                FastVector<MZGLint> indices;
                indices.resize(uniform->getCount());
                for (S32 i = 0; i < (S32)uniform->getCount(); ++i)
                    indices[i] = textureIndex + i;

                uniform->castRaw<ShaderUniformOpenGL>()->setTextureIndices(&indices[0], uniform->getCount());
                textureIndex += uniform->getCount();
            }

        }
    }

    //////////////////////////////////////////
    void ShaderOpenGL::bindTextures()
    {
        for (auto& uniformCache : m_uniformsCache)
        {
            ShaderUniformPtr const& uniform = uniformCache.second;

            if (!uniform)
                continue;

            if (uniform->getType() == ShaderUniformType::UniformTexture2D)
            {
                Maze::ContextOpenGLScopeBind contextOpenGLScopedLock(m_context);
                
                MZGLint textureIndex = uniform->castRaw<ShaderUniformOpenGL>()->getTextureIndex();

                m_context->activeTexture(textureIndex);
                m_context->bindTexture2D(uniform->getTexture2D());
            }
            else
            if (uniform->getType() == ShaderUniformType::UniformTextureCube)
            {
                Maze::ContextOpenGLScopeBind contextOpenGLScopedLock(m_context);

                MZGLint textureIndex = uniform->castRaw<ShaderUniformOpenGL>()->getTextureIndex();

                m_context->activeTexture(textureIndex);
                m_context->bindTextureCube(uniform->getTextureCube());
            }
            else
            if (uniform->getType() == ShaderUniformType::UniformTexture2DArray)
            {
                Maze::ContextOpenGLScopeBind contextOpenGLScopedLock(m_context);

                MZGLint textureIndex = uniform->castRaw<ShaderUniformOpenGL>()->getTextureIndex();

                for (U32 i = 0; i < uniform->getCount(); ++i)
                {
                    m_context->activeTexture(textureIndex + (S32)i);
                    m_context->bindTexture2D(*((Texture2D**)uniform->getPtr() + i));
                }
            }
        }
    }

    //////////////////////////////////////////
    void ShaderOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            m_context->eventDestroyed.unsubscribe(this, &ShaderOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &ShaderOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &ShaderOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &ShaderOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &ShaderOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &ShaderOpenGL::notifyContextOpenGLContextSetup);
        }
    }

    //////////////////////////////////////////
    void ShaderOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;
        m_programId = 0;

        m_uniformsCache.clear();
        resetDefaultUniforms();
    }

    //////////////////////////////////////////
    void ShaderOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        cacheUniformVariants();

        m_programId = 0;        

        m_uniformsCache.clear();
        resetDefaultUniforms();
    }

    //////////////////////////////////////////
    void ShaderOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        if (m_programId == 0)
        {
            if (m_assetFile)
            {
                loadFromAssetFile(m_assetFile);
                applyCachedUniformVariants();
            }
            else
            if (   !m_vertexShaderSource.empty()
                && !m_fragmentShaderSource.empty())
            {
                loadGLShader(
                    m_vertexShaderSource,
                    m_fragmentShaderSource);
                applyCachedUniformVariants();
            }
        }
    }

    //////////////////////////////////////////
    UnorderedMap<MZGLenum, String> ShaderOpenGL::preProcessSource(String const& _source)
    {
        UnorderedMap<MZGLenum, String> shaderSources;

        CString typeToken = "#type";
        Size typeTokenLength = strlen(typeToken);
        Size pos = _source.find(typeToken, 0);
        while (pos != String::npos)
        {
            Size eol = _source.find_first_of("\r\n", pos);
            MAZE_ERROR_IF(eol == String::npos, "Syntax error");
            Size begin = pos + typeTokenLength + 1;
            String type = _source.substr(begin, eol - begin);
            MAZE_ERROR_IF(!ShaderTypeFromString(type), "Invalid shader type specified");

            Size nextLinePos = _source.find_first_not_of("\r\n", eol); 
            MAZE_ERROR_IF(nextLinePos == String::npos, "Syntax error");
            pos = _source.find(typeToken, nextLinePos);

            shaderSources[ShaderTypeFromString(type)] = (pos == String::npos) ? _source.substr(nextLinePos) : _source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    //////////////////////////////////////////
    String ShaderOpenGL::buildLocalShaderFeatures()
    {
        String result;

        for (auto const& localFeatureData : m_localFeatures)
            result += (String)"#define " + localFeatureData.first + " " + localFeatureData.second + '\n';

        return result;
    }

    //////////////////////////////////////////
    String ShaderOpenGL::buildMissingShaderDefines(String const _shaderText)
    {
        Set<String> foundDefines;
        Size pos = 0u;

        String const definedPattern = "defined(";

        while ((pos = _shaderText.find(definedPattern, pos)) != std::string::npos)
        {
            pos += definedPattern.length();

            Size endPos = _shaderText.find(")", pos);
            if (endPos != String::npos) {
                String definedValue = _shaderText.substr(pos, endPos - pos);
                foundDefines.insert(definedValue);
                pos = endPos + 1;
            }
            else {
                break;
            }
        }

        String result;

        for (String const& foundDefine : foundDefines)
        {
            if (_shaderText.find(" (" + foundDefine + ")") != _shaderText.npos)
                result +=
                    "#if !defined(" + foundDefine + ") \n"
                    "#define " + foundDefine + " (0)\n"
                    "#endif\n";
        }

        return result;
    }

    //////////////////////////////////////////
    void ShaderOpenGL::recompile()
    {
        cacheUniformVariants();

        loadGLShader(
            m_vertexShaderSource,
            m_fragmentShaderSource);

        applyCachedUniformVariants();
    }

    //////////////////////////////////////////
    void ShaderOpenGL::assignUniforms()
    {
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        ShaderOpenGLScopeBind scopeBind(this);

        MZGLint count;
        mzglGetProgramiv(m_programId, MAZE_GL_ACTIVE_UNIFORMS, &count);

        MZGLint size;
        MZGLenum type;
        MZGLsizei const bufSize = 128;
        MZGLchar name[bufSize];
        MZGLsizei length;
        for (S32 i = 0; i < count; i++)
        {
            mzglGetActiveUniform(m_programId, (MZGLuint)i, bufSize, &length, &size, &type, name);
            ShaderUniformType uniformType = GetShaderUniformTypeFromOpenGL(type);
            ensureUniform(name, uniformType);
        }
    }

} // namespace Maze
//////////////////////////////////////////
