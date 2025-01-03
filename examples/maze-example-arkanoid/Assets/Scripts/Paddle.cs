using Maze;
using Maze.Core;

public class Paddle : GameObject
{
    [SerializeField]
    Ball m_AttachedBall;
    public Ball AttachedBall => m_AttachedBall;


    public override GameObjectType GetGameObjectType() => GameObjectType.Paddle;


    [EntitySystem]
    public void OnCreate()
    {
        m_Transform = GetComponent<Transform3D>();

        if (GameController.Instance != null)
            GameController.Instance.AddGameObject(this);
    }

    [EntitySystem]
    public void OnUpdate(float _dt)
    {
        if (m_AttachedBall != null && m_AttachedBall.Transform != null)
        {
            float paddleProgress = m_Transform.X / (GameController.Instance.FieldSize.X * 0.5f);

            m_AttachedBall.Transform.X = m_Transform.X + paddleProgress * m_Transform.Scale.X * 0.4f;
            m_AttachedBall.Transform.Y = m_Transform.Y + m_Transform.Scale.Y * 0.5f + m_AttachedBall.Transform.Scale.Y * 0.5f;
        }
    }

    public void LaunchBall()
    {
        if (m_AttachedBall == null)
            return;

        m_AttachedBall.Activate();

        Vec2F ballPos = m_AttachedBall.Transform.Position.XY;
        Vec2F toBallVec = ballPos - Transform.Position.XY;
        toBallVec.Normalize();

        m_AttachedBall.Direction = toBallVec;

        m_AttachedBall = null;
    }
}
