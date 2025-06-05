using Maze.Core;

namespace Maze.UI
{
    public class UITweenTransitionScale : NativeComponent
    {
        public UITweenTransitionScale(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public bool Hidden
        {
            get { InternalCalls.UITweenTransitionScaleGetHidden(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.UITweenTransitionScaleSetHidden(NativeComponentPtr, value); }
        }

        public void SetHidden(bool value, bool resetProgress = false) => InternalCalls.UITweenTransitionScaleSetHidden(NativeComponentPtr, value, resetProgress);
    }
}
