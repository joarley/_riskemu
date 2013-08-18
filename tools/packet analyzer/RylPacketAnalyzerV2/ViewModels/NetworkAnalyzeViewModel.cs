namespace RylPacketAnalyzerV2.ViewModels
{
    class NetworkAnalyzeViewModel: Infrastructure.CloseTabItem
    {
        System.Windows.Visibility[] v = new System.Windows.Visibility[] { System.Windows.Visibility.Hidden, System.Windows.Visibility.Collapsed, System.Windows.Visibility.Visible };
        int i = 0;

        public NetworkAnalyzeViewModel()
        {
            
        }
        public void teste()
        {
            CloseButtonVisibility = v[i++ % 3];
        }
    }
}
