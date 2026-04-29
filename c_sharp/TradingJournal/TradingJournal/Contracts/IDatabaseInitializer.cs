using System.Threading.Tasks;

namespace TradingJournal.Contracts;

/// <summary>
/// Ensures the SQLite database and all required tables exist before the app is used.
/// Called once at application startup.
/// </summary>
public interface IDatabaseInitializer
{
    /// <summary>
    /// Creates the database file and all required tables if they do not already exist.
    /// Safe to call on every startup — uses CREATE TABLE IF NOT EXISTS.
    /// </summary>
    Task InitializeAsync();
}
