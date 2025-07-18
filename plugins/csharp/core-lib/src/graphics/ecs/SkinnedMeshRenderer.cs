using Maze.Core;

namespace Maze.Graphics
{
    public class SkinnedMeshRenderer : NativeComponent
    {
        public RenderMesh RenderMesh
        {
            get { InternalCalls.SkinnedMeshRendererGetRenderMesh(NativeComponentPtr, out int resId); return new RenderMesh(resId); }
            set { InternalCalls.SkinnedMeshRendererSetRenderMesh(NativeComponentPtr, value.ResourceId); }
        }

        public Material Material
        {
            get { InternalCalls.SkinnedMeshRendererGetMaterial(NativeComponentPtr, out int resId); return new Material(resId); }
            set { InternalCalls.SkinnedMeshRendererSetMaterial(NativeComponentPtr, value.ResourceId); }
        }

        public SkinnedMeshRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public void SetMaterial(AssetUnitId auid)
        {
            InternalCalls.SkinnedMeshRendererSetMaterialAssetUnit(NativeComponentPtr, auid);
        }

        public int PlayAnimation(
            string animationName,
            bool loop = true,
            float blendTime = 0.2f)
        {
            return InternalCalls.SkinnedMeshRendererPlayAnimation(NativeComponentPtr, animationName, loop, blendTime);
        }

        public float GetPlayerAnimationTime(int playerIndex)
        {
            if (InternalCalls.SkinnedMeshRendererGetPlayerAnimationTime(NativeComponentPtr, playerIndex, out float outAnimationTime))
                return outAnimationTime;
            return 0.0f;
        }

        public float GetPlayerCurrentTime(int playerIndex)
        {
            if (InternalCalls.SkinnedMeshRendererGetPlayerCurrentTime(NativeComponentPtr, playerIndex, out float outCurrentTime))
                return outCurrentTime;
            return 0.0f;
        }

        public void RewindPlayerTo(int playerIndex, float time)
        {
            InternalCalls.SkinnedMeshRendererPlayerRewindTo(NativeComponentPtr, playerIndex, time);
        }
    }
}
