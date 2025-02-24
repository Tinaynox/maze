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

        public MeshRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public void SetMaterial(AssetUnitId auid)
        {
            InternalCalls.MeshRendererSetMaterial(NativeComponentPtr, auid);
        }
    }
}
