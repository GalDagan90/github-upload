using System;
using System.Globalization;
using Avalonia.Data.Converters;

namespace TradingJournal.Converters;

/// <summary>
/// Converts between <see cref="DateOnly"/> / <see cref="DateOnly?"/> and <see cref="string"/>
/// for use in DataGrid editing templates, where popup-based date pickers fail because the
/// editing control is transiently detached from the visual tree.
/// </summary>
public sealed class DateOnlyStringConverter : IValueConverter
{
    /// <summary>Shared singleton instance for use as a static resource in AXAML.</summary>
    public static readonly DateOnlyStringConverter Instance = new();

    /// <inheritdoc/>
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) =>
        value is DateOnly d ? d.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture) : null;

    /// <inheritdoc/>
    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not string s || string.IsNullOrWhiteSpace(s))
            return targetType == typeof(DateOnly?) ? (object?)(DateOnly?)null : null;

        if (DateOnly.TryParse(s, CultureInfo.InvariantCulture, out var parsed))
            return targetType == typeof(DateOnly?) ? (object?)(DateOnly?)parsed : parsed;

        if (DateTime.TryParse(s, CultureInfo.InvariantCulture, DateTimeStyles.None, out var dt))
        {
            var d = DateOnly.FromDateTime(dt);
            return targetType == typeof(DateOnly?) ? (object?)(DateOnly?)d : d;
        }

        return null;
    }
}
