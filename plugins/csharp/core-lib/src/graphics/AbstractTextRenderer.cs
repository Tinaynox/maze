using Maze.Core;

namespace Maze.Graphics
{
    public class AbstractTextRenderer : NativeComponent
    {
        public string Text
        {
            get { InternalCalls.AbstractTextRendererGetText(NativeComponentPtr, out string value); return value; }
            set { InternalCalls.AbstractTextRendererSetText(NativeComponentPtr, value); }
        }

        public AbstractTextRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

    }
}
