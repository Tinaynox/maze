using Maze.Core;

namespace Maze.Graphics
{
    // Mirrors Maze::FoliageInstance, in the local space of the FoliageRenderer entity
    public struct FoliageInstanceData
    {
        public Vec3F Position;

        // Quaternion (x, y, z, w)
        public Vec4F Rotation;

        public float Scale;
    }


    // Shared in-memory foliage data, addressed by the asset file id. Calls on data that is not loaded do nothing
    public static class FoliageData
    {
        public static bool AddInstance(uint dataFileId, uint prefabAuid, FoliageInstanceData instance)
        {
            return InternalCalls.FoliageDataAddInstance(dataFileId, prefabAuid, ref instance.Position, ref instance.Rotation, instance.Scale);
        }

        public static bool RemoveInstance(uint dataFileId, uint prefabAuid, Vec3F localPosition)
        {
            return InternalCalls.FoliageDataRemoveInstance(dataFileId, prefabAuid, ref localPosition);
        }
    }


    public class FoliageRenderer : NativeComponent
    {
        public const string DataFileExtension = ".mzfoliage";

        // Instances are copied as 8 floats: position (3), rotation (4), scale (1)
        const int c_FloatsPerInstance = 8;

        public uint DataFileId
        {
            get => InternalCalls.FoliageRendererGetDataFileId(NativeComponentPtr);
            set => InternalCalls.FoliageRendererSetDataFileId(NativeComponentPtr, value);
        }

        public bool HasData => InternalCalls.FoliageRendererHasData(NativeComponentPtr);

        public int LayersCount => InternalCalls.FoliageRendererGetLayersCount(NativeComponentPtr);

        public FoliageRenderer(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        // Creates the data file (or reuses an existing one) and assigns it. Editor only
        public bool CreateDataFile(string fullPath)
        {
            return InternalCalls.FoliageRendererCreateDataFile(NativeComponentPtr, fullPath);
        }

        public uint GetLayerPrefabAuid(int layerIndex)
        {
            return InternalCalls.FoliageRendererGetLayerPrefabAuid(NativeComponentPtr, layerIndex);
        }

        public int GetInstancesCount(uint prefabAuid)
        {
            return InternalCalls.FoliageRendererGetInstancesCount(NativeComponentPtr, prefabAuid);
        }

        // The world transform is converted to the local space and quantized, the stored instance is returned
        public bool AddInstance(uint prefabAuid, TMat worldTransform, out FoliageInstanceData instance)
        {
            instance = default(FoliageInstanceData);
            return InternalCalls.FoliageRendererAddInstanceWorld(
                NativeComponentPtr,
                prefabAuid,
                ref worldTransform,
                out instance.Position,
                out instance.Rotation,
                out instance.Scale);
        }

        public bool AddInstance(uint prefabAuid, FoliageInstanceData instance)
        {
            return InternalCalls.FoliageRendererAddInstance(
                NativeComponentPtr,
                prefabAuid,
                ref instance.Position,
                ref instance.Rotation,
                instance.Scale);
        }

        public bool RemoveInstance(uint prefabAuid, Vec3F localPosition)
        {
            return InternalCalls.FoliageRendererRemoveInstance(NativeComponentPtr, prefabAuid, ref localPosition);
        }

        public FoliageInstanceData[] GetInstances(uint prefabAuid)
        {
            int count = GetInstancesCount(prefabAuid);
            if (count <= 0)
                return new FoliageInstanceData[0];

            float[] values = new float[count * c_FloatsPerInstance];
            int written = InternalCalls.FoliageRendererCopyInstances(NativeComponentPtr, prefabAuid, values);

            FoliageInstanceData[] result = new FoliageInstanceData[written];
            for (int i = 0; i < written; ++i)
            {
                int o = i * c_FloatsPerInstance;
                result[i].Position = new Vec3F(values[o], values[o + 1], values[o + 2]);
                result[i].Rotation = new Vec4F(values[o + 3], values[o + 4], values[o + 5], values[o + 6]);
                result[i].Scale = values[o + 7];
            }

            return result;
        }
    }
}
