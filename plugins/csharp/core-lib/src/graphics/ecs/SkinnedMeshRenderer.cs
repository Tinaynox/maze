using Maze.Core;

namespace Maze.Graphics
{
    public enum MeshSkeletonAnimationStartFlags
    {
        None = 0,
        Looped                      = 1 << (0),
        Additive                    = 1 << (1),
        StopCurrentAnimations       = 1 << (2),
        Important                   = 1 << (3)
    };

    public struct MeshSkeletonAnimationStartParams
    {
        public float BlendTime;
        public float Weight;
        public float Speed;
        public MeshSkeletonAnimationStartFlags Flags;

        public MeshSkeletonAnimationStartParams(
            float blendTime = 0.2f,
            float weight = 1.0f,
            float speed = 1.0f,
            MeshSkeletonAnimationStartFlags flags = 
                MeshSkeletonAnimationStartFlags.Looped |
                MeshSkeletonAnimationStartFlags.StopCurrentAnimations |
                MeshSkeletonAnimationStartFlags.Important)
        {
            BlendTime = blendTime;
            Weight = weight;
            Speed = speed;
            Flags = flags;
        }
    };


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

        public float AnimationSpeed
        {
            get { return InternalCalls.SkinnedMeshRendererGetAnimationSpeed(NativeComponentPtr); }
            set { InternalCalls.SkinnedMeshRendererSetAnimationSpeed(NativeComponentPtr, value); }
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
            float blendTime = 0.2f,
            float weight = 1.0f,
            float speed = 1.0f,
            bool loop = true,
            bool additive = false,
            bool stopCurrentAnimations = true,
            bool important = true)
        {
            return InternalCalls.SkinnedMeshRendererPlayAnimation(
                NativeComponentPtr,
                animationName,
                blendTime,
                weight,
                speed,
                loop,
                additive,
                stopCurrentAnimations,
                important);
        }

        public int PlayAnimation(
            string animationName,
            MeshSkeletonAnimationStartParams param)
        {
            return InternalCalls.SkinnedMeshRendererPlayAnimation(
                NativeComponentPtr,
                animationName,
                param.BlendTime,
                param.Weight,
                param.Speed,
                (param.Flags & MeshSkeletonAnimationStartFlags.Looped) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.Additive) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.StopCurrentAnimations) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.Important) != 0u);
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

        public void StopPlayer(int playerIndex)
        {
            InternalCalls.SkinnedMeshRendererPlayerStop(NativeComponentPtr, playerIndex);
        }
    }
}
