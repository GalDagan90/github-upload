using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.Data.Sqlite;
using TradingJournal.Contracts;

namespace TradingJournal.Services;

/// <summary>
/// Creates the SQLite database file and schema on first run.
/// Exposes the resolved database path as a shared static property so
/// other services can build their connection strings without duplicating the logic.
/// </summary>
public class DatabaseInitializer : IDatabaseInitializer
{
    /// <summary>
    /// Full path to the SQLite database file.
    /// Debug builds use <c>trades-dev.db</c> to keep development data
    /// separate from the production database.
    /// </summary>
    public static string DatabasePath { get; } = ResolveDatabasePath();

    private static string ResolveDatabasePath()
    {
#if DEBUG
        const string fileName = "trades-dev.db";
#else
        const string fileName = "trades.db";
#endif
        var folder = Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
            "TradingJournal");

        Directory.CreateDirectory(folder);
        return Path.Combine(folder, fileName);
    }

    /// <inheritdoc/>
    public async Task InitializeAsync()
    {
        try
        {
            await using var connection = new SqliteConnection($"Data Source={DatabasePath}");
            await connection.OpenAsync();

            await using var command = connection.CreateCommand();
            command.CommandText = """
                CREATE TABLE IF NOT EXISTS Trades (
                    Id              INTEGER PRIMARY KEY AUTOINCREMENT,
                    Ticker          TEXT    NOT NULL,
                    StrategyType    INTEGER NOT NULL,
                    Strike          TEXT,
                    OpenDate        TEXT    NOT NULL,
                    CloseDate       TEXT,
                    ExpirationDate  TEXT,
                    EntryPrice      TEXT    NOT NULL,
                    ClosingPrice    TEXT,
                    Quantity        INTEGER NOT NULL,
                    PL              TEXT,
                    GL              TEXT,
                    Status          INTEGER NOT NULL DEFAULT 0,
                    Notes           TEXT
                );

                CREATE TABLE IF NOT EXISTS Settings (
                    Key     TEXT PRIMARY KEY,
                    Value   TEXT NOT NULL
                );

                INSERT OR IGNORE INTO Settings (Key, Value) VALUES ('SchemaVersion', '1');
                """;

            await command.ExecuteNonQueryAsync();
        }
        catch (Exception ex)
        {
            throw new InvalidOperationException(
                $"Failed to initialize the TradingJournal database at '{DatabasePath}'.", ex);
        }
    }
}
