using Maze.Core;

namespace Maze.Graphics
{
    public class LineRenderer2D : NativeComponent
    {
        public float Width
        {
            get { InternalCalls.LineRenderer2DGetWidth(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.LineRenderer2DSetWidth(NativeComponentPtr, value);
        }

        public float FeatherWidth
        {
            get { InternalCalls.LineRenderer2DGetFeatherWidth(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.LineRenderer2DSetFeatherWidth(NativeComponentPtr, value);
        }

        public ColorU32 Color
        {
            get { InternalCalls.LineRenderer2DGetColor(NativeComponentPtr, out uint value); return new ColorU32(value); }
            set => InternalCalls.LineRenderer2DSetColor(NativeComponentPtr, value.ToUInt());
        }

        public LineRenderer2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetPositions(Vec2F[] positions, int count)
        {
            InternalCalls.LineRenderer2DSetPositions(NativeComponentPtr, positions, count);
        }

        public void SetPositions(Vec2F[] positions)
        {
            SetPositions(positions, positions != null ? positions.Length : 0);
        }
    }
}
