namespace RylPacketAnalyzerV2.Infrastructure
{
    using Caliburn.Micro;

    public interface IMessageBox : IScreen, IResult
    {
        string Message { get; set; }
        MessageBoxOption Options { get; set; }
        MessageBoxOption SelectedOptions { get; set; }

        void Ok();
        void Cancel();
        void Yes();
        void No();
    }
}
