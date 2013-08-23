namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using RylPacketAnalyzerV2.Model.Packet.Parts;

    class PacketEditViewModel : Screen
    {
        public Packet Packet { get; set; }
        public Packet BindedPacket { get; set; }

        public bool Saved { get; private set; }

        public PacketEditViewModel(Packet packet)
        {
            Packet = packet;
        }

        public void Save()
        {
            CopyFromTo(Packet, BindedPacket);
            Saved = true;
            TryClose();
        }

        private void CopyFromTo(Packet from, Packet to)
        {
            BindedPacket.Id = Packet.Id;
            BindedPacket.Name = Packet.Name;
        }

        public void Cancel()
        {

            Saved = false;
            TryClose();
        }

        public override void CanClose(System.Action<bool> callback)
        {
            callback(true);
        }
    }
}
