using Maze.Core;

namespace Maze.Graphics
{

    public class SkinnedMeshRenderer : NativeComponent
    {
        public RenderMesh RenderMesh
        {
            get { InternalCalls.SkinnedMeshRendererGetRenderMesh(NativeComponentPtr, out int resId); return new RenderMesh(resId); }
            set { InternalCalls.SkinnedMeshRendererSetRenderMesh(NativeComponentPtr, value.ResourceId); }
        }

        public Material Material
        {
            get { InternalCalls.SkinnedMeshRendererGetMaterial(NativeComponentPtr, out int resId); return new Material(resId); }
            set { InternalCalls.SkinnedMeshRendererSetMaterial(NativeComponentPtr, value.ResourceId); }
        }

        public SkinnedMeshRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public void SetMaterial(AssetUnitId auid)
        {
            InternalCalls.SkinnedMeshRendererSetMaterialAssetUnit(NativeComponentPtr, auid);
        }

        public void SetMaterial(int idx, AssetUnitId auid)
        {
            InternalCalls.SkinnedMeshRendererSetMaterialAssetUnitAt(NativeComponentPtr, idx, auid);
        }

        public void SetMaterial(int idx, int resourceId)
        {
            InternalCalls.SkinnedMeshRendererSetMaterialResourceIdAt(NativeComponentPtr, idx, resourceId);
        }

        public void SetMaterial(int idx, Material material)
        {
            SetMaterial(idx, material?.ResourceId ?? -1);
        }

        public void ResizeMaterialsCount(uint count)
        {
            InternalCalls.SkinnedMeshRendererResizeMaterialsCount(NativeComponentPtr, count);
        }
    }
}
