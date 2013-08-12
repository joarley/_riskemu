using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class StructPart : PktPart, ContainerPart
    {
        public List<PktPart> Content { get; set; }
        public string Name { get; set; }
    }
}
