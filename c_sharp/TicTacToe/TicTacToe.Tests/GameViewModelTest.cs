using System;
using TicTacToe.Model;
using TicTacToe.Utility;
using TicTacToe.Commands;
using TicTacToe.ViewModel;
using Microsoft.VisualStudio.TestTools.UnitTesting;


namespace TicTacToe.ViewModel.Tests
{
    [TestClass]
    public class GameViewModelTest
    {
        [TestMethod]
        public void TestGameInitialization()
        {
            var gameViewModel = new GameViewModel();

            var boardCells = gameViewModel.BoardCells;

            Assert.IsNotNull(boardCells);
            Assert.AreEqual(9, boardCells.Count, "Board should have 9 cells.");
            Assert.AreEqual("Game in progress", gameViewModel.GameStatusMessage);
            Assert.AreEqual("Current Player: X", gameViewModel.CurrentPlayerMessage);
            Assert.IsFalse(gameViewModel.IsGameOver);
        }

        [TestMethod]
        public void GameViewModel_TestGameUpdateToWin()
        {
            var gameViewModel = new GameViewModel();

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 0));
            Assert.AreEqual("Current Player: O", gameViewModel.CurrentPlayerMessage, "After first move, current player should be PlayerO.");
            Assert.AreEqual("Game in progress", gameViewModel.GameStatusMessage, "Game status should still be in progress after first move.");

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 1));
            Assert.AreEqual("Current Player: X", gameViewModel.CurrentPlayerMessage, "After second move, current player should be PlayerX.");
            Assert.AreEqual("Game in progress", gameViewModel.GameStatusMessage, "Game status should still be in progress after second move.");

            gameViewModel.ExecutePlayMove(new CellViewModel(1, 0));
            Assert.IsFalse(gameViewModel.IsGameOver, "Game should not be over after three moves.");

            gameViewModel.ExecutePlayMove(new CellViewModel(1, 1));
            Assert.IsFalse(gameViewModel.IsGameOver, "Game should not be over after four moves.");

            gameViewModel.ExecutePlayMove(new CellViewModel(2, 0));
            Assert.IsTrue(gameViewModel.IsGameOver, "Game should be over after PlayerX wins.");
            Assert.AreEqual("Player X wins!", gameViewModel.GameStatusMessage, "Game status should indicate PlayerX has won.");
            Assert.AreEqual(string.Empty, gameViewModel.CurrentPlayerMessage, "Current player should still be PlayerX after the game ends.");
        }

        [TestMethod]
        public void GameViewModel_TestGameUpdateToDraw()
        {
            var gameViewModel = new GameViewModel();
            /*
             x o x
             x o o
             o x x
             */

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 0)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(0, 1)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(0, 2)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 1)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 0)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 0)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 1)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 2)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 2)); // PlayerX

            Assert.IsTrue(gameViewModel.IsGameOver, "Game should be over after all cells are filled.");
            Assert.AreEqual("It's a draw!", gameViewModel.GameStatusMessage, "Game status should indicate a draw.");
            Assert.AreEqual(string.Empty, gameViewModel.CurrentPlayerMessage, "Current player should be empty after the game ends.");
        }

        [TestMethod]
        public void GameViewModel_TestInvalidMove()
        {
            var gameViewModel = new GameViewModel();

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 0)); // PlayerX
            Assert.AreEqual("Current Player: O", gameViewModel.CurrentPlayerMessage, "Current player should still be PlayerO after invalid move.");

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 0)); // Invalid move on occupied cell
            Assert.AreEqual("Current Player: O", gameViewModel.CurrentPlayerMessage, "Current player should remain PlayerO after invalid move.");

            gameViewModel.ExecutePlayMove(new CellViewModel(1, 1)); // PlayerO
            Assert.AreEqual("Current Player: X", gameViewModel.CurrentPlayerMessage, "Current player should now be PlayerX after valid move.");
        }

        [TestMethod]
        public void GameViewModel_TestResetGame()
        {
            var gameViewModel = new GameViewModel();

            gameViewModel.ExecutePlayMove(new CellViewModel(0, 0)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(0, 1)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(0, 2)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 1)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 0)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 0)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 1)); // PlayerX
            gameViewModel.ExecutePlayMove(new CellViewModel(1, 2)); // PlayerO
            gameViewModel.ExecutePlayMove(new CellViewModel(2, 2)); // PlayerX

            Assert.IsTrue(gameViewModel.IsGameOver, "Game should be over after all cells are filled.");
            gameViewModel.ExecuteResetGame(null);
            Assert.IsFalse(gameViewModel.IsGameOver, "Game should not be over after reset.");
            Assert.AreEqual("Game in progress", gameViewModel.GameStatusMessage, "Game status should be reset to in progress.");
        }
    }
}
