using System.Runtime.CompilerServices;

namespace Maze
{
    public class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void MazeLog(string _text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void MazeLogWarning(string _text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void MazeLogError(string _text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetKeyState(int _keyCode);
    }
}
