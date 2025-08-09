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

        public Rect2F RenderTargetRect
        {
            get { InternalCalls.CanvasGetRenderTargetRect(NativeComponentPtr, out Rect2F value); return value; }
        }

        public Canvas(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

        public Vec2F ConvertRenderTargetCoordsToViewportCoords(Vec2F renderTargetPosition)
        {
            InternalCalls.CanvasConvertRenderTargetCoordsToViewportCoords(
                NativeComponentPtr,
                ref renderTargetPosition,
                out Vec2F outViewportCoords);
            return outViewportCoords;
        }
    }
}
