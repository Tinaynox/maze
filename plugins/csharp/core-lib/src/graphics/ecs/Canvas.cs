using Maze.Core;

namespace Maze.Graphics
{
    public class Canvas : NativeComponent
    {
        public int RenderTarget
        {
            get { InternalCalls.CanvasGetRenderTarget(NativeComponentPtr, out int value); return value; }
            set { InternalCalls.CanvasSetRenderTarget(NativeComponentPtr, value); }
        }

        public Canvas(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

    }
}
