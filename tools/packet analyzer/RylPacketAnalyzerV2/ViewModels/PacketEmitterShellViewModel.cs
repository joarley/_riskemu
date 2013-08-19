namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;

    class PacketEmitterShellViewModel: Infrastructure.ScreenWithCloseVisibility
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }

        public PacketEmitterShellViewModel(ObservableCollection<Model.NetworkComunication.PacketEmitter> packetEmitters)
        {
            PacketEmitters = packetEmitters;
        }
    }
}
