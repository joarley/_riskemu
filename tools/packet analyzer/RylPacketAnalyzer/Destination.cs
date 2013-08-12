using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    class Destination
    {
        public string Name;
        public int Port;
        public DestinationType Type;        
    }

    enum DestinationType
    {
        Server, Client
    }
}
