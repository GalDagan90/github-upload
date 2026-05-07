using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using TradingJournal.Contracts;

namespace TradingJournal.ViewModels;

/// <summary>
/// ViewModel for the application's main window.
/// Owns the three tab ViewModels, drives the theme toggle, and persists the
/// selected theme via <see cref="ISettingsService"/>.
/// </summary>
public partial class MainWindowViewModel : ViewModelBase
{
    private readonly ISettingsService _settingsService;

    /// <summary>Sub-ViewModel displayed in the Trade Log tab.</summary>
    public TradeLogViewModel TradeLog { get; }

    /// <summary>Sub-ViewModel displayed in the Analytics tab.</summary>
    public AnalyticsViewModel Analytics { get; }

    /// <summary>Sub-ViewModel displayed in the Calendar tab.</summary>
    public CalendarViewModel Calendar { get; }

    /// <summary>
    /// True when the application is currently using the dark theme.
    /// Changing this property triggers <see cref="ThemeToggleLabel"/> to update
    /// and notifies <c>App</c> (via PropertyChanged) to apply the new variant.
    /// </summary>
    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(ThemeToggleIcon))]
    [NotifyPropertyChangedFor(nameof(ThemeToggleTooltip))]
    private bool _isDarkTheme;

    /// <summary>
    /// Icon shown on the theme toggle button.
    /// ☀ = currently dark, click to go light. 🌙 = currently light, click to go dark.
    /// </summary>
    public string ThemeToggleIcon => IsDarkTheme ? "☀" : "🌙";

    /// <summary>
    /// Tooltip text for the theme toggle button.
    /// </summary>
    public string ThemeToggleTooltip => IsDarkTheme ? "Switch to Light Mode" : "Switch to Dark Mode";

    /// <summary>
    /// Initialises the MainWindowViewModel and creates the three tab ViewModels.
    /// </summary>
    /// <param name="settingsService">Used to persist and restore the selected theme.</param>
    /// <param name="tradeRepository">Passed through to all tab ViewModels for data access.</param>
    /// <param name="dialogs">Passed through to all tab ViewModels for user prompts and error dialogs.</param>
    public MainWindowViewModel(ISettingsService settingsService, ITradeRepository tradeRepository, IDialogService dialogs)
    {
        _settingsService = settingsService;
        TradeLog  = new TradeLogViewModel(tradeRepository, dialogs);
        Analytics = new AnalyticsViewModel(tradeRepository, dialogs);
        Calendar  = new CalendarViewModel(tradeRepository, dialogs);
    }

    /// <summary>
    /// Reads the persisted theme from the database and sets <see cref="IsDarkTheme"/> accordingly.
    /// Must be awaited before the window is shown so the correct theme is applied on first paint.
    /// </summary>
    public async Task InitializeAsync()
    {
        var saved = await _settingsService.GetAsync("AppTheme");
        IsDarkTheme = saved == "Dark";
    }

    /// <summary>
    /// Toggles between dark and light theme and persists the new choice.
    /// The actual Avalonia ThemeVariant is applied in App.axaml.cs via PropertyChanged.
    /// </summary>
    [RelayCommand]
    private async Task ToggleThemeAsync()
    {
        IsDarkTheme = !IsDarkTheme;
        await _settingsService.SetAsync("AppTheme", IsDarkTheme ? "Dark" : "Light");
    }
}
