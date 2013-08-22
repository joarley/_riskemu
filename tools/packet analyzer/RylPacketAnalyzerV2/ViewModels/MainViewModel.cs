namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;
    using System.Windows;
    using Caliburn.Micro;

    class MainViewModel : Conductor<IScreen>.Collection.OneActive
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }

        public MainViewModel()
        {
            PacketEmitters = new ObservableCollection<Model.NetworkComunication.PacketEmitter>();

            #region Initialize Test
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
            #endregion

            Items.Add(new NetworkAnalyzeViewModel(PacketEmitters) { DisplayName = "Network Analyze" });
            Items.Add(new PacketEmitterShellViewModel(PacketEmitters, this) { DisplayName = "Packets" });
            ActivateItem(Items[0]);
        }
    }
}
