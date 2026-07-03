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
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AbstractTextRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AbstractTextRenderer2D, AbstractTextRenderer,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Bool, pixelPerfect, true, getPixelPerfect, setPixelPerfect));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AbstractTextRenderer2D);


    //////////////////////////////////////////
    AbstractTextRenderer2D::AbstractTextRenderer2D()
    {
        
    }

    //////////////////////////////////////////
    AbstractTextRenderer2D::~AbstractTextRenderer2D()
    {
        
    }

    //////////////////////////////////////////
    bool AbstractTextRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!AbstractTextRenderer::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool AbstractTextRenderer2D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!AbstractTextRenderer::init(
            _component,
            _copyData))
            return false;

        enableFlag(Flags::MeshDataDirty);
        enableFlag(Flags::MaterialDirty);

        return true;
    }

    //////////////////////////////////////////
    void AbstractTextRenderer2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_canvasRenderer = getEntityRaw()->ensureComponent<CanvasRenderer>();
        m_canvasRenderer->setFlag(Component::Flags::SerializationDisabled, true);

        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRendererInstanced>();
        m_meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultQuadNullPivotMesh());
        m_meshRenderer->setFlag(Component::Flags::SerializationDisabled, true);

        enableFlag(Flags::MeshDataDirty);
        enableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    void AbstractTextRenderer2D::prepareForRender()
    {
        if (m_updatingMeshData)
            return;

        m_updatingMeshData = true;

        if (getFlag(Flags::MeshDataDirty))
            updateMeshDataNow();

        if (getFlag(Flags::GlyphColorsDirty))
            updateGlyphColors();

        if (getFlag(Flags::MaterialDirty))
            updateMaterial();

        if (getFlag(Flags::ModelMatricesDirty))
            updateMeshRendererModelMatrices();

        if (getFlag(Flags::ColorDirty))
            updateMeshRendererColors();

        m_updatingMeshData = false;
    }

    //////////////////////////////////////////
    void AbstractTextRenderer2D::updateMeshData()
    {
        enableFlag(Flags::MeshDataDirty);
        enableFlag(Flags::MaterialDirty);
        prepareForRender();
    }

    //////////////////////////////////////////
    Vec4F AbstractTextRenderer2D::calculateMeshColor() const
    {
        return Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer ? m_canvasRenderer->getAlpha() : 1.0f);
    }

    //////////////////////////////////////////
    void AbstractTextRenderer2D::updateMeshRendererColors()
    {
        if (!m_meshRenderer)
            return;

        Vec4F const meshColor = calculateMeshColor();

        Size colorsCount = m_meshRenderer->getColors().size();
        for (Size i = 0; i < colorsCount; ++i)
            m_meshRenderer->setColor(i, m_localColors[i] * meshColor);

        disableFlag(Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void AbstractTextRenderer2D::updateMeshRendererModelMatrices()
    {
        if (!m_meshRenderer)
            return;

        Size transformCount = m_meshRenderer->getModelMatrices().size();
        MAZE_DEBUG_ERROR_IF(transformCount != m_localMatrices.size(), "Invalid characters count!");
        if (transformCount == 0)
            return;

        Vec2F pixelPerfectShift = Vec2F::c_zero;
        if (m_pixelPerfect)
        {
            TMat initTm = m_transform->getWorldTransform().transform(m_localMatrices[0]);
            Vec2F translation = initTm.getTranslation2D();
            pixelPerfectShift = Math::Round(translation) - translation;
        }

        for (Size i = 0; i < transformCount; ++i)
        {
            TMat tm = m_transform->getWorldTransform().transform(m_localMatrices[i]);
            if (m_pixelPerfect)
                tm.setTranslation(tm.getTranslation2D() + pixelPerfectShift);
            m_meshRenderer->setModelMatrix(i, tm);
        }

        disableFlag(Flags::ModelMatricesDirty);
    }

} // namespace Maze
//////////////////////////////////////////
