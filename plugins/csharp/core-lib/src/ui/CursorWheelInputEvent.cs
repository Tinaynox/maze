using Maze.Core;
using System;
using System.Runtime.InteropServices;

namespace Maze.UI
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CursorWheelInputEvent
    {
        public int Index;
        public Vec2F Position;
        public float DeltaWheel;
        public int CapturerHandle;
        public int WindowId;
        public int CanvasId;
        public int RootCanvasId;

        public CursorWheelInputEvent(
            int index,
            Vec2F position,
            float deltaWheel,
            int capturerHandle,
            int windowId,
            int canvasId,
            int rootCanvasId)
        {
            Index = index;
            Position = position;
            DeltaWheel = deltaWheel;
            CapturerHandle = capturerHandle;
            WindowId = windowId;
            CanvasId = canvasId;
            RootCanvasId = rootCanvasId;
        }
    }
}
