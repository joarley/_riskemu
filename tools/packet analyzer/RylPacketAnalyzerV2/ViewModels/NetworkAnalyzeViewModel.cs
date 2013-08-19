﻿namespace RylPacketAnalyzerV2.ViewModels
{
    using System.Collections.ObjectModel;

    class NetworkAnalyzeViewModel: Infrastructure.ScreenWithCloseVisibility
    {
        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }

        public NetworkAnalyzeViewModel(ObservableCollection<Model.NetworkComunication.PacketEmitter> packetEmitters)
        {
            PacketEmitters = packetEmitters;
        }        
    }
}