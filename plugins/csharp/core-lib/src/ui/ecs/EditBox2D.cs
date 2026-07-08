using Maze.Core;
using Maze.Graphics;

namespace Maze.UI
{
    public class EditBox2D : NativeComponent
    {
        public string Text
        {
            get => InternalCalls.EditBox2DGetText(NativeComponentPtr);
            set => InternalCalls.EditBox2DSetText(NativeComponentPtr, value);
        }

        public bool Selected
        {
            get { InternalCalls.EditBox2DGetSelected(NativeComponentPtr, out bool value); return value; }
            set => InternalCalls.EditBox2DSetSelected(NativeComponentPtr, value);
        }

        public int CursorPosition
        {
            get { InternalCalls.EditBox2DGetCursorPosition(NativeComponentPtr, out int value); return value; }
            set => InternalCalls.EditBox2DSetCursorPosition(NativeComponentPtr, value);
        }

        public int EventReceiverEid
        {
            get { InternalCalls.EditBox2DGetEventReceiverEid(NativeComponentPtr, out int value); return value; }
            set => InternalCalls.EditBox2DSetEventReceiverEid(NativeComponentPtr, value);
        }

        // Returns true if any EditBox2D in the application is currently selected for text editing.
        // Use this to skip global keyboard shortcuts (hotkeys, camera controls, etc) while the user is typing.
        public static bool IsTextInputActive => InternalCalls.EditBox2DIsTextInputActive();

        public EditBox2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetTextRenderer(AbstractTextRenderer2D textRenderer)
        {
            InternalCalls.EditBox2DSetTextRenderer(
                NativeComponentPtr,
                textRenderer != null ? textRenderer.GetNativePtr() : NativePtr.Zero);
        }
    }
}
