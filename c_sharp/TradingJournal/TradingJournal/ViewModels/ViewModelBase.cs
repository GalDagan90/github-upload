using CommunityToolkit.Mvvm.ComponentModel;

namespace TradingJournal.ViewModels;

/// <summary>
/// Base class for all ViewModels in the application.
/// Inherits <see cref="ObservableObject"/> to provide INotifyPropertyChanged support
/// and makes every ViewModel matchable by the ViewLocator.
/// </summary>
public abstract class ViewModelBase : ObservableObject
{
}
