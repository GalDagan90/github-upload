namespace TradingJournal.ViewModels;

/// <summary>
/// Predefined date range options for the Analytics tab filter.
/// Applied against a trade's <see cref="TradingJournal.Models.Trade.CloseDate"/>.
/// </summary>
public enum DateRangeOption
{
    /// <summary>No date restriction — all closed trades are included.</summary>
    AllTime,

    /// <summary>Trades closed in the current calendar month.</summary>
    ThisMonth,

    /// <summary>Trades closed in the current calendar year.</summary>
    ThisYear,

    /// <summary>User-specified start and end date range.</summary>
    Custom,
}
