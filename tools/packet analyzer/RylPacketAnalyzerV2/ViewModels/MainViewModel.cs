namespace RylPacketAnalyzerV2.ViewModels
{
    using Caliburn.Micro;
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;

    class MainViewModel : Conductor<IScreen>.Collection.OneActive
    {
        public MainViewModel()
        {
            ActivateItem(new NetworkAnalyzeViewModel() { CloseButtonVisibility = Visibility.Collapsed, DisplayName="Network Analyze"});
            ActivateItem(new NetworkAnalyzeViewModel() { CloseButtonVisibility = Visibility.Hidden, DisplayName = "Network Analyze" });
            ActivateItem(new NetworkAnalyzeViewModel() { CloseButtonVisibility = Visibility.Visible, DisplayName = "Network Analyze" });
        }
    }
}
