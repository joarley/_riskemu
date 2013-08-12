using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class ForPart : PktPart, ContainerPart
    {
        public int SizeFixed { get; set; }
        public IntergerPart SizeIntergerPart { get; set; }
        public List<PktPart> Content { get; set; }
        public string Name { get; set; }
    }
}
