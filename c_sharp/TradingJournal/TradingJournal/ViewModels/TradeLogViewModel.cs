using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the Trade Log tab.
/// Owns the trade collection, handles inline editing, add/delete, and live filtering.
/// </summary>
public partial class TradeLogViewModel : ViewModelBase
{
    private readonly ITradeRepository _repository;
    private readonly IDialogService _dialogs;

    /// <summary>Master collection — every trade regardless of active filters.</summary>
    public ObservableCollection<Trade> Trades { get; } = new();

    /// <summary>Filtered view of <see cref="Trades"/> — what the DataGrid is bound to.</summary>
    public ObservableCollection<Trade> FilteredTrades { get; } = new();

    /// <summary>The currently selected DataGrid row. Kept in sync by the View.</summary>
    [ObservableProperty]
    private Trade? _selectedTrade;

    /// <summary>Case-insensitive ticker substring filter. Empty string means no filter.</summary>
    [ObservableProperty]
    private string _filterTicker = string.Empty;

    /// <summary>One entry per <see cref="StrategyType"/> value; all selected by default.</summary>
    public IReadOnlyList<FilterItem<StrategyType>> StrategyFilters { get; }

    /// <summary>One entry per <see cref="TradeStatus"/> value; all selected by default.</summary>
    public IReadOnlyList<FilterItem<TradeStatus>> StatusFilters { get; }

    /// <summary>Button label for the Strategy filter dropdown.</summary>
    public string StrategyFilterLabel =>
        StrategyFilters.All(f => f.IsSelected)
            ? "Strategy: All"
            : $"Strategy: {StrategyFilters.Count(f => f.IsSelected)} selected";

    /// <summary>Button label for the Status filter dropdown.</summary>
    public string StatusFilterLabel =>
        StatusFilters.All(f => f.IsSelected)
            ? "Status: All"
            : $"Status: {StatusFilters.Count(f => f.IsSelected)} selected";

    /// <summary>
    /// Initialises the ViewModel and builds the filter item lists.
    /// </summary>
    /// <param name="repository">Read/write access to the Trades database table.</param>
    /// <param name="dialogs">Used to prompt the user before destructive operations.</param>
    public TradeLogViewModel(ITradeRepository repository, IDialogService dialogs)
    {
        _repository = repository;
        _dialogs = dialogs;

        StrategyFilters = Enum.GetValues<StrategyType>()
            .Select(s => new FilterItem<StrategyType>(s, s.ToString(), ApplyFilters))
            .ToList();

        StatusFilters = Enum.GetValues<TradeStatus>()
            .Select(s => new FilterItem<TradeStatus>(s, s.ToString(), ApplyFilters))
            .ToList();
    }

    /// <summary>
    /// Loads all persisted trades from the database into <see cref="Trades"/> then applies filters.
    /// Seeds three sample rows on first launch so the grid is never empty.
    /// </summary>
    public async Task InitializeAsync()
    {
        try
        {
            var trades = await _repository.GetAllAsync();
            foreach (var t in trades)
                Trades.Add(t);

            if (Trades.Count == 0)
                await SeedDummyDataAsync();

            ApplyFilters();
        }
        catch (Exception ex)
        {
            await _dialogs.ShowErrorAsync("Database Error",
                $"Could not load trades from the database.\n\n{ex.Message}");
        }
    }

    /// <summary>
    /// Persists an edited trade row. Normalises <see cref="Trade.Ticker"/> to upper-case.
    /// Called from the View's event handlers on every committed edit.
    /// </summary>
    /// <param name="trade">The trade row that was just edited.</param>
    public async Task SaveTradeAsync(Trade trade)
    {
        trade.Ticker = trade.Ticker?.ToUpperInvariant() ?? string.Empty;
        try
        {
            await _repository.UpdateAsync(trade);
            WeakReferenceMessenger.Default.Send(new TradesChangedMessage());
        }
        catch (Exception ex)
        {
            await _dialogs.ShowErrorAsync("Database Error",
                $"Could not save the trade.\n\n{ex.Message}");
        }
    }

    /// <summary>Appends a new blank trade row and selects it for immediate editing.</summary>
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
        try
        {
            trade.Id = await _repository.AddAsync(trade);
            WeakReferenceMessenger.Default.Send(new TradesChangedMessage());
            Trades.Add(trade);
            ApplyFilters();
            SelectedTrade = trade;
        }
        catch (Exception ex)
        {
            await _dialogs.ShowErrorAsync("Database Error",
                $"Could not add the trade.\n\n{ex.Message}");
        }
    }

    /// <summary>
    /// Deletes the given trade after a confirmation prompt.
    /// Removes the row from both <see cref="Trades"/> and the database.
    /// </summary>
    /// <param name="trade">Trade to delete. Does nothing when <see langword="null"/>.</param>
    [RelayCommand]
    private async Task DeleteTradeAsync(Trade? trade)
    {
        if (trade is null) return;

        if (!await _dialogs.ConfirmAsync($"Delete the {trade.Ticker} trade? This cannot be undone."))
            return;

        try
        {
            await _repository.DeleteAsync(trade.Id);
            WeakReferenceMessenger.Default.Send(new TradesChangedMessage());
            Trades.Remove(trade);
            ApplyFilters();
        }
        catch (Exception ex)
        {
            await _dialogs.ShowErrorAsync("Database Error",
                $"Could not delete the trade.\n\n{ex.Message}");
        }
    }

    /// <summary>
    /// Rebuilds <see cref="FilteredTrades"/> from <see cref="Trades"/> using all active filters.
    /// All three filters are combined with AND logic; an unset filter matches every trade.
    /// </summary>
    private void ApplyFilters()
    {
        var ticker = FilterTicker?.Trim() ?? string.Empty;
        var strategies = StrategyFilters.Where(f => f.IsSelected).Select(f => f.Value).ToHashSet();
        var statuses = StatusFilters.Where(f => f.IsSelected).Select(f => f.Value).ToHashSet();

        FilteredTrades.Clear();
        foreach (var t in Trades)
        {
            if (!string.IsNullOrEmpty(ticker) &&
                !(t.Ticker?.Contains(ticker, StringComparison.OrdinalIgnoreCase) ?? false))
                continue;
            if (!strategies.Contains(t.StrategyType))
                continue;
            if (!statuses.Contains(t.Status))
                continue;
            FilteredTrades.Add(t);
        }

        OnPropertyChanged(nameof(StrategyFilterLabel));
        OnPropertyChanged(nameof(StatusFilterLabel));
    }

    /// <summary>Selects all strategy filter options.</summary>
    [RelayCommand]
    private void SelectAllStrategyFilters()
    {
        foreach (var f in StrategyFilters)
            f.IsSelected = true;
    }

    /// <summary>Deselects all strategy filter options.</summary>
    [RelayCommand]
    private void ClearStrategyFilters()
    {
        foreach (var f in StrategyFilters)
            f.IsSelected = false;
    }

    /// <summary>Selects all status filter options.</summary>
    [RelayCommand]
    private void SelectAllStatusFilters()
    {
        foreach (var f in StatusFilters)
            f.IsSelected = true;
    }

    /// <summary>Deselects all status filter options.</summary>
    [RelayCommand]
    private void ClearStatusFilters()
    {
        foreach (var f in StatusFilters)
            f.IsSelected = false;
    }

    partial void OnFilterTickerChanged(string value) => ApplyFilters();

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
