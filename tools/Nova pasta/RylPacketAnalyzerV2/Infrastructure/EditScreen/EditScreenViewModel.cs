namespace RylPacketAnalyzerV2.Infrastructure.EditScreen
{
    using System;
    using Caliburn.Micro;

    public class EditScreenViewModel : ScreenWithCloseVisibility, IResult
    {
        public event System.EventHandler<ResultCompletionEventArgs> Completed = delegate { };

        public EditScreenViewModel(IScreen screen)
        {
            ActivateItem(screen);
            DisplayName = ActiveItem.DisplayName;
            screen.PropertyChanged += screen_PropertyChanged;
        }

        void screen_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "DisplayName")
                DisplayName = ActiveItem.DisplayName;
        }

        public void Execute(ActionExecutionContext context)
        {
            ActiveItem.Activate();
            Action<object, DeactivationEventArgs> ev = null;
            ev = (sender, e) =>
                {
                    if (e.WasClosed)
                    {

                        ActiveItem.Deactivated -= new EventHandler<DeactivationEventArgs>(ev);
                        if (this.IsActive) this.TryClose();
                        Completed(this, new ResultCompletionEventArgs());
                    }
                };

            ActiveItem.Deactivated += new EventHandler<DeactivationEventArgs>(ev);
        }
    }
}
