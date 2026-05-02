using System;
using Avalonia.Controls;
using TradingJournal.ViewModels;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Analytics tab.
/// All business logic lives in <see cref="AnalyticsViewModel"/>.
/// The only code here propagates custom date-picker selections back to the ViewModel.
/// </summary>
public partial class AnalyticsView : UserControl
{
    /// <summary>Initialises the AnalyticsView and compiles the AXAML bindings.</summary>
    public AnalyticsView() => InitializeComponent();

    private void OnCustomStartChanged(object? sender, SelectionChangedEventArgs e)
    {
        if (e.RemovedItems.Count == 0) return;
        if (sender is CalendarDatePicker picker && DataContext is AnalyticsViewModel vm)
            vm.CustomStart = picker.SelectedDate is { } dt ? DateOnly.FromDateTime(dt) : null;
    }

    private void OnCustomEndChanged(object? sender, SelectionChangedEventArgs e)
    {
        if (e.RemovedItems.Count == 0) return;
        if (sender is CalendarDatePicker picker && DataContext is AnalyticsViewModel vm)
            vm.CustomEnd = picker.SelectedDate is { } dt ? DateOnly.FromDateTime(dt) : null;
    }
}
