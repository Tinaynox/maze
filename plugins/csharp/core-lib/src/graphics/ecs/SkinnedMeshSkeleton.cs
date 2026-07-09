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

        public const int InvalidBoneIndex = -1;

        // Resolves a bone name to its index within the skeleton, for repeated lookups
        // via TryGetBoneWorldPosition(int, ...) without re-hashing the name every time.
        // Returns InvalidBoneIndex if the bone name is not found.
        public int FindBoneIndex(string boneName)
        {
            return InternalCalls.SkinnedMeshSkeletonFindBoneIndex(NativeComponentPtr, boneName);
        }

        // Current (post-animation) world-space position of a bone, resolved by name.
        // Returns false if the bone name is not found in the skeleton.
        public bool TryGetBoneWorldPosition(string boneName, out Vec3F outPosition)
        {
            return InternalCalls.SkinnedMeshSkeletonGetBoneWorldPosition(NativeComponentPtr, boneName, out outPosition);
        }

        // Current (post-animation) world-space position of a bone, resolved by index
        // (see FindBoneIndex). Cheaper than the name-based overload for repeated queries.
        public bool TryGetBoneWorldPosition(int boneIndex, out Vec3F outPosition)
        {
            return InternalCalls.SkinnedMeshSkeletonGetBoneWorldPositionByIndex(NativeComponentPtr, boneIndex, out outPosition);
        }
    }
}
