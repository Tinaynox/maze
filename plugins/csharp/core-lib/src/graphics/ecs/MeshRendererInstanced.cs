using Maze.Core;

namespace Maze.Graphics
{
    public class MeshRendererInstanced : NativeComponent
    {
        public RenderMesh RenderMesh
        {
            get { InternalCalls.MeshRendererInstancedGetRenderMesh(NativeComponentPtr, out int resId); return new RenderMesh(resId); }
            set { InternalCalls.MeshRendererInstancedSetRenderMesh(NativeComponentPtr, value.ResourceId); }
        }

        public Material Material
        {
            get { InternalCalls.MeshRendererInstancedGetMaterial(NativeComponentPtr, out int resId); return new Material(resId); }
            set { InternalCalls.MeshRendererInstancedSetMaterial(NativeComponentPtr, value.ResourceId); }
        }

        public MeshRendererInstanced(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public void ResizeModelMatrices(int size)
        {
            InternalCalls.MeshRendererInstancedResizeModelMatrices(NativeComponentPtr, size);
        }

        public void SetModelMatrix(int index, TMat tm)
        {
            InternalCalls.MeshRendererInstancedSetModelMatrix(NativeComponentPtr, index, ref tm);
        }

        public void ResizeColors(int size)
        {
            InternalCalls.MeshRendererInstancedResizeColors(NativeComponentPtr, size);
        }

        public void SetColor(int index, Vec4F color)
        {
            InternalCalls.MeshRendererInstancedSetColor(NativeComponentPtr, index, ref color);
        }
    }
}
