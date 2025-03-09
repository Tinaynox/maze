using System;
using System.Runtime.CompilerServices;

namespace Maze.Editor
{
    public class InternalCalls
    {
        #region MenuBar
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MenuBarAddOption(string menuName, string option);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MenuBarClearOptions();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static sbyte GetEditorMainSceneEcsWorld();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int GetEditorOpenedScene();
        #endregion

    }
}
