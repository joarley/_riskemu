namespace RylPacketAnalyzerV2.Infrastructure
{
    using Caliburn.Micro;
    using System.Windows;
    using System;

    class ScreenWithCloseVisibilityViewModel : Conductor<IScreen>
    {
        protected Visibility closeVisibility;

        public Type Type { get { return GetType(); } }
        public Visibility CloseVisibility
        {
            get { return closeVisibility; }
            set { closeVisibility = value; NotifyOfPropertyChange(() => CloseVisibility); }
        }

        public ScreenWithCloseVisibilityViewModel(IScreen screen)
        {
            this.ActivateItem(screen);
        }

        public override void TryClose()
        {
            

            base.TryClose();
        }
    }
}
