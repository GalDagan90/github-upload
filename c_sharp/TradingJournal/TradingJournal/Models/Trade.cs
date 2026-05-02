using System;
using CommunityToolkit.Mvvm.ComponentModel;

namespace TradingJournal.Models;

/// <summary>
/// Represents a single trade entry in the journal.
/// Covers both stock trades and multi-leg options strategies.
/// Properties map 1-to-1 to the Trades SQLite table columns.
/// <see cref="PL"/> and <see cref="GL"/> are computed from other fields and are never set directly.
/// </summary>
public class Trade : ObservableObject
{
    /// <summary>Database primary key. Zero until the row has been persisted.</summary>
    public int Id { get; set; }

    /// <summary>Ticker symbol (e.g. "AAPL"). Always stored upper-case.</summary>
    public string Ticker { get; set; } = string.Empty;

    private StrategyType _strategyType;
    /// <summary>The trading strategy applied to this position.</summary>
    public StrategyType StrategyType
    {
        get => _strategyType;
        set
        {
            if (SetProperty(ref _strategyType, value))
            {
                OnPropertyChanged(nameof(DaysToExpiration));
                OnPropertyChanged(nameof(PL));
                OnPropertyChanged(nameof(GL));
            }
        }
    }

    /// <summary>
    /// Strike price(s) as free-text. Null for stock trades.
    /// Single-leg: "150". Multi-leg spread: "150/160". Iron condor: "145/150/160/165".
    /// No parsing or validation is performed — stored exactly as entered.
    /// </summary>
    public string? Strike { get; set; }

    private DateOnly _openDate;
    /// <summary>Date the position was opened.</summary>
    public DateOnly OpenDate
    {
        get => _openDate;
        set
        {
            if (SetProperty(ref _openDate, value))
                OnPropertyChanged(nameof(DaysOpen));
        }
    }

    private DateOnly? _closeDate;
    /// <summary>Date the position was closed. Null when the position is still open.</summary>
    public DateOnly? CloseDate
    {
        get => _closeDate;
        set
        {
            if (SetProperty(ref _closeDate, value))
            {
                OnPropertyChanged(nameof(DaysOpen));
                OnPropertyChanged(nameof(DaysToExpiration));
            }
        }
    }

    private DateOnly? _expirationDate;
    /// <summary>Options expiration date. Null for stock trades.</summary>
    public DateOnly? ExpirationDate
    {
        get => _expirationDate;
        set
        {
            if (SetProperty(ref _expirationDate, value))
                OnPropertyChanged(nameof(DaysToExpiration));
        }
    }

    private decimal _entryPrice;
    /// <summary>
    /// Premium collected or price paid per share / per contract at entry.
    /// Stored as <see langword="decimal"/> to avoid floating-point drift.
    /// </summary>
    public decimal EntryPrice
    {
        get => _entryPrice;
        set
        {
            if (SetProperty(ref _entryPrice, value))
            {
                OnPropertyChanged(nameof(PL));
                OnPropertyChanged(nameof(GL));
            }
        }
    }

    private decimal? _closingPrice;
    /// <summary>
    /// Closing price per share / per contract.
    /// Null when the position is still open.
    /// </summary>
    public decimal? ClosingPrice
    {
        get => _closingPrice;
        set
        {
            if (SetProperty(ref _closingPrice, value))
            {
                OnPropertyChanged(nameof(PL));
                OnPropertyChanged(nameof(GL));
            }
        }
    }

    private int _quantity;
    /// <summary>Number of contracts (options) or shares (stock).</summary>
    public int Quantity
    {
        get => _quantity;
        set
        {
            if (SetProperty(ref _quantity, value))
                OnPropertyChanged(nameof(GL));
        }
    }

    /// <summary>
    /// Realized profit or loss per unit.
    /// Stock: <c>ClosingPrice − EntryPrice</c>.
    /// Options: <c>EntryPrice − ClosingPrice</c> (premium collected minus buyback cost).
    /// Null when <see cref="ClosingPrice"/> is not yet set.
    /// </summary>
    public decimal? PL
    {
        get
        {
            if (ClosingPrice is null) return null;
            return StrategyType == StrategyType.Stock
                ? ClosingPrice.Value - EntryPrice
                : EntryPrice - ClosingPrice.Value;
        }
    }

    /// <summary>
    /// Total dollar gain or loss.
    /// Stock: <c>PL × Quantity</c>.
    /// Options: <c>PL × Quantity × 100</c> (standard contract multiplier).
    /// Null when <see cref="PL"/> is null.
    /// </summary>
    public decimal? GL
    {
        get
        {
            if (PL is null) return null;
            return StrategyType == StrategyType.Stock
                ? PL.Value * Quantity
                : PL.Value * Quantity * 100m;
        }
    }

    private TradeStatus _status;
    /// <summary>Current lifecycle state of the position.</summary>
    public TradeStatus Status
    {
        get => _status;
        set
        {
            if (SetProperty(ref _status, value))
            {
                OnPropertyChanged(nameof(DaysOpen));
                OnPropertyChanged(nameof(DaysToExpiration));
            }
        }
    }

    /// <summary>Free-form notes or tags associated with this trade.</summary>
    public string? Notes { get; set; }

    /// <summary>
    /// Number of calendar days the position has been (or was) open.
    /// Open positions: today − <see cref="OpenDate"/>.
    /// Closed/assigned positions: <see cref="CloseDate"/> − <see cref="OpenDate"/>.
    /// </summary>
    public int DaysOpen
    {
        get
        {
            var end = Status == TradeStatus.Open
                ? DateOnly.FromDateTime(DateTime.Today)
                : CloseDate ?? DateOnly.FromDateTime(DateTime.Today);
            return end.DayNumber - OpenDate.DayNumber;
        }
    }

    /// <summary>
    /// Days between the reference date and <see cref="ExpirationDate"/>.
    /// Open positions: expiration − today (positive = time remaining, negative = past expiry).
    /// Closed/assigned positions: expiration − close date.
    /// Returns <see langword="null"/> for stock trades or when no expiration date is set.
    /// </summary>
    public int? DaysToExpiration
    {
        get
        {
            if (StrategyType == StrategyType.Stock || ExpirationDate is null)
                return null;
            var reference = Status == TradeStatus.Open
                ? DateOnly.FromDateTime(DateTime.Today)
                : CloseDate ?? DateOnly.FromDateTime(DateTime.Today);
            return ExpirationDate.Value.DayNumber - reference.DayNumber;
        }
    }
}
