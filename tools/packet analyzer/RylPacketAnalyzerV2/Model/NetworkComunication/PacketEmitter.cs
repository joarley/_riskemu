namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    using System;
    using System.Collections.ObjectModel;

    public class PacketEmitter
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public SourceAddress Source { get; set; }
        public ObservableCollection<Packet.Packet> EmitPackets { get; set; }

        public PacketEmitter()
        {
            Id = Guid.NewGuid();
            EmitPackets = new ObservableCollection<Packet.Packet>();
        }
    }
}
