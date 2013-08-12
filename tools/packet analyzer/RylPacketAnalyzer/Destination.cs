using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Imaging;

namespace RylPacketAnalyzer
{
    public class Destination
    {
        public Guid Id { get; set; }
        public int Port { get; set; }
        public string Name { get; set; }
        public DstType Type { get; set; }        
    }
}
