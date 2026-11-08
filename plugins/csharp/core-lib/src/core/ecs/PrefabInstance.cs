namespace Maze.Core
{
    public class PrefabInstance : NativeComponent
    {
        public AssetUnitId AssetUnitId => new AssetUnitId(InternalCalls.PrefabInstanceGetAssetUnitId(NativeComponentPtr));

        public PrefabInstance(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }
    }
}
