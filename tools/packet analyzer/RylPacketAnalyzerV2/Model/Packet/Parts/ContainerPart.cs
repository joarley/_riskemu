namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System.Collections.Generic;

    public abstract class ContainerPart : PacketPart
    {
        public List<PacketPart> Content;
    }
}
