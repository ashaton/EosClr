using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace TestUI
{
    internal class EnumDescriptionConverter : IValueConverter
    {
        private string GetDescriptionValue(Enum EnumValue)
        {
            Type enumType = EnumValue.GetType();
            string fieldName = EnumValue.ToString();
            FieldInfo fieldInfo = enumType.GetField(fieldName);

            object[] descriptionAttrs = fieldInfo.GetCustomAttributes(typeof(DescriptionAttribute), false);
            if (descriptionAttrs.Length == 0)
            {
                return fieldName;
            }
            DescriptionAttribute descriptionAttr = (DescriptionAttribute)descriptionAttrs[0];
            return descriptionAttr.Description;
        }

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return GetDescriptionValue((Enum)value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
