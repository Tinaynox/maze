using Maze;
using Maze.Core;
using System;

public class Ball : GameObject
{

    bool m_Active = false;
    Vec2F m_Direction = new Vec2F(0.0f, 1.0f);
    public Vec2F Direction { get => m_Direction; set => m_Direction = value; }


    public override GameObjectType GetGameObjectType() => GameObjectType.Ball;


    [SerializeField]
    float m_Speed = 10.0f;

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
        if (m_Active)
        {
            Vec2F speedVec = m_Direction * m_Speed;
            Vec2F deltaPos = speedVec * _dt;
            m_Transform.Translate(new Vec3F(deltaPos.X, deltaPos.Y));

            Vec2F fieldHalfSize = GameController.Instance.FieldSize * 0.5f;

            if (Transform.Y < Transform.Scale.Y * 0.5f)
            {
                GameController.Instance.LoadLevel();
                return;
            }
            else
            if (Transform.Y > GameController.Instance.FieldSize.Y - Transform.Scale.Y * 0.5f)
            {
                Transform.Y = GameController.Instance.FieldSize.Y - Transform.Scale.Y * 0.5f;
                m_Direction = m_Direction.Reflect(new Vec2F(0.0f, -1.0f));
            }

            if (Transform.X < -fieldHalfSize.X + Transform.Scale.X * 0.5f)
            {
                Transform.X = -fieldHalfSize.X + Transform.Scale.X * 0.5f;
                m_Direction = m_Direction.Reflect(new Vec2F(1.0f, 0.0f));

                if (Math.Abs(m_Direction.Y) < 0.25f)
                {
                    m_Direction.Y *= 4.0f;
                    m_Direction.Normalize();
                }
            }
            else
            if (Transform.X > fieldHalfSize.X - Transform.Scale.X * 0.5f)
            {
                Transform.X = fieldHalfSize.X - Transform.Scale.X * 0.5f;
                m_Direction = m_Direction.Reflect(new Vec2F(-1.0f, 0.0f));

                if (Math.Abs(m_Direction.Y) < 0.25f)
                {
                    m_Direction.Y *= 4.0f;
                    m_Direction.Normalize();
                }
            }

            if (GameController.Instance.OverlapTest(this, out CollisionTestResult testResult))
            {
                switch (testResult.obj.GetGameObjectType())
                {
                    case GameObjectType.Paddle:
                    {
                        if (Transform.Y < testResult.obj.Transform.Y + testResult.obj.Transform.Scale.Y * 0.5f + Transform.Scale.Y * 0.5f)
                            Transform.Y = testResult.obj.Transform.Y + testResult.obj.Transform.Scale.Y * 0.5f + Transform.Scale.Y * 0.5f;

                        Vec2F toPaddle = testResult.obj.Transform.Position.XY - Transform.Position.XY;
                        toPaddle.Normalize();
                        m_Direction = -toPaddle;
                        break;
                    }
                    case GameObjectType.Brick:
                    {
                        m_Transform.Translate(new Vec3F(-deltaPos.X, -deltaPos.Y));

                        if (Math.Abs(m_Direction.Y) > Math.Abs(m_Direction.X) ||
                            Math.Abs(m_Transform.Y - testResult.obj.Transform.Y) >= (testResult.obj.Transform.Scale.Y * 0.5f + Transform.Scale.Y * 0.5f))
                        {
                            if (m_Direction.Y > 0.0f)
                            {
                                m_Direction = m_Direction.Reflect(new Vec2F(0.0f, -1.0f));
                            }
                            else
                            {
                                m_Direction = m_Direction.Reflect(new Vec2F(0.0f, 1.0f));
                            }
                        }
                        else
                        {
                            if (m_Direction.X > 0.0f)
                            {
                                m_Direction = m_Direction.Reflect(new Vec2F(-1.0f, 0.0f));
                            }
                            else
                            {
                                m_Direction = m_Direction.Reflect(new Vec2F(1.0f, 0.0f));
                            }
                        }

                        ((Brick)testResult.obj).Damage();
                        break;
                    }
                    case GameObjectType.Ball:
                    {
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    public void Activate()
    {
        m_Active = true;
    }
}
