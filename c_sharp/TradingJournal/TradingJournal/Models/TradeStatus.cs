namespace TradingJournal.Models;

/// <summary>
/// Represents the current lifecycle state of a trade position.
/// Integer values are persisted to the database — do not renumber existing members.
/// </summary>
public enum TradeStatus
{
    /// <summary>Position is still open — not yet closed or assigned.</summary>
    Open = 0,

    /// <summary>Position has been closed for a realized gain or loss.</summary>
    Closed = 1,

    /// <summary>Option was assigned — shares were taken or called away.</summary>
    Assigned = 2,
}
