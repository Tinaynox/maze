using Maze.Core;

namespace Maze.Graphics
{
    public class Camera3D : NativeComponent
    {
        public int RenderTarget
        {
            get { InternalCalls.Camera3DGetRenderTarget(NativeComponentPtr, out int value); return value; }
            set { InternalCalls.Camera3DSetRenderTarget(NativeComponentPtr, value); }
        }

        public float AspectRatio
        {
            get { InternalCalls.Camera3DGetAspectRatio(NativeComponentPtr, out float value); return value; }
        }

        public float OrthographicSize
        {
            get { InternalCalls.Camera3DGetOrthographicSize(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Camera3DSetOrthographicSize(NativeComponentPtr, value); }
        }

        public float NearZ
        {
            get { InternalCalls.Camera3DGetNearZ(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Camera3DSetNearZ(NativeComponentPtr, value); }
        }

        public float FarZ
        {
            get { InternalCalls.Camera3DGetFarZ(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Camera3DSetFarZ(NativeComponentPtr, value); }
        }

        public int RenderMask
        {
            get { InternalCalls.Camera3DGetRenderMask(NativeComponentPtr, out int value); return value; }
            set { InternalCalls.Camera3DSetRenderMask(NativeComponentPtr, value); }
        }

        public Camera3D(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

    }
}
