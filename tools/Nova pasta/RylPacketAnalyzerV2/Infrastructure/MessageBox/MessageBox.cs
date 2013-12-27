namespace RylPacketAnalyzerV2.Infrastructure.MessageBox
{
    using System.Dynamic;
    using System.Windows;
    using System.Windows.Controls.Primitives;
    using Caliburn.Micro;

    class MessageBox : IMessageBox
    {
        public event System.EventHandler<ResultCompletionEventArgs> Completed = delegate { };

        public MessageBoxButton Options { get; set; }
        public MessageBoxResult SelectedOption { get; set; }
        public IViewAware Owner { get; set; }
        public string MessageBoxText { get; set; }
        public string Caption { get; set; }

        public MessageBox(IViewAware owner, string messageBoxText, string caption, MessageBoxButton options)
        {
            Owner = owner;
            MessageBoxText = messageBoxText;
            Caption = caption;
            Options = options;
        }

        public void Execute(ActionExecutionContext context)
        {
            if (Owner == null || Owner.GetView() == null)
                SelectedOption = System.Windows.MessageBox.Show(
                    MessageBoxText, Caption, Options);
            else
                SelectedOption = System.Windows.MessageBox.Show(
                    (Window)Owner.GetView(), MessageBoxText, Caption, Options);
            Completed(this, new ResultCompletionEventArgs());
        }
    }
}
