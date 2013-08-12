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

namespace RylPacketAnalyzer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    class teste
    {
        public int Command{get;set;}
    }

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            captureGrid.Items.Add(new teste() {Command = 100 });
            captureGrid.Items.Add(new teste() { Command = 101 });
            captureGrid.Items.Add(new teste() { Command = 200 });
            captureGrid.Items.Add(new teste() { Command = 231 });
        }
    }
}
