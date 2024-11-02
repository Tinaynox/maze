using Maze;
using Maze.Core;

namespace Sandbox
{
    public class TestRotor : MonoBehaviour
    {
        public float Speed { get; set; } = 3.0f;

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
            transform.Rotate(new Vec3F(1.0f, 0.0f, 0.0f), -Speed * _dt);
        }
    }
}
