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

        public float ShowTime
        {
            get { InternalCalls.UITweenTransitionTranslationGetShowTime(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.UITweenTransitionTranslationSetShowTime(NativeComponentPtr, value); }
        }

        public float HideKoef
        {
            get { InternalCalls.UITweenTransitionTranslationGetHideKoef(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.UITweenTransitionTranslationSetHideKoef(NativeComponentPtr, value); }
        }

        public Vec2F HiddenPosition
        {
            get { InternalCalls.UITweenTransitionTranslationGetHiddenPosition(NativeComponentPtr, out Vec2F value); return value; }
            set { InternalCalls.UITweenTransitionTranslationSetHiddenPosition(NativeComponentPtr, ref value); }
        }

        public Vec2F ShownPosition
        {
            get { InternalCalls.UITweenTransitionTranslationGetShownPosition(NativeComponentPtr, out Vec2F value); return value; }
            set { InternalCalls.UITweenTransitionTranslationSetShownPosition(NativeComponentPtr, ref value); }
        }

        public void SetHidden(bool value, bool resetProgress = false) => InternalCalls.UITweenTransitionTranslationSetHidden(NativeComponentPtr, value, resetProgress);
    }
}
