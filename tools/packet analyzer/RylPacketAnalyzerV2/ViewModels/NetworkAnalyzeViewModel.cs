namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;
    using Caliburn.Micro;

    class NetworkAnalyzeViewModel: Screen
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }

        public NetworkAnalyzeViewModel(ObservableCollection<Model.NetworkComunication.PacketEmitter> packetEmitters)
        {
            PacketEmitters = packetEmitters;
        }        
    }
}
