namespace RylPacketAnalyzerV2.Infrastructure
{
    using System.Windows;
    using Caliburn.Micro;

    public interface IMessageBox : IResult
    {
        MessageBoxButton Options { get; set; }
        MessageBoxResult SelectedOption { get; set; }
        IViewAware Owner { get; set; }
        string MessageBoxText { get; set; }
        string Caption { get; set; }
    }
}
