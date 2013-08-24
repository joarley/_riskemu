namespace RylPacketAnalyzerV2.Model.Packet
{
    using System;

    public interface IPacketPart
    {
        Guid Id { get; set; }
        int Order { get; set; }
        string Name { get; set; }
        string Notes { get; set; }
    }
}
