namespace RylPacketAnalyzerV2.Infrastructure
{
    using Caliburn.Micro;
    using System.Windows;

    class ScreenWithCloseVisibility : Screen
    {
        Visibility closeVisibility;

        public Visibility CloseVisibility
        {
            get { return closeVisibility; }
            set
            {
                closeVisibility = value;
                NotifyOfPropertyChange(() => CloseVisibility);
            }
        }
    }
}
