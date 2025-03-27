
using Maze.Core;

namespace Maze.Graphics
{
    public static class GraphicsHelper
    {
        public static RenderTarget GetRenderTarget(NativePtr nativeComponentPtr)
        {
            return new RenderTarget(InternalCalls.EcsRenderSceneGetRenderTarget(nativeComponentPtr));
        }
    }
}
