using System;
using System.Threading.Tasks;
using Microsoft.Data.Sqlite;
using TradingJournal.Contracts;

namespace TradingJournal.Services;

/// <summary>
/// SQLite-backed implementation of <see cref="ISettingsService"/>.
/// Reads and writes key-value pairs in the Settings table.
/// </summary>
public class SqliteSettingsService : ISettingsService
{
    private readonly string _connectionString =
        $"Data Source={DatabaseInitializer.DatabasePath}";

    /// <inheritdoc/>
    public async Task<string?> GetAsync(string key)
    {
        try
        {
            await using var connection = new SqliteConnection(_connectionString);
            await connection.OpenAsync();

            await using var command = connection.CreateCommand();
            command.CommandText = "SELECT Value FROM Settings WHERE Key = @Key";
            command.Parameters.AddWithValue("@Key", key);

            var result = await command.ExecuteScalarAsync();
            return result as string;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TradingJournal] Settings read failed for '{key}': {ex.Message}");
            return null;
        }
    }

    /// <inheritdoc/>
    public async Task SetAsync(string key, string value)
    {
        try
        {
            await using var connection = new SqliteConnection(_connectionString);
            await connection.OpenAsync();

            await using var command = connection.CreateCommand();

            // INSERT OR REPLACE would reset the primary key; UPSERT preserves it.
            command.CommandText = """
                INSERT INTO Settings (Key, Value) VALUES (@Key, @Value)
                ON CONFLICT(Key) DO UPDATE SET Value = excluded.Value
                """;

            command.Parameters.AddWithValue("@Key", key);
            command.Parameters.AddWithValue("@Value", value);
            await command.ExecuteNonQueryAsync();
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TradingJournal] Settings write failed for '{key}': {ex.Message}");
        }
    }
}
