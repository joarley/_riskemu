namespace RylPacketAnalyzerV2
{
    using Caliburn.Micro;
    using System.Windows.Threading;

    class AppBootstrapper : Bootstrapper<ViewModels.MainViewModel>
    {

        protected override void OnUnhandledException(object sender, DispatcherUnhandledExceptionEventArgs e)
        {
            base.OnUnhandledException(sender, e);
        }
    }
}
