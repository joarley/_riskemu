namespace RylPacketAnalyzerV2.Infrastructure.MessageBox
{
    using System.Windows;
    using Caliburn.Micro;

    class MessageBoxViewModel : Screen, IMessageBox
    {
        public event System.EventHandler<ResultCompletionEventArgs> Completed = delegate { };

        MessageBoxOption selection;
        public string Message { get; set; }
        public MessageBoxOption Options { get; set; }
        public MessageBoxOption SelectedOptions { get; set; }
        public IWindowManager WindowManager { get; set; }
        public Visibility OkVisible { get { return IsVisible(MessageBoxOption.Ok); } }
        public Visibility CancelVisible { get { return IsVisible(MessageBoxOption.Cancel); } }
        public Visibility YesVisible { get { return IsVisible(MessageBoxOption.Yes); } }
        public Visibility NoVisible { get { return IsVisible(MessageBoxOption.No); } }

        public MessageBoxViewModel(IWindowManager windowManager)
        {
            this.WindowManager = windowManager;
        }

        public void Ok()
        {
            Select(MessageBoxOption.Ok);
        }

        public void Cancel()
        {
            Select(MessageBoxOption.Cancel);
        }

        public void Yes()
        {
            Select(MessageBoxOption.Yes);
        }

        public void No()
        {
            Select(MessageBoxOption.No);
        }

        public bool WasSelected(MessageBoxOption option)
        {
            return (selection & option) == option;
        }

        Visibility IsVisible(MessageBoxOption option)
        {
            return (Options & option) == option ? System.Windows.Visibility.Hidden :
                System.Windows.Visibility.Collapsed;
        }

        void Select(MessageBoxOption option)
        {
            selection = option;
            TryClose();
        }

        public void Execute(ActionExecutionContext context)
        {
            WindowManager.ShowPopup(this);
            this.Deactivated += MessageBoxViewModel_Deactivated;
        }

        void MessageBoxViewModel_Deactivated(object sender, DeactivationEventArgs e)
        {
            Completed(this, new ResultCompletionEventArgs());
        }
    }
}
