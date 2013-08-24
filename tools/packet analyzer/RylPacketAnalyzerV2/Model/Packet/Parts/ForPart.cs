namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;
    using System.Collections.ObjectModel;

    public struct ForPart : IContainerPart
    {
        public Guid Id { get; set; }
        public int Order { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public ObservableCollection<IPacketPart> Content { get; set; }
        public Guid IntergerPartSizeId { get; set; }
        public int Size { get; set; }
    }
}
