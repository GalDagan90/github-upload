namespace TradingJournal.ViewModels;

/// <summary>
/// Lightweight summary of a single trade shown in the Calendar day flyout.
/// </summary>
/// <param name="Ticker">The traded ticker symbol.</param>
/// <param name="Strategy">Display name of the strategy type.</param>
/// <param name="GL">Total dollar gain/loss for this trade.</param>
public record TradeSummary(string Ticker, string Strategy, decimal? GL)
{
    /// <summary>G/L formatted as currency (e.g. "$1,234.56"), or "—" when not yet realized.</summary>
    public string GLFormatted => GL.HasValue ? $"${GL.Value:N2}" : "—";
}
