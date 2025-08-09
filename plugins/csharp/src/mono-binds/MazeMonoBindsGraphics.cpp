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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsGraphics.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeSkinnedMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeGlobalShaderUniform.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    inline void MeshRendererSetMaterialAssetUnit(Component* _component, S32 _index, AssetUnitId _auid)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(_auid);
        Vector<MaterialAssetRef> materials = _component->castRaw<MeshRenderer>()->getMaterialRefs();

        if (_index >= materials.size())
            materials.resize(_index + 1);
        materials[_index] = { MaterialAssetRef(material) };

        _component->castRaw<MeshRenderer>()->setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    inline void MeshRendererSetMaterialResourceId(Component* _component, S32 _index, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        Material* material = Material::GetResource(_resourceId);
        Vector<MaterialAssetRef> materials = _component->castRaw<MeshRenderer>()->getMaterialRefs();

        if (_index >= materials.size())
            materials.resize(_index + 1);
        materials[_index] = { MaterialAssetRef(material) };

        _component->castRaw<MeshRenderer>()->setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    inline void MeshRendererSetMaterial(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        Material* material = Material::GetResource(_resourceId);
        _component->castRaw<MeshRenderer>()->setMaterial(material ? material->getSharedPtr() : MaterialPtr());
    }

    //////////////////////////////////////////
    inline void MeshRendererGetMaterial(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        MaterialPtr const& material = _component->castRaw<MeshRenderer>()->getMaterial();
        if (material)
            _outResourceId = material->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void MeshRendererSetRenderMesh(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        RenderMesh* renderMesh = RenderMesh::GetResource(_resourceId);
        _component->castRaw<MeshRenderer>()->setRenderMesh(renderMesh ? renderMesh->getSharedPtr() : RenderMeshPtr());
    }

    //////////////////////////////////////////
    inline void MeshRendererGetRenderMesh(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRenderer);

        RenderMeshPtr const& renderMesh = _component->castRaw<MeshRenderer>()->getRenderMesh();
        if (renderMesh)
            _outResourceId = renderMesh->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedSetMaterial(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);

        Material* material = Material::GetResource(_resourceId);
        _component->castRaw<MeshRendererInstanced>()->setMaterial(material ? material->getSharedPtr() : MaterialPtr());
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedGetMaterial(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);

        MaterialPtr const& material = _component->castRaw<MeshRendererInstanced>()->getMaterial();
        if (material)
            _outResourceId = material->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedSetRenderMesh(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);

        RenderMesh* renderMesh = RenderMesh::GetResource(_resourceId);
        _component->castRaw<MeshRendererInstanced>()->setRenderMesh(renderMesh ? renderMesh->getSharedPtr() : RenderMeshPtr());
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedGetRenderMesh(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);

        RenderMeshPtr const& renderMesh = _component->castRaw<MeshRendererInstanced>()->getRenderMesh();
        if (renderMesh)
            _outResourceId = renderMesh->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedResizeModelMatrices(Component* _component, S32 _size)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);
        _component->castRaw<MeshRendererInstanced>()->resizeModelMatrices(_size);
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedSetModelMatrix(Component* _component, S32 _index, TMat const& _tm)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);
        _component->castRaw<MeshRendererInstanced>()->setModelMatrix(_index, _tm);
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedResizeColors(Component* _component, S32 _size)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);
        _component->castRaw<MeshRendererInstanced>()->resizeColors(_size);
    }

    //////////////////////////////////////////
    inline void MeshRendererInstancedSetColor(Component* _component, S32 _index, Vec4F const& _color)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(MeshRendererInstanced);
        _component->castRaw<MeshRendererInstanced>()->setColor(_index, _color);
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererSetMaterialAssetUnit(Component* _component, AssetUnitId _auid)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(_auid);
        _component->castRaw<SkinnedMeshRenderer>()->setMaterial(material);
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererSetMaterial(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        Material* material = Material::GetResource(_resourceId);
        _component->castRaw<SkinnedMeshRenderer>()->setMaterial(material ? material->getSharedPtr() : MaterialPtr());
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererGetMaterial(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        MaterialPtr const& material = _component->castRaw<SkinnedMeshRenderer>()->getMaterial();
        if (material)
            _outResourceId = material->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererSetRenderMesh(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        RenderMesh* renderMesh = RenderMesh::GetResource(_resourceId);
        _component->castRaw<SkinnedMeshRenderer>()->setRenderMesh(renderMesh ? renderMesh->getSharedPtr() : RenderMeshPtr());
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererGetRenderMesh(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        RenderMeshPtr const& renderMesh = _component->castRaw<SkinnedMeshRenderer>()->getRenderMesh();
        if (renderMesh)
            _outResourceId = renderMesh->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline S32 SkinnedMeshRendererPlayAnimation(
        Component* _component,
        MonoString* _animationName,
        bool _loop,
        F32 _blendTime)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT_RETURN_VALUE(SkinnedMeshRenderer, -1);

        Char* cstr = mono_string_to_utf8(_animationName);
        S32 playerIndex = _component->castRaw<SkinnedMeshRenderer>()->playAnimation(HashedCString(cstr), _loop, _blendTime);
        mono_free(cstr);

        return playerIndex;
    }

    //////////////////////////////////////////
    inline bool SkinnedMeshRendererGetPlayerAnimationTime(
        Component* _component,
        S32 _playerIndex,
        F32& _outAnimationTime)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT_RETURN_VALUE(SkinnedMeshRenderer, false);
        MAZE_ERROR_RETURN_VALUE_IF(_playerIndex < 0 || _playerIndex >= MESH_SKELETON_ANIMATOR_PLAYERS_COUNT, false, "PlayerIndex is invalid - %d", _playerIndex);

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        MeshSkeletonAnimatorPlayerPtr const& player = animator->getPlayer(_playerIndex);
        if (!player)
            return false;

        MeshSkeletonAnimationPtr const& animation = player->getAnimation();
        if (!animation)
            return false;

        _outAnimationTime = animation->getAnimationTime();
        return true;
    }

    //////////////////////////////////////////
    inline bool SkinnedMeshRendererGetPlayerCurrentTime(
        Component* _component,
        S32 _playerIndex,
        F32& _outCurrentTime)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT_RETURN_VALUE(SkinnedMeshRenderer, false);
        MAZE_ERROR_RETURN_VALUE_IF(_playerIndex < 0 || _playerIndex >= MESH_SKELETON_ANIMATOR_PLAYERS_COUNT, false, "PlayerIndex is invalid - %d", _playerIndex);

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        MeshSkeletonAnimatorPlayerPtr const& player = animator->getPlayer(_playerIndex);
        if (!player)
            return false;

        _outCurrentTime = player->getCurrentTime();
        return true;
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererPlayerRewindTo(
        Component* _component,
        S32 _playerIndex,
        F32 _time)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);
        MAZE_ERROR_RETURN_IF(_playerIndex < 0 || _playerIndex >= MESH_SKELETON_ANIMATOR_PLAYERS_COUNT, "PlayerIndex is invalid - %d", _playerIndex);

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        MeshSkeletonAnimatorPlayerPtr const& player = animator->getPlayer(_playerIndex);
        if (!player)
            return;

        player->rewindTo(_time);
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererPlayerStop(
        Component* _component,
        S32 _playerIndex)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);
        MAZE_ERROR_RETURN_IF(_playerIndex < 0 || _playerIndex >= MESH_SKELETON_ANIMATOR_PLAYERS_COUNT, "PlayerIndex is invalid - %d", _playerIndex);

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        MeshSkeletonAnimatorPlayerPtr const& player = animator->getPlayer(_playerIndex);
        if (!player)
            return;

        player->stop();
    }

    //////////////////////////////////////////
    inline F32 SkinnedMeshRendererGetAnimationSpeed(Component* _component)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_component->getClassUID() != ClassInfo<SkinnedMeshRenderer>::UID(), 0.0f, "Component is not SkinnedMeshRenderer!");

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        return animator->getAnimationSpeed();
    }

    //////////////////////////////////////////
    inline void SkinnedMeshRendererSetAnimationSpeed(
        Component* _component,
        F32 _animationSpeed)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SkinnedMeshRenderer);

        MeshSkeletonAnimatorPtr const& animator = _component->castRaw<SkinnedMeshRenderer>()->getAnimator();
        animator->setAnimationSpeed(_animationSpeed);
    }


    //////////////////////////////////////////
    inline void SpriteRenderer2DSetMaterial(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SpriteRenderer2D);

        Material* material = Material::GetResource(_resourceId);
        _component->castRaw<SpriteRenderer2D>()->setMaterial(material ? material->getSharedPtr() : MaterialPtr());
    }

    //////////////////////////////////////////
    inline void SpriteRenderer2DGetMaterial(Component* _component, S32& _outResourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(SpriteRenderer2D);

        MaterialPtr const& material = _component->castRaw<SpriteRenderer2D>()->getMaterial();
        if (material)
            _outResourceId = material->getResourceId();
        else
            _outResourceId = c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void Camera3DGetAspectRatio(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _outValue = _component->castRaw<Camera3D>()->getAspectRatio();
    }

    //////////////////////////////////////////
    inline void Camera3DGetOrthographicSize(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _outValue = _component->castRaw<Camera3D>()->getOrthographicSize();
    }

    //////////////////////////////////////////
    inline void Camera3DSetOrthographicSize(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _component->castRaw<Camera3D>()->setOrthographicSize(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetNearZ(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _outValue = _component->castRaw<Camera3D>()->getNearZ();
    }

    //////////////////////////////////////////
    inline void Camera3DSetNearZ(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _component->castRaw<Camera3D>()->setNearZ(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetFarZ(Component* _component, F32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _outValue = _component->castRaw<Camera3D>()->getFarZ();
    }

    //////////////////////////////////////////
    inline void Camera3DSetFarZ(Component* _component, F32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _component->castRaw<Camera3D>()->setFarZ(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetRenderMask(Component* _component, S32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _outValue = _component->castRaw<Camera3D>()->getRenderMask();
    }

    //////////////////////////////////////////
    inline void Camera3DSetRenderMask(Component* _component, S32 _value)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        _component->castRaw<Camera3D>()->setRenderMask(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetRenderTarget(Component* _component, S32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);
        RenderTargetPtr const& renderTarget = _component->castRaw<Camera3D>()->getRenderTarget();
        _outValue = renderTarget ? renderTarget->getResourceId() : c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void Camera3DSetRenderTarget(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);

        RenderTarget* renderTarget = RenderWindow::GetResource(_resourceId);
        _component->castRaw<Camera3D>()->setRenderTarget(renderTarget ? renderTarget->getSharedPtr() : nullptr);
    }

    //////////////////////////////////////////
    inline void Camera3DConvertViewportCoordsToRay(Component* _component, Vec2F const& _positionV, Ray& _ray)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Camera3D);

        _ray = _component->castRaw<Camera3D>()->convertViewportCoordsToRay(_positionV);
    }

    //////////////////////////////////////////
    inline S32 CreateSubMesh()
    {
        SubMeshPtr subMesh = SubMesh::Create();
        subMesh.incRef();
        return subMesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroySubMesh(S32 _subMeshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_subMeshId]()
            {
                if (SubMesh* subMesh = SubMesh::GetResource(_subMeshId))
                    subMesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void SubMeshSetRenderDrawTopology(
        S32 _subMeshId,
        S32 _topology)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setRenderDrawTopology((RenderDrawTopology)_topology);
    }

    //////////////////////////////////////////
    inline void SubMeshSetIndicesU16(
        S32 _subMeshId,
        U16 const* _indices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setIndices(_indices, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetIndicesU32(
        S32 _subMeshId,
        U32 const* _indices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setIndices(_indices, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetPositions(
        S32 _subMeshId,
        Vec3F const* _positions,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setPositions(_positions, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetNormals(
        S32 _subMeshId,
        Vec3F const* _normals,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setNormals(_normals, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTangents(
        S32 _subMeshId,
        Vec3F const* _tangents,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTangents(_tangents, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBitangents(
        S32 _subMeshId,
        Vec3F const* _bitangents,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBitangents(_bitangents, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetColors(
        S32 _subMeshId,
        Vec4F const* _colors,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setColors(_colors, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTexCoordsVec2F(
        S32 _subMeshId,
        S32 _uvIndex,
        Vec2F const* _uv,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTexCoords(_uvIndex, _uv, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTexCoordsVec4F(
        S32 _subMeshId,
        S32 _uvIndex,
        Vec4F const* _uv,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTexCoords(_uvIndex, _uv, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBlendWeights(
        S32 _subMeshId,
        Vec4F const* _blendWeights,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBlendWeights(_blendWeights, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBlendIndices(
        S32 _subMeshId,
        Vec4F const* _blendIndices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBlendIndices(_blendIndices, (Size)_count);
    }

    //////////////////////////////////////////
    inline S32 CreateMesh()
    {
        MeshPtr mesh = Mesh::Create();
        mesh.incRef();
        return mesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyMesh(S32 _meshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_meshId]()
            {
                if (Mesh* mesh = Mesh::GetResource(_meshId))
                    mesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void MeshAddSubMesh(S32 _meshId, S32 _subMeshId)
    {
        if (Mesh* mesh = Mesh::GetResource(_meshId))
            if (SubMesh* subMesh = SubMesh::GetResource(_subMeshId))
                mesh->addSubMesh(subMesh->getSharedPtr());
    }

    //////////////////////////////////////////
    inline void MeshClear(S32 _meshId)
    {
        if (Mesh* mesh = Mesh::GetResource(_meshId))
            mesh->clear();
    }

    //////////////////////////////////////////
    inline S32 CreateRenderMesh()
    {
        RenderMeshPtr renderMesh = RenderMesh::Create();
        renderMesh.incRef();
        return renderMesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyRenderMesh(S32 _renderMeshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_renderMeshId]()
            {
                if (RenderMesh* renderMesh = RenderMesh::GetResource(_renderMeshId))
                    renderMesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void RenderMeshLoadFromMesh(S32 _renderMeshId, S32 _meshId)
    {
        if (RenderMesh* renderMesh = RenderMesh::GetResource(_renderMeshId))
            if (Mesh* mesh = Mesh::GetResource(_meshId))
                renderMesh->loadFromMesh(mesh->getSharedPtr());
    }

    //////////////////////////////////////////
    inline void RenderMeshClear(S32 _renderMeshId)
    {
        if (RenderMesh* renderMesh = RenderMesh::GetResource(_renderMeshId))
            renderMesh->clear();
    }

    //////////////////////////////////////////
    inline S32 CreateRenderWindow(
        Vec2U const& _size,
        MonoString* _title)
    {
        Char* cstr = mono_string_to_utf8(_title);

        RenderWindowParams params;
        params.windowParams =
            WindowParams::Create(
                _size,
                32,
                cstr);

        mono_free(cstr);

        RenderWindowPtr renderWindow = RenderWindow::Create(params);
        renderWindow.incRef();
        return renderWindow->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyRenderWindow(S32 _resourceId)
    {
        auto func =
            [_resourceId]()
            {
                if (RenderWindow* renderTarget = (RenderWindow*)RenderWindow::GetResource(_resourceId))
                    renderTarget->getSharedPtr().decRef();
            };


        if (TaskManager::IsMainThread())
            func();
        else
            TaskManager::GetInstancePtr()->addMainThreadTask(func);
    }

    //////////////////////////////////////////
    inline S32 EcsRenderSceneGetRenderTarget(Component* _component)
    {
        EcsRenderScene* scene = _component->getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>();
        return scene->getRenderTarget()->getResourceId();
    }

    //////////////////////////////////////////
    inline void CanvasGetRenderTarget(Component* _component, S32& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Canvas);
        RenderTargetPtr const& renderTarget = _component->castRaw<Canvas>()->getRenderTarget();
        _outValue = renderTarget ? renderTarget->getResourceId() : c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void CanvasSetRenderTarget(Component* _component, S32 _resourceId)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Canvas);

        RenderTarget* renderTarget = RenderWindow::GetResource(_resourceId);
        _component->castRaw<Canvas>()->setRenderTarget(renderTarget ? renderTarget->getSharedPtr() : nullptr);
    }

    //////////////////////////////////////////
    inline void CanvasGetRenderTargetRect(Component* _component, Rect2F& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Canvas);
        _outValue = _component->castRaw<Canvas>()->getRenderTargetRect();
    }

    //////////////////////////////////////////
    inline void CanvasConvertRenderTargetCoordsToViewportCoords(
        Component* _component,
        Vec2F const& _renderTargetPosition,
        Vec2F& _outValue)
    {
        MAZE_MONO_BIND_VALIDATE_COMPONENT(Canvas);
        _outValue = _component->castRaw<Canvas>()->convertRenderTargetCoordsToViewportCoords(_renderTargetPosition);
    }

    //////////////////////////////////////////
    inline void AbstractTextRendererGetText(Component* _component, MonoString* _text)
    {
        MAZE_ERROR_RETURN_IF(!_component->getMetaClass()->isInheritedFrom<AbstractTextRenderer>(), "Component is not AbstractTextRenderer!");
        String const& text = _component->castRaw<AbstractTextRenderer>()->getText();
        mono_string_new(MonoEngine::GetMonoDomain(), text.c_str());
    }

    //////////////////////////////////////////
    inline void AbstractTextRendererSetText(Component* _component, MonoString* _text)
    {
        MAZE_ERROR_RETURN_IF(!_component->getMetaClass()->isInheritedFrom<AbstractTextRenderer>(), "Component is not AbstractTextRenderer!");
        Char* cstr = mono_string_to_utf8(_text);
        _component->castRaw<AbstractTextRenderer>()->setText(cstr);
        mono_free(cstr);
    }

    //////////////////////////////////////////
    inline bool GlobalShaderUniformIdIsValid(S32 _globalShaderUniformId)
    {
        GlobalShaderUniform* uniform = GlobalShaderUniform::GetResource(_globalShaderUniformId);
        return !!uniform;
    }

    //////////////////////////////////////////
    inline S32 GlobalShaderUniformEnsure(MonoString* _name)
    {
        S32 result = -1;

        Char* cstr = mono_string_to_utf8(_name);

        GlobalShaderUniformPtr const& uniform = ShaderSystem::GetCurrentInstancePtr()->ensureGlobalShaderUniform(
            HashedCString(cstr));
        if (uniform)
            result = uniform->getResourceId();

        mono_free(cstr);

        return result;
    }

    //////////////////////////////////////////
    #define IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(DType)                                \
        inline void GlobalShaderUniformSet ## DType (S32 _id, DType const& _value)    \
        {                                                                             \
            if (GlobalShaderUniform* uniform = GlobalShaderUniform::GetResource(_id)) \
                uniform->setValue(_value);                                            \
        }
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(S32);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(F32);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(F64);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Bool);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec2F);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec3F);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec4F);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec2S);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec3S);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec4S);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec2U);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec3U);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec4U);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec2B);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec3B);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Vec4B);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Mat3F);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(Mat4F);
    IMPLEMENT_GLOBAL_SHADER_UNIFORM_SET(TMat);

    inline void GlobalShaderUniformSetColorF128(S32 _id, Vec4F const& _value)
    {
        if (GlobalShaderUniform* uniform = GlobalShaderUniform::GetResource(_id))
            uniform->setValue(ColorF128(_value));
    }


    //////////////////////////////////////////
    inline bool MaterialIsValid(S32 _materialId)
    {
        Material* material = Material::GetResource(_materialId);
        return !!material;
    }

    //////////////////////////////////////////
    inline S32 MaterialCreateCopy(S32 _materialId)
    {
        Material* material = Material::GetResource(_materialId);
        if (material == nullptr)
            return c_invalidResourceId;

        MaterialPtr materialCopy = material->createCopy();
        materialCopy.incRef();
        return materialCopy->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyMaterial(S32 _materialId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_materialId]()
            {
                if (Material* material = Material::GetResource(_materialId))
                    material->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline S32 MaterialEnsureUniformIndex(S32 _materialId, MonoString* _name)
    {
        if (Material* material = Material::GetResource(_materialId))
        {
            Char* cstr = mono_string_to_utf8(_name);
            ShaderUniformVariantPtr const& shaderUniformVariant = material->ensureUniform(HashedCString(cstr));
            mono_free(cstr);

            Size index = material->getUniforms().find(shaderUniformVariant) - material->getUniforms().begin();
            return (S32)index;
        }

        return -1;
    }

    //////////////////////////////////////////
    #define IMPLEMENT_MATERIAL_UNIFORM_SET(DType)                                                             \
        inline void MaterialUniformSet ## DType (S32 _materialId, S32 _uniformId, DType const& _value)        \
        {                                                                                                     \
            if (Material* material = Material::GetResource(_materialId))                                      \
            {                                                                                                 \
                ShaderUniformVariantPtr const& uniform = material->getUniform(_uniformId);                    \
                if (uniform)                                                                                  \
                    uniform->set(_value);                                                                     \
            }                                                                                                 \
        }
    IMPLEMENT_MATERIAL_UNIFORM_SET(S32);
    IMPLEMENT_MATERIAL_UNIFORM_SET(F32);
    IMPLEMENT_MATERIAL_UNIFORM_SET(F64);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Bool);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec2F);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec3F);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec4F);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec2S);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec3S);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec4S);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec2U);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec3U);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec4U);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec2B);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec3B);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Vec4B);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Mat3F);
    IMPLEMENT_MATERIAL_UNIFORM_SET(Mat4F);
    IMPLEMENT_MATERIAL_UNIFORM_SET(TMat);


    //////////////////////////////////////////
    inline void MaterialUniformSetColorF128(S32 _materialId, S32 _uniformId, Vec4F const& _value)
    {
        if (Material* material = Material::GetResource(_materialId))
        {
            ShaderUniformVariantPtr const& uniform = material->getUniform(_uniformId);
            if (uniform)
                uniform->set(ColorF128(_value));
        }
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsGraphics()
    {
        // MeshRenderer
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetMaterialAssetUnit);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetMaterialResourceId);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererGetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererGetRenderMesh);

        // MeshRendererInstanced
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedSetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedGetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedSetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedGetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedResizeModelMatrices);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedSetModelMatrix);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedResizeColors);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererInstancedSetColor);

        // SkinnedMeshRenderer
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererSetMaterialAssetUnit);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererSetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererGetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererSetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererGetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererPlayAnimation);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererGetPlayerAnimationTime);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererGetPlayerCurrentTime);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererPlayerRewindTo);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererPlayerStop);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererGetAnimationSpeed);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SkinnedMeshRendererSetAnimationSpeed);

        // SpriteRenderer2D
        MAZE_GRAPHICS_MONO_BIND_FUNC(SpriteRenderer2DSetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SpriteRenderer2DGetMaterial);

        // Camera3D
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetOrthographicSize);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetOrthographicSize);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetAspectRatio);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetNearZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetNearZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetFarZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetFarZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetRenderMask);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetRenderMask);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DConvertViewportCoordsToRay);
        
        // SubMesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateSubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroySubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetRenderDrawTopology);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetIndicesU16);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetIndicesU32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetPositions);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetNormals);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTangents);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBitangents);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetColors);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTexCoordsVec2F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTexCoordsVec4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBlendWeights);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBlendIndices);

        // Mesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshAddSubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshClear);

        // RenderMesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(RenderMeshLoadFromMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(RenderMeshClear);

        // RenderWindow
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateRenderWindow);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyRenderWindow);

        // EcsRenderScene
        MAZE_GRAPHICS_MONO_BIND_FUNC(EcsRenderSceneGetRenderTarget);

        // Canvas
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasGetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasSetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasGetRenderTargetRect);
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasConvertRenderTargetCoordsToViewportCoords);

        // AbstractTextRenderer
        MAZE_GRAPHICS_MONO_BIND_FUNC(AbstractTextRendererGetText);
        MAZE_GRAPHICS_MONO_BIND_FUNC(AbstractTextRendererSetText);

        // GlobalShaderUniform
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformIdIsValid);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformEnsure);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetS32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetF32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetF64);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetBool);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec2F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec3F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec2S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec3S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec4S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec2U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec3U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec4U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec2B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec3B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetVec4B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetMat3F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetMat4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetTMat);
        MAZE_GRAPHICS_MONO_BIND_FUNC(GlobalShaderUniformSetColorF128);

        // Material
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialIsValid);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialCreateCopy);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialEnsureUniformIndex);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetS32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetF32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetF64);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetBool);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec2F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec3F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec2S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec3S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec4S);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec2U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec3U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec4U);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec2B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec3B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetVec4B);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetMat3F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetMat4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetTMat);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MaterialUniformSetColorF128);
        
    }

} // namespace Maze
//////////////////////////////////////////
