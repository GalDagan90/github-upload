namespace TradingJournal.ViewModels;

/// <summary>
/// Broadcast by <see cref="TradeLogViewModel"/> after any trade is saved, added, or deleted.
/// <see cref="AnalyticsViewModel"/> subscribes to reload and refresh its stats and charts.
/// </summary>
public record TradesChangedMessage;
