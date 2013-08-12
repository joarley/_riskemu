using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class Packet
    {
        public Destination Destination { get; set; }
        public List<PktPart> Content { get; set; }
    }
}
