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
#include "maze-graphics/ecs/components/MazeFoliageRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeAABB3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace
    {
        // Merged vertices must stay addressable by U16 indices
        static Size const c_maxMergedVerticesU16 = 65535u;

        //////////////////////////////////////////
        struct FoliageMergeGroup
        {
            MaterialPtr material;
            SubMeshPtr subMesh;
        };

        //////////////////////////////////////////
        void CollectFoliagePrefabMeshes(
            Entity* _entity,
            TMat const& _relativeTransform,
            Vector<FoliageMergeGroup>& _groups)
        {
            MeshRenderer* meshRenderer = _entity->getComponentRaw<MeshRenderer>();
            if (meshRenderer && meshRenderer->getEnabled() && meshRenderer->getRenderMesh())
            {
                MeshPtr const& mesh = meshRenderer->getRenderMesh()->getMesh();
                Vector<MaterialAssetRef> const& materials = meshRenderer->getMaterialRefs();

                if (mesh && !materials.empty())
                {
                    for (Size i = 0, in = mesh->getSubMeshesCount(); i < in; ++i)
                    {
                        MaterialPtr const& material = materials[i % materials.size()].getMaterial();
                        SubMeshPtr const& source = mesh->getSubMesh(i);
                        if (!material || !source)
                            continue;

                        SubMeshPtr subMesh = SubMesh::Create(source);
                        subMesh->transform(_relativeTransform);

                        bool merged = false;
                        for (FoliageMergeGroup& group : _groups)
                        {
                            if (group.material != material)
                                continue;

                            if (group.subMesh->getIndicesType() == VertexAttributeType::U16 &&
                                group.subMesh->getVerticesCount(VertexAttributeSemantic::Position) +
                                subMesh->getVerticesCount(VertexAttributeSemantic::Position) > c_maxMergedVerticesU16)
                                continue;

                            if (group.subMesh->mergeWith(subMesh))
                            {
                                merged = true;
                                break;
                            }
                        }

                        if (!merged)
                            _groups.push_back({ material, subMesh });
                    }
                }
            }

            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (!transform)
                return;

            for (Transform3D* child : transform->getChildren())
            {
                Entity* childEntity = child->getEntityRaw();
                if (!childEntity || !childEntity->getActiveSelf())
                    continue;

                CollectFoliagePrefabMeshes(
                    childEntity,
                    _relativeTransform.transform(child->getLocalTransform()),
                    _groups);
            }
        }

        //////////////////////////////////////////
        inline F32 GetMaxScale(TMat const& _transform)
        {
            Vec3F scale = _transform.getScaleSignless();
            return Math::Max(scale.x, scale.y, scale.z);
        }
    }


    //////////////////////////////////////////
    // Class FoliageRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(FoliageRenderer, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AssetFileId, dataFileId, c_invalidAssetFileId, getDataFileId, setDataFileId),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, drawDistance, 80.0f, getDrawDistance, setDrawDistance),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, castShadows, false, getCastShadows, setCastShadows));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(FoliageRenderer);


    //////////////////////////////////////////
    FoliageRenderer::FoliageRenderer()
    {
    }

    //////////////////////////////////////////
    FoliageRenderer::~FoliageRenderer()
    {
    }

    //////////////////////////////////////////
    FoliageRendererPtr FoliageRenderer::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        FoliageRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(FoliageRenderer, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<FoliageRenderer>()->m_renderSystem;

        if (!Component::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void FoliageRenderer::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
    }

    //////////////////////////////////////////
    void FoliageRenderer::setDataFileId(AssetFileId _value)
    {
        if (m_dataFileId == _value && (m_data || _value == c_invalidAssetFileId))
            return;

        m_dataFileId = _value;
        m_data = FoliageData::GetOrLoad(m_dataFileId);

        m_renderLayers.clear();
        m_syncedDataVersion = U32_MAX;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::createDataFile(Path const& _fullPath)
    {
        AssetManager* assetManager = AssetManager::GetInstancePtr();
        MAZE_ERROR_RETURN_VALUE_IF(!assetManager, false, "AssetManager is not initialized!");

        if (!FileHelper::IsFileExists(_fullPath))
        {
            FoliageDataPtr emptyData = FoliageData::Create();
            MAZE_ERROR_RETURN_VALUE_IF(!emptyData->saveToFile(_fullPath), false,
                "Failed to create foliage data file: %s", _fullPath.toUTF8().c_str());
        }

        assetManager->updateAssets();

        AssetFilePtr const& assetFile = assetManager->getAssetFileByFullPath(_fullPath);
        MAZE_ERROR_RETURN_VALUE_IF(!assetFile, false,
            "Foliage data file is not registered: %s", _fullPath.toUTF8().c_str());

        // New files get their ids from the editor later, the reference is needed right now
        if (assetFile->getAssetFileId() == c_invalidAssetFileId)
        {
            MAZE_ERROR_RETURN_VALUE_IF(!assetManager->getGenerateIdsForNewAssetFiles(), false,
                "Asset file ids generation is disabled: %s", _fullPath.toUTF8().c_str());

            assetFile->setAssetFileId(assetManager->generateAssetFileId());
            assetManager->updateAndSaveMetaData(assetFile);
        }

        setDataFileId(assetFile->getAssetFileId());
        return m_data != nullptr;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::addInstance(
        AssetUnitId _prefabAuid,
        TMat const& _worldTransform,
        FoliageInstance& _outInstance)
    {
        if (!m_data)
            return false;

        TMat localTransform = _worldTransform;
        if (Transform3D* transform = getEntityRaw() ? getEntityRaw()->getComponentRaw<Transform3D>() : nullptr)
            localTransform = _worldTransform * transform->getWorldTransform().inversed();

        Vec3F scale = localTransform.getScaleSignless();

        _outInstance.position = localTransform.getTranslation();
        _outInstance.scale = (scale.x + scale.y + scale.z) / 3.0f;
        _outInstance.setRotation(Quaternion(localTransform));

        m_data->addInstance(_prefabAuid, _outInstance);
        return true;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::addInstance(
        AssetUnitId _prefabAuid,
        FoliageInstance const& _instance)
    {
        if (!m_data)
            return false;

        m_data->addInstance(_prefabAuid, _instance);
        return true;
    }

    //////////////////////////////////////////
    bool FoliageRenderer::removeInstance(
        AssetUnitId _prefabAuid,
        Vec3F const& _localPosition)
    {
        if (!m_data)
            return false;

        return m_data->removeInstance(_prefabAuid, _localPosition);
    }

    //////////////////////////////////////////
    void FoliageRenderer::rebuildRenderLayers()
    {
        for (RenderLayer& layer : m_renderLayers)
        {
            layer.partsBuilt = false;
            layer.parts.clear();
            layer.chunks.clear();
        }
    }

    //////////////////////////////////////////
    void FoliageRenderer::releaseRenderData()
    {
        m_renderLayers.clear();
        m_syncedDataVersion = U32_MAX;
    }

    //////////////////////////////////////////
    void FoliageRenderer::syncRenderData(TMat const& _worldTransform)
    {
        if (!m_data)
        {
            if (!m_renderLayers.empty())
                releaseRenderData();
            return;
        }

        if (_worldTransform != m_syncedWorldTransform)
        {
            m_syncedWorldTransform = _worldTransform;
            m_syncedWorldScale = GetMaxScale(_worldTransform);
            for (RenderLayer& layer : m_renderLayers)
                layer.chunks.clear();
        }

        if (m_syncedDataVersion != m_data->getVersion())
        {
            m_syncedDataVersion = m_data->getVersion();

            if ((S32)m_renderLayers.size() > m_data->getLayersCount())
                m_renderLayers.clear();

            // Layers are only appended to the data, so the indices stay in sync
            for (S32 i = (S32)m_renderLayers.size(), in = m_data->getLayersCount(); i < in; ++i)
            {
                m_renderLayers.emplace_back();
                m_renderLayers.back().prefabAuid = m_data->getLayer(i).prefabAuid;
            }

            for (S32 i = 0, in = (S32)m_renderLayers.size(); i < in; ++i)
            {
                RenderLayer& layer = m_renderLayers[i];
                FoliageLayer const& dataLayer = m_data->getLayer(i);
                for (auto it = layer.chunks.begin(); it != layer.chunks.end();)
                {
                    if (dataLayer.chunks.find(it->first) == dataLayer.chunks.end())
                        it = layer.chunks.erase(it);
                    else
                        ++it;
                }
            }
        }

        for (S32 i = 0, in = (S32)m_renderLayers.size(); i < in; ++i)
        {
            RenderLayer& layer = m_renderLayers[i];
            FoliageLayer const& dataLayer = m_data->getLayer(i);
            if (dataLayer.instancesCount == 0u)
                continue;

            if (!layer.partsBuilt)
                buildRenderParts(layer);

            if (layer.parts.empty())
                continue;

            for (auto const& chunkData : dataLayer.chunks)
            {
                ChunkCache& cache = layer.chunks[chunkData.first];
                if (cache.version != chunkData.second.version)
                    rebuildChunkCache(layer, chunkData.second, cache);
            }
        }
    }

    //////////////////////////////////////////
    void FoliageRenderer::buildRenderParts(RenderLayer& _layer)
    {
        _layer.partsBuilt = true;
        _layer.parts.clear();
        _layer.localRadius = 0.0f;
        _layer.chunks.clear();

        MAZE_ERROR_RETURN_IF(!EntityPrefabManager::GetInstancePtr(), "EntityPrefabManager is not initialized!");

        EntityPtr const& prefab = EntityPrefabManager::GetInstancePtr()->getOrLoadEntityPrefab(_layer.prefabAuid);
        MAZE_ERROR_RETURN_IF(!prefab, "Foliage prefab is not found! auid=%u", _layer.prefabAuid);

        // The prefab root is placed by the instance transform itself
        Vector<FoliageMergeGroup> groups;
        CollectFoliagePrefabMeshes(prefab.get(), TMat::c_identity, groups);

        MAZE_WARNING_IF(groups.empty(), "Foliage prefab has no meshes! auid=%u", _layer.prefabAuid);

        for (FoliageMergeGroup const& group : groups)
        {
            _layer.localRadius = Math::Max(_layer.localRadius, group.subMesh->calculateBoundingSphereRadius());

            MeshPtr mesh = Mesh::Create(group.subMesh);
            RenderMeshPtr renderMesh = RenderMesh::Create(mesh, m_renderSystem);
            if (!renderMesh || renderMesh->getVertexArrayObjects().empty())
                continue;

            _layer.parts.push_back({ renderMesh, group.material });
        }

        // Encoded into the render unit index
        MAZE_ERROR_IF(_layer.parts.size() > 0xFF, "Too many foliage prefab materials! auid=%u", _layer.prefabAuid);
        if (_layer.parts.size() > 0xFF)
            _layer.parts.resize(0xFF);
    }

    //////////////////////////////////////////
    void FoliageRenderer::rebuildChunkCache(
        RenderLayer& _layer,
        FoliageChunk const& _chunk,
        ChunkCache& _cache)
    {
        _cache.version = _chunk.version;
        _cache.matrices.resize(_chunk.instances.size());

        if (_chunk.instances.empty())
        {
            _cache.boundsCenter = Vec3F::c_zero;
            _cache.boundsRadius = 0.0f;
            return;
        }

        AABB3D positionsBounds(m_syncedWorldTransform.transform(_chunk.instances[0].position));
        F32 maxScale = 0.0f;

        for (Size i = 0, in = _chunk.instances.size(); i < in; ++i)
        {
            FoliageInstance const& instance = _chunk.instances[i];
            m_syncedWorldTransform.transform(instance.buildTransform(), _cache.matrices[i]);

            positionsBounds.applyUnion(AABB3D(_cache.matrices[i].getTranslation()));
            maxScale = Math::Max(maxScale, instance.scale);
        }

        _cache.boundsCenter = (positionsBounds.getMin() + positionsBounds.getMax()) * 0.5f;
        _cache.boundsRadius =
            (positionsBounds.getMax() - positionsBounds.getMin()).length() * 0.5f +
            _layer.localRadius * maxScale * m_syncedWorldScale;
    }

    //////////////////////////////////////////
    void FoliageRenderer::drawInstances(
        RenderQueuePtr const& _renderQueue,
        Frustum const& _frustum,
        Vec3F const* _cameraPosition,
        RenderUnit const& _renderUnit)
    {
        S32 layerIndex;
        S32 partIndex;
        DecodeFoliageRenderUnitIndex(_renderUnit.index, layerIndex, partIndex);

        if (layerIndex < 0 || layerIndex >= (S32)m_renderLayers.size())
            return;

        RenderLayer const& layer = m_renderLayers[layerIndex];
        if (partIndex < 0 || partIndex >= (S32)layer.parts.size())
            return;

        VertexArrayObject* vao = layer.parts[partIndex].renderMesh->getVertexArrayObject().get();
        if (!vao)
            return;

        for (auto const& chunkData : layer.chunks)
        {
            ChunkCache const& cache = chunkData.second;
            if (cache.matrices.empty())
                continue;

            if (!_frustum.containsSphere(cache.boundsCenter, cache.boundsRadius))
                continue;

            if (_cameraPosition && m_drawDistance > 0.0f)
            {
                F32 distance = (cache.boundsCenter - *_cameraPosition).length() - cache.boundsRadius;
                if (distance > m_drawDistance)
                    continue;
            }

            _renderQueue->addDrawVAOInstancedCommand(
                vao,
                (S32)cache.matrices.size(),
                cache.matrices.data());
        }
    }

    //////////////////////////////////////////
    void FoliageRenderer::drawDefaultPass(
        RenderQueuePtr const& _renderQueue,
        DefaultPassParams const& _params,
        RenderUnit const& _renderUnit)
    {
        // Orthographic cameras (editor top views) see the whole frustum regardless of the distance
        bool orthographic = _params.projectionMatrix[3][3] == 1.0f;
        Vec3F cameraPosition = _params.cameraTransform.getTranslation();

        drawInstances(
            _renderQueue,
            _params.cameraFrustum,
            orthographic ? nullptr : &cameraPosition,
            _renderUnit);
    }

    //////////////////////////////////////////
    void FoliageRenderer::drawShadowPass(
        RenderQueuePtr const& _renderQueue,
        ShadowPassParams const& _params,
        RenderUnit const& _renderUnit)
    {
        drawInstances(
            _renderQueue,
            _params.mainLightFrustum,
            nullptr,
            _renderUnit);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(FoliageRendererDefaultPassGatherRenderUnits,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render3DDefaultPassGatherRenderUnitsEvent& _event,
        Entity* _entity,
        FoliageRenderer* _foliageRenderer,
        Transform3D* _transform3D)
    {
        if (!_foliageRenderer->getEnabled())
            return;

        if (!_foliageRenderer->getRenderMask() || !(_foliageRenderer->getRenderMask()->getMask() & _event.getPassParams()->renderMask))
            return;

        _foliageRenderer->syncRenderData(_transform3D->getWorldTransform());

        Vector<FoliageRenderer::RenderLayer> const& layers = _foliageRenderer->getRenderLayers();
        for (S32 l = 0, ln = (S32)layers.size(); l < ln; ++l)
        {
            FoliageRenderer::RenderLayer const& layer = layers[l];
            if (layer.chunks.empty())
                continue;

            for (S32 p = 0, pn = (S32)layer.parts.size(); p < pn; ++p)
            {
                FoliageRenderer::RenderPart const& part = layer.parts[p];
                RenderPassPtr const& renderPass = part.material->getFirstRenderPass();
                if (!renderPass)
                    continue;

#if (MAZE_DEBUG)
                if (!renderPass->getShader())
                {
                    Debug::LogError("Foliage(EID: %u): Shader is null!", _entity->getId());
                    continue;
                }
#endif

                _event.getRenderUnits()->emplace_back(
                    renderPass.get(),
                    _transform3D->getWorldPosition(),
                    _foliageRenderer,
                    EncodeFoliageRenderUnitIndex(l, p),
                    0u,
                    static_cast<S32>(part.renderMesh->getVertexArrayObject()->getResourceId().getIndex()));
            }
        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(FoliageRendererShadowPassGatherRenderUnits,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render3DShadowPassGatherRenderUnitsEvent& _event,
        Entity* _entity,
        FoliageRenderer* _foliageRenderer,
        Transform3D* _transform3D)
    {
        if (!_foliageRenderer->getEnabled() || !_foliageRenderer->getCastShadows())
            return;

        if (!_foliageRenderer->getRenderMask() || !(_foliageRenderer->getRenderMask()->getMask() & _event.getPassParams()->renderMask))
            return;

        _foliageRenderer->syncRenderData(_transform3D->getWorldTransform());

        Vector<FoliageRenderer::RenderLayer> const& layers = _foliageRenderer->getRenderLayers();
        for (S32 l = 0, ln = (S32)layers.size(); l < ln; ++l)
        {
            FoliageRenderer::RenderLayer const& layer = layers[l];
            if (layer.chunks.empty())
                continue;

            for (S32 p = 0, pn = (S32)layer.parts.size(); p < pn; ++p)
            {
                FoliageRenderer::RenderPart const& part = layer.parts[p];
                RenderPassPtr const& renderPass = part.material->getFirstRenderPass(RenderPassType::Shadow);
                if (!renderPass)
                    continue;

                _event.getRenderUnits()->emplace_back(
                    renderPass.get(),
                    _transform3D->getWorldPosition(),
                    _foliageRenderer,
                    EncodeFoliageRenderUnitIndex(l, p),
                    0u,
                    static_cast<S32>(part.renderMesh->getVertexArrayObject()->getResourceId().getIndex()));
            }
        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(FoliageRendererOnEcsWorldWillBeDestroyed,
        {},
        {},
        EcsWorldWillBeDestroyedEvent const& _event,
        Entity* _entity,
        FoliageRenderer* _foliageRenderer)
    {
        // Release render meshes before the render system resources are destroyed
        _foliageRenderer->releaseRenderData();
    }

} // namespace Maze
//////////////////////////////////////////
