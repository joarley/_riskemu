namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;
    public struct FloatPart : IPacketPart
    {
        public Guid Id { get; set; }
        public int Order { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public FloatPartPrecision Precision { get; set; }
    }
}
