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
#include "maze-render-system-opengl-core/MazeShaderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ShaderSystemOpenGL
    //
    //////////////////////////////////////////
    ShaderSystemOpenGL::ShaderSystemOpenGL()
        : m_glslVersion(0)
    {

    }

    //////////////////////////////////////////
    ShaderSystemOpenGL::~ShaderSystemOpenGL()
    {
        if (m_contextOpenGL)
        {
            m_contextOpenGL->eventGLContextWillBeDestroyed.unsubscribe(this);
            m_contextOpenGL->eventGLContextSetup.unsubscribe(this);
            m_contextOpenGL.reset();
        }
    }

    //////////////////////////////////////////
    void ShaderSystemOpenGL::Initialize(ShaderSystemPtr& _object, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderSystemOpenGL, _object, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool ShaderSystemOpenGL::init(RenderSystemPtr const& _renderSystem)
    {
        if (!ShaderSystem::init(_renderSystem))
            return false;

        CString shadingLanguageVersion = (CString)mzglGetString(MAZE_GL_SHADING_LANGUAGE_VERSION);
        if (shadingLanguageVersion)
        {
            static String const numbers = "0123456789";
            String str = shadingLanguageVersion;
            String croppedGlslVersion;
            Size i = str.find_first_of(numbers);
            MAZE_ERROR_RETURN_VALUE_IF(i == -1, false, "Unrecognized GLSL version = '%s'", shadingLanguageVersion);
            bool dotFound = false;

            for (; i < str.size(); ++i)
            {
                S8 ch = str[i];

                if (ch == '.' && !dotFound)
                {
                    dotFound = true;
                    continue;
                }
                else
                if (numbers.find(ch) == String::npos)
                    break;
                
                croppedGlslVersion.push_back(ch);
            }

            m_glslVersion = MZGLint(StringHelper::StringToS32(croppedGlslVersion));
            if (m_glslVersion < 100)
                m_glslVersion *= 10;


            Debug::Log("GLSL version = '%s' (%d)", shadingLanguageVersion, m_glslVersion);
        }

        ContextOpenGL* context = getRenderSystemOpenGL()->ensureCurrentContext();

        MAZE_ERROR_RETURN_VALUE_IF(!context, false, "Context is null");

        m_contextOpenGL = context->getSharedPtr();
        m_contextOpenGL->eventGLContextWillBeDestroyed.subscribe(this, &ShaderSystemOpenGL::notifyGLContextWillBeDestroyed);
        m_contextOpenGL->eventGLContextSetup.subscribe(this, &ShaderSystemOpenGL::notifyGLContextSetup);

        return true;
    }

    //////////////////////////////////////////
    void ShaderSystemOpenGL::notifyGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_systemInited = false;
    }

    //////////////////////////////////////////
    void ShaderSystemOpenGL::notifyGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        // createBuiltinShaders();

        processSystemInited();
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystemOpenGL::createBuiltinShader(BuiltinShaderType _shaderType)
    {
        ShaderPtr& shader = m_builtinShaders[(Size)_shaderType];
        CString shaderSource = nullptr;

        switch (_shaderType)
        {
            case BuiltinShaderType::Error:
                shaderSource =
#include "shaders/MazeErrorShader.mzglsl"
                ; break;
            case BuiltinShaderType::UV:
                shaderSource =
#include "shaders/MazeUVShader.mzglsl"
                ; break;
            case BuiltinShaderType::Normal:
                shaderSource =
#include "shaders/MazeNormalShader.mzglsl"
                ; break;
            case BuiltinShaderType::Color:
                shaderSource =
#include "shaders/MazeColorShader.mzglsl"
                ; break;
            case BuiltinShaderType::ColorTexture:
                shaderSource =
#include "shaders/MazeColorTextureShader.mzglsl"
                ; break;
            case BuiltinShaderType::ColorTextureCustomUV:
                shaderSource =
#include "shaders/MazeColorTextureCustomUVShader.mzglsl"
                ; break;
            case BuiltinShaderType::ColorHDR:
                shaderSource =
#include "shaders/MazeColorHDRShader.mzglsl"
                ; break;
            case BuiltinShaderType::HSVRect:
                shaderSource =
#include "shaders/MazeHSVRectShader.mzglsl"
                ; break;
            case BuiltinShaderType::HSVBand:
                shaderSource =
#include "shaders/MazeHSVBandShader.mzglsl"
                ; break;
            case BuiltinShaderType::ColorPickerChannel:
                shaderSource =
#include "shaders/MazeColorPickerChannelShader.mzglsl"
                ; break;
            case BuiltinShaderType::DebugGrid:
                shaderSource =
#include "shaders/MazeDebugGridShader.mzglsl"
                ; break;
            case BuiltinShaderType::DebugAxis:
                shaderSource =
#include "shaders/MazeDebugAxisShader.mzglsl"
                ; break;
            case BuiltinShaderType::Skybox:
                shaderSource =
#include "shaders/MazeSkyboxShader.mzglsl"
                ; break;
            case BuiltinShaderType::Specular:
                shaderSource =
#include "shaders/MazeSpecularShader.mzglsl"
                ; break;
            case BuiltinShaderType::MeshPreview:
                shaderSource =
#include "shaders/MazeMeshPreviewShader.mzglsl"
                    ; break;
            case BuiltinShaderType::Font:
                shaderSource =
#include "shaders/MazeFontShader.mzglsl"
                    ; break;
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(!shaderSource, shader, "ShaderSource is null! ShaderType: %s", _shaderType.toCString());

        shader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
            shaderSource,
            _shaderType.toCString());

        return shader;
    }

    //////////////////////////////////////////
    RenderSystemOpenGL* ShaderSystemOpenGL::getRenderSystemOpenGL()
    {
        return m_renderSystemRaw->castRaw<RenderSystemOpenGL>();
    }

    //////////////////////////////////////////
    ShaderPtr ShaderSystemOpenGL::createShader(AssetFilePtr const& _shaderFile)
    {
        return ShaderOpenGL::CreateFromFile(
            getRenderSystem(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
            _shaderFile);
    }


} // namespace Maze
//////////////////////////////////////////
