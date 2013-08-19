namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;
    using System.Windows;
    using Caliburn.Micro;

    class MainViewModel : Conductor<Infrastructure.ScreenWithCloseVisibility>.Collection.OneActive
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }
        
        public MainViewModel()
        {
            PacketEmitters = new ObservableCollection<Model.NetworkComunication.PacketEmitter>();

            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter()
                {
                    EmitPackets = new System.Collections.Generic.List<Model.Packet.Packet>()
                    {
                        new Model.Packet.Packet(),
                        new Model.Packet.Packet(),
                        new Model.Packet.Packet()
                    }
                });
            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter());
            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter());

            Items.AddRange(
                new Infrastructure.ScreenWithCloseVisibility[] 
                {
                    new NetworkAnalyzeViewModel(PacketEmitters) { CloseVisibility = Visibility.Collapsed, DisplayName = "Network Analyze" },
                    new PacketEmitterShellViewModel(PacketEmitters) { CloseVisibility = Visibility.Collapsed, DisplayName = "Packets" }
                });

            ActivateItem(Items[0]);
        }

        public void CloseTab(Infrastructure.ScreenWithCloseVisibility item)
        {
            Items.Remove(item);
        }
    }
}
