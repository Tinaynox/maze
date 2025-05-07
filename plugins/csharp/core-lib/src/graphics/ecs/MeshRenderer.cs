using Maze.Core;

namespace Maze.Graphics
{
    public class MeshRenderer : NativeComponent
    {
        public RenderMesh RenderMesh
        {
            get { InternalCalls.MeshRendererGetRenderMesh(NativeComponentPtr, out int resId); return new RenderMesh(resId); }
            set { InternalCalls.MeshRendererSetRenderMesh(NativeComponentPtr, value.ResourceId); }
        }

        public Material Material
        {
            get { InternalCalls.MeshRendererGetMaterial(NativeComponentPtr, out int resId); return new Material(resId); }
            set { InternalCalls.MeshRendererSetMaterial(NativeComponentPtr, value.ResourceId); }
        }

        public MeshRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public void SetMaterial(AssetUnitId auid)
        {
            InternalCalls.MeshRendererSetMaterialAssetUnit(NativeComponentPtr, auid);
        }
    }
}
