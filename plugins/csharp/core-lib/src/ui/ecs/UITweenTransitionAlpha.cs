using Maze.Core;

namespace Maze.UI
{
    public class UITweenTransitionAlpha : NativeComponent
    {
        public UITweenTransitionAlpha(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public bool Hidden
        {
            get { InternalCalls.UITweenTransitionAlphaGetHidden(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.UITweenTransitionAlphaSetHidden(NativeComponentPtr, value); }
        }

        public void SetHidden(bool value, bool resetProgress = false) => InternalCalls.UITweenTransitionAlphaSetHidden(NativeComponentPtr, value, resetProgress);

        public void AddDelayTimer(float time) => InternalCalls.UITweenTransitionAlphaAddDelayTimer(NativeComponentPtr, time);
        public void SetDelayTimer(float time) => InternalCalls.UITweenTransitionAlphaSetDelayTimer(NativeComponentPtr, time);
    }
}
