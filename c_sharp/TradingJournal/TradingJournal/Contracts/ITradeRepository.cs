using System.Collections.Generic;
using System.Threading.Tasks;
using TradingJournal.Models;

namespace TradingJournal.Contracts;

/// <summary>
/// Provides CRUD access to the Trades table in the database.
/// </summary>
public interface ITradeRepository
{
    /// <summary>
    /// Returns all trade entries ordered by open date descending.
    /// </summary>
    Task<IReadOnlyList<Trade>> GetAllAsync();

    /// <summary>
    /// Inserts a new trade into the database and returns the assigned primary key.
    /// </summary>
    /// <param name="trade">The trade to insert. Its <see cref="Trade.Id"/> is ignored and overwritten by the database.</param>
    /// <returns>The database-assigned <see cref="Trade.Id"/> for the new row.</returns>
    Task<int> AddAsync(Trade trade);

    /// <summary>
    /// Updates every field of an existing trade row, matched by <see cref="Trade.Id"/>.
    /// </summary>
    /// <param name="trade">The trade with updated values. Must have a valid <see cref="Trade.Id"/>.</param>
    Task UpdateAsync(Trade trade);

    /// <summary>
    /// Permanently deletes the trade row with the given primary key.
    /// </summary>
    /// <param name="id">The <see cref="Trade.Id"/> of the row to delete.</param>
    Task DeleteAsync(int id);
}
