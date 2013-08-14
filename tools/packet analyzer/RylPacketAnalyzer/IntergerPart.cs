using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class IntergerPart : PktPart
    {
        public bool IsUnsigned { get; set; }
        public int Size { get; set; }
    }
}
