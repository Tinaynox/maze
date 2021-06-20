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
        m_contextOpenGL->eventGLContextSetup.subscribe(this, &ShaderSystemOpenGL::notifyGLContextSetup);

        return true;
    }

    //////////////////////////////////////////
    void ShaderSystemOpenGL::notifyGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        m_errorShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeErrorShader.mzglsl"
        );
        m_errorShader->setName("error");

        m_uvShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeUVShader.mzglsl"
            );
        m_uvShader->setName("uv");

        m_normalShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeNormalShader.mzglsl"
        );
        m_normalShader->setName("normal");


        m_colorShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeColorShader.mzglsl"
            );
        m_colorShader->setName("color");

        m_colorTextureShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeColorTextureShader.mzglsl"
            );
        m_colorTextureShader->setName("color_texture");

        m_colorTextureCustomUVShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeColorTextureCustomUVShader.mzglsl"
            );
            m_colorTextureCustomUVShader->setName("color_texture_customUV");

            m_colorHDRShader = ShaderOpenGL::CreateFromSource(
                m_renderSystem.lock(),
                getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeColorHDRShader.mzglsl"
            );
            m_colorHDRShader->setName("color_hdr");

        m_hsvRectShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeHSVRectShader.mzglsl"
        );
        m_hsvRectShader->setName("hsv_rect");

        m_hsvBandShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeHSVBandShader.mzglsl"
        );
        m_hsvBandShader->setName("hsv_band");

        m_colorPickerChannelShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeColorPickerChannelShader.mzglsl"
        );
        m_colorPickerChannelShader->setName("color_picker_channel");

        m_debugGridShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeDebugGridShader.mzglsl"
        );
        m_debugGridShader->setName("debug_grid");

        m_debugAxisShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeDebugAxisShader.mzglsl"
        );
        m_debugAxisShader->setName("debug_axis");

        m_skyboxShader = ShaderOpenGL::CreateFromSource(
            m_renderSystem.lock(),
            getRenderSystemOpenGL()->ensureCurrentContext(),
#include "shaders/MazeSkyboxShader.mzglsl"
        );
        m_skyboxShader->setName("skybox");

        processSystemInited();
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
