namespace RylPacketAnalyzerV2.Infrastructure
{
    using System;
    using System.Windows;
    using Caliburn.Micro;

    public class ScreenWithCloseVisibility : Conductor<IScreen>
    {
        protected Visibility closeVisibility;
        public Type Type { get { return GetType(); } }
        public Visibility CloseVisibility
        {
            get { return closeVisibility; }
            set { closeVisibility = value; NotifyOfPropertyChange(() => CloseVisibility); }
        }
    }
}