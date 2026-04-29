namespace TradingJournal.Models;

/// <summary>
/// Identifies the trading strategy used for a given trade entry.
/// Integer values are persisted to the database — do not renumber existing members.
/// </summary>
public enum StrategyType
{
    /// <summary>Stock trade (long or short equity position).</summary>
    Stock = 0,

    /// <summary>Short Put — sell a put option, collecting premium.</summary>
    ShortPut = 1,

    /// <summary>Short Call — sell a call option, collecting premium.</summary>
    ShortCall = 2,

    /// <summary>Covered Call — own shares and sell a call against them.</summary>
    CoveredCall = 3,

    /// <summary>Cash-Secured Put — sell a put with enough cash reserved to buy shares if assigned.</summary>
    CashSecuredPut = 4,

    /// <summary>Bull Put Credit Spread — sell a higher-strike put and buy a lower-strike put.</summary>
    BullPutCreditSpread = 5,

    /// <summary>Bear Call Credit Spread — sell a lower-strike call and buy a higher-strike call.</summary>
    BearCallCreditSpread = 6,

    /// <summary>Iron Condor — combine a bull put spread and a bear call spread on the same underlying.</summary>
    IronCondor = 7,

    /// <summary>Long Call — buy a call option.</summary>
    LongCall = 8,

    /// <summary>Long Put — buy a put option.</summary>
    LongPut = 9,

    /// <summary>Any strategy not covered by the predefined values.</summary>
    Custom = 10,
}
