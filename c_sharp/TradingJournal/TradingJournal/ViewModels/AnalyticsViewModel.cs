using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the Analytics tab.
/// Loads closed/assigned trades from the database, applies date-range and ticker filters,
/// and exposes summary stats and chart series. Subscribes to <see cref="TradesChangedMessage"/>
/// so it stays in sync with the Trade Log tab without polling or shared state.
/// </summary>
public partial class AnalyticsViewModel : ViewModelBase
{
    private readonly ITradeRepository _repository;
    private List<Trade> _closedTrades = new();

    // ── Filters ────────────────────────────────────────────────────────────

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

    // ── Summary stats ───────────────────────────────────────────────────────

    /// <summary>Raw total P/L — used for sign-based foreground colour binding.</summary>
    public decimal TotalPL { get; private set; }

    /// <summary>Raw total G/L — used for sign-based foreground colour binding.</summary>
    public decimal TotalGL { get; private set; }

    /// <summary>Count of filtered closed/assigned trades.</summary>
    public int TotalTrades { get; private set; }

    /// <summary>Formatted total P/L for display (e.g. "$1,234.56").</summary>
    public string TotalPLFormatted     => $"${TotalPL:N2}";

    /// <summary>Formatted total G/L for display (e.g. "$3,200.00").</summary>
    public string TotalGLFormatted     => $"${TotalGL:N2}";

    /// <summary>Formatted trade count for display.</summary>
    public string TotalTradesFormatted => TotalTrades.ToString();

    // ── Chart series ────────────────────────────────────────────────────────

    /// <summary>Shared Y-axis for all bar charts — formats tick values as currency.</summary>
    public Axis[] CurrencyYAxes { get; } =
    [
        new Axis { Labeler = value => $"${value:N0}", TextSize = 11 }
    ];

    /// <summary>Per-ticker sum of P/L — green for gains, red for losses.</summary>
    public ISeries[] PLByTickerSeries { get; private set; } = [];

    /// <summary>X-axis labels for the P/L by Ticker chart.</summary>
    public Axis[] PLByTickerXAxes { get; private set; } = [new Axis()];

    /// <summary>Per-ticker sum of G/L — green for gains, red for losses.</summary>
    public ISeries[] GLByTickerSeries { get; private set; } = [];

    /// <summary>X-axis labels for the G/L by Ticker chart.</summary>
    public Axis[] GLByTickerXAxes { get; private set; } = [new Axis()];

    /// <summary>Per-strategy sum of P/L — green for gains, red for losses.</summary>
    public ISeries[] PLByStrategySeries { get; private set; } = [];

    /// <summary>X-axis labels for the P/L by Strategy chart.</summary>
    public Axis[] PLByStrategyXAxes { get; private set; } = [new Axis()];

    /// <summary>Strategy distribution — one pie slice per strategy sized by trade count.</summary>
    public ISeries[] StrategiesPieSeries { get; private set; } = [];

    // ── Constructor / lifecycle ─────────────────────────────────────────────

    /// <summary>
    /// Initialises the ViewModel with data access and subscribes to trade-change notifications.
    /// </summary>
    /// <param name="repository">Used to reload trade data when notified of changes.</param>
    public AnalyticsViewModel(ITradeRepository repository)
    {
        _repository = repository;
        WeakReferenceMessenger.Default.Register<TradesChangedMessage>(this,
            async (_, _) => await ReloadAsync());
    }

    /// <summary>Loads all closed/assigned trades from the database and computes initial stats and charts.</summary>
    public async Task InitializeAsync() => await ReloadAsync();

    private async Task ReloadAsync()
    {
        var all = await _repository.GetAllAsync();
        _closedTrades = all
            .Where(t => t.Status != TradeStatus.Open && t.CloseDate.HasValue)
            .ToList();
        Refresh();
    }

    // ── Filtering & refresh ─────────────────────────────────────────────────

