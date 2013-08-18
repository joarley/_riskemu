namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    public class IntergerPart : PacketPart
    {
        public bool IsUnsigned {get;set;}
        public IntergerPartPrecision Precision { get; set; }
    }
}
