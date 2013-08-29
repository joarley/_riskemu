namespace RylPacketAnalyzerV2.Model.Packet
{
    using System;
    using System.ComponentModel;

    public interface IPacketPart : INotifyPropertyChanged
    {
        Guid Id { get; set; }
        int Order { get; set; }
        string Name { get; set; }
        string Notes { get; set; }
    }
}
