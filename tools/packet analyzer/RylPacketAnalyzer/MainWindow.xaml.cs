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
        public List<PacketSource> Sources { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            PacketSource d1 = new PacketSource() { Id = Guid.NewGuid(), Name = "D1"};
            PacketSource d2 = new PacketSource() { Id = Guid.NewGuid(), Name = "D2", Type=PacketSourceType.Client};                        

            List<PktPart> c1 = new List<PktPart>();

            StructPart s1 = new StructPart() { Name = "Struct 1", Content = new List<PktPart>() };
            ForPart f1 = new ForPart { Name = "For 1", Content = new List<PktPart>() };
            IntergerPart i1 = new IntergerPart() { };
            StringPart str1 = new StringPart();

            s1.Content.Add(i1);
            s1.Content.Add(i1);
            s1.Content.Add(str1);
            s1.Content.Add(f1);

            f1.Content.Add(i1);
            f1.Content.Add(i1);
            f1.Content.Add(str1);

            c1.Add(s1);
            c1.Add(i1);
            c1.Add(i1);

            Packet p1 = new Packet() { Source = d1, Content = c1, Name = "p1" };
            Packet p2 = new Packet() { Source = d2, Content = c1, Name = "p2" };
            Packet p3 = new Packet() { Source = d1, Content = c1, Name = "p3" };
            Packet p4 = new Packet() { Source = d2, Content = c1, Name = "p4" };
            Packet p5 = new Packet() { Source = d1, Content = c1, Name = "p5" };
            Packet p6 = new Packet() { Source = d2, Content = c1, Name = "p6" };
            Packet p7 = new Packet() { Source = d1, Content = c1, Name = "p7" };
            Packet p8 = new Packet() { Source = d2, Content = c1, Name = "p8" };

            Packets = new List<Packet>();
            Sources = new List<PacketSource>();

            Packets.Add(p1); Packets.Add(p2); Packets.Add(p3); Packets.Add(p4); Packets.Add(p5); Packets.Add(p6); Packets.Add(p7); Packets.Add(p8);
            Sources.Add(d1); Sources.Add(d2); Sources.Add(new PacketSource() { Id = Guid.NewGuid(), Name = "D3", Type = PacketSourceType.Client });

            tabPacket.Content = new CtrlPacketEdit(Packets, Sources);
        }
    }
}
