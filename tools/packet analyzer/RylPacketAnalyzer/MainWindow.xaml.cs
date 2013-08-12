using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    public partial class MainWindow : Window
    {
        public List<Packet> Packets { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            Destination d1 = new Destination() { Id = Guid.NewGuid(), Name = "D1"};
            Destination d2 = new Destination() { Id = Guid.NewGuid(), Name = "D2" };            

            List<PktPart> c1 = new List<PktPart>();

            StructPart s1 = new StructPart() { Name = "Struct 1", Content = new List<PktPart>() };
            IntergerPart i1 = new IntergerPart() { };
            StringPart str1 = new StringPart();

            s1.Content.Add(i1);
            s1.Content.Add(i1);
            s1.Content.Add(str1);

            c1.Add(s1);
            c1.Add(i1);
            c1.Add(i1);

            Packet p1 = new Packet() { Destination = d1, Content = c1 };
            Packet p2 = new Packet() { Destination = d2, Content = c1 };

            Packets = new List<Packet>();
            Packets.Add(p1); Packets.Add(p2);

            tabPacket.Content = new CtrlPacketEdit(Packets);
        }
    }
}
