using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using TradingJournal.Contracts;
using TradingJournal.Views;

namespace TradingJournal.Services;

/// <summary>
/// Avalonia implementation of <see cref="IDialogService"/>.
/// Obtains the main window from the application lifetime so it can be used as a dialog owner.
/// </summary>
public sealed class AvaloniaDialogService : IDialogService
{
    /// <inheritdoc/>
    public async Task<bool> ConfirmAsync(string message)
    {
        var owner = (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
                    ?.MainWindow;

        if (owner is null) return false;

        return await new ConfirmDeleteDialog(message).ShowDialog<bool>(owner);
    }
}