    private void Refresh()
    {
        var filtered = ApplyFilters(_closedTrades);
        RefreshStats(filtered);
        RefreshCharts(filtered);
    }

    private void RefreshStats(List<Trade> filtered)
    {
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

    private void RefreshCharts(List<Trade> filtered)
    {
        (PLByTickerSeries, PLByTickerXAxes) = BuildBarsByGroup(
            filtered, t => t.Ticker, t => (double)(t.PL ?? 0m));

        (GLByTickerSeries, GLByTickerXAxes) = BuildBarsByGroup(
            filtered, t => t.Ticker, t => (double)(t.GL ?? 0m));

        (PLByStrategySeries, PLByStrategyXAxes) = BuildBarsByGroup(
            filtered, t => FormatStrategy(t.StrategyType), t => (double)(t.PL ?? 0m));

        StrategiesPieSeries = BuildStrategiesPie(filtered);

        OnPropertyChanged(nameof(PLByTickerSeries));
        OnPropertyChanged(nameof(PLByTickerXAxes));
        OnPropertyChanged(nameof(GLByTickerSeries));
        OnPropertyChanged(nameof(GLByTickerXAxes));
        OnPropertyChanged(nameof(PLByStrategySeries));
        OnPropertyChanged(nameof(PLByStrategyXAxes));
        OnPropertyChanged(nameof(StrategiesPieSeries));
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

    // ── Chart builders ──────────────────────────────────────────────────────

    private static (ISeries[], Axis[]) BuildBarsByGroup(
        List<Trade> trades,
        Func<Trade, string> keySelector,
        Func<Trade, double> valueSelector)
    {
        var groups = trades
            .GroupBy(keySelector)
            .Select(g => (Label: g.Key, Value: g.Sum(valueSelector)))
            .OrderBy(x => x.Label)
            .ToList();

        if (groups.Count == 0)
            return ([new ColumnSeries<ObservableValue> { Values = [] }], [new Axis()]);

        var labels = groups.Select(g => g.Label).ToArray();

        ISeries[] series =
        [
            new ColumnSeries<ObservableValue>
            {
                Values        = groups.Select(g => new ObservableValue(g.Value)).ToArray(),
                Fill          = new SolidColorPaint(SKColor.Parse("#16A34A")),
                NegativesFill = new SolidColorPaint(SKColor.Parse("#DC2626")),
                MaxBarWidth   = 40,
            }
        ];

        Axis[] xAxes =
        [
            new Axis
            {
                Labels         = labels,
                TextSize       = 11,
                LabelsRotation = -45,
            }
        ];

        return (series, xAxes);
    }

    private static ISeries[] BuildStrategiesPie(List<Trade> trades)
    {
        var groups = trades
            .GroupBy(t => t.StrategyType)
            .Select(g => (Label: FormatStrategy(g.Key), Count: g.Count()))
            .OrderByDescending(x => x.Count)
            .ToList();

        if (groups.Count == 0)
            return [];

        return groups.Select(g =>
            (ISeries)new PieSeries<ObservableValue>
            {
                Values = [new ObservableValue(g.Count)],
                Name   = g.Label,
            })
            .ToArray();
    }

    // ── Helpers ─────────────────────────────────────────────────────────────

    private static string FormatStrategy(StrategyType s) => s switch
    {
        StrategyType.Stock              => "Stock",
        StrategyType.ShortPut           => "Short Put",
        StrategyType.ShortCall          => "Short Call",
        StrategyType.CoveredCall        => "Covered Call",
        StrategyType.CashSecuredPut     => "CSP",
        StrategyType.BullPutCreditSpread => "Bull Put",
        StrategyType.BearCallCreditSpread => "Bear Call",
        StrategyType.IronCondor         => "Iron Condor",
        StrategyType.LongCall           => "Long Call",
        StrategyType.LongPut            => "Long Put",
        StrategyType.Custom             => "Custom",
        _                               => s.ToString(),
    };

    // ── Range commands ──────────────────────────────────────────────────────

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
