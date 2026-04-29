using System.Threading.Tasks;

namespace TradingJournal.Contracts;

/// <summary>
/// Provides persistent key-value storage backed by the Settings table in SQLite.
/// Used for storing user preferences such as the selected app theme.
/// </summary>
public interface ISettingsService
{
    /// <summary>
    /// Retrieves the stored value for the given key.
    /// </summary>
    /// <param name="key">The settings key to look up (e.g. <c>"AppTheme"</c>).</param>
    /// <returns>The stored value, or <see langword="null"/> if the key does not exist.</returns>
    Task<string?> GetAsync(string key);

    /// <summary>
    /// Stores a value for the given key, inserting or overwriting as needed.
    /// </summary>
    /// <param name="key">The settings key (e.g. <c>"AppTheme"</c>).</param>
    /// <param name="value">The value to store (e.g. <c>"Dark"</c>).</param>
    Task SetAsync(string key, string value);
}
