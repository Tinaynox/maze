using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.Graphics
{
    public static class InternalCalls
    {
        #region MeshRenderer
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetMaterialAssetUnit(NativePtr nativeComponentPtr, AssetUnitId material);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetMaterial(NativePtr nativeComponentPtr, int materialResourceId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererGetMaterial(NativePtr nativeComponentPtr, out int outMaterialResourceId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetRenderMesh(NativePtr nativeComponentPtr, int renderMeshResourceId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererGetRenderMesh(NativePtr nativeComponentPtr, out int outRenderMeshResourceId);
        #endregion

        #region SkinnedMeshRenderer
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererSetMaterialAssetUnit(NativePtr nativeComponentPtr, AssetUnitId material);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererSetMaterial(NativePtr nativeComponentPtr, int materialResourceId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererGetMaterial(NativePtr nativeComponentPtr, out int outMaterialResourceId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererSetRenderMesh(NativePtr nativeComponentPtr, int renderMeshResourceId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererGetRenderMesh(NativePtr nativeComponentPtr, out int outRenderMeshResourceId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SkinnedMeshRendererPlayAnimation(NativePtr nativeComponentPtr, string animationName);
        #endregion

        #region Camera3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetAspectRatio(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetOrthographicSize(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetOrthographicSize(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetNearZ(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetNearZ(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetFarZ(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetFarZ(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetRenderMask(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetRenderMask(NativePtr nativeComponentPtr, int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetRenderTarget(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetRenderTarget(NativePtr nativeComponentPtr, int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DConvertViewportCoordsToRay(NativePtr nativeComponentPtr, ref Vec2F positionViewport, out Ray ray);
        #endregion

        #region Mesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyMesh(int meshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshAddSubMesh(int meshId, int subMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshClear(int meshId);
        #endregion

        #region SubMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateSubMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroySubMesh(int subMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetRenderDrawTopology(int subMeshId, int topology);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetIndicesU16(int subMeshId, ushort* indices, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetIndicesU32(int subMeshId, uint* indices, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetPositions(int subMeshId, Vec3F* positions, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetNormals(int subMeshId, Vec3F* normals, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTangents(int subMeshId, Vec3F* tangents, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBitangents(int subMeshId, Vec3F* bitangents, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetColors(int subMeshId, Vec4F* colors, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTexCoordsVec2F(int subMeshId, int uvIndex, Vec2F* uvs, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTexCoordsVec4F(int subMeshId, int uvIndex, Vec4F* uvs, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBlendWeights(int subMeshId, Vec4F* blendWeights, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBlendIndices(int subMeshId, Vec4F* blendIndices, int count);
        #endregion

        #region RenderMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateRenderMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyRenderMesh(int renderMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void RenderMeshLoadFromMesh(int renderMeshId, int meshId);
        #endregion

        #region RenderWindow
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateRenderWindow(ref Vec2U windowSize, string title);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyRenderWindow(int resourceId);
        #endregion

        #region EcsRenderScene
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int EcsRenderSceneGetRenderTarget(NativePtr nativeComponentPtr);
        #endregion

        #region Canvas
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void CanvasGetRenderTarget(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void CanvasSetRenderTarget(NativePtr nativeComponentPtr, int value);
        #endregion

        #region AbstractTextRenderer
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void AbstractTextRendererGetText(NativePtr nativeComponentPtr, out string outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void AbstractTextRendererSetText(NativePtr nativeComponentPtr, string value);
        #endregion

        #region GlobalShaderUniform
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool GlobalShaderUniformIdIsValid(int globalShaderUniformId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int GlobalShaderUniformEnsure(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetS32(int globalShaderUniformId, ref int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetF32(int globalShaderUniformId, ref float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetF64(int globalShaderUniformId, ref double value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetBool(int globalShaderUniformId, ref bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec2F(int globalShaderUniformId, ref Vec2F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec3F(int globalShaderUniformId, ref Vec3F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec4F(int globalShaderUniformId, ref Vec4F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec2S(int globalShaderUniformId, ref Vec2S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec3S(int globalShaderUniformId, ref Vec3S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec4S(int globalShaderUniformId, ref Vec4S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec2U(int globalShaderUniformId, ref Vec2U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec3U(int globalShaderUniformId, ref Vec3U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec4U(int globalShaderUniformId, ref Vec4U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec2B(int globalShaderUniformId, ref Vec2B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec3B(int globalShaderUniformId, ref Vec3B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetVec4B(int globalShaderUniformId, ref Vec4B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetMat3F(int globalShaderUniformId, ref Mat3F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetMat4F(int globalShaderUniformId, ref Mat4F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetTMat(int globalShaderUniformId, ref TMat value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GlobalShaderUniformSetColorF128(int globalShaderUniformId, ref ColorF128 value);
        #endregion

        #region Material
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool MaterialIsValid(int materialId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int MaterialEnsureUniformIndex(int materialId, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetS32(int materialId, int uniformIndex, ref int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetF32(int materialId, int uniformIndex, ref float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetF64(int materialId, int uniformIndex, ref double value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetBool(int materialId, int uniformIndex, ref bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec2F(int materialId, int uniformIndex, ref Vec2F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec3F(int materialId, int uniformIndex, ref Vec3F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec4F(int materialId, int uniformIndex, ref Vec4F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec2S(int materialId, int uniformIndex, ref Vec2S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec3S(int materialId, int uniformIndex, ref Vec3S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec4S(int materialId, int uniformIndex, ref Vec4S value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec2U(int materialId, int uniformIndex, ref Vec2U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec3U(int materialId, int uniformIndex, ref Vec3U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec4U(int materialId, int uniformIndex, ref Vec4U value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec2B(int materialId, int uniformIndex, ref Vec2B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec3B(int materialId, int uniformIndex, ref Vec3B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetVec4B(int materialId, int uniformIndex, ref Vec4B value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetMat3F(int materialId, int uniformIndex, ref Mat3F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetMat4F(int materialId, int uniformIndex, ref Mat4F value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetTMat(int materialId, int uniformIndex, ref TMat value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MaterialUniformSetColorF128(int materialId, int uniformIndex, ref ColorF128 value);
        #endregion
    }
}
