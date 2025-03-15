using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Rect2F
    {
        public float X;
        public float Y;
        public float W;
        public float H;

        public Rect2F(
            float x = 0.0f,
            float y = 0.0f,
            float w = 0.0f,
            float h = 0.0f)
        {
            X = x;
            Y = y;
            W = w;
            H = h;
        }

        public override string ToString()
        {
            return $"[{X}, {Y}][{W}, {H}]";
        }
    }
}
