using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.ComponentModel;
using TaskManagerApp.Utility;

namespace TaskManagerApp.Tests
{
    public class ObservableObjectTestHelper : ObservableObject
    {
        private string _str;
        
        public string StrProperty
        {
            get => _str;
            set => SetProperty(ref _str, value);
        }

        public ObservableObjectTestHelper(string str = null, int i = 0, bool b = false)
        {
            StrProperty = str;
        }
    }

    [TestClass]
    public class ObservableObjectTest
    {
        [TestMethod]
        public void SetProperty_RaisesPropertyChanged_WhenValueChanges()
        {
            var obj = new ObservableObjectTestHelper("initial");
            string receviedPropertyName = null;

            obj.PropertyChanged += (sender, e) => receviedPropertyName = e.PropertyName;
            obj.StrProperty = "New Value";

            Assert.AreEqual("StrProperty", receviedPropertyName);
            Assert.AreEqual("New Value", obj.StrProperty);
        }

        [TestMethod]
        public void SetProperty_NotRaisesPropertyChanged_WhenValueIsSame()
        {
            var obj = new ObservableObjectTestHelper("initial");
            string receviedPropertyName = null;

            obj.PropertyChanged += (sender, e) => receviedPropertyName = e.PropertyName;
            obj.StrProperty = "initial";

            Assert.IsNull(receviedPropertyName);
            Assert.AreEqual("initial", obj.StrProperty);
        }
    }
}
