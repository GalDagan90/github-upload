using System;

namespace TradingJournal.Models;

/// <summary>
/// Represents a single trade entry in the journal.
/// Covers both stock trades and multi-leg options strategies.
/// Properties map 1-to-1 to the Trades SQLite table columns.
/// </summary>
public class Trade
{
    /// <summary>Database primary key. Zero until the row has been persisted.</summary>
    public int Id { get; set; }

    /// <summary>Ticker symbol (e.g. "AAPL"). Always stored upper-case.</summary>
    public string Ticker { get; set; } = string.Empty;

    /// <summary>The trading strategy applied to this position.</summary>
    public StrategyType StrategyType { get; set; }

    /// <summary>
    /// Strike price(s) as free-text. Null for stock trades.
    /// Single-leg: "150". Multi-leg spread: "150/160". Iron condor: "145/150/160/165".
    /// No parsing or validation is performed — stored exactly as entered.
    /// </summary>
    public string? Strike { get; set; }

    /// <summary>Date the position was opened.</summary>
    public DateOnly OpenDate { get; set; }

    /// <summary>Date the position was closed. Null when the position is still open.</summary>
    public DateOnly? CloseDate { get; set; }

    /// <summary>Options expiration date. Null for stock trades.</summary>
    public DateOnly? ExpirationDate { get; set; }

    /// <summary>
    /// Premium collected or price paid per share / per contract at entry.
    /// Stored as <see langword="decimal"/> to avoid floating-point drift.
    /// </summary>
    public decimal EntryPrice { get; set; }

    /// <summary>
    /// Closing price per share / per contract.
    /// Null when the position is still open.
    /// </summary>
    public decimal? ClosingPrice { get; set; }

    /// <summary>Number of contracts (options) or shares (stock).</summary>
    public int Quantity { get; set; }

    /// <summary>
    /// Realized profit or loss per unit (per share or per contract).
    /// Calculated as <c>ClosingPrice − EntryPrice</c>.
    /// Null when the position is still open.
    /// </summary>
    public decimal? PL { get; set; }

    /// <summary>
    /// Total dollar gain or loss: <c>PL × 100 × Quantity</c>.
    /// The ×100 factor is the standard options multiplier; for stock trades it
    /// reduces to <c>PL × Quantity</c> (Quantity is in shares, not contracts).
    /// Null when the position is still open.
    /// </summary>
    public decimal? GL { get; set; }

    /// <summary>Current lifecycle state of the position.</summary>
    public TradeStatus Status { get; set; }

    /// <summary>Free-form notes or tags associated with this trade.</summary>
    public string? Notes { get; set; }
}
