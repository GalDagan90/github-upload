using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using TaskManagerApp.Models;
using MyTask = TaskManagerApp.Models.Task;

namespace TaskManagerApp.Tests
{
    [TestClass]
    public class TaskTests
    {
        [TestMethod]
        public void Task_DefaultConstructor_SetsDefaultsCorrectly()
        {
            var task = new MyTask();

            Assert.IsNotNull(task.Id); // ID should be generated
            Assert.AreNotEqual(Guid.Empty, task.Id); // ID should not be empty Guid

            Assert.AreEqual(MyTaskStatus.NotStarted, task.Status);
            Assert.AreEqual(MyTaskPriority.Low, task.Priority);
        }

        [TestMethod]
        public void Task_ParameterizedConstructor_SetsProvidedValuesCorrectly()
        {
            string title = "Test Task";
            string description = "This is a test description.";
            DateTime dueDate = new DateTime(2025, 12, 31);
            MyTaskPriority priority = MyTaskPriority.High;
            MyTaskStatus status = MyTaskStatus.InProgress;

            var task = new MyTask(title, description, dueDate, status, priority);

            Assert.AreEqual(title, task.Title);
            Assert.AreEqual(description, task.Description);
            Assert.AreEqual(dueDate, task.DueDate);
            Assert.AreEqual(priority, task.Priority);
            Assert.AreEqual(status, task.Status);
            Assert.IsNotNull(task.Id);
            Assert.AreNotEqual(Guid.Empty, task.Id);
        }

        [TestMethod]
        public void Task_ParameterizedConstructor_UsesDefaultParamsWhenNotProvided()
        {
            string title = "Task with some defaults";
            string description = "Desc";
            DateTime dueDate = DateTime.Today.AddDays(5);

            var task = new MyTask(title, description, dueDate);

            Assert.AreEqual(title, task.Title);
            Assert.AreEqual(description, task.Description);
            Assert.AreEqual(dueDate, task.DueDate);
            Assert.AreEqual(MyTaskPriority.Low, task.Priority);
            Assert.AreEqual(MyTaskStatus.NotStarted, task.Status);
        }

        [TestMethod]
        public void Task_ToString_ReturnsExpectedFormat()
        {
            var task = new MyTask("Test Title", "Desc", new DateTime(2025, 7, 26), MyTaskStatus.InProgress, MyTaskPriority.Medium);

            string taskString = task.ToString();

            string expectedDate = new DateTime(2025, 7, 26).ToShortDateString();
            string expectedString = $"Test Title - InProgress - Medium - Due: {expectedDate}";
            Assert.AreEqual(expectedString, taskString);
        }
    }
}