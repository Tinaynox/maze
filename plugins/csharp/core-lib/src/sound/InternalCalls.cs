using Maze.Core;
using System.Runtime.CompilerServices;

namespace Maze.Sound
{
    public static class InternalCalls
    {
        #region AudioSource
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string AudioSourceGetClipName(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetClipByName(NativePtr nativeComponentPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetVolume(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetVolume(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetPitch(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetPitch(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetLoop(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetLoop(NativePtr nativeComponentPtr, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetMute(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetMute(NativePtr nativeComponentPtr, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetPlayOnAwake(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetPlayOnAwake(NativePtr nativeComponentPtr, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetSpatialBlend(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetSpatialBlend(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetMinDistance(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetMinDistance(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetMaxDistance(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetMaxDistance(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceGetRolloffFactor(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceSetRolloffFactor(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourcePlay(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceStop(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioSourceIsPlaying(NativePtr nativeComponentPtr, out bool outValue);
        #endregion

        #region AudioListener
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioListenerGetActive(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioListenerSetActive(NativePtr nativeComponentPtr, bool value);
        #endregion
    }
}
