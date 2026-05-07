using System;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;
using TradingJournal.Services;
using TradingJournal.ViewModels;
using TradingJournal.Views;

namespace TradingJournal;

/// <summary>
/// Application entry point responsible for bootstrapping services,
/// initialising the database, restoring the saved theme, and creating the main window.
/// </summary>
public partial class App : Application
{
    /// <inheritdoc/>
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    /// <inheritdoc/>
    public override async void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            try
            {
                // 1. Initialise the database (creates tables if they don't exist).
                var dbInitializer = new DatabaseInitializer();
                await dbInitializer.InitializeAsync();

                // 2. Build services.
                var settingsService  = new SqliteSettingsService();
                var tradeRepository  = new SqliteTradeRepository();
                var dialogService    = new AvaloniaDialogService();

                // 3. Create the main ViewModel and restore persisted settings.
                var mainVm = new MainWindowViewModel(settingsService, tradeRepository, dialogService);
                await mainVm.InitializeAsync();
                await mainVm.TradeLog.InitializeAsync();
                await mainVm.Analytics.InitializeAsync();
                await mainVm.Calendar.InitializeAsync();

                // 4. Apply the restored theme before the window is shown.
                ApplyTheme(mainVm.IsDarkTheme);

                // 5. Re-apply whenever the user toggles the theme.
                mainVm.PropertyChanged += (_, e) =>
                {
                    if (e.PropertyName == nameof(MainWindowViewModel.IsDarkTheme))
                        ApplyTheme(mainVm.IsDarkTheme);
                };

                desktop.MainWindow = new MainWindow { DataContext = mainVm };
            }
            catch (Exception ex)
            {
                var errorWin = new ErrorDialog(
                    "Startup Error",
                    $"The application failed to start.\n\n{ex.Message}\n\nThe application will now close.");
                desktop.MainWindow = errorWin;
                errorWin.Closed += (_, _) => desktop.Shutdown();
            }
        }

        base.OnFrameworkInitializationCompleted();
    }

    /// <summary>
    /// Applies the given theme variant to the running application.
    /// </summary>
    /// <param name="isDark">True for dark theme; false for light theme.</param>
    private static void ApplyTheme(bool isDark) =>
        Current!.RequestedThemeVariant = isDark ? ThemeVariant.Dark : ThemeVariant.Light;
}
