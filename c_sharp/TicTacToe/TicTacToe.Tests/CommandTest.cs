using System;
using TicTacToe.Commands;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.CompilerServices;
using System.Windows.Input;

namespace TicTacToe.Commands.Tests
{
    [TestClass]
    public class CommandTest
    {
        private bool _actionExecuted;

        private void ActionToExecute(object parameter)
        {
            _actionExecuted = true;
        }

        private bool CanExecuteAction(object parameter)
        {
            return true;
        }

        [TestMethod]
        public void RelayCommand_ShouldExecuteAction_WhenCanExecuteReturnsTrue()
        {
            _actionExecuted = false;
            var command = new RelayCommand(ActionToExecute, CanExecuteAction);
            command.Execute(null);
            Assert.IsTrue(_actionExecuted, "Action should be executed when CanExecute returns true.");
        }

        [TestMethod]
        public void RelayCommand_ShouldNotExecuteAction_WhenCanExecuteReturnsFalse()
        {
            _actionExecuted = false;
            var command = new RelayCommand(ActionToExecute, _ => false);
            
            Assert.IsFalse(command.CanExecute(null), "Action should not be executed when CanExecute returns false.");
        }
    }
}
