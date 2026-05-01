using System;
using System.Globalization;
using Avalonia.Data.Converters;

namespace TradingJournal.Converters;

/// <summary>
/// Converts between <see cref="DateOnly"/> / <see cref="DateOnly?"/> and <see cref="DateTime?"/>
/// for Avalonia's <see cref="Avalonia.Controls.CalendarDatePicker"/>, whose
/// <c>SelectedDate</c> property is typed as <see cref="DateTime?"/>.
/// </summary>
public sealed class DateOnlyConverter : IValueConverter
{
    /// <summary>Shared singleton instance for use as a static resource in AXAML.</summary>
    public static readonly DateOnlyConverter Instance = new();

    /// <inheritdoc/>
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) =>
        value is DateOnly d ? new DateTime(d.Year, d.Month, d.Day) : (object?)null;

    /// <inheritdoc/>
    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not DateTime dt)
            return null;

        var date = DateOnly.FromDateTime(dt);
        return targetType == typeof(DateOnly?) ? (object?)(DateOnly?)date : date;
    }
}
