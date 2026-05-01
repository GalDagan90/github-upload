using Avalonia;
using Avalonia.Controls;
using Avalonia.Layout;
using Avalonia.Media;

namespace TradingJournal.Views;

/// <summary>
/// A minimal modal confirmation dialog shown before a trade is permanently deleted.
/// Show via <see cref="Window.ShowDialog{TResult}"/>; returns <see langword="true"/> when the user
/// confirms deletion or <see langword="false"/> when they cancel.
/// </summary>
internal sealed class ConfirmDeleteDialog : Window
{
    /// <summary>Creates the dialog with the supplied confirmation message.</summary>
    /// <param name="message">Full text shown in the dialog body.</param>
    public ConfirmDeleteDialog(string message)
    {
        Title = "Confirm Delete";
        Width = 380;
        Height = 148;
        CanResize = false;
        WindowStartupLocation = WindowStartupLocation.CenterOwner;

        var messageBlock = new TextBlock
        {
            Text = message,
            TextWrapping = TextWrapping.Wrap,
        };

        var deleteBtn = new Button { Content = "Delete" };
        var cancelBtn = new Button { Content = "Cancel" };
        deleteBtn.Click += (_, _) => Close(true);
        cancelBtn.Click += (_, _) => Close(false);

        var buttons = new StackPanel
        {
            Orientation = Orientation.Horizontal,
            HorizontalAlignment = HorizontalAlignment.Right,
            Spacing = 8,
        };
        buttons.Children.Add(deleteBtn);
        buttons.Children.Add(cancelBtn);

        var root = new StackPanel { Margin = new Thickness(20), Spacing = 16 };
        root.Children.Add(messageBlock);
        root.Children.Add(buttons);
        Content = root;
    }
}
