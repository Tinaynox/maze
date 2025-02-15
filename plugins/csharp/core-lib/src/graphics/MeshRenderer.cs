using Maze.Core;

namespace Maze.Graphics
{
    public class MeshRenderer : NativeComponent
    {
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
