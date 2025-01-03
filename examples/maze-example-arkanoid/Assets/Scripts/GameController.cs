using Maze;
using Maze.Core;
using System.Collections.Generic;
using System;

public class GameController : MonoBehaviour
{
    [SerializeField]
    Vec2F m_FieldSize;
    public Vec2F FieldSize => m_FieldSize;

    [SerializeField]
    Paddle m_Paddle;

    [SerializeField]
    float m_PaddleSpeed = 4.0f;
    
    [SerializeField]
    AssetUnitId m_BrickPrefab;

    Transform3D m_RootTransform;
    List<GameObject> m_GameObjects = new List<GameObject>();


    static GameController s_instance;
    public static GameController Instance => s_instance;

    GameController()
    {
        s_instance = this;
    }


    [EntitySystem]
    public void OnCreate()
    {
        m_RootTransform = GetComponent<Transform3D>();


        const float blockWidth = 0.66f + 0.1f;
        const float blockHeight = 0.33f + 0.1f;

        const int rowsCount = 11; 
        const int columnCount = 10;

        const float lineWidth = blockWidth * columnCount;
        const float lineHeight = blockHeight * rowsCount;

        for (int r = 0; r < rowsCount; ++r)
        {
            for (int c = 0; c < columnCount; ++c)
            {
                Entity newBrick = InstantiateEntity(m_BrickPrefab);
                Transform3D newBrickTransform = newBrick.GetComponent<Transform3D>();
                newBrickTransform.SetParent(m_RootTransform);
                newBrickTransform.X = blockWidth * 0.5f + blockWidth * (float)c - lineWidth * 0.5f;
                newBrickTransform.Y = 3.5f + blockHeight * r;
            }
        }
    }

    [EntitySystem]
    public void OnUpdate(float _dt)
    {
        if (m_Paddle == null || m_Paddle.Transform == null)
            return;

        Vec3F paddleSize = m_Paddle.Transform.Scale;

        if (Input.GetKeyState(KeyCode.Left) || Input.GetKeyState(KeyCode.A))
        {
            m_Paddle.Transform.Translate(new Vec3F(-m_PaddleSpeed * _dt, 0.0f, 0.0f));

            float leftPosition = -m_FieldSize.X * 0.5f + paddleSize.X * 0.5f;
            if (m_Paddle.Transform.X < leftPosition)
                m_Paddle.Transform.X = leftPosition;
        }

        if (Input.GetKeyState(KeyCode.Right) || Input.GetKeyState(KeyCode.D))
        {
            m_Paddle.Transform.Translate(new Vec3F(m_PaddleSpeed * _dt, 0.0f, 0.0f));

            float rightPosition = m_FieldSize.X * 0.5f - paddleSize.X * 0.5f;
            if (m_Paddle.Transform.X > rightPosition)
                m_Paddle.Transform.X = rightPosition;
        }

        if (Input.GetKeyState(KeyCode.Space))
        {
            if (m_Paddle.AttachedBall != null)
            {
                m_Paddle.LaunchBall();
            }
        }
    }

    public void AddGameObject(GameObject _obj)
    {
        m_GameObjects.Add(_obj);
    }

    public void RemoveGameObject(GameObject _obj)
    {
        m_GameObjects.Remove(_obj);
    }

    public void DestroyBrick(GameObject _obj)
    {
        RemoveGameObject(_obj);
        _obj.GetEntity().Destroy();
    }

    public bool OverlapTest(GameObject _obj, out CollisionTestResult _result)
    {
        foreach (GameObject gameObject in m_GameObjects)
        {
            if (gameObject == _obj)
                continue;

            if (gameObject.IsOverlap(_obj))
            {
                CollisionTestResult testResult;
                testResult.obj = gameObject;

                _result = testResult;
                return true;
            }
        }

        _result = default;
        return false;
    }
}
