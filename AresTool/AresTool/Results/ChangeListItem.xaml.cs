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
    /// Interaction logic for ChangeListItem.xaml
    /// </summary>
    public partial class ChangeListItem : UserControl
    {
        public string originalName;
        public string originalDirectory;
        public ChangeListItem()
        {
            originalName= string.Empty;
            InitializeComponent();

        }

        private void fileName_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
