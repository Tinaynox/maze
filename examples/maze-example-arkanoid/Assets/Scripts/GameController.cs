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
    float m_PaddleSpeed = 4.0f;

    [SerializeField]
    AssetUnitId m_PaddlePrefab;

    [SerializeField]
    AssetUnitId m_BallPrefab;

    [SerializeField]
    AssetUnitId m_BrickPrefab;

    Transform3D m_RootTransform;
    List<GameObject> m_GameObjects = new List<GameObject>();

    Paddle m_Paddle;

    int m_BricksCount = 0;

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

        LoadLevel();
    }

    [EntitySystem]
    public void OnUpdate(float _dt)
    {
        if (Input.GetKeyState(KeyCode.R))
        {
            LoadLevel();
            return;
        }

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

    void ClearLevel()
    {
        foreach (GameObject obj in m_GameObjects)
            obj.GetEntity().Destroy();
        m_GameObjects.Clear();
        m_Paddle = null;
        m_BricksCount = 0;
    }

    public void LoadLevel()
    {
        ClearLevel();

        // Create Paddle
        Entity paddle = InstantiateEntity(m_PaddlePrefab);
        Transform3D paddleTransform = paddle.GetComponent<Transform3D>();
        m_Paddle = paddle.GetComponent<Paddle>();
        paddleTransform.SetParent(m_RootTransform);
        paddleTransform.Y = 0.3f;

        // Create Ball
        Entity ball = InstantiateEntity(m_BallPrefab);
        Transform3D ballTransform = ball.GetComponent<Transform3D>();
        m_Paddle.AttachedBall = ball.GetComponent<Ball>();
        ballTransform.SetParent(m_RootTransform);

        // Create Bricks
        const float blockWidth = 0.66f + 0.1f;
        const float blockHeight = 0.33f + 0.1f;

        const int rowsCount = 1;
        const int columnCount = 10;

        const float lineWidth = blockWidth * columnCount;
        // const float lineHeight = blockHeight * rowsCount;

        for (int r = 0; r < rowsCount; ++r)
        {
            for (int c = 0; c < columnCount; ++c)
            {
                Entity newBrick = InstantiateEntity(m_BrickPrefab);
                Transform3D newBrickTransform = newBrick.GetComponent<Transform3D>();
                newBrickTransform.SetParent(m_RootTransform);
                newBrickTransform.X = blockWidth * 0.5f + blockWidth * c - lineWidth * 0.5f;
                newBrickTransform.Y = 3.5f + blockHeight * r;

                Brick brick = newBrick.GetComponent<Brick>();

                ++m_BricksCount;

                if (r == 0 || Math.Abs(newBrickTransform.X) < 1.0f)
                    brick.SetHealth(3);
                else
                if (r <= rowsCount / 3)
                    brick.SetHealth(1);
                else
                if (r <= 2 * rowsCount / 3)
                    brick.SetHealth(2);
                else
                    brick.SetHealth(3);
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

        if (--m_BricksCount <= 0)
            LoadLevel();
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
