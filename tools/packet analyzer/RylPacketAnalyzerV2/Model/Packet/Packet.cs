namespace RylPacketAnalyzerV2.Model.Packet
{
    using System;
    using System.Collections.Generic;

    public class Packet
    {
        public Guid Id { get; set; }
        public byte Command { get; set; }
        public string Name { get; set; }
        public List<PacketPart> Content { get; set; }
    }
}
