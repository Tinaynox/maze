using Maze.Core;

namespace Maze.Graphics
{
    public class MeshRenderer : NativeComponent
    {
        public MeshRenderer(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

        public void SetMaterial(AssetUnitId _auid)
        {
            InternalCalls.MeshRendererSetMaterial(NativeComponentPtr, _auid);
        }
    }
}
