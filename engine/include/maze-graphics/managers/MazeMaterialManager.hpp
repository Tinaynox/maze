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
#if (!defined(_MazeMaterialManager_hpp_))
#define _MazeMaterialManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Material);


    //////////////////////////////////////////
    // Class MaterialManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MaterialManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~MaterialManager();

        //////////////////////////////////////////
        static void Initialize(MaterialManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static MaterialManagerPtr const& GetCurrentInstance();


        //////////////////////////////////////////
        MaterialPtr const& getMaterial(String const& _assetFileName);

        //////////////////////////////////////////
        MaterialPtr const& getMaterial(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        String const& getMaterialName(Material const* _material);


        //////////////////////////////////////////
        MaterialPtr const& getErrorMaterial() const { return m_errorMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getUVMaterial() const { return m_uvMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getNormalMaterial() const { return m_normalMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getColorMaterial() const { return m_colorMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getColorTextureMaterial() const { return m_colorTextureMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getColorTextureCustomUVMaterial() const { return m_colorTextureCustomUVMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getColorHDRMaterial() const { return m_colorHDRMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getHSVRectMaterial() const { return m_hsvRectMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getHSVBandMaterial() const { return m_hsvBandMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getColorPickerChannelMaterial() const { return m_colorPickerChannelMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getDebugGridMaterial() const { return m_debugGridMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getDebugAxisMaterial() const { return m_debugAxisMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getSkyboxMaterial() const { return m_skyboxMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getSpecularMaterial() const { return m_specularMaterial; }
        

        //////////////////////////////////////////
        MaterialPtr const& addMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        Vector<MaterialPtr> getMaterialsSorted();


        //////////////////////////////////////////
        void loadAllAssetMaterials();

    public:
        //////////////////////////////////////////
        MultiDelegate<> eventSpecialMaterialsCreated;

    protected:

        //////////////////////////////////////////
        MaterialManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);
    
        //////////////////////////////////////////
        void notifyRenderSystemInited();

        //////////////////////////////////////////
        void notifyShaderSystemInited();

        //////////////////////////////////////////
        void createSpecialMaterials();

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        UnorderedMap<String, MaterialPtr> m_materialsByName;

        MaterialPtr m_errorMaterial;
        MaterialPtr m_uvMaterial;
        MaterialPtr m_normalMaterial;
        MaterialPtr m_colorMaterial;
        MaterialPtr m_colorTextureMaterial;
        MaterialPtr m_colorTextureCustomUVMaterial;
        MaterialPtr m_colorHDRMaterial;
        MaterialPtr m_hsvRectMaterial;
        MaterialPtr m_hsvBandMaterial;
        MaterialPtr m_colorPickerChannelMaterial;
        MaterialPtr m_debugGridMaterial;
        MaterialPtr m_debugAxisMaterial;
        MaterialPtr m_skyboxMaterial;
        MaterialPtr m_specularMaterial;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialManager_hpp_
//////////////////////////////////////////
