using Maze.Core;

namespace Maze.Graphics
{
    public enum MeshSkeletonAnimationStartFlags
    {
        None = 0,
        Looped                      = 1 << (0),
        Additive                    = 1 << (1),
        StopCurrentAnimations       = 1 << (2),
        Important                   = 1 << (3),
        PauseEnding                 = 1 << (4),
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


    public class SkinnedMeshSkeleton : NativeComponent
    {
        public RenderMesh RenderMesh
        {
            get { InternalCalls.SkinnedMeshSkeletonGetRenderMesh(NativeComponentPtr, out int resId); return new RenderMesh(resId); }
            set { InternalCalls.SkinnedMeshSkeletonSetRenderMesh(NativeComponentPtr, value.ResourceId); }
        }

        public float AnimationSpeed
        {
            get { return InternalCalls.SkinnedMeshSkeletonGetAnimationSpeed(NativeComponentPtr); }
            set { InternalCalls.SkinnedMeshSkeletonSetAnimationSpeed(NativeComponentPtr, value); }
        }

        public SkinnedMeshSkeleton(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }
        public int PlayAnimation(
            string animationName,
            float blendTime = 0.2f,
            float weight = 1.0f,
            float speed = 1.0f,
            bool loop = true,
            bool additive = false,
            bool stopCurrentAnimations = true,
            bool important = true,
            bool pauseEnding = false)
        {
            return InternalCalls.SkinnedMeshSkeletonPlayAnimation(
                NativeComponentPtr,
                animationName,
                blendTime,
                weight,
                speed,
                loop,
                additive,
                stopCurrentAnimations,
                important,
                pauseEnding);
        }

        public int PlayAnimation(
            string animationName,
            MeshSkeletonAnimationStartParams param)
        {
            return InternalCalls.SkinnedMeshSkeletonPlayAnimation(
                NativeComponentPtr,
                animationName,
                param.BlendTime,
                param.Weight,
                param.Speed,
                (param.Flags & MeshSkeletonAnimationStartFlags.Looped) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.Additive) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.StopCurrentAnimations) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.Important) != 0u,
                (param.Flags & MeshSkeletonAnimationStartFlags.PauseEnding) != 0u);
        }

        public float GetPlayerAnimationTime(int playerIndex)
        {
            if (InternalCalls.SkinnedMeshSkeletonGetPlayerAnimationTime(NativeComponentPtr, playerIndex, out float outAnimationTime))
                return outAnimationTime;
            return 0.0f;
        }

        public float GetPlayerCurrentTime(int playerIndex)
        {
            if (InternalCalls.SkinnedMeshSkeletonGetPlayerCurrentTime(NativeComponentPtr, playerIndex, out float outCurrentTime))
                return outCurrentTime;
            return 0.0f;
        }

        public void RewindPlayerTo(int playerIndex, float time)
        {
            InternalCalls.SkinnedMeshSkeletonPlayerRewindTo(NativeComponentPtr, playerIndex, time);
        }

        public void StopPlayer(int playerIndex)
        {
            InternalCalls.SkinnedMeshSkeletonPlayerStop(NativeComponentPtr, playerIndex);
        }
    }
}
