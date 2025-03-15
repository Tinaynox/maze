using Maze.Core;
using System;
using System.Runtime.InteropServices;

namespace Maze.UI
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CursorInputEvent
    {
        public int Type;
        public int Index;
        public Vec2F Position;
        public int CapturerHandle;
        public int Button;
        public int InputSource;
        public int WindowId;
        public int CanvasId;
        public int RootCanvasId;

        public CursorInputEvent(
            int type,
            int index,
            Vec2F position,
            int capturerHandle,
            int button,
            int inputSource,
            int windowId,
            int canvasId,
            int rootCanvasId)
        {
            Type = type;
            Index = index;
            Position = position;
            CapturerHandle = capturerHandle;
            Button = button;
            InputSource = inputSource;
            WindowId = windowId;
            CanvasId = canvasId;
            RootCanvasId = rootCanvasId;
        }
    }
}
