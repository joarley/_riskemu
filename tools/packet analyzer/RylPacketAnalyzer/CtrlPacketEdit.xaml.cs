using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
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
    /// Interaction logic for CtrlPacketEdit.xaml
    /// </summary>
    public partial class CtrlPacketEdit : UserControl
    {
        public List<Packet> Packets { get; set; }
        public List<PacketSource> Sources { get; set; }

        public CtrlPacketEdit(List<Packet> packets, List<PacketSource> sources)
        {
            Packets = packets;
            Sources = sources;

            InitializeComponent();

            if (packets != null)
                AssembleTree();
        }

        #region AssembleTree
        private void AssembleTree()
        {
            pktTree.Items.Clear();

            var grouped = Packets.GroupBy(x => x.Source);
            List<TreeViewItem> sources = new List<TreeViewItem>();

            foreach (var g in grouped)
            {
                TreeViewItem root = new TreeViewItem() { Header = g.Key };
                foreach (var i in g)
                {
                    TreeViewItem p = new TreeViewItem() { Header = i };
                    AssembleNode(p, i.Content);
                    root.Items.Add(p);
                }
                sources.Add(root);
            }

            var tmp = Sources.Except(sources.Select(x => x.Header));
            foreach (var a in tmp)
            {
                sources.Add(new TreeViewItem() { Header = a });
            }

            foreach (var a in sources)
            {
                pktTree.Items.Add(a);
            }
        }

        private void AssembleNode(TreeViewItem node, List<PktPart> content)
        {
            foreach (var part in content)
            {
                TreeViewItem child = new TreeViewItem() { Header = part };
                node.Items.Add(child);
                if (part is ContainerPart)
                    AssembleNode(child, (part as ContainerPart).Content);
            }
        }

        #endregion

        #region SourcePacket
        private void SourcePacket_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount >= 2)
            {
                PacketSource src = (PacketSource)(sender as FrameworkElement).DataContext;
                PacketSource edit = new PacketSource();
                edit.Id = src.Id;
                edit.Name = src.Name;
                edit.Port = src.Port;
                edit.Type = src.Type;

                sourceEdit.DataContext = edit;
                sourceEdit.ShowModalContent();
                e.Handled = true;
            }
        }

        private void SaveSource_Click(object sender, RoutedEventArgs e)
        {
            sourceEdit.HideModalContent();
            PacketSource src = (PacketSource)(sender as FrameworkElement).DataContext;

            var ori = Sources.FirstOrDefault(x => x.Id == src.Id);
            if (ori == null)
            {
                Sources.Add(src);
                pktTree.Items.Add(new TreeViewItem() { Header = src });
            }
            else
            {
                ori.Name = src.Name;
                ori.Port = src.Port;
                ori.Type = src.Type;
            }
        }

        private void CancelEditSource(object sender, RoutedEventArgs e)
        {
            sourceEdit.HideModalContent();
        }

        private void DeleteSource_Click(object sender, RoutedEventArgs e)
        {
            PacketSource src = (PacketSource)(sender as FrameworkElement).DataContext;

            var item = pktTree.Items.Cast<TreeViewItem>().Where(x => x.Header == src);
            if (item.Count() > 0)
            {
                pktTree.Items.Remove(item.First());
            }

            Sources.Remove(src);
            Packets.RemoveAll(x => x.Source.Id == src.Id);
        }

        private void AddSource_Click(object sender, RoutedEventArgs e)
        {
            PacketSource src = new PacketSource();
            sourceEdit.DataContext = src;
            sourceEdit.ShowModalContent();
        }

        #endregion

        #region Packet
        private void AddPacket_Click(object sender, RoutedEventArgs e)
        {
            Packet pkt = new Packet();
            object dc = (sender as FrameworkElement).DataContext;
            if (dc is PacketSource) pkt.Source = dc as PacketSource;
            if (dc is Packet) pkt.Source = (dc as Packet).Source;

            packetEdit.DataContext = pkt;
            packetEdit.ShowModalContent();
        }

        private void DeletePacket_Click(object sender, RoutedEventArgs e)
        {
            Packet src = (Packet)(sender as FrameworkElement).DataContext;
            var items = pktTree.Items.Cast<TreeViewItem>().First(x => x.Header == src.Source);
            var item = items.Items.Cast<TreeViewItem>().First(x => x.Header == src);
            items.Items.Remove(item);
            Packets.Remove(src);
        }

        private void SavePacket_Click(object sender, RoutedEventArgs e)
        {
            packetEdit.HideModalContent();
            Packet src = (Packet)(sender as FrameworkElement).DataContext;

            var ori = Packets.FirstOrDefault(x => x.Id == src.Id);
            if (ori == null)
            {
                Packets.Add(src);
                pktTree.Items.Cast<TreeViewItem>().
                    First(x => (x.Header as PacketSource).Id == src.Source.Id).
                    Items.Add(new TreeViewItem() { Header = src });
            }
            else
            {
                ori.Name = src.Name;
                ori.Command = src.Command;
            }
        }

        private void CancelEditPacket_Click(object sender, RoutedEventArgs e)
        {
            packetEdit.HideModalContent();
        }

        private void Packet_MouseDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
            if (e.ClickCount >= 2)
            {
                Packet src = (Packet)(sender as FrameworkElement).DataContext;
                Packet edit = new Packet();
                edit.Id = src.Id;
                edit.Name = src.Name;
                edit.Command = src.Command;

                packetEdit.DataContext = edit;
                packetEdit.ShowModalContent();
            }
        }

        #endregion

        #region PacketPart
        private void AddPacketPart_Click(object sender, RoutedEventArgs e)
        {

        }
        #endregion
    }
}
