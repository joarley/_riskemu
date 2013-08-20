namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using System.Linq;
    using Caliburn.Micro;

    class PacketEditViewModel : Infrastructure.ScreenWithCloseVisibility
    {
        public Model.Packet.Packet Packet;

        public PacketEditViewModel(Model.Packet.Packet packet)
        {
            Packet = packet;
        }

        public void Save()
        {
        }

        public void Cancel()
        {
        }

        public override bool CanClose()
        {
            return true;
        }
    }
}
