using Avalonia.Controls;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Analytics tab view.
/// All logic lives in <see cref="TradingJournal.ViewModels.AnalyticsViewModel"/>.
/// </summary>
public partial class AnalyticsView : UserControl
{
    /// <summary>Initialises the AnalyticsView and compiles the AXAML bindings.</summary>
    public AnalyticsView()
    {
        InitializeComponent();
    }
}
