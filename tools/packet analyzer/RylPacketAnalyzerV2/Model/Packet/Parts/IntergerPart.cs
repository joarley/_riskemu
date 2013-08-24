namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;

    public struct IntergerPart : IPacketPart
    {
        public Guid Id { get; set; }
        public int Order { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public bool IsUnsigned {get;set;}
        public IntergerPartPrecision Precision { get; set; }
    }
}
