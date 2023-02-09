using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AresTool.Results
{
    /// <summary>
    /// Interaction logic for ImplementListItem.xaml
    /// </summary>
    public partial class ImplementListItem : UserControl
    {
        public bool WasExisted { get; set; }
        public ImplementListItem()
        {
            InitializeComponent();
            WasExisted = false;
            existCheckBox.IsChecked = false;
        }

        public void setColorOfBox()
        {
            if(existCheckBox.IsChecked == false)
            {
                NamePanel.Background = new SolidColorBrush(Colors.Transparent);
            }
            else
            {
                if (WasExisted)
                {
                    NamePanel.Background = new SolidColorBrush(Colors.GreenYellow);
                }
                else
                {
                    NamePanel.Background = new SolidColorBrush(Colors.OrangeRed);
                }
            }
        }
        private void existCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            setColorOfBox();
        }
        private void existCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            NamePanel.Background = new SolidColorBrush(Colors.Transparent);
        }
    }
}
