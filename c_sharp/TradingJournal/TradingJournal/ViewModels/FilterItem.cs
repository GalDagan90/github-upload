using System;
using CommunityToolkit.Mvvm.ComponentModel;

namespace TradingJournal.ViewModels;

/// <summary>
/// A single selectable option in a multi-select filter control.
/// Invokes <paramref name="onChanged"/> whenever <see cref="IsSelected"/> toggles
/// so the owning ViewModel can re-apply its filter predicate.
/// </summary>
/// <typeparam name="T">The enum type this item represents.</typeparam>
public partial class FilterItem<T> : ObservableObject
{
    private readonly Action _onChanged;

    /// <summary>The enum value this item represents.</summary>
    public T Value { get; }

    /// <summary>Display text shown next to the checkbox in the filter dropdown.</summary>
    public string Label { get; }

    /// <summary>Whether this option is currently included in filter results.</summary>
    [ObservableProperty]
    private bool _isSelected = true;

    /// <summary>
    /// Initialises a filter item that starts selected.
    /// </summary>
    /// <param name="value">Enum value this item wraps.</param>
    /// <param name="label">Display text.</param>
    /// <param name="onChanged">Callback invoked whenever <see cref="IsSelected"/> changes.</param>
    public FilterItem(T value, string label, Action onChanged)
    {
        Value = value;
        Label = label;
        _onChanged = onChanged;
    }

    partial void OnIsSelectedChanged(bool value) => _onChanged();
}
