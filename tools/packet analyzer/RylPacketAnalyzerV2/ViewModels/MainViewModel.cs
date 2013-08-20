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
                    EmitPackets = new ObservableCollection<Model.Packet.Packet>
                    {
                        new Model.Packet.Packet(){Command = 255, Name="Teste1"},
                        new Model.Packet.Packet(){Command = 2, Name="Teste2"},
                        new Model.Packet.Packet(){Command = 3, Name="Teste3"}
                    },
                    Source = new Model.NetworkComunication.SourceAddress() { Port = 101 },
                    Name = "Teste"
                });
            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter()
                {
                    Source = new Model.NetworkComunication.SourceAddress() { Type = Model.NetworkComunication.SourceAddressType.Server }
                });
            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter()
                {
                    Source = new Model.NetworkComunication.SourceAddress() { Type = Model.NetworkComunication.SourceAddressType.Server }
                });

            Items.AddRange(
                new Infrastructure.ScreenWithCloseVisibility[] 
                {
                    new NetworkAnalyzeViewModel(PacketEmitters) { CloseVisibility = Visibility.Collapsed, DisplayName = "Network Analyze" },
                    new PacketEmitterShellViewModel(PacketEmitters, this) { CloseVisibility = Visibility.Collapsed, DisplayName = "Packets" }
                });

            ActivateItem(Items[0]);
        }

        public void CloseTab(Infrastructure.ScreenWithCloseVisibility item)
        {
            if (item.CanClose())
                Items.Remove(item);
        }
    }
}
