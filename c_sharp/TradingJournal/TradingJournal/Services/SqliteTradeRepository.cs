using System;
using System.Collections.Generic;
using System.Globalization;
using System.Threading.Tasks;
using Microsoft.Data.Sqlite;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.Services;

/// <summary>
/// SQLite-backed implementation of <see cref="ITradeRepository"/>.
/// Each method opens its own connection — SQLite connections are lightweight
/// and this avoids shared-state bugs across async calls.
/// </summary>
public class SqliteTradeRepository : ITradeRepository
{
    private readonly string _connectionString =
        $"Data Source={DatabaseInitializer.DatabasePath}";

    /// <inheritdoc/>
    public async Task<IReadOnlyList<Trade>> GetAllAsync()
    {
        var trades = new List<Trade>();

        await using var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        await using var command = connection.CreateCommand();
        command.CommandText = """
            SELECT Id, Ticker, StrategyType, Strike, OpenDate, CloseDate,
                   ExpirationDate, EntryPrice, ClosingPrice, Quantity, Status, Notes
            FROM Trades
            ORDER BY OpenDate DESC
            """;

        await using var reader = await command.ExecuteReaderAsync();
        while (await reader.ReadAsync())
            trades.Add(MapRow(reader));

        return trades;
    }

    /// <inheritdoc/>
    public async Task<int> AddAsync(Trade trade)
    {
        await using var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        await using var command = connection.CreateCommand();
        command.CommandText = """
            INSERT INTO Trades (Ticker, StrategyType, Strike, OpenDate, CloseDate,
                                ExpirationDate, EntryPrice, ClosingPrice, Quantity,
                                PL, GL, Status, Notes)
            VALUES (@Ticker, @StrategyType, @Strike, @OpenDate, @CloseDate,
                    @ExpirationDate, @EntryPrice, @ClosingPrice, @Quantity,
                    @PL, @GL, @Status, @Notes);
            SELECT last_insert_rowid();
            """;

        BindAll(command, trade);
        var rowId = (long)(await command.ExecuteScalarAsync())!;
        return (int)rowId;
    }

    /// <inheritdoc/>
    public async Task UpdateAsync(Trade trade)
    {
        await using var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        await using var command = connection.CreateCommand();
        command.CommandText = """
            UPDATE Trades
            SET Ticker = @Ticker, StrategyType = @StrategyType, Strike = @Strike,
                OpenDate = @OpenDate, CloseDate = @CloseDate, ExpirationDate = @ExpirationDate,
                EntryPrice = @EntryPrice, ClosingPrice = @ClosingPrice, Quantity = @Quantity,
                PL = @PL, GL = @GL, Status = @Status, Notes = @Notes
            WHERE Id = @Id
            """;

        BindAll(command, trade);
        command.Parameters.AddWithValue("@Id", trade.Id);
        await command.ExecuteNonQueryAsync();
    }

    /// <inheritdoc/>
    public async Task DeleteAsync(int id)
    {
        await using var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        await using var command = connection.CreateCommand();
        command.CommandText = "DELETE FROM Trades WHERE Id = @Id";
        command.Parameters.AddWithValue("@Id", id);
        await command.ExecuteNonQueryAsync();
    }

    /// <summary>
    /// Binds all Trade fields to the command parameters shared by INSERT and UPDATE.
    /// </summary>
    private static void BindAll(SqliteCommand cmd, Trade t)
    {
        cmd.Parameters.AddWithValue("@Ticker", t.Ticker);
        cmd.Parameters.AddWithValue("@StrategyType", (int)t.StrategyType);
        cmd.Parameters.AddWithValue("@Strike", (object?)t.Strike ?? DBNull.Value);
        cmd.Parameters.AddWithValue("@OpenDate", t.OpenDate.ToString("yyyy-MM-dd"));
        cmd.Parameters.AddWithValue("@CloseDate",
            t.CloseDate is DateOnly cd ? cd.ToString("yyyy-MM-dd") : DBNull.Value);
        cmd.Parameters.AddWithValue("@ExpirationDate",
            t.ExpirationDate is DateOnly ed ? ed.ToString("yyyy-MM-dd") : DBNull.Value);
        cmd.Parameters.AddWithValue("@EntryPrice",
            t.EntryPrice.ToString(CultureInfo.InvariantCulture));
        cmd.Parameters.AddWithValue("@ClosingPrice",
            t.ClosingPrice is decimal cp ? cp.ToString(CultureInfo.InvariantCulture) : DBNull.Value);
        cmd.Parameters.AddWithValue("@Quantity", t.Quantity);
        cmd.Parameters.AddWithValue("@PL",
            t.PL is decimal pl ? pl.ToString(CultureInfo.InvariantCulture) : DBNull.Value);
        cmd.Parameters.AddWithValue("@GL",
            t.GL is decimal gl ? gl.ToString(CultureInfo.InvariantCulture) : DBNull.Value);
        cmd.Parameters.AddWithValue("@Status", (int)t.Status);
        cmd.Parameters.AddWithValue("@Notes", (object?)t.Notes ?? DBNull.Value);
    }

    /// <summary>
    /// Maps a single reader row to a <see cref="Trade"/> instance.
    /// Column order must match the SELECT in <see cref="GetAllAsync"/>.
    /// </summary>
    private static Trade MapRow(SqliteDataReader r) => new()
    {
        Id             = r.GetInt32(0),
        Ticker         = r.GetString(1),
        StrategyType   = (StrategyType)r.GetInt32(2),
        Strike         = r.IsDBNull(3) ? null : r.GetString(3),
        OpenDate       = ParseDate(r.GetString(4)),
        CloseDate      = r.IsDBNull(5) ? null : ParseDate(r.GetString(5)),
        ExpirationDate = r.IsDBNull(6) ? null : ParseDate(r.GetString(6)),
        EntryPrice     = ParseDecimal(r.GetString(7)),
        ClosingPrice   = r.IsDBNull(8) ? null : ParseDecimal(r.GetString(8)),
        Quantity       = r.GetInt32(9),
        Status         = (TradeStatus)r.GetInt32(10),
        Notes          = r.IsDBNull(11) ? null : r.GetString(11),
    };

    private static DateOnly ParseDate(string s) =>
        DateOnly.ParseExact(s, "yyyy-MM-dd", CultureInfo.InvariantCulture);

    private static decimal ParseDecimal(string s) =>
        decimal.Parse(s, CultureInfo.InvariantCulture);
}
