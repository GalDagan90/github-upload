using System;
using System.Windows.Input;
using System.Collections.ObjectModel;
using TicTacToe.Model;
using TicTacToe.Utility;
using TicTacToe.Commands;

namespace TicTacToe.ViewModel
{
    public class GameViewModel : ObservableObject
    {
        private TicTacToeGame _game;
        private bool _isGameOver;
        private string _gameStatusMessage;
        private string _currentPlayerMessage;
        private GameStatus _currentGameStatus;

        public ObservableCollection<CellViewModel> BoardCells { get; }

        public ICommand PlayMoveCommand { get; private set; }

        public GameViewModel()
        {
            _game = new TicTacToeGame();
            _currentGameStatus = _game.GameStatus;
            BoardCells = new ObservableCollection<CellViewModel>();
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    BoardCells.Add(new CellViewModel(row, col));
                }
            }

            PlayMoveCommand = new RelayCommand(ExecutePlayMove, CanExecutePlayMove);
            
            GameStatusMessage = "Game in progress";
            CurrentPlayerMessage = $"Current Player: {_game.CurrentPlayer}";
            IsGameOver = false;

            UpdateAllCellsUI();
            UpdateGameUI();
        }


        public string GameStatusMessage
        {
            get => _gameStatusMessage;
            private set
            {
                SetProperty(ref _gameStatusMessage, value);
            }
        }

        public string CurrentPlayerMessage
        {
            get => _currentPlayerMessage;
            private set
            {
                SetProperty(ref _currentPlayerMessage, value);
            }
        }

        public bool IsGameOver
        {
            get => _isGameOver;
            private set
            {
                SetProperty(ref _isGameOver, value);
            }
        }

        public void ExecutePlayMove(object parameter)
        {
            if (parameter is CellViewModel cell)
            {
                if (_game.MakeMove(cell.Row, cell.Col))
                {
                    //update the cell UI
                    UpdateCellUI(cell.Row, cell.Col);
                    cell.IsEnabled = false;

                    //Check if the game is not over => switch player
                    if (_game.CheckGameStatus() == GameStatus.InProgress)
                    {
                        _game.SwitchPlayer();
                    }

                    //update overall game UI display
                    UpdateGameUI();
                }
            }
        }

        public bool CanExecutePlayMove(object parameter)
        {
            if (parameter is CellViewModel cell)
            {
                return !IsGameOver && cell.IsEnabled;
            }

            return false;
        }

        private void UpdateAllCellsUI()
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    UpdateCellUI(row, col);
                }
            }
        }

        private void UpdateCellUI(int row, int col)
        {
            var cell = BoardCells[row * 3 + col];
            cell.CellValue = _game.GetCellValue(row, col);
        }

        private void UpdateGameUI()
        {
            IsGameOver = _game.IsGameOver;
            if (IsGameOver)
            {
                CurrentPlayerMessage = string.Empty;
                switch (_game.CheckGameStatus())
                {
                    case GameStatus.Win:
                        GameStatusMessage = $"Player {_game.PlayerTickerSymbol} wins!";
                        break;
                    case GameStatus.Draw:
                        GameStatusMessage = "It's a draw!";
                        break;
                    default:
                        break;
                }
            }
            else
            {
                CurrentPlayerMessage = $"Current Player: {_game.PlayerTickerSymbol}";
            }
        }
    }
}
