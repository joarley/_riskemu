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
    /// Interaction logic for CtrlPacketEdit.xaml
    /// </summary>
    public partial class CtrlPacketEdit : UserControl
    {
        public List<Packet> Packets { get; set; }

        public CtrlPacketEdit(List<Packet> packets)
        {
            Packets = packets;
            InitializeComponent();

            if (packets != null)
                AssembleTree();
        }

        private void AssembleTree()
        {
            var grouped = Packets.GroupBy(x => x.Destination);

            foreach (var g in grouped)
            {
                TreeViewItem root = new TreeViewItem() { Header = g.Key };
                foreach (var i in g)
                    AssembleNode(root, i.Content);
                pktTree.Items.Add(root);
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
    }
}
