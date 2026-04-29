using Avalonia.Controls;

namespace TradingJournal.Views;

/// <summary>
/// Code-behind for the Trade Log tab view.
/// All logic lives in <see cref="TradingJournal.ViewModels.TradeLogViewModel"/>.
/// </summary>
public partial class TradeLogView : UserControl
{
    /// <summary>Initialises the TradeLogView and compiles the AXAML bindings.</summary>
    public TradeLogView()
    {
        InitializeComponent();
    }
}
