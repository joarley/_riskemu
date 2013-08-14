namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    using System;
    using System.Collections.Generic;

    public class PacketEmitter
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public SourceAddress Source { get; set; }
        public List<Packet.Packet> EmitPackets { get; set; }
    }
}
