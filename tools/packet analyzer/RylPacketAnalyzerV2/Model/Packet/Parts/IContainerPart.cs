namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System.Collections.ObjectModel;

    public interface IContainerPart : IPacketPart
    {
        ObservableCollection<IPacketPart> Content { get; set; }
    }
}
