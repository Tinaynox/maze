using Maze.Core;

namespace Maze.Sound
{
    public class AudioListener : NativeComponent
    {
        public bool Active
        {
            get { InternalCalls.AudioListenerGetActive(NativeComponentPtr, out bool value); return value; }
            set => InternalCalls.AudioListenerSetActive(NativeComponentPtr, value);
        }

        public AudioListener(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }
    }
}
