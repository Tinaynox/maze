using System;
using Maze;


namespace Sandbox
{
    public class Player2 : MonoBehaviour
    {
        Transform3D transform;

        public void OnCreate()
        {
            transform = GetComponent<Transform3D>();
        }
        
        public void OnUpdate(float _dt)
        {
            transform.Translate(new Vec3F(0.2f * _dt, 0.2f * _dt, 0.0f));
        }
    }
}
