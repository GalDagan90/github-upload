using System.Linq;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.Media;
using Avalonia.VisualTree;
using TradingJournal.Models;
using TradingJournal.ViewModels;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Trade Log tab.
/// All business logic lives in <see cref="TradeLogViewModel"/>.
/// </summary>
public partial class TradeLogView : UserControl
{
    /// <summary>Initialises the TradeLogView and compiles the AXAML bindings.</summary>
    public TradeLogView() => InitializeComponent();

    private void OnLoadingRow(object? sender, DataGridRowEventArgs e)
    {
        if (e.Row.DataContext is Trade trade)
            e.Row.Background = RowBrush(trade.Status);
    }

    private static IBrush RowBrush(TradeStatus status) => status switch
    {
        TradeStatus.Open     => new SolidColorBrush(Color.FromArgb(30, 59,  130, 246)),
        TradeStatus.Assigned => new SolidColorBrush(Color.FromArgb(30, 245, 158,  11)),
        _                    => Brushes.Transparent,
    };

    private async void OnDatePickerChanged(object? sender, SelectionChangedEventArgs e)
    {
        if (e.RemovedItems.Count == 0) return;
        if (sender is CalendarDatePicker { DataContext: Trade trade } && DataContext is TradeLogViewModel vm)
            await vm.SaveTradeAsync(trade);
    }

    // RemovedItems.Count > 0 guards against the initial binding firing SelectionChanged on load.
    private async void OnEnumComboChanged(object? sender, SelectionChangedEventArgs e)
    {
        if (e.RemovedItems.Count == 0) return;
        if (sender is not ComboBox { DataContext: Trade trade }) return;
        if (DataContext is not TradeLogViewModel vm) return;

        await vm.SaveTradeAsync(trade);

        var row = (sender as Avalonia.Visual)?.GetSelfAndVisualAncestors().OfType<DataGridRow>().FirstOrDefault();
        if (row is not null)
            row.Background = RowBrush(trade.Status);
    }

    private async void OnRowEditEnded(object? sender, DataGridRowEditEndedEventArgs e)
    {
        if (e.EditAction == DataGridEditAction.Commit
            && e.Row.DataContext is Trade trade
            && DataContext is TradeLogViewModel vm)
        {
            await vm.SaveTradeAsync(trade);
        }
    }

    // Selects the row under the pointer on right-click so the ContextMenu command
    // always targets the intended row rather than the previously selected one.
    private void OnDataGridPointerPressed(object? sender, PointerPressedEventArgs e)
    {
        if (!e.GetCurrentPoint(this).Properties.IsRightButtonPressed) return;

        var row = (e.Source as Avalonia.Visual)
            ?.GetSelfAndVisualAncestors()
            .OfType<DataGridRow>()
            .FirstOrDefault();

        if (row?.DataContext is Trade trade && DataContext is TradeLogViewModel vm)
            vm.SelectedTrade = trade;
    }
}
