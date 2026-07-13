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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeShaderManagerDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ShaderManagerDX11
    //
    //////////////////////////////////////////
    ShaderManagerDX11::ShaderManagerDX11()
    {

    }

    //////////////////////////////////////////
    ShaderManagerDX11::~ShaderManagerDX11()
    {

    }

    //////////////////////////////////////////
    void ShaderManagerDX11::Initialize(ShaderManagerPtr& _object, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderManagerDX11, _object, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool ShaderManagerDX11::init(RenderSystemPtr const& _renderSystem)
    {
        if (!ShaderManager::init(_renderSystem))
            return false;

        processSystemInited();

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* ShaderManagerDX11::getRenderSystemDX11()
    {
        return m_renderSystemRaw->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderManagerDX11::createBuiltinShader(BuiltinShaderType _shaderType)
    {
        ShaderPtr& shader = m_builtinShaders[(Size)_shaderType];
        CString shaderSource = nullptr;

        switch (_shaderType)
        {
            case BuiltinShaderType::Error:
                shaderSource =
#include "shaders/MazeErrorShader.mzhlsl"
                ; break;
            case BuiltinShaderType::UV:
                shaderSource =
#include "shaders/MazeUVShader.mzhlsl"
                ; break;
            case BuiltinShaderType::Normal:
                shaderSource =
#include "shaders/MazeNormalShader.mzhlsl"
                ; break;
            case BuiltinShaderType::Color:
                shaderSource =
#include "shaders/MazeColorShader.mzhlsl"
                ; break;
            case BuiltinShaderType::ColorTexture:
                shaderSource =
#include "shaders/MazeColorTextureShader.mzhlsl"
                ; break;
            case BuiltinShaderType::ColorTextureCustomUV:
                shaderSource =
#include "shaders/MazeColorTextureCustomUVShader.mzhlsl"
                ; break;
            case BuiltinShaderType::ColorHDR:
                shaderSource =
#include "shaders/MazeColorHDRShader.mzhlsl"
                ; break;
            case BuiltinShaderType::HSVRect:
                shaderSource =
#include "shaders/MazeHSVRectShader.mzhlsl"
                ; break;
            case BuiltinShaderType::HSVBand:
                shaderSource =
#include "shaders/MazeHSVBandShader.mzhlsl"
                ; break;
            case BuiltinShaderType::ColorPickerChannel:
                shaderSource =
#include "shaders/MazeColorPickerChannelShader.mzhlsl"
                ; break;
            case BuiltinShaderType::DebugGrid:
                shaderSource =
#include "shaders/MazeDebugGridShader.mzhlsl"
                ; break;
            case BuiltinShaderType::DebugAxis:
                shaderSource =
#include "shaders/MazeDebugAxisShader.mzhlsl"
                ; break;
            case BuiltinShaderType::Skybox:
                shaderSource =
#include "shaders/MazeSkyboxShader.mzhlsl"
                ; break;
            case BuiltinShaderType::Specular:
                shaderSource =
#include "shaders/MazeSpecularShader.mzhlsl"
                ; break;
            case BuiltinShaderType::MeshPreview:
                shaderSource =
#include "shaders/MazeMeshPreviewShader.mzhlsl"
                    ; break;
            case BuiltinShaderType::Font:
                shaderSource =
#include "shaders/MazeFontShader.mzhlsl"
                    ; break;
            case BuiltinShaderType::ShadowCaster:
                shaderSource =
#include "shaders/MazeShadowCasterShader.mzhlsl"
                    ; break;
            case BuiltinShaderType::Outline:
                shaderSource =
#include "shaders/MazeOutlineShader.mzhlsl"
                    ; break;
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(!shaderSource, shader, "ShaderSource is null! ShaderType: %s", _shaderType.toCString());

        shader = ShaderDX11::CreateFromSource(
            m_renderSystem.lock(),
            shaderSource,
            _shaderType.toCString());

        return shader;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderManagerDX11::createShader()
    {
        return ShaderDX11::Create(getRenderSystem());
    }

    //////////////////////////////////////////
    ShaderPtr ShaderManagerDX11::createShader(AssetFilePtr const& _shaderFile)
    {
        return ShaderDX11::CreateFromFile(getRenderSystem(), _shaderFile);
    }

    //////////////////////////////////////////
    void ShaderManagerDX11::reloadShaders()
    {
        // Dev-time shader reload - drop cached #include contents
        // so edited shared header files are picked up
        ShaderDX11::ClearShaderIncludeFilesCache();

        ShaderManager::reloadShaders();
    }


} // namespace Maze
//////////////////////////////////////////
