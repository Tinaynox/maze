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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerDefault.hpp"
#include "maze-particles/inspectors/entities/MazeComponentEditorParticleSystem3D.hpp"
#include "maze-particles/property-drawers/MazePropertyDrawerParticleSystemParameterF32.hpp"
#include "maze-particles/property-drawers/MazePropertyDrawerParticleSystemParameterColor.hpp"
#include "maze-particles/property-drawers/MazePropertyDrawerParticleSystemBurst.hpp"
#include "maze-particles/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DMainModule.hpp"
#include "maze-particles/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DRendererModule.hpp"
#include "maze-particles/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DShapeModule.hpp"
#include "maze-particles/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DZoneData.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32 =
        MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32);
    template class MAZE_PARTICLES_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32Positive =
        MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32Positive>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Positive, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Positive);
    template class MAZE_PARTICLES_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32Positive>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32Degrees =
        MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32Degrees>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Degrees, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Degrees);
    template class MAZE_PARTICLES_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32Degrees>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterColor =
        MetaPropertyDrawerDefault<
            ParticleSystemParameterColor,
            PropertyDrawerParticleSystemParameterColor>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterColor, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterColor);
    template class MAZE_PARTICLES_API MetaPropertyDrawerDefault<ParticleSystemParameterColor, PropertyDrawerParticleSystemParameterColor>;


    //////////////////////////////////////////
    // Class ParticlesManager
    //
    //////////////////////////////////////////
    ParticlesManager* ParticlesManager::s_instance = nullptr;

    //////////////////////////////////////////
    ParticlesManager::ParticlesManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    ParticlesManager::~ParticlesManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void ParticlesManager::Initialize(ParticlesManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticlesManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool ParticlesManager::init()
    {
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ParticleSystem3D>("FX");


        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32>();
        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemParameterColor, PropertyDrawerParticleSystemParameterColor>();

        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemBurst, PropertyDrawerParticleSystemBurst>();



        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystemParameterF32, MetaPropertyDraweParticleSystemParameterF32>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystemParameterColor, MetaPropertyDraweParticleSystemParameterColor>();

        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DMainModule, MetaPropertyDrawerParticleSystem3DMainModule>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DRendererModule, MetaPropertyDrawerParticleSystem3DRendererModule>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DShapeModule, MetaPropertyDrawerParticleSystem3DShapeModule>();

        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DZoneData, MetaPropertyDrawerParticleSystem3DZoneData>();



        InspectorManager::GetInstancePtr()->registerComponentEditor<ParticleSystem3D, ComponentEditorParticleSystem3D>();
        
        return true;
    }

    //////////////////////////////////////////
    void ParticlesManager::createBuiltinAssets()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        S32 const chunkSize = 32;

        PixelSheet2D defaultParticleSheet(Vec2DS(1, 1) * chunkSize, PixelFormat::RGBA_U8);
        defaultParticleSheet.fill(ColorU32(255, 255, 255, 255));

        m_defaultParticleTexture = Texture2D::Create();
        m_defaultParticleTexture->setName("default_particle");
        m_defaultParticleTexture->setMagFilter(TextureFilter::Linear);
        m_defaultParticleTexture->setMinFilter(TextureFilter::Linear);

        // Drop down button collapsed
        {
            Vec2DS sheetPos(0, 0);
            Vec2DS startPos = chunkSize * sheetPos;

            for (S32 r = 0; r < chunkSize; ++r)
            {
                for (S32 c = 0; c < chunkSize; ++c)
                {
                    Vec2DF toCenter(
                        chunkSize * 0.5f - (F32)c,
                        chunkSize * 0.5f - (F32)r);

                    F32 p = 1.0f - Math::Clamp01(toCenter.length() / (chunkSize * 0.5f));

                    defaultParticleSheet.setPixel(
                        startPos + Vec2DS(c, r),
                        ColorU32(255, 255, 255, U8(255 * p)));
                }
            }
            
            m_defaultParticleSprite = Sprite::Create(
                m_defaultParticleTexture,
                Vec2DF(startPos),
                Vec2DF(chunkSize, chunkSize));
        }

        m_defaultParticleTexture->loadTexture(defaultParticleSheet);
        renderSystem->getTextureManager()->addTexture(m_defaultParticleTexture);

        m_defaultParticleMaterial = renderSystem->getMaterialManager()->getColorTextureMaterial()->createCopy();
        m_defaultParticleMaterial->setName("DefaultParticle");

        RenderPassPtr const& renderPass = m_defaultParticleMaterial->getFirstRenderPass();
        renderPass->setRenderQueueIndex(3000);
        renderPass->setDepthWriteEnabled(false);
        renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
        ShaderPtr shader = renderPass->getShader()->createCopy();
        shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
        shader->addLocalFeature("MAZE_UV_STREAM", "(1)");
        shader->recompile();
        renderPass->setShader(shader);

        m_defaultParticleMaterial->setUniform("u_baseMap", m_defaultParticleTexture);
        renderSystem->getMaterialManager()->addMaterial(m_defaultParticleMaterial);


        // m_defaultParticleTexture->saveToFileAsTGA("defaultParticleTexture.tga");

    }
    
} // namespace Maze
//////////////////////////////////////////
