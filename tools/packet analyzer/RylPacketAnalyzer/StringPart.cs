using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class StringPart : PktPart
    {
        public bool Fixed { get; set; }
        public int FixedSize { get; set; }
        public IntergerPart SizeInt  { get; set; }
    }
}
