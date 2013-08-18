namespace RylPacketAnalyzerV2.Model.Packet
{
    using System;

    public abstract class PacketPart
    {
        public Guid Id { get; set; }
        public int Order { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
    }
}
