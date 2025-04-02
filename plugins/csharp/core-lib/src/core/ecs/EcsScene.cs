using System;
using Maze;

namespace Maze.Core
{
    public struct EcsScene
    {
        readonly int m_Id;
        public int Id => m_Id;
        public static EcsScene Invalid => new EcsScene(-1);

        public string AssetFilePath => GetAssetFilePath();

        public EcsScene(int sceneId = -1)
        {
            m_Id = sceneId;
        }

        public override bool Equals(object obj)
        {
            return m_Id.Equals(obj);
        }

        public override int GetHashCode()
        {
            return m_Id.GetHashCode();
        }

        public static bool operator ==(EcsScene value0, EcsScene value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(EcsScene value0, EcsScene value1)
        {
            return value0.m_Id != value1.m_Id;
        }

        public string GetAssetFilePath()
        {
            return InternalCalls.EcsSceneGetAssetFilePath(m_Id);
        }
    }
}
