namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;

    class PacketEditViewModel : Screen
    {
        Packet Packet { get; set; }

        public Guid PacketId { get; set; }
        public byte PacketCommand { get; set; }
        public string PacketName { get; set; }
        public ObservableCollection<IPacketPart> PacketContent { get; set; }

        public bool Saved { get; private set; }

        public PacketEditViewModel()
        {
            Create();
        }

        public PacketEditViewModel(Packet packet)
        {
            Load(packet);
        }

        public void Create()
        {
            Load(new Packet());
        }

        private void Load(Packet packet)
        {
            Packet = packet;
            PacketId = Packet.Id;
            PacketCommand = Packet.Command;
            PacketName = Packet.Name;
            PacketContent = Packet.Content;            
        }

        public void Save()
        {
            Packet.Id = PacketId;
            Packet.Command = PacketCommand;
            Packet.Name = PacketName;
            Packet.Content = PacketContent;

            Saved = true;
            TryClose();            
        }

        public void Cancel()
        {
            Saved = false;
            TryClose(true);
        }

        public override void CanClose(System.Action<bool> callback)
        {
            callback(true);
        }
    }
}
