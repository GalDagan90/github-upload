using System;
using System.Globalization;
using Avalonia.Data.Converters;
using Avalonia.Media;
using TradingJournal.Models;

namespace TradingJournal.Converters;

/// <summary>
/// Converts a <see cref="TradeStatus"/> value to a foreground <see cref="IBrush"/>.
/// Open → green; Closed / Assigned → red.
/// </summary>
public sealed class TradeStatusBrushConverter : IValueConverter
{
    /// <summary>Shared singleton instance for use as a static resource in AXAML.</summary>
    public static readonly TradeStatusBrushConverter Instance = new();

    private static readonly IBrush GreenBrush = new SolidColorBrush(Color.Parse("#16A34A"));
    private static readonly IBrush RedBrush   = new SolidColorBrush(Color.Parse("#DC2626"));

    /// <inheritdoc/>
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) =>
        value is TradeStatus.Open ? GreenBrush : RedBrush;

    /// <inheritdoc/>
    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture) =>
        throw new NotSupportedException();
}
