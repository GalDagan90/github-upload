using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq; 
using TaskManagerApp.ViewModels; 
using TaskManagerApp.Models;     
using System.ComponentModel;
using System.Collections.ObjectModel; //ObservableCollection

namespace TaskManagerApp.Tests
{
    [TestClass]
    public class MainViewModelTest
    {
        [TestMethod]
        public void TestTasksInitialization()
        {
            var viewModel = new MainViewModel();
            var tasks = viewModel.Tasks;
            
            Assert.IsNotNull(tasks, "Tasks collection should not be null.");
            Assert.IsTrue(tasks.Count > 0, "Tasks collection should contain some initial tasks.");

            var task = tasks.First();
            Assert.AreEqual("Task 1", task.Title, "First task title should be 'Task 1'.");
            Assert.AreEqual("Description for Task 1", task.Description, "First task description should match the expected value.");
        }

        [TestMethod]
        public void MainViewModel_ShouldRaisePropertyChanged_WhenTasksChanged()
        {
            var viewModel = new MainViewModel();
            bool propertyChangedRaised = false;
            string receivedPropertyName = null;

            viewModel.PropertyChanged += (sender, e) =>
            {
                propertyChangedRaised = true;
                receivedPropertyName = e.PropertyName;
            };

            viewModel.Tasks = new ObservableCollection<TaskManagerApp.Models.Task>
            {
                new TaskManagerApp.Models.Task("New Task", "New Description", DateTime.Now.AddDays(1))
            };

            Assert.IsTrue(propertyChangedRaised, "PropertyChanged event should be raised when Tasks collection is changed.");
            Assert.AreEqual(nameof(viewModel.Tasks), receivedPropertyName, "PropertyChanged event should indicate that 'Tasks' property has changed.");
            Assert.IsTrue(viewModel.Tasks.Count > 0, "Tasks collection should not be empty after change.");
            Assert.AreEqual("New Task", viewModel.Tasks.First().Title, "The first task in the collection should be 'New Task'.");
            Assert.AreEqual("New Description", viewModel.Tasks.First().Description, "The first task description should match the expected value.");
        }

        [TestMethod]
        public void MainViewModel_ShouldRaisePropertyChanged_WhenSelectedTaskChanged()
        {
            var viewModel = new MainViewModel();
            bool propertyChangedRaised = false;
            string receivedPropertyName = null;

            viewModel.PropertyChanged += (sender, e) =>
            {
                propertyChangedRaised = true;
                receivedPropertyName = e.PropertyName;
            };

            var selectedTask = viewModel.Tasks.First();
            Assert.IsNotNull(selectedTask, "There should be at least one task to select.");
            viewModel.SelectedTask = selectedTask;

            Assert.IsTrue(propertyChangedRaised, "PropertyChanged event should be raised when SelectedTask is changed.");
            Assert.AreEqual(nameof(viewModel.SelectedTask), receivedPropertyName, "PropertyChanged event should indicate that 'SelectedTask' property has changed.");
            Assert.AreEqual(selectedTask, viewModel.SelectedTask, "SelectedTask should match the task that was set.");
        }

        [TestMethod]
        public void MainViewModel_ShouldDeleteSelectedTask_WhenDeleteCommandExecuted()
        {
            var viewModel = new MainViewModel();
            var initialTaskCount = viewModel.Tasks.Count;
            
            viewModel.SelectedTask = viewModel.Tasks.First();
            Assert.IsNotNull(viewModel.SelectedTask, "There should be a selected task to delete.");
            
            viewModel.DeleteTaskCommand.Execute(null);
            
            Assert.AreEqual(initialTaskCount - 1, viewModel.Tasks.Count, "Tasks collection should have one less task after deletion.");
            Assert.IsNull(viewModel.SelectedTask, "SelectedTask should be null after deletion.");
        }

        [TestMethod]
        public void MainViewModel_ShouldNotDeleteTask_WhenTaskIsNotInList()
        {
            var viewModel = new MainViewModel();
            var initialTaskCount = viewModel.Tasks.Count;
            viewModel.SelectedTask = null;

            viewModel.DeleteTaskCommand.Execute(null);
            
            Assert.AreEqual(initialTaskCount, viewModel.Tasks.Count, "Tasks collection should remain unchanged when no task is selected for deletion.");
            Assert.IsNull(viewModel.SelectedTask, "SelectedTask should remain null when no task is selected.");
        }
    }
}
