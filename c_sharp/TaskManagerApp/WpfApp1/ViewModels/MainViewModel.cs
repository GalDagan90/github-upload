using System.Collections.ObjectModel; //ObservableCollection
using TaskManagerApp.Models;        
using TaskManagerApp.Utility;
using TaskManagerApp.Commands;
using System.Linq;
using System;
using System.Collections.Generic;

namespace TaskManagerApp.ViewModels
{
    public class MainViewModel : ObservableObject
    {
        private ObservableCollection<TaskManagerApp.Models.Task> _tasks;
        private TaskManagerApp.Models.Task _selectedTask;
        private string _newTaskTitle;
        private string _newTaskDescription;
        private DateTime _newTaskDueDate;
        private MyTaskPriority _newTaskPriority;
        private MyTaskStatus _newTaskStatus;
        private bool _isEditing;
        public RelayCommands DeleteTaskCommand { get; private set; }
        public RelayCommands SaveTaskCommand { get; private set; }

        public IEnumerable<TaskManagerApp.Models.MyTaskStatus> MyTaskStatuses
        {
            get { return Enum.GetValues(typeof(TaskManagerApp.Models.MyTaskStatus)).Cast<TaskManagerApp.Models.MyTaskStatus>(); }
        }

        public IEnumerable<TaskManagerApp.Models.MyTaskPriority> MyTaskPriorities
        {
            get { return Enum.GetValues(typeof(TaskManagerApp.Models.MyTaskPriority)).Cast<TaskManagerApp.Models.MyTaskPriority>(); }
        }

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

                if (_selectedTask != null)
                {
                    _isEditing = true;
                    NewTaskTitle = _selectedTask.Title;
                    NewTaskDescription = _selectedTask.Description;
                    NewTaskDueDate = _selectedTask.DueDate;
                    NewTaskPriority = _selectedTask.Priority;
                    NewTaskStatus = _selectedTask.Status;
                }
                else
                {
                    _isEditing = false;
                    NewTaskTitle = string.Empty;
                    NewTaskDescription = string.Empty;
                    NewTaskDueDate = DateTime.Now.AddDays(1);
                    NewTaskPriority = MyTaskPriority.Low;
                    NewTaskStatus = MyTaskStatus.NotStarted;
                }
                
                DeleteTaskCommand?.OnCanExecuteChanged();
                SaveTaskCommand?.OnCanExecuteChanged();
            }
        }

        public string NewTaskTitle
        {
            get => _newTaskTitle;
            set
            {
                SetProperty(ref _newTaskTitle, value);
            }
        }

        public string NewTaskDescription
        {
            get => _newTaskDescription;
            set => SetProperty(ref _newTaskDescription, value);
        }

        public DateTime NewTaskDueDate
        {
            get => _newTaskDueDate;
            set => SetProperty(ref _newTaskDueDate, value);
        }

        public MyTaskPriority NewTaskPriority
        {
            get => _newTaskPriority;
            set => SetProperty(ref _newTaskPriority, value);
        }

        public MyTaskStatus NewTaskStatus
        {
            get => _newTaskStatus;
            set => SetProperty(ref _newTaskStatus, value);
        }

        public MainViewModel()
        {
            Tasks = new ObservableCollection<TaskManagerApp.Models.Task>
            {
                new TaskManagerApp.Models.Task("Task 1", "Description for Task 1", DateTime.Now.AddDays(1)),
                new TaskManagerApp.Models.Task("Task 2", "Description for Task 2", DateTime.Now.AddDays(2)),
                new TaskManagerApp.Models.Task("Task 3", "Description for Task 3", DateTime.Now.AddDays(3))
            };

            NewTaskTitle = string.Empty;
            NewTaskDescription = string.Empty;
            NewTaskDueDate = DateTime.Now.AddDays(1);
            NewTaskPriority = MyTaskPriority.Medium;
            NewTaskStatus = MyTaskStatus.NotStarted;

            InitializeCommands();
        }

        private void InitializeCommands()
        {
            DeleteTaskCommand = new RelayCommands(ExecuteDeleteSelectedTask, CanExecuteDeleteSelectedTask);
            SaveTaskCommand = new RelayCommands(ExecuteSaveNewTask, CanExecuteSaveNewTask);
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

        private void ExecuteSaveNewTask(object parameter)
        {
            if (_isEditing && SelectedTask != null)
            {
                TaskManagerApp.Models.Task originalTask = SelectedTask; // Keep reference to original for removal

                var updatedTask = new TaskManagerApp.Models.Task(
                    NewTaskTitle,
                    NewTaskDescription,
                    NewTaskDueDate,
                    NewTaskStatus,
                    NewTaskPriority
                );

                Tasks.Remove(originalTask);
                Tasks.Add(updatedTask);
            }
            else
            {
                // We are adding a new task
                var newTask = new TaskManagerApp.Models.Task(
                    NewTaskTitle,
                    NewTaskDescription,
                    NewTaskDueDate,
                    NewTaskStatus,
                    NewTaskPriority
                );
                Tasks.Add(newTask);
            }

            SelectedTask = null;
        }

        private bool CanExecuteSaveNewTask(object parameter)
        {
            return !string.IsNullOrWhiteSpace(NewTaskTitle) &&
                   !string.IsNullOrWhiteSpace(NewTaskDescription);
        }
    }
}
