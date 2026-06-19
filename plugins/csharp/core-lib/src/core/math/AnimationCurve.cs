using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public enum AnimationCurveMinMaxMode
    {
        NormalizedPositive,
        Normalized,
    }

    public class AnimationCurve
    {
        public struct Keyframe
        {
            public float Time;
            public float Value;
            public float InTangent;
            public float OutTangent;

            public Keyframe(float time, float value)
            {
                Time = time;
                Value = value;
                InTangent = 0.0f;
                OutTangent = 0.0f;
            }

            public Keyframe(float time, float value, float inTangent, float outTangent)
            {
                Time = time;
                Value = value;
                InTangent = inTangent;
                OutTangent = outTangent;
            }

            public void ToDataBlock(DataBlock dataBlock)
            {
                dataBlock.SetF32("time", Time);
                dataBlock.SetF32("value", Value);
                dataBlock.SetF32("inTangent", InTangent);
                dataBlock.SetF32("outTangent", OutTangent);
            }

            public void LoadFromDataBlock(DataBlock dataBlock)
            {
                Time = dataBlock.GetF32("time");
                Value = dataBlock.GetF32("value");
                InTangent = dataBlock.GetF32("inTangent");
                OutTangent = dataBlock.GetF32("outTangent");
            }
        }

        public enum EvaluateMode
        {
            Fixed,
            Linear,
            Smooth,
        }

        List<Keyframe> m_Keys = new List<Keyframe>();
        public IReadOnlyList<Keyframe> Keys => m_Keys;

        float m_Scalar = 1.0f;
        public float Scalar { get => m_Scalar; set => m_Scalar = value; }

        EvaluateMode m_Mode = EvaluateMode.Linear;
        public EvaluateMode Mode { get => m_Mode; set => m_Mode = value; }

        AnimationCurveMinMaxMode m_MinMaxMode = AnimationCurveMinMaxMode.NormalizedPositive;
        public AnimationCurveMinMaxMode MinMaxMode { get => m_MinMaxMode; set => m_MinMaxMode = value; }

        public AnimationCurve()
        {
        }

        public AnimationCurve(float time, float value)
        {
            AddKey(time, value);
        }

        public AnimationCurve(float time0, float value0, float time1, float value1)
        {
            AddKey(time0, value0);
            AddKey(time1, value1);
        }

        public bool IsEmpty => m_Keys.Count == 0;

        public void AddKey(Keyframe keyframe)
        {
            m_Keys.Add(keyframe);
            SortKeys();
        }

        public void AddKey(float time, float value)
        {
            AddKey(new Keyframe(time, value));
        }

        public void AddKey(float time, float value, float inTangent, float outTangent)
        {
            AddKey(new Keyframe(time, value, inTangent, outTangent));
        }

        public void AddKey(float time, float value, float tangent)
        {
            AddKey(new Keyframe(time, value, tangent, tangent));
        }

        public void SetKeyValue(int index, float value)
        {
            Keyframe keyframe = m_Keys[index];
            keyframe.Value = value;
            m_Keys[index] = keyframe;
        }

        public void SetKeyTime(int index, float time)
        {
            Keyframe keyframe = m_Keys[index];
            keyframe.Time = time;
            m_Keys[index] = keyframe;
            SortKeys();
        }

        public void SetKeyInTangent(int index, float inTangent)
        {
            Keyframe keyframe = m_Keys[index];
            keyframe.InTangent = inTangent;
            m_Keys[index] = keyframe;
        }

        public void SetKeyOutTangent(int index, float outTangent)
        {
            Keyframe keyframe = m_Keys[index];
            keyframe.OutTangent = outTangent;
            m_Keys[index] = keyframe;
        }

        public void SetKeyTangent(int index, float tangent)
        {
            SetKeyInTangent(index, tangent);
            SetKeyOutTangent(index, tangent);
        }

        public void RemoveKey(int index)
        {
            m_Keys.RemoveAt(index);
        }

        public void ClearKeys()
        {
            m_Keys.Clear();
        }

        void SortKeys()
        {
            m_Keys.Sort((a, b) => a.Time.CompareTo(b.Time));
        }

        public float GetStartTime()
        {
            return m_Keys.Count > 0 ? m_Keys[0].Time : 0.0f;
        }

        public float GetEndTime()
        {
            return m_Keys.Count > 0 ? m_Keys[m_Keys.Count - 1].Time : 1.0f;
        }

        public float GetTime()
        {
            return GetEndTime() - GetStartTime();
        }

        public float EvaluateUnscaled(float time)
        {
            if (m_Keys.Count == 0)
                return 0.0f;

            if (time < m_Keys[0].Time)
                return m_Keys[0].Value;

            if (time >= m_Keys[m_Keys.Count - 1].Time)
                return m_Keys[m_Keys.Count - 1].Value;

            for (int i = m_Keys.Count - 2; i >= 0; --i)
            {
                Keyframe keyframe0 = m_Keys[i];
                if (time >= keyframe0.Time)
                {
                    Keyframe keyframe1 = m_Keys[i + 1];
                    return EvaluateBetween(keyframe0, keyframe1, time);
                }
            }

            return m_Keys[0].Value;
        }

        public float Evaluate(float time)
        {
            return EvaluateUnscaled(time) * m_Scalar;
        }

        float EvaluateBetween(Keyframe keyframe0, Keyframe keyframe1, float time)
        {
            switch (m_Mode)
            {
                case EvaluateMode.Fixed:
                    return keyframe1.Value;

                case EvaluateMode.Smooth:
                {
                    float dt = keyframe1.Time - keyframe0.Time;
                    float t = (time - keyframe0.Time) / dt;

                    float m0 = keyframe0.OutTangent * dt;
                    float m1 = keyframe1.InTangent * dt;

                    float t2 = t * t;
                    float t3 = t2 * t;

                    float a = 2.0f * t3 - 3 * t2 + 1;
                    float b = t3 - 2 * t2 + t;
                    float c = t3 - t2;
                    float d = -2.0f * t3 + 3 * t2;

                    return a * keyframe0.Value + b * m0 + c * m1 + d * keyframe1.Value;
                }

                case EvaluateMode.Linear:
                default:
                {
                    float dt = keyframe1.Time - keyframe0.Time;
                    float t = (time - keyframe0.Time) / dt;
                    return keyframe0.Value + (keyframe1.Value - keyframe0.Value) * t;
                }
            }
        }

        public void Normalize()
        {
            if (m_Keys.Count == 0)
                return;

            float maxScalar = 0.0f;
            foreach (Keyframe keyframe in m_Keys)
                maxScalar = Math.Max(maxScalar, Math.Abs(keyframe.Value));

            if (maxScalar > 1.0f && maxScalar > m_Scalar)
            {
                float invMaxScalar = 1.0f / maxScalar;
                for (int i = 0; i < m_Keys.Count; ++i)
                {
                    Keyframe keyframe = m_Keys[i];
                    keyframe.Value *= invMaxScalar;
                    m_Keys[i] = keyframe;
                }

                m_Scalar = maxScalar;
            }
        }

        public void MultiplyValues(float value)
        {
            for (int i = 0; i < m_Keys.Count; ++i)
            {
                Keyframe keyframe = m_Keys[i];
                keyframe.Value *= value;
                m_Keys[i] = keyframe;
            }
        }

        public DataBlock ToDataBlock()
        {
            DataBlock dataBlock = new DataBlock();

            if (m_Keys.Count > 0)
            {
                DataBlock keyframesBlock = dataBlock.EnsureDataBlock("keyframes");
                foreach (Keyframe keyframe in m_Keys)
                    keyframe.ToDataBlock(keyframesBlock.AddNewDataBlock("item"));
            }

            dataBlock.SetF32("scalar", m_Scalar);
            dataBlock.SetS32("mode", (int)m_Mode);
            dataBlock.SetS32("minMaxMode", (int)m_MinMaxMode);

            return dataBlock;
        }

        public void LoadFromDataBlock(DataBlock dataBlock)
        {
            m_Keys.Clear();

            DataBlock keyframesBlock = dataBlock.GetDataBlock("keyframes");
            if (keyframesBlock != null)
            {
                foreach (DataBlock itemBlock in keyframesBlock)
                {
                    Keyframe keyframe = new Keyframe();
                    keyframe.LoadFromDataBlock(itemBlock);
                    m_Keys.Add(keyframe);
                }

                SortKeys();
            }

            m_Scalar = dataBlock.GetF32("scalar", 1.0f);
            m_Mode = (EvaluateMode)dataBlock.GetS32("mode", (int)EvaluateMode.Linear);
            m_MinMaxMode = (AnimationCurveMinMaxMode)dataBlock.GetS32("minMaxMode", (int)AnimationCurveMinMaxMode.NormalizedPositive);
        }
    }
}
