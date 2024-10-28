using System;

namespace Maze
{
    public class MonoBehaviour : Component
    {
        public MonoBehaviour()
        {
            Debug.Log("MonoBehaviour constructor!");
        }

        public virtual void OnCreate()
        {
            Debug.LogWarning("OnCreate!");
        }

        public virtual void OnUpdate(float _dt)
        {
            Debug.LogError($"OnUpdate {_dt}!");
        }
    }
}
