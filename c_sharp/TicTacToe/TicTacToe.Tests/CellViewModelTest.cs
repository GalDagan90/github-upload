using Microsoft.VisualStudio.TestTools.UnitTesting;
using TicTacToe.ViewModel;
using TicTacToe.Model;

namespace TicTacToe.ViewModel.Tests
{
    [TestClass]
    public class CellViewModelTest
    {
        [TestMethod]
        public void CellViewModel_ShouldInitializeWithCorrectValues()
        {
            int row = 1;
            int col = 2;
            var cellViewModel = new CellViewModel(row, col);

            Assert.AreEqual(row, cellViewModel.Row, "Row should be initialized correctly.");
            Assert.AreEqual(col, cellViewModel.Col, "Column should be initialized correctly.");
            Assert.AreEqual(PlayerTicker.None, cellViewModel.CellValue, "CellValue should be initialized to None.");
            Assert.IsTrue(cellViewModel.IsEnabled, "Cell should be enabled by default.");
        }

        [TestMethod]
        public void CellValue_ShouldUpdateAndNotify()
        {
            var cellViewModel = new CellViewModel(0, 0);
            var notifiedProperties = new List<string>();

            cellViewModel.PropertyChanged += (sender, args) =>
            {
                notifiedProperties.Add(args.PropertyName);
            };
            cellViewModel.CellValue = PlayerTicker.PlayerX;
            
            Assert.AreEqual(PlayerTicker.PlayerX, cellViewModel.CellValue, "CellValue should be updated correctly.");
            Assert.IsTrue(notifiedProperties.Contains(nameof(cellViewModel.CellValue)), "PropertyChanged should notify CellValue change.");
            Assert.IsTrue(notifiedProperties.Contains(nameof(cellViewModel.PlayerSymbol)), "PropertyChanged should notify CellValue change.");
            Assert.AreEqual("X", cellViewModel.PlayerSymbol, "PlayerSymbol should reflect the updated CellValue.");
        }

        [TestMethod]
        public void IsEnabled_ShouldUpdateAndNotify()
        {
            var cellViewModel = new CellViewModel(0, 0);
            bool newIsEnabledValue = false;
            string receivedPropertyName = null;

            cellViewModel.PropertyChanged += (sender, args) =>
            {
                receivedPropertyName = args.PropertyName;
            };

            cellViewModel.IsEnabled = newIsEnabledValue;
            Assert.AreEqual(newIsEnabledValue, cellViewModel.IsEnabled, "IsEnabled should be updated correctly.");
            Assert.AreEqual("IsEnabled", receivedPropertyName, "PropertyChanged should notify IsEnabled change.");
        }

        [TestMethod]
        public void PlayerSymbol_ShouldReturnCorrectSymbolBasedOnCellValue()
        {
            var cellViewModel = new CellViewModel(0, 0);

            cellViewModel.CellValue = PlayerTicker.PlayerX;
            Assert.AreEqual("X", cellViewModel.PlayerSymbol, "PlayerSymbol should return 'X' for PlayerX.");
            
            cellViewModel.CellValue = PlayerTicker.PlayerO;
            Assert.AreEqual("O", cellViewModel.PlayerSymbol, "PlayerSymbol should return 'O' for PlayerO.");
            
            cellViewModel.CellValue = PlayerTicker.None;
            Assert.AreEqual(string.Empty, cellViewModel.PlayerSymbol, "PlayerSymbol should return empty string for None.");
        }
    }
}
