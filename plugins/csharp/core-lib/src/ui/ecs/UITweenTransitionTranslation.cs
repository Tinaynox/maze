using Maze.Core;

namespace Maze.UI
{
    public class UITweenTransitionTranslation : NativeComponent
    {
        public UITweenTransitionTranslation(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public bool Hidden
        {
            get { InternalCalls.UITweenTransitionTranslationGetHidden(NativeComponentPtr, out bool value); return value; }
            set { InternalCalls.UITweenTransitionTranslationSetHidden(NativeComponentPtr, value); }
        }

        public void SetHidden(bool value, bool resetProgress = false) => InternalCalls.UITweenTransitionTranslationSetHidden(NativeComponentPtr, value, resetProgress);
    }
}
