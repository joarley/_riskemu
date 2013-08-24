namespace RylPacketAnalyzerV2.Infrastructure
{
    using Caliburn.Micro;

    public interface IRootWindow : IScreen, IShell
    {
        IResult ShowEditScreen(IScreen dialogModel);
        Infrastructure.IMessageBox ShowMessageBox(string message, string title = null, MessageBoxOption options = MessageBoxOption.Ok);
    }
}
