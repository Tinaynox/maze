using Maze.Core;

namespace Maze.Sound
{
    public class AudioSource : NativeComponent
    {
        public string Clip
        {
            get => InternalCalls.AudioSourceGetClipName(NativeComponentPtr);
            set => InternalCalls.AudioSourceSetClipByName(NativeComponentPtr, value);
        }

        public float Volume
        {
            get { InternalCalls.AudioSourceGetVolume(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetVolume(NativeComponentPtr, value);
        }

        public float Pitch
        {
            get { InternalCalls.AudioSourceGetPitch(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetPitch(NativeComponentPtr, value);
        }

        public bool Loop
        {
            get { InternalCalls.AudioSourceGetLoop(NativeComponentPtr, out bool value); return value; }
            set => InternalCalls.AudioSourceSetLoop(NativeComponentPtr, value);
        }

        public bool Mute
        {
            get { InternalCalls.AudioSourceGetMute(NativeComponentPtr, out bool value); return value; }
            set => InternalCalls.AudioSourceSetMute(NativeComponentPtr, value);
        }

        public bool PlayOnAwake
        {
            get { InternalCalls.AudioSourceGetPlayOnAwake(NativeComponentPtr, out bool value); return value; }
            set => InternalCalls.AudioSourceSetPlayOnAwake(NativeComponentPtr, value);
        }

        // 0 = fully non-spatial (2D), 1 = fully 3D positional
        public float SpatialBlend
        {
            get { InternalCalls.AudioSourceGetSpatialBlend(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetSpatialBlend(NativeComponentPtr, value);
        }

        public float MinDistance
        {
            get { InternalCalls.AudioSourceGetMinDistance(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetMinDistance(NativeComponentPtr, value);
        }

        public float MaxDistance
        {
            get { InternalCalls.AudioSourceGetMaxDistance(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetMaxDistance(NativeComponentPtr, value);
        }

        public float RolloffFactor
        {
            get { InternalCalls.AudioSourceGetRolloffFactor(NativeComponentPtr, out float value); return value; }
            set => InternalCalls.AudioSourceSetRolloffFactor(NativeComponentPtr, value);
        }

        public bool IsPlaying
        {
            get { InternalCalls.AudioSourceIsPlaying(NativeComponentPtr, out bool value); return value; }
        }

        public void Play() => InternalCalls.AudioSourcePlay(NativeComponentPtr);

        public void Stop() => InternalCalls.AudioSourceStop(NativeComponentPtr);

        public AudioSource(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }
    }
}
