using System;
using System.Windows.Input;

namespace TaskManagerApp.Commands
{
    public class RelayCommands : ICommand
    {
        private readonly Action<object> _executeAction;
        private readonly Func<object, bool> _canExecute;

        private EventHandler _canExecuteChangedInternal;

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; _canExecuteChangedInternal += value; }
            remove { CommandManager.RequerySuggested -= value; _canExecuteChangedInternal -= value; }
        }

        public RelayCommands(Action<object> execute, Func<object, bool> canExecute = null)
        {
            _executeAction = execute ?? throw new ArgumentNullException(nameof(execute));
            _canExecute = canExecute;
        }

        public bool CanExecute(object parameter)
        {
            return _canExecute == null || _canExecute(parameter);
        }

        public void Execute(object parameter)
        {
            _executeAction(parameter);
        }

        public void OnCanExecuteChanged()
        {
            _canExecuteChangedInternal?.Invoke(this, EventArgs.Empty);
        }
    }
}
