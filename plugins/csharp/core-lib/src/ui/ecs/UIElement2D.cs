using Maze.Core;

namespace Maze.UI
{
    public class UIElement2D : NativeComponent
    {
        public int Flags
        {
            get { InternalCalls.UIElement2DGetFlags(NativeComponentPtr, out int flags); return flags; }
            set { InternalCalls.UIElement2DSetFlags(NativeComponentPtr, value); }
        }

        public bool Focused
        {
            get { InternalCalls.UIElement2DGetFocused(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.UIElement2DSetFocused(NativeComponentPtr, value); }
        }

        public bool Pressed
        {
            get { InternalCalls.UIElement2DGetPressed(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.UIElement2DSetPressed(NativeComponentPtr, value); }
        }

        public UIElement2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        // Returns true if this element is the top-most element under renderTargetCoords
        // (not covered by another cursor-capturing element)
        public bool TraceElement(Vec2F renderTargetCoords, int[] ignoreEntityIds = null)
        {
            return InternalCalls.UIElement2DTraceElement(NativeComponentPtr, ref renderTargetCoords, ignoreEntityIds);
        }
    }
}
