using TicTacToe.Utility;
using TicTacToe.Model;

namespace TicTacToe.ViewModel
{
    public class CellViewModel : ObservableObject
    {
        private PlayerTicker _cellValue;
        private bool _isEnabled;
        public CellViewModel(int row, int col)
        {
            Row = row;
            Col = col;
            CellValue = PlayerTicker.None;
            IsEnabled = true;
        }
        public int Row { get; }
        public int Col { get; }
        public PlayerTicker CellValue
        {
            get => _cellValue;
            set
            {
                if (_cellValue != value)
                {
                    _cellValue = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(PlayerSymbol));
                }
            }
        }
        public bool IsEnabled
        {
            get => _isEnabled;
            set 
            { 
                if (_isEnabled != value)
                {
                    _isEnabled = value;
                    OnPropertyChanged();
                }
            }
        }
        
        public string PlayerSymbol
        {
            get 
            {
                switch (CellValue)
                {
                    case PlayerTicker.PlayerX:
                        return "X";
                    case PlayerTicker.PlayerO:
                        return "O";
                    default:
                        return string.Empty;
                }
            }
        }
    }
}
