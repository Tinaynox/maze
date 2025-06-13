using System;
using System.Collections.Generic;

public class ObservableValue<TType> : IReadOnlyObservableValue<TType>
{
    TType m_Value;
    readonly Func<TType, TType, bool> m_IsEqualFunc;
    public event Action<TType> ValueChangedEvent;

    public ObservableValue(TType value) : this()
    {
        SetValueWithoutNotify(value);
    }

    public ObservableValue()
    {
        if (typeof(IComparable<TType>).IsAssignableFrom(typeof(TType)))
        {
            m_IsEqualFunc =
                (value0, value1) => ((IComparable<TType>)value0).CompareTo(value1) == 0;
        }
        else if (typeof(IComparable).IsAssignableFrom(typeof(TType)))
        {
            m_IsEqualFunc =
                (value0, value1) => ((IComparable)value0).CompareTo(value1) == 0;
        }
        else if (typeof(IEquatable<TType>).IsAssignableFrom(typeof(TType)))
        {
            m_IsEqualFunc =
                (value0, value1) => ((IEquatable<TType>)value0).Equals(value1);
        }
        else if (typeof(object).IsAssignableFrom(typeof(TType)))
        {
            m_IsEqualFunc =
                (value0, value1) => (object)value0 == (object)value1;
        }
        else
        {
            throw new NotSupportedException($"Unsupported value type: {nameof(TType)}");
        }
    }

    public TType Value
    {
        get => m_Value;
        set
        {
            if (!m_IsEqualFunc.Invoke(m_Value, value))
            {
                SetValueWithoutNotify(value);
                InvokeValueChangedEvent();
            }
        }
    }

    public void SetValueWithoutNotify(TType value)
    {
        m_Value = value;
    }
    
    public void InvokeValueChangedEvent()
    {
        ValueChangedEvent?.Invoke(m_Value);
    }

    public void RemoveAllListeners()
    {
        ValueChangedEvent = null;
    }
    
    public static implicit operator TType(ObservableValue<TType> property)
    {
        return property.Value;
    }

    public override string ToString() => m_Value.ToString();
}

