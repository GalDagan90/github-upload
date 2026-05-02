using System.Linq;
using Avalonia.Controls;
using Avalonia.Input;
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

    private void OnLoadingRow(object? sender, DataGridRowEventArgs e) { }

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
