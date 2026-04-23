using Maze.Core;

namespace Maze.UI
{
    public class Layout2D : NativeComponent
    {
        public float PaddingLeft
        {
            get { InternalCalls.Layout2DGetPaddingLeft(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Layout2DSetPaddingLeft(NativeComponentPtr, value); }
        }

        public float PaddingRight
        {
            get { InternalCalls.Layout2DGetPaddingRight(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Layout2DSetPaddingRight(NativeComponentPtr, value); }
        }

        public float PaddingTop
        {
            get { InternalCalls.Layout2DGetPaddingTop(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Layout2DSetPaddingTop(NativeComponentPtr, value); }
        }

        public float PaddingBottom
        {
            get { InternalCalls.Layout2DGetPaddingBottom(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Layout2DSetPaddingBottom(NativeComponentPtr, value); }
        }

        public bool AutoWidth
        {
            get { InternalCalls.Layout2DGetAutoWidth(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.Layout2DSetAutoWidth(NativeComponentPtr, value); }
        }

        public bool AutoHeight
        {
            get { InternalCalls.Layout2DGetAutoHeight(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.Layout2DSetAutoHeight(NativeComponentPtr, value); }
        }
        
        public Layout2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void AlignChildren()
        {
            InternalCalls.Layout2DAlignChildren(NativeComponentPtr);
        }
    }
}
