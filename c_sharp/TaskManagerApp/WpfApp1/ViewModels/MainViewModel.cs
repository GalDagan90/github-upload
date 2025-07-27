using System.Collections.ObjectModel; //ObservableCollection
using TaskManagerApp.Models;        
using TaskManagerApp.Utility;
using TaskManagerApp.Commands;
using System.Linq;
using System;

namespace TaskManagerApp.ViewModels
{
    public class MainViewModel : ObservableObject
    {
        private ObservableCollection<TaskManagerApp.Models.Task> _tasks;
        private TaskManagerApp.Models.Task _selectedTask;
        
        public RelayCommands DeleteTaskCommand { get; private set; }
        public ObservableCollection<TaskManagerApp.Models.Task> Tasks
        {
            get => _tasks;
            set => SetProperty(ref _tasks, value);
        }

        public TaskManagerApp.Models.Task SelectedTask
        {
            get => _selectedTask;
            set
            {
                SetProperty(ref _selectedTask, value);
                DeleteTaskCommand?.OnCanExecuteChanged();
            }
        }

        public MainViewModel()
        {
            InitializeCommands();

            Tasks = new ObservableCollection<TaskManagerApp.Models.Task>
            {
                new TaskManagerApp.Models.Task("Task 1", "Description for Task 1", DateTime.Now.AddDays(1)),
                new TaskManagerApp.Models.Task("Task 2", "Description for Task 2", DateTime.Now.AddDays(2)),
                new TaskManagerApp.Models.Task("Task 3", "Description for Task 3", DateTime.Now.AddDays(3))
            };
        }

        private void InitializeCommands()
        {
            DeleteTaskCommand = new RelayCommands(ExecuteDeleteSelectedTask, CanExecuteDeleteSelectedTask);
        }

        private void ExecuteDeleteSelectedTask(object parameter)
        {
            if (SelectedTask != null && Tasks.Contains(SelectedTask))
            {
                Tasks.Remove(SelectedTask);
                SelectedTask = null;
            }
        }

        private bool CanExecuteDeleteSelectedTask(object parameter)
        {
            return SelectedTask != null && Tasks.Contains(SelectedTask);
        }
    }
}
