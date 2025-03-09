
using Maze.Core;

namespace Maze.Graphics
{
    public class GraphicsHelper
    {
        private GraphicsHelper() { }

        public static RenderTarget GetRenderTarget(NativePtr nativeComponentPtr)
        {
            return new RenderTarget(InternalCalls.EcsRenderSceneGetRenderTarget(nativeComponentPtr));
        }
    }
}
