using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RylPacketAnalyzer
{
    public class ContainerPart : PktPart
    {
        public List<PktPart> Content { get; set; }
    }
}
