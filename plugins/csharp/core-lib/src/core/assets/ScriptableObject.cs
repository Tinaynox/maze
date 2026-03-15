using System;
using System.Runtime.InteropServices;
using Maze;

namespace Maze.Core
{
    public class ScriptableObject : IndexedResource
    {
        public ScriptableObject(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        public override void ReleaseResource() { }


        public static ScriptableObject Get(AssetUnitId assetUnitId)
        {
            return InternalCalls.GetScriptableObject(assetUnitId.Id);
        }

        public static ScriptableObject Get(string assetUnitName)
        {
            return Get(AssetUnitId.Get(assetUnitName));
        }

        public static T Get<T>(AssetUnitId assetUnitId) where T : ScriptableObject
        {
            ScriptableObject scriptableObject = Get(assetUnitId);
            return scriptableObject as T;
        }

        public static T Get<T>(string assetUnitName) where T : ScriptableObject
        {
            ScriptableObject scriptableObject = Get(assetUnitName);
            return scriptableObject as T;
        }
    }
}
