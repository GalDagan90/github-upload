using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using TaskManagerApp.Commands;

namespace TaskManagerApp.Tests
{
    [TestClass]
    public class RelayCommandsTests
    {
        [TestMethod]
        public void RelayCommands_Execute_CallsExecuteAction()
        {
            // Arrange
            bool executed = false;
            Action<object> executeAction = param => executed = true;
            var command = new RelayCommands(executeAction);

            command.Execute(null);

            Assert.IsTrue(executed, "Execute action was not called.");
        }

        [TestMethod]
        public void RelayCommands_Execute_WithParameter()
        {
            int expectedValue = 42;
            int recievedValue = 0;
            Action<object> executeAction = param => recievedValue = (int)param;
            var command = new RelayCommands(executeAction);

            command.Execute(expectedValue);

            Assert.AreEqual(expectedValue, recievedValue, "Parameters should be the same after execute");
        }

        [TestMethod]
        public void RelayCommands_CanExecute_ReturnsTrue_WhenCanExecuteIsNull()
        {
            Action<object> executeAction = param => { };
            var command = new RelayCommands(executeAction);

            bool canExecute = command.CanExecute(null);

            Assert.IsTrue(canExecute, "CanExecute should return true when canExecute is null.");
        }

        [TestMethod]
        public void RelayCommands_CanExecute_ReturnsTrue_WhenCanExecuteConditionIsMet()
        {
            Action<object> executeAction = param => { };
            Func<object, bool> canExecuteCondition = param => true;
            var command = new RelayCommands(executeAction, canExecuteCondition);

            bool canExecute = command.CanExecute(null);

            Assert.IsTrue(canExecute, "CanExecute should return true when condition is met.");
        }

        [TestMethod]
        public void RelayCommande_CanExecute_PassesParameterToPredicate()
        {
            int expectedValue = 42;
            int recievedValue = 0;
            Action<object> executeAction = param => recievedValue = (int)param;
            Func<object, bool> canExecuteCondition = (param) => { recievedValue = expectedValue ; return true; };
            var command = new RelayCommands(executeAction, canExecuteCondition);

            bool canExecute = command.CanExecute(expectedValue);

            Assert.AreEqual(expectedValue, recievedValue, "Parameter should be passed to the execute action.");
        }
    }
}
