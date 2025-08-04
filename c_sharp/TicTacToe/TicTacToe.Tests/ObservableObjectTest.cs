using Microsoft.VisualStudio.TestTools.UnitTesting;
using TicTacToe.Utility;

namespace TicTacToe.Tests
{
    public class ObservanleObjectHelper: ObservableObject
    {
        private string _testProperty = null;
        
        public string TestProperty
        {
            get => _testProperty;
            set => SetProperty(ref _testProperty, value);
        }

        public ObservanleObjectHelper(string testProperty)
        {
            TestProperty = testProperty;
        }
    }

    [TestClass]
    public class ObservableObjectTest
    {
        [TestMethod]
        public void SetProperty_ShouldUpdatePropertyAndNotify()
        {
            var obj = new ObservanleObjectHelper("InitialValue");
            string receivedPropertyName = null;

            obj.PropertyChanged += (sender, args) =>
            {
                receivedPropertyName = args.PropertyName;
            };
            obj.TestProperty = "UpdatedValue";

            Assert.AreEqual("TestProperty", receivedPropertyName, "TestProperty should be updated to the recevied property name");
            Assert.AreEqual("UpdatedValue", obj.TestProperty, "String value shpuld be changes");
        }

        [TestMethod]
        public void SetProperty_ShouldNotNotifyIfValueUnchanged()
        {
            var obj = new ObservanleObjectHelper("InitialValue");
            string receivedPropertyName = null;
            
            obj.PropertyChanged += (sender, args) =>
            {
                receivedPropertyName = args.PropertyName;
            };
            obj.TestProperty = "InitialValue"; // Setting to the same value
            
            Assert.IsNull(receivedPropertyName, "No property change should be notified when value is unchanged");
            Assert.AreEqual("InitialValue", obj.TestProperty, "String value should remain unchanged");
        }
    }
}
