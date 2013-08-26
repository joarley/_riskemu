namespace RylPacketAnalyzerV2.Infrastructure
{
    using System.Windows;
    using Caliburn.Micro;

    public interface IShell : IScreen
    {
        IResult ShowEditScreen(IScreen dialogModel);
        IMessageBox ShowMessageBox(string message, string title = null, MessageBoxButton options = MessageBoxButton.OK);
    }
}
