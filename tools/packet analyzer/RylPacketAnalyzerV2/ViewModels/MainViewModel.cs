namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;
    using System.ComponentModel.Composition;
    using System.Dynamic;
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

            Items.Add(new NetworkAnalyzeViewModel(PacketEmitters));
            Items.Add(new PacketEmitterShellViewModel(PacketEmitters, this));
            ActivateItem(Items[0]);
        }

        public Infrastructure.IMessageBox ShowMessageBox(string message, string title = null, MessageBoxButton options = MessageBoxButton.OK)
        {
            return new Infrastructure.MessageBox.MessageBox(this, message, title, options);
        }

        public IResult ShowEditScreen(IScreen dialogModel)
        {
            var edit = new Infrastructure.EditScreen.EditScreenViewModel(dialogModel)
            {
                CloseVisibility = Visibility.Visible
            };
            Items.Add(edit);
            return edit;
        }

        public void ShowModalScreen(IScreen dialogModel)
        {
            dynamic settings = new ExpandoObject();
            settings.ResizeMode = ResizeMode.NoResize;
            settings.ShowInTaskbar = false;
            WindowManager.ShowDialog(dialogModel, null, settings);
        }
    }
}
