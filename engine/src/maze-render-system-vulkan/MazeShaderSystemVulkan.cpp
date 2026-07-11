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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeShaderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ShaderSystemVulkan
    //
    //////////////////////////////////////////
    ShaderSystemVulkan::ShaderSystemVulkan()
    {

    }

    //////////////////////////////////////////
    ShaderSystemVulkan::~ShaderSystemVulkan()
    {

    }

    //////////////////////////////////////////
    void ShaderSystemVulkan::Initialize(ShaderSystemPtr& _object, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderSystemVulkan, _object, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool ShaderSystemVulkan::init(RenderSystemPtr const& _renderSystem)
    {
        if (!ShaderSystem::init(_renderSystem))
            return false;

        processSystemInited();

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* ShaderSystemVulkan::getRenderSystemVulkan()
    {
        return m_renderSystemRaw->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystemVulkan::createBuiltinShader(BuiltinShaderType _shaderType)
    {
        ShaderPtr& shader = m_builtinShaders[(Size)_shaderType];
        CString shaderSource = nullptr;

        switch (_shaderType)
        {
            case BuiltinShaderType::Error:
                shaderSource =
#include "shaders/MazeErrorShader.mzglslvk"
                ; break;
            case BuiltinShaderType::UV:
                shaderSource =
#include "shaders/MazeUVShader.mzglslvk"
                ; break;
            case BuiltinShaderType::Normal:
                shaderSource =
#include "shaders/MazeNormalShader.mzglslvk"
                ; break;
            case BuiltinShaderType::Color:
                shaderSource =
#include "shaders/MazeColorShader.mzglslvk"
                ; break;
            case BuiltinShaderType::ColorTexture:
                shaderSource =
#include "shaders/MazeColorTextureShader.mzglslvk"
                ; break;
            case BuiltinShaderType::ColorTextureCustomUV:
                shaderSource =
#include "shaders/MazeColorTextureCustomUVShader.mzglslvk"
                ; break;
            case BuiltinShaderType::ColorHDR:
                shaderSource =
#include "shaders/MazeColorHDRShader.mzglslvk"
                ; break;
            case BuiltinShaderType::HSVRect:
                shaderSource =
#include "shaders/MazeHSVRectShader.mzglslvk"
                ; break;
            case BuiltinShaderType::HSVBand:
                shaderSource =
#include "shaders/MazeHSVBandShader.mzglslvk"
                ; break;
            case BuiltinShaderType::ColorPickerChannel:
                shaderSource =
#include "shaders/MazeColorPickerChannelShader.mzglslvk"
                ; break;
            case BuiltinShaderType::DebugGrid:
                shaderSource =
#include "shaders/MazeDebugGridShader.mzglslvk"
                ; break;
            case BuiltinShaderType::DebugAxis:
                shaderSource =
#include "shaders/MazeDebugAxisShader.mzglslvk"
                ; break;
            case BuiltinShaderType::Skybox:
                shaderSource =
#include "shaders/MazeSkyboxShader.mzglslvk"
                ; break;
            case BuiltinShaderType::Specular:
                shaderSource =
#include "shaders/MazeSpecularShader.mzglslvk"
                ; break;
            case BuiltinShaderType::MeshPreview:
                shaderSource =
#include "shaders/MazeMeshPreviewShader.mzglslvk"
                    ; break;
            case BuiltinShaderType::Font:
                shaderSource =
#include "shaders/MazeFontShader.mzglslvk"
                    ; break;
            case BuiltinShaderType::ShadowCaster:
                shaderSource =
#include "shaders/MazeShadowCasterShader.mzglslvk"
                    ; break;
            case BuiltinShaderType::Outline:
                shaderSource =
#include "shaders/MazeOutlineShader.mzglslvk"
                    ; break;
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(!shaderSource, shader, "ShaderSource is null! ShaderType: %s", _shaderType.toCString());

        shader = ShaderVulkan::CreateFromSource(
            m_renderSystem.lock(),
            shaderSource,
            _shaderType.toCString());

        return shader;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderSystemVulkan::createShader()
    {
        return ShaderVulkan::Create(getRenderSystem());
    }

    //////////////////////////////////////////
    ShaderPtr ShaderSystemVulkan::createShader(AssetFilePtr const& _shaderFile)
    {
        return ShaderVulkan::CreateFromFile(getRenderSystem(), _shaderFile);
    }

    //////////////////////////////////////////
    void ShaderSystemVulkan::reloadShaders()
    {
        // Dev-time shader reload - drop cached #include contents
        // so edited shared header files are picked up
        ShaderVulkan::ClearShaderIncludeFilesCache();

        ShaderSystem::reloadShaders();
    }


} // namespace Maze
//////////////////////////////////////////
