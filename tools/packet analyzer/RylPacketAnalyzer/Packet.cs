using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class Packet
    {
        public PacketSource Source { get; set; }
        public List<PktPart> Content { get; set; }
        public string Name { get; set; }
    }
}
