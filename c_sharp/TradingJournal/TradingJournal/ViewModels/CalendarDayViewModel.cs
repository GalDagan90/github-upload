using System;
using System.Collections.Generic;
using Avalonia.Media;
using Avalonia.Media.Immutable;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for a single day cell in the monthly calendar grid.
/// </summary>
public sealed class CalendarDayViewModel
{
    private static readonly IBrush GreenBrush =
        new ImmutableSolidColorBrush(Color.FromArgb(80, 22, 163, 74));
    private static readonly IBrush RedBrush =
        new ImmutableSolidColorBrush(Color.FromArgb(80, 220, 38, 38));
    private static readonly IBrush TransparentBrush = Brushes.Transparent;

    /// <summary>Calendar date this cell represents.</summary>
    public DateOnly Date { get; }

    /// <summary>Day-of-month number to display in the cell header.</summary>
    public int DayNumber { get; }

    /// <summary>False for padding days from the previous or next month.</summary>
    public bool IsCurrentMonth { get; }

    /// <summary>Sum of G/L for all closed/assigned trades on this day. Null when no trades.</summary>
    public decimal? TotalGL { get; }

    /// <summary>G/L formatted as currency (e.g. "$1,234.56"), or empty when no trades.</summary>
    public string TotalGLFormatted => TotalGL.HasValue ? $"${TotalGL.Value:N2}" : string.Empty;

    /// <summary>1.0 for current-month days; 0.35 for padding days.</summary>
    public double TextOpacity => IsCurrentMonth ? 1.0 : 0.35;

    /// <summary>Background tint: green for profitable days, red for losing days, transparent otherwise.</summary>
    public IBrush CellColor { get; }

    /// <summary>Trades closed on this day, used to populate the flyout list.</summary>
    public IReadOnlyList<TradeSummary> Trades { get; }

    /// <summary>
    /// Creates a day cell ViewModel.
    /// </summary>
    /// <param name="date">Calendar date for this cell.</param>
    /// <param name="isCurrentMonth">False when this is a padding day from an adjacent month.</param>
    /// <param name="totalGL">Aggregated G/L; null if no trades were closed this day.</param>
    /// <param name="trades">Trade summaries to display in the flyout.</param>
    public CalendarDayViewModel(
        DateOnly date, bool isCurrentMonth, decimal? totalGL, IReadOnlyList<TradeSummary> trades)
    {
        Date           = date;
        DayNumber      = date.Day;
        IsCurrentMonth = isCurrentMonth;
        TotalGL        = totalGL;
        Trades         = trades;

        CellColor = isCurrentMonth && totalGL.HasValue
            ? (totalGL > 0 ? GreenBrush : RedBrush)
            : TransparentBrush;
    }
}
