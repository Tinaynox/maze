using Maze.Core;

namespace Maze.UI
{
    public class TextRenderer2D : NativeComponent
    {
        public string Text
        {
            get => InternalCalls.TextRenderer2DGetText(NativeComponentPtr);
            set => InternalCalls.TextRenderer2DSetText(NativeComponentPtr, value);
        }

        public TextRenderer2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public Vec2F CalculateRequiredSizeForText(string text)
        {
            InternalCalls.TextRenderer2DCalculateRequiredSizeForText(NativeComponentPtr, text, out Vec2F outSize);
            return outSize;
        }

        public Vec2F CalculateRequiredSizeForText()
        {
            return CalculateRequiredSizeForText(Text);
        }
    }
}
