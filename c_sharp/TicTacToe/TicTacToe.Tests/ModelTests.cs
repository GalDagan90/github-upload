using Microsoft.VisualStudio.TestTools.UnitTesting; 
using TicTacToe.Model;

namespace TicTacToe.Model.Tests
{
    [TestClass]
    public class TicTacToeGameModelTests
    {
        [TestMethod]
        public void TestGameInitialization()
        {
            var game = new TicTacToeGame();

            Assert.IsNotNull(game);
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);
            Assert.IsFalse(game.IsGameOver);
            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);

            int[] cols = { 0, 1, 2 };
            int[] rows = { 0, 1, 2 };
            foreach (var row in rows)
            {
                foreach (var col in cols)
                {
                    Assert.AreEqual(PlayerTicker.None, game.GetCellValue(row, col));
                }
            }
        }

        [TestMethod]
        public void TestMakeMove()
        {
            var game = new TicTacToeGame();
            
            game.MakeMove(0, 0);
            Assert.AreEqual(PlayerTicker.PlayerX, game.GetCellValue(0,0), "Should have X in cell (0,0)");
        }

        [TestMethod]
        public void TestMakeMoveInvalidCell()
        {
            var game = new TicTacToeGame();
            
            bool result = game.MakeMove(-1, -1);
            Assert.IsFalse(result, "Should not allow move to invalid cell (-1,-1)");
            
            result = game.MakeMove(3, 3);
            Assert.IsFalse(result, "Should not allow move to invalid cell (3,3)");

            result = game.MakeMove(0, 0);
            Assert.IsTrue(result, "Should allow move to valid cell (0,0)");

            result = game.MakeMove(0, 0);
            Assert.IsFalse(result, "Should not allow move to already occupied cell (0,0)");
        }

        [TestMethod]
        public void TestGameEndInWinInRows()
        {
            var game = new TicTacToeGame();

            /*
             x x x
             o o -
             - - -
            */

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(0, 0); // X
            var gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerO, game.CurrentPlayer);
            game.MakeMove(1, 0); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(0, 1); // X
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerO, game.CurrentPlayer);
            game.MakeMove(1, 1); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(0, 2); // X
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.Win, game.GameStatus);
            Assert.IsTrue(game.IsGameOver);
        }

        [TestMethod]
        public void TestGameEndInWinInColumns() 
        { 
            var game = new TicTacToeGame();

            /*
             x o -
             x o -
             x - -
            */

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(0, 0); // X
            var gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerO, game.CurrentPlayer);
            game.MakeMove(0, 1); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(1, 0); // X
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerO, game.CurrentPlayer);
            game.MakeMove(1, 1); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            Assert.AreEqual(PlayerTicker.PlayerX, game.CurrentPlayer);
            game.MakeMove(2, 0); // X
            gameStatus = game.CheckGameStatus();

            Assert.AreEqual(GameStatus.Win, game.GameStatus);
            Assert.IsTrue(game.IsGameOver);
        }

        [TestMethod]
        public void TestGameEndInWinInDiagonal()
        {
            var game = new TicTacToeGame();

            /*
             x o -
             o x -
             - - x
            */

            game.MakeMove(0, 0); // X
            var gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);
            Assert.AreEqual(PlayerTicker.PlayerO, game.CurrentPlayer);

            game.MakeMove(0, 1); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            game.MakeMove(1, 1); // X
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            game.MakeMove(1, 0); // O
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.InProgress, game.GameStatus);

            game.MakeMove(2, 2); // X
            gameStatus = game.CheckGameStatus();
            Assert.AreEqual(GameStatus.Win, game.GameStatus);
            Assert.IsTrue(game.IsGameOver);
        }

        [TestMethod]
        public void GameEndInDraw()
        {
            var game = new TicTacToeGame();
            /*
             o x x
             x x o
             o o x
            */
            game.MakeMove(1, 1); // X
            game.CheckGameStatus();
            game.MakeMove(0, 0); // O
            game.CheckGameStatus();
            game.MakeMove(1, 0); // X
            game.CheckGameStatus();
            game.MakeMove(1, 2); // O
            game.CheckGameStatus();
            game.MakeMove(0, 1); // X
            game.CheckGameStatus();
            game.MakeMove(2, 1); // O
            game.CheckGameStatus();
            game.MakeMove(0, 2); // X
            game.CheckGameStatus();
            game.MakeMove(2, 0); // O
            game.CheckGameStatus();
            game.MakeMove(2, 2); // X
            var status = game.CheckGameStatus();
            
            Assert.AreEqual(GameStatus.Draw, status);
            Assert.IsTrue(game.IsGameOver);
        }
    }
}
