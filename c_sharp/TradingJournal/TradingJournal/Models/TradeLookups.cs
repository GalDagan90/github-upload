using System.Collections.Generic;

namespace TradingJournal.Models;

/// <summary>Static lookup collections used as item sources for enum dropdowns in AXAML via <c>x:Static</c>.</summary>
public static class TradeLookups
{
    /// <summary>All <see cref="StrategyType"/> values in declaration order.</summary>
    public static IReadOnlyList<StrategyType> StrategyTypes { get; } = System.Enum.GetValues<StrategyType>();

    /// <summary>All <see cref="TradeStatus"/> values in declaration order.</summary>
    public static IReadOnlyList<TradeStatus> TradeStatuses { get; } = System.Enum.GetValues<TradeStatus>();
}
