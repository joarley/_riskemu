namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;

    public struct StringPart : IPacketPart
    {
        public Guid Id { get; set; }
        public int Order { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public Guid IntergerPartSizeId;
        public int Size;        
    }
}
