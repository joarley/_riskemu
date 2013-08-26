namespace RylPacketAnalyzerV2.Infrastructure
{
    using Caliburn.Micro;

    public interface IShell : IScreen
    {
        IResult ShowEditScreen(IScreen dialogModel);
        Infrastructure.IMessageBox ShowMessageBox(string message, string title = null, MessageBoxOption options = MessageBoxOption.Ok);
    }
}
