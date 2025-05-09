using Maze.Core;
using System.Runtime.InteropServices;


namespace Maze.Editor
{
    [StructLayout(LayoutKind.Sequential)]
    public class OpenEditorEvent : NativeEvent
    {
        public string editorName;
        public Vec2F testValue;
    }

    [StructLayout(LayoutKind.Sequential)]
    public class PlaytestModeEnabledChangedEvent : NativeEvent
    {
        public bool playtestModeEnabled;
    }
}