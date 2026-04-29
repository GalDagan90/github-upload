using Avalonia.Controls;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Calendar tab view.
/// All logic lives in <see cref="TradingJournal.ViewModels.CalendarViewModel"/>.
/// </summary>
public partial class CalendarView : UserControl
{
    /// <summary>Initialises the CalendarView and compiles the AXAML bindings.</summary>
    public CalendarView()
    {
        InitializeComponent();
    }
}
