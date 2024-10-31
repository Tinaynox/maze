using System;
using Maze;


namespace Sandbox
{
    public class Player2 : MonoBehaviour
    {
        Transform3D transform;

        [EntitySystem]
        public void OnCreate()
        {
            transform = GetComponent<Transform3D>();
        }

        [EntitySystem(
            new string[] { "default" },
            default,
            new string[] { "Sandbox.Player::OnUpdate" })]
        public void OnUpdate(float _dt)
        {
            transform.Rotate(new Vec3F(1.0f, 0.0f, 0.0f), -3.0f * _dt);
        }
    }
}
