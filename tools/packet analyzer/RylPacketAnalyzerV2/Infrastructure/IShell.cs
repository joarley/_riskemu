namespace RylPacketAnalyzerV2.Infrastructure
{
    using System.Windows;
    using Caliburn.Micro;

    public interface IShell : IScreen
    {
        void ShowModalScreen(IScreen dialogModel);
        IResult ShowEditScreen(IScreen dialogModel);
        IMessageBox ShowMessageBox(string message, string title = null, MessageBoxButton options = MessageBoxButton.OK);
    }
}
