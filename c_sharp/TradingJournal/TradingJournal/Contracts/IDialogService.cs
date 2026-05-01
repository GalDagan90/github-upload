using System.Threading.Tasks;

namespace TradingJournal.Contracts;

/// <summary>
/// Provides platform-level dialog interactions so ViewModels can prompt the user
/// without depending on Avalonia or any concrete UI type.
/// </summary>
public interface IDialogService
{
    /// <summary>
    /// Shows a modal confirmation dialog with the supplied message.
    /// </summary>
    /// <param name="message">Text displayed in the dialog body.</param>
    /// <returns><see langword="true"/> when the user confirms; <see langword="false"/> when they cancel.</returns>
    Task<bool> ConfirmAsync(string message);
}
