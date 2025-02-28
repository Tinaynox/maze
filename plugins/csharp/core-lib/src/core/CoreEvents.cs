using System;

namespace Maze.Core
{
    public class PostUpdateEvent : NativeEvent
    {
        float m_DeltaTime;
        public float DeltaTime => m_DeltaTime;
    }
}
