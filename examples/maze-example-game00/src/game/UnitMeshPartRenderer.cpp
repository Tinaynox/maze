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
#include "UnitMeshPartRenderer.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UnitMeshPartRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UnitMeshPartRenderer, UnitPartRenderer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UnitMeshPartRenderer);

    //////////////////////////////////////////
    UnitMeshPartRenderer::UnitMeshPartRenderer()
    {

    }

    //////////////////////////////////////////
    UnitMeshPartRenderer::~UnitMeshPartRenderer()
    {
        
    }

    //////////////////////////////////////////
    UnitMeshPartRendererPtr UnitMeshPartRenderer::Create()
    {
        UnitMeshPartRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UnitMeshPartRenderer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UnitMeshPartRenderer::init()
    {
        if (!UnitPartRenderer::init())
            return false;

        return true;
    }    

    //////////////////////////////////////////
    void UnitMeshPartRenderer::processEntityAwakened()
    {
        UnitPartRenderer::processEntityAwakened();

        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();

        U32 vaoCount= (U32)m_meshRenderer->getMaterials().size();
        setDefaultMaterialsCount(vaoCount);
    }

    //////////////////////////////////////////
    void UnitMeshPartRenderer::prepare()
    {
        UnitPartRenderer::prepare();
    }

    //////////////////////////////////////////
    void UnitMeshPartRenderer::updateRenderMaterials()
    {
        Size extraMaterialsCount = 0;

        for (bool extraMaterialStatus : m_extraMaterialsStatus)
        {
            if (extraMaterialStatus)
                ++extraMaterialsCount;
        }

        Size defaultMaterialsCount = m_defaultMaterialsCount;
        Size totalMaterialsCount = defaultMaterialsCount + extraMaterialsCount * m_defaultMaterialsCount;

        Vector<MaterialPtr> materials = m_meshRenderer->getMaterials();
        materials.resize(totalMaterialsCount);

        Size extraMaterialsAddedCount = 0;
        for (Size j = 0; j < (Size)UnitExtraMaterialType::MAX; ++j)
        {
            if (m_extraMaterialsStatus[j])
            {
                for (Size i = 0; i < defaultMaterialsCount; i++)
                {
                    materials[defaultMaterialsCount + extraMaterialsAddedCount] = m_extraMaterials[j];
                    ++extraMaterialsAddedCount;
                }
            }
        }

        m_meshRenderer->setMaterials(materials);
    }

    //////////////////////////////////////////
    void UnitMeshPartRenderer::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        Size vaosCount = m_meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
        Vector<MaterialPtr> materials;
        materials.resize(vaosCount);

        switch (m_state)
        {
            case UnitPartRendererState::DeathProgress:
            case UnitPartRendererState::Death:
            {
                for (Size i = 0; i < vaosCount; ++i)
                    materials[i] = m_deathMaterial;

                getMeshRenderer()->setMaterials(materials);
                F32 deathProgress = Math::Clamp(m_deathTimer / m_deathTime, 0.0f, 1.0f);
                m_deathProgressUniform->set(pow(deathProgress, 0.35f));

                break;
            }
            default:
            {
                for (Size i = 0; i < vaosCount; ++i)
                    materials[i] = m_defaultMaterial;

                getMeshRenderer()->setMaterials(materials);

                break;
            }
        }        
    }

} // namespace Maze
//////////////////////////////////////////
