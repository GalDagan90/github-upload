using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using TradingJournal.Contracts;
using TradingJournal.Models;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the Calendar tab.
/// Displays a monthly grid of 42 cells with per-day G/L totals and a flyout trade list.
/// Subscribes to <see cref="TradesChangedMessage"/> to stay in sync with the Trade Log.
/// </summary>
public partial class CalendarViewModel : ViewModelBase
{
    private readonly ITradeRepository _repository;
    private List<Trade> _allTrades = [];
    private int _currentYear;
    private int _currentMonth;

    /// <summary>Label for the currently displayed month (e.g. "May 2026").</summary>
    public string CurrentMonthLabel { get; private set; } = string.Empty;

    /// <summary>Sum of G/L across all closed/assigned trades in the currently viewed month.</summary>
    public decimal MonthTotalGL { get; private set; }

    /// <summary>Month total G/L formatted as currency (e.g. "$1,234.56").</summary>
    public string MonthTotalGLFormatted => $"${MonthTotalGL:N2}";

    /// <summary>Fixed Monday-first column headers for the calendar grid.</summary>
    public static IReadOnlyList<string> DayOfWeekHeaders { get; } =
        ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"];

    /// <summary>Always 42 items (6 weeks × 7 days) representing the current month grid.</summary>
    public ObservableCollection<CalendarDayViewModel> Days { get; } = [];

    /// <summary>
    /// Initialises the ViewModel for the current month and subscribes to trade-change notifications.
    /// </summary>
    /// <param name="repository">Provides access to trade data for G/L calculations.</param>
    public CalendarViewModel(ITradeRepository repository)
    {
        _repository = repository;
        var today = DateOnly.FromDateTime(DateTime.Today);
        _currentYear  = today.Year;
        _currentMonth = today.Month;

        WeakReferenceMessenger.Default.Register<TradesChangedMessage>(this,
            async (_, _) => await ReloadAsync());
    }

    /// <summary>Loads all trades from the database and builds the initial calendar grid.</summary>
    public async Task InitializeAsync() => await ReloadAsync();

    private async Task ReloadAsync()
    {
        var all = await _repository.GetAllAsync();
        _allTrades = [.. all];
        BuildGrid();
    }

    /// <summary>Navigates to the previous calendar month.</summary>
    [RelayCommand]
    private void PreviousMonth()
    {
        if (_currentMonth == 1) { _currentMonth = 12; _currentYear--; }
        else _currentMonth--;
        BuildGrid();
    }

    /// <summary>Navigates to the next calendar month.</summary>
    [RelayCommand]
    private void NextMonth()
    {
        if (_currentMonth == 12) { _currentMonth = 1; _currentYear++; }
        else _currentMonth++;
        BuildGrid();
    }

    private void BuildGrid()
    {
        var firstDay    = new DateOnly(_currentYear, _currentMonth, 1);
        int daysInMonth = DateTime.DaysInMonth(_currentYear, _currentMonth);

        CurrentMonthLabel = firstDay.ToString("MMMM yyyy", CultureInfo.InvariantCulture);
        OnPropertyChanged(nameof(CurrentMonthLabel));

        var tradesByDate = _allTrades
            .Where(t => t.Status != TradeStatus.Open && t.CloseDate.HasValue)
            .GroupBy(t => t.CloseDate!.Value)
            .ToDictionary(g => g.Key, g => g.ToList());

        // Monday-first: Monday=0, Sunday=6
        int startDow = ((int)firstDay.DayOfWeek + 6) % 7;

        Days.Clear();

        // Padding from previous month
        if (startDow > 0)
        {
            var prevFirst  = firstDay.AddMonths(-1);
            int daysInPrev = DateTime.DaysInMonth(prevFirst.Year, prevFirst.Month);
            for (int i = startDow - 1; i >= 0; i--)
                Days.Add(MakeDayVm(
                    new DateOnly(prevFirst.Year, prevFirst.Month, daysInPrev - i),
                    isCurrentMonth: false, tradesByDate));
        }

        // Current month
        for (int d = 1; d <= daysInMonth; d++)
            Days.Add(MakeDayVm(
                new DateOnly(_currentYear, _currentMonth, d),
                isCurrentMonth: true, tradesByDate));

        // Pad to 42 with next month
        var nextFirst = firstDay.AddMonths(1);
        int pad = 42 - Days.Count;
        for (int d = 1; d <= pad; d++)
            Days.Add(MakeDayVm(
                new DateOnly(nextFirst.Year, nextFirst.Month, d),
                isCurrentMonth: false, tradesByDate));

        MonthTotalGL = Days
            .Where(d => d.IsCurrentMonth && d.TotalGL.HasValue)
            .Sum(d => d.TotalGL!.Value);
        OnPropertyChanged(nameof(MonthTotalGL));
        OnPropertyChanged(nameof(MonthTotalGLFormatted));
    }

    private static CalendarDayViewModel MakeDayVm(
        DateOnly date, bool isCurrentMonth, Dictionary<DateOnly, List<Trade>> tradesByDate)
    {
        if (!isCurrentMonth || !tradesByDate.TryGetValue(date, out var trades))
            return new CalendarDayViewModel(date, isCurrentMonth, null, []);

        var summaries = trades
            .Select(t => new TradeSummary(t.Ticker, FormatStrategy(t.StrategyType), t.GL))
            .ToList();

        return new CalendarDayViewModel(date, isCurrentMonth: true,
            trades.Sum(t => t.GL ?? 0m), summaries);
    }

    private static string FormatStrategy(StrategyType s) => s switch
    {
        StrategyType.Stock                => "Stock",
        StrategyType.ShortPut             => "Short Put",
        StrategyType.ShortCall            => "Short Call",
        StrategyType.CoveredCall          => "Covered Call",
        StrategyType.CashSecuredPut       => "CSP",
        StrategyType.BullPutCreditSpread  => "Bull Put",
        StrategyType.BearCallCreditSpread => "Bear Call",
        StrategyType.IronCondor           => "Iron Condor",
        StrategyType.LongCall             => "Long Call",
        StrategyType.LongPut              => "Long Put",
        StrategyType.Custom               => "Custom",
        _                                 => s.ToString(),
    };
}
