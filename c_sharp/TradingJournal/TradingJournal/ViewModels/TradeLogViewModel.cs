using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the Trade Log tab.
/// Owns the live trade collection, handles inline editing, and coordinates add/delete operations.
/// </summary>
public partial class TradeLogViewModel : ViewModelBase
{
    private readonly ITradeRepository _repository;
    private readonly IDialogService _dialogs;

    /// <summary>The live collection of trade rows bound to the DataGrid.</summary>
    public ObservableCollection<Trade> Trades { get; } = new();

    /// <summary>The currently selected DataGrid row. Kept in sync by the View.</summary>
    [ObservableProperty]
    private Trade? _selectedTrade;

    /// <summary>
    /// Initialises the ViewModel with its dependencies.
    /// </summary>
    /// <param name="repository">Read/write access to the Trades database table.</param>
    /// <param name="dialogs">Used to prompt the user before destructive operations.</param>
    public TradeLogViewModel(ITradeRepository repository, IDialogService dialogs)
    {
        _repository = repository;
        _dialogs = dialogs;
    }

    /// <summary>
    /// Loads all persisted trades from the database into <see cref="Trades"/>.
    /// Seeds three sample rows on first launch so the grid is never empty.
    /// </summary>
    public async Task InitializeAsync()
    {
        var trades = await _repository.GetAllAsync();
        foreach (var t in trades)
            Trades.Add(t);

        if (Trades.Count == 0)
            await SeedDummyDataAsync();
    }

    /// <summary>
    /// Persists an edited trade row. Normalises <see cref="Trade.Ticker"/> to upper-case.
    /// Called from the View's <c>RowEditEnded</c> handler on every committed row edit.
    /// </summary>
    /// <param name="trade">The trade row that was just edited.</param>
    public async Task SaveTradeAsync(Trade trade)
    {
        trade.Ticker = trade.Ticker?.ToUpperInvariant() ?? string.Empty;
        await _repository.UpdateAsync(trade);
    }

    /// <summary>Appends a new blank trade row to the grid and selects it for immediate editing.</summary>
    [RelayCommand]
    private async Task AddTradeAsync()
    {
        var trade = new Trade
        {
            Ticker = "NEW",
            StrategyType = StrategyType.Stock,
            OpenDate = DateOnly.FromDateTime(DateTime.Today),
            Status = TradeStatus.Open,
            Quantity = 1,
            EntryPrice = 0m,
        };
        trade.Id = await _repository.AddAsync(trade);
        Trades.Add(trade);
        SelectedTrade = trade;
    }

    /// <summary>
    /// Deletes the given trade after an optional confirmation prompt.
    /// Removes the row from both <see cref="Trades"/> and the database.
    /// </summary>
    /// <param name="trade">Trade to delete. Does nothing when <see langword="null"/>.</param>
    [RelayCommand]
    private async Task DeleteTradeAsync(Trade? trade)
    {
        if (trade is null) return;

        if (!await _dialogs.ConfirmAsync($"Delete the {trade.Ticker} trade? This cannot be undone."))
            return;

        await _repository.DeleteAsync(trade.Id);
        Trades.Remove(trade);
    }

    private async Task SeedDummyDataAsync()
    {
        var samples = new[]
        {
            new Trade
            {
                Ticker = "AAPL",
                StrategyType = StrategyType.CoveredCall,
                Strike = "185",
                OpenDate = new DateOnly(2026, 3, 10),
                CloseDate = new DateOnly(2026, 4, 5),
                ExpirationDate = new DateOnly(2026, 4, 18),
                EntryPrice = 3.50m,
                ClosingPrice = 0.10m,
                Quantity = 2,
                PL = 3.40m,
                GL = 680m,
                Status = TradeStatus.Closed,
                Notes = "Closed early at 97% profit",
            },
            new Trade
            {
                Ticker = "SPY",
                StrategyType = StrategyType.BullPutCreditSpread,
                Strike = "500/495",
                OpenDate = new DateOnly(2026, 4, 1),
                ExpirationDate = new DateOnly(2026, 4, 30),
                EntryPrice = 1.25m,
                Quantity = 5,
                Status = TradeStatus.Open,
            },
            new Trade
            {
                Ticker = "TSLA",
                StrategyType = StrategyType.ShortPut,
                Strike = "200",
                OpenDate = new DateOnly(2026, 3, 20),
                CloseDate = new DateOnly(2026, 4, 10),
                ExpirationDate = new DateOnly(2026, 4, 17),
                EntryPrice = 4.00m,
                ClosingPrice = 0.00m,
                Quantity = 1,
                PL = 4.00m,
                GL = 400m,
                Status = TradeStatus.Assigned,
                Notes = "Assigned at expiration",
            },
        };

        foreach (var t in samples)
        {
            t.Id = await _repository.AddAsync(t);
            Trades.Add(t);
        }
    }
}
