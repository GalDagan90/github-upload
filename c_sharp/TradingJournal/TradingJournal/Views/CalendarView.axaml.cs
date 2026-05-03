using Avalonia.Controls;
using Avalonia.Controls.Primitives;
using Avalonia.Input;
using TradingJournal.ViewModels;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Calendar tab view.
/// Handles flyout opening on day cell click — one of the rare acceptable uses of code-behind
/// in this project, as Avalonia requires <see cref="FlyoutBase.ShowAttachedFlyout"/> to be
/// called from a control reference rather than via a command binding.
/// </summary>
public partial class CalendarView : UserControl
{
    /// <summary>Initialises the CalendarView and compiles the AXAML bindings.</summary>
    public CalendarView()
    {
        InitializeComponent();
    }

    private void DayCell_PointerPressed(object? sender, PointerPressedEventArgs e)
    {
        if (sender is Control control &&
            control.DataContext is CalendarDayViewModel day &&
            day.IsCurrentMonth &&
            day.Trades.Count > 0)
        {
            FlyoutBase.ShowAttachedFlyout(control);
        }
    }
}
