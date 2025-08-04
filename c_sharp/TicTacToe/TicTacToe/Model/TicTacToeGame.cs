using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TicTacToe.Model
{
    public enum PlayerEnum
    {
        None,
        PlayerX,
        PlayerO
    }

    public enum GameStatus
    {
        InProgress,
        Draw,
        Win
    }


    public class GameBoard
    {
        private PlayerEnum[,] _board;
        
        public GameBoard()
        {
            _board = new PlayerEnum[3, 3];
            ResetBoard();
        }
        public PlayerEnum GetCellValue(int row, int col)
        {
            if (row < 0 || row >= 3 || col < 0 || col >= 3)
                throw new ArgumentOutOfRangeException("Invalid cell coordinates.");

            return _board[row, col];
        }

        public bool TryMakeMove(int row, int col, PlayerEnum player)
        {
            if (IsMoveValid(row, col, player) == false)
            {
                return false;
            }

            _board[row, col] = player;
            return true;
        }

        private bool IsMoveValid(int row, int col, PlayerEnum player)
        {
            if (row < 0 || row >= 3 || col < 0 || col >= 3)
            {
                return false;
            }

            return _board[row, col] == PlayerEnum.None;
        }

        public void ResetBoard()
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    _board[row, col] = PlayerEnum.None;
                }
            }
        }
    }

    public class TicTacToeGame
    {
        private PlayerEnum _currentPlayer;
        private bool _isGameOver;
        private GameStatus _gameStatus;
        private int _movesCount;
        private GameBoard _gameBoard;

        public TicTacToeGame()
        {
            _gameBoard = new GameBoard();
            ResetGame();
        }

        public PlayerEnum CurrentPlayer
        {
            get { return _currentPlayer; }
            private set { _currentPlayer = value; }
        }

        public bool IsGameOver
        {
            get { return _isGameOver; }
            private set { _isGameOver = value; }
        }

        public GameStatus GameStatus
        {             
            get { return _gameStatus; }
            private set { _gameStatus = value; }
        }

        public PlayerEnum GetCellValue(int row, int col)
        {
            return _gameBoard.GetCellValue(row, col); // Example: return value of the first cell
        }

        public bool MakeMove(int row, int col)
        {
            if (IsGameOver)
            {
                return false;
            }

            if (!_gameBoard.TryMakeMove(row, col, CurrentPlayer))
            {
                return false;
            }

            _movesCount++;

            return true;
        }

        public GameStatus CheckGameStatus()
        {
            if (CheckWin())
            {
                IsGameOver = true;
                GameStatus = GameStatus.Win;
            }
            else if (CheckDraw())
            {
                IsGameOver = true;
                GameStatus = GameStatus.Draw;
            }
            else
            {
                SwitchPlayer();
            }

            return GameStatus;
        }

        private void ResetGame()
        {
            _gameBoard.ResetBoard();
            CurrentPlayer = PlayerEnum.PlayerX;
            IsGameOver = false;
            GameStatus = GameStatus.InProgress;
            _movesCount = 0;
        }

        private bool CheckWin()
        {
            return CheckWinInRows() || CheckWinInCols() || CheckWinInDiagonal();
        }

        private void SwitchPlayer()
        {
            CurrentPlayer = CurrentPlayer == PlayerEnum.PlayerX ? PlayerEnum.PlayerO : PlayerEnum.PlayerX;
        }

        private bool CheckDraw()
        {
            return _movesCount >= 9;
        }

        private bool CheckWinInRows()
        {
            bool win = false;
            for (int row = 0; row < 3; row++)
            {
                if (_gameBoard.GetCellValue(row, 0) == CurrentPlayer &&
                    _gameBoard.GetCellValue(row, 1) == CurrentPlayer &&
                    _gameBoard.GetCellValue(row, 2) == CurrentPlayer)
                {
                    win = true;
                    break;
                }
            }
            return win;
        }

        private bool CheckWinInCols()
        {
            bool win = false;
            for (int col = 0; col < 3; col++)
            {
                if (_gameBoard.GetCellValue(0, col) == CurrentPlayer &&
                    _gameBoard.GetCellValue(1, col) == CurrentPlayer &&
                    _gameBoard.GetCellValue(2, col) == CurrentPlayer)
                {
                    win = true;
                    break;
                }
            }
            return win;
        }

        private bool CheckWinInDiagonal()
        {
            if (_gameBoard.GetCellValue(0, 0) == CurrentPlayer &&
                    _gameBoard.GetCellValue(1, 1) == CurrentPlayer &&
                    _gameBoard.GetCellValue(2, 2) == CurrentPlayer)
            {
                return true;
            }

            if (_gameBoard.GetCellValue(0, 2) == CurrentPlayer &&
                    _gameBoard.GetCellValue(1, 1) == CurrentPlayer &&
                    _gameBoard.GetCellValue(2, 0) == CurrentPlayer)
            {
                return true;
            }

            return false;
        }
    }
}
