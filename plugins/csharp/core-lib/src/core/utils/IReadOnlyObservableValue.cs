using System;

public interface IReadOnlyObservableValue<TType>
{
    event Action<TType> ValueChangedEvent;

    TType Value { get; }
}
