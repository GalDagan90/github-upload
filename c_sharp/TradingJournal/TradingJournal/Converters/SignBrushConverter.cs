using System;
using System.Globalization;
using Avalonia.Data.Converters;
using Avalonia.Media;

namespace TradingJournal.Converters;

/// <summary>
/// Converts a <see cref="decimal"/> value to a foreground <see cref="IBrush"/> based on its sign.
/// Positive → green; Negative → red; Zero → <see langword="null"/> (inherits default foreground).
/// </summary>
public sealed class SignBrushConverter : IValueConverter
{
    /// <summary>Shared singleton instance for use as a static resource in AXAML.</summary>
    public static readonly SignBrushConverter Instance = new();

    private static readonly IBrush GreenBrush = new SolidColorBrush(Color.Parse("#16A34A"));
    private static readonly IBrush RedBrush   = new SolidColorBrush(Color.Parse("#DC2626"));

    /// <inheritdoc/>
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        var d = value is decimal dec ? dec : 0m;
        return d > 0m ? GreenBrush : d < 0m ? RedBrush : null;
    }

    /// <inheritdoc/>
    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture) =>
        throw new NotSupportedException();
}
