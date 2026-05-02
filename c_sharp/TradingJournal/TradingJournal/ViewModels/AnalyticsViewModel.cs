using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the Analytics tab.
/// Loads closed/assigned trades from the database, applies date-range and ticker filters,
/// and exposes summary stats. Subscribes to <see cref="TradesChangedMessage"/> so it stays
/// in sync with the Trade Log tab without polling or shared state.
/// </summary>
public partial class AnalyticsViewModel : ViewModelBase
{
    private readonly ITradeRepository _repository;
    private List<Trade> _closedTrades = new();

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(IsCustomRange))]
    [NotifyPropertyChangedFor(nameof(IsAllTimeSelected))]
    [NotifyPropertyChangedFor(nameof(IsThisMonthSelected))]
    [NotifyPropertyChangedFor(nameof(IsThisYearSelected))]
    [NotifyPropertyChangedFor(nameof(IsCustomSelected))]
    private DateRangeOption _selectedRange = DateRangeOption.AllTime;

    [ObservableProperty]
    private DateOnly? _customStart;

    [ObservableProperty]
    private DateOnly? _customEnd;

    [ObservableProperty]
    private string _filterTicker = string.Empty;

    /// <summary>True when the Custom date range option is active.</summary>
    public bool IsCustomRange       => SelectedRange == DateRangeOption.Custom;

    /// <summary>True when All Time is the active range selection.</summary>
    public bool IsAllTimeSelected   => SelectedRange == DateRangeOption.AllTime;

    /// <summary>True when This Month is the active range selection.</summary>
    public bool IsThisMonthSelected => SelectedRange == DateRangeOption.ThisMonth;

    /// <summary>True when This Year is the active range selection.</summary>
    public bool IsThisYearSelected  => SelectedRange == DateRangeOption.ThisYear;

    /// <summary>True when Custom is the active range selection.</summary>
    public bool IsCustomSelected    => SelectedRange == DateRangeOption.Custom;

    /// <summary>Raw total P/L used for sign-based foreground color binding.</summary>
    public decimal TotalPL { get; private set; }

    /// <summary>Raw total G/L used for sign-based foreground color binding.</summary>
    public decimal TotalGL { get; private set; }

    /// <summary>Count of filtered closed/assigned trades.</summary>
    public int TotalTrades { get; private set; }

    /// <summary>Formatted total P/L for display (e.g. "$1,234.56").</summary>
    public string TotalPLFormatted     => $"${TotalPL:N2}";

    /// <summary>Formatted total G/L for display (e.g. "$3,200.00").</summary>
    public string TotalGLFormatted     => $"${TotalGL:N2}";

    /// <summary>Formatted trade count for display.</summary>
    public string TotalTradesFormatted => TotalTrades.ToString();

    /// <summary>
    /// Initialises the ViewModel, wires data access, and subscribes to trade-change notifications.
    /// </summary>
    /// <param name="repository">Used to load trade data on initialisation and after changes.</param>
    public AnalyticsViewModel(ITradeRepository repository)
    {
        _repository = repository;
        WeakReferenceMessenger.Default.Register<TradesChangedMessage>(this,
            async (_, _) => await ReloadAsync());
    }

    /// <summary>Loads all closed/assigned trades and computes the initial stats.</summary>
    public async Task InitializeAsync() => await ReloadAsync();

    private async Task ReloadAsync()
    {
        var all = await _repository.GetAllAsync();
        _closedTrades = all
            .Where(t => t.Status != TradeStatus.Open && t.CloseDate.HasValue)
            .ToList();
        Refresh();
    }

    private void Refresh()
    {
        var filtered = ApplyFilters(_closedTrades);

        TotalPL     = filtered.Sum(t => t.PL ?? 0m);
        TotalGL     = filtered.Sum(t => t.GL ?? 0m);
        TotalTrades = filtered.Count;

        OnPropertyChanged(nameof(TotalPL));
        OnPropertyChanged(nameof(TotalGL));
        OnPropertyChanged(nameof(TotalTrades));
        OnPropertyChanged(nameof(TotalPLFormatted));
        OnPropertyChanged(nameof(TotalGLFormatted));
        OnPropertyChanged(nameof(TotalTradesFormatted));
    }

    private List<Trade> ApplyFilters(List<Trade> source)
    {
        var today = DateOnly.FromDateTime(DateTime.Today);

        IEnumerable<Trade> trades = SelectedRange switch
        {
            DateRangeOption.ThisMonth => source.Where(t =>
                t.CloseDate!.Value.Year  == today.Year &&
                t.CloseDate!.Value.Month == today.Month),
            DateRangeOption.ThisYear => source.Where(t =>
                t.CloseDate!.Value.Year == today.Year),
            DateRangeOption.Custom => source.Where(t =>
                (CustomStart == null || t.CloseDate!.Value >= CustomStart) &&
                (CustomEnd   == null || t.CloseDate!.Value <= CustomEnd)),
            _ => source,
        };

        if (!string.IsNullOrWhiteSpace(FilterTicker))
            trades = trades.Where(t =>
                t.Ticker.Contains(FilterTicker.Trim(), StringComparison.OrdinalIgnoreCase));

        return trades.ToList();
    }

    partial void OnSelectedRangeChanged(DateRangeOption value) => Refresh();
    partial void OnCustomStartChanged(DateOnly? value)         => Refresh();
    partial void OnCustomEndChanged(DateOnly? value)           => Refresh();
    partial void OnFilterTickerChanged(string value)           => Refresh();

    /// <summary>Sets the date range to All Time.</summary>
    [RelayCommand]
    private void SetRangeAllTime()   => SelectedRange = DateRangeOption.AllTime;

    /// <summary>Sets the date range to the current calendar month.</summary>
    [RelayCommand]
    private void SetRangeThisMonth() => SelectedRange = DateRangeOption.ThisMonth;

    /// <summary>Sets the date range to the current calendar year.</summary>
    [RelayCommand]
    private void SetRangeThisYear()  => SelectedRange = DateRangeOption.ThisYear;

    /// <summary>Activates the custom date range and reveals the date pickers.</summary>
    [RelayCommand]
    private void SetRangeCustom()    => SelectedRange = DateRangeOption.Custom;
}
