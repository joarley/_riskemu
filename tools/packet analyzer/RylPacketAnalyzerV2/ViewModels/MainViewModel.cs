namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;
    using System.ComponentModel.Composition;
    using System.Windows;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Infrastructure;

    [Export(typeof(IShell))]
    public class MainViewModel : Conductor<IScreen>.Collection.OneActive, IShell
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }
        public IWindowManager WindowManager { get; set; }

        [ImportingConstructor]
        public MainViewModel(IWindowManager windowManager)
        {
            DisplayName = "Packet Analyzer";
            WindowManager = windowManager;

            PacketEmitters = new ObservableCollection<Model.NetworkComunication.PacketEmitter>();            

            #region Initialize Test
            PacketEmitters.Add(new Model.NetworkComunication.PacketEmitter()
                {
                    EmitPackets = new ObservableCollection<Model.Packet.Packet>
                    {
                        new Model.Packet.Packet(){Command = 255, Name="Teste1", 
                            Content = new ObservableCollection<Model.Packet.IPacketPart>(){
                                new Model.Packet.Parts.FloatPart(),
                                new Model.Packet.Parts.IntergerPart(),
                                new Model.Packet.Parts.StructPart(){
                                    Content = new ObservableCollection<Model.Packet.IPacketPart>(){
                                        new Model.Packet.Parts.FloatPart(),
                                        new Model.Packet.Parts.IntergerPart()
                                    }
                                }
                            }},
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

            Items.Add(new NetworkAnalyzeViewModel(PacketEmitters));
            Items.Add(new PacketEmitterShellViewModel(PacketEmitters, this));
            ActivateItem(Items[0]);
        }

        public Infrastructure.IMessageBox ShowMessageBox(string message, string title = null, Infrastructure.MessageBoxOption options = MessageBoxOption.Ok)
        {
            return new Infrastructure.MessageBox.MessageBoxViewModel(WindowManager);
        }

        public IResult ShowEditScreen(IScreen dialogModel)
        {
            var edit = new Infrastructure.EditScreen.EditScreenViewModel(dialogModel) { 
                CloseVisibility =Visibility.Visible
            };
            Items.Add(edit);
            return edit;
        }
    }
}
