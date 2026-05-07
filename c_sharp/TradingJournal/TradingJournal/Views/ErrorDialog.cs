using Avalonia;
using Avalonia.Controls;
using Avalonia.Layout;
using Avalonia.Media;

namespace TradingJournal.Views;

/// <summary>
/// A modal error dialog that displays a title and message with a single OK button.
/// Show via <see cref="Window.ShowDialog(Window)"/> for ViewModel errors,
/// or <see cref="Window.Show"/> as a standalone window for startup failures.
/// </summary>
internal sealed class ErrorDialog : Window
{
    /// <summary>Creates the dialog with the supplied title and message.</summary>
    /// <param name="title">Text shown in the window title bar.</param>
    /// <param name="message">Detail message shown in the dialog body.</param>
    public ErrorDialog(string title, string message)
    {
        Title = title;
        Width = 420;
        Height = 220;
        CanResize = false;
        WindowStartupLocation = WindowStartupLocation.CenterOwner;

        var messageBlock = new TextBlock
        {
            Text = message,
            TextWrapping = TextWrapping.Wrap,
        };

        var okBtn = new Button
        {
            Content = "OK",
            HorizontalAlignment = HorizontalAlignment.Right,
        };
        okBtn.Click += (_, _) => Close();

        var root = new StackPanel { Margin = new Thickness(20), Spacing = 16 };
        root.Children.Add(messageBlock);
        root.Children.Add(okBtn);
        Content = root;
    }
}
