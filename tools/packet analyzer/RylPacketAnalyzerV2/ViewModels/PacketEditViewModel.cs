namespace RylPacketAnalyzerV2.ViewModels
{
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;

    class PacketEditViewModel : Screen
    {
        Guid packetId;
        byte packetCommand;
        string packetName;
        ObservableCollection<IPacketPart> packetContent;        

        public Guid PacketId
        {
            get { return packetId; }
            set { packetId = value; NotifyOfPropertyChange(() => PacketId); }
        }
        public byte PacketCommand
        {
            get { return packetCommand; }
            set { packetCommand = value; NotifyOfPropertyChange(() => PacketCommand); }
        }
        public string PacketName
        {
            get { return packetName; }
            set { packetName = value; NotifyOfPropertyChange(() => PacketName); }
        }
        public ObservableCollection<IPacketPart> PacketContent
        {
            get { return packetContent; }
            set { packetContent = value; NotifyOfPropertyChange(() => PacketContent); }
        }

        public Packet Packet { get; set; }
        public bool Saved { get; private set; }
        public bool Changed { get; private set; }
        public IPacketPart SelectedPacketPart { get; private set; }

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
            PropertyChanged += PacketEditViewModel_PropertyChanged;

            Packet = packet;
            PacketId = Packet.Id;
            PacketCommand = Packet.Command;
            PacketName = Packet.Name;
            PacketContent = Packet.Content;
        }

        void PacketEditViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "PacketCommand" || e.PropertyName == "PacketName")
            {
                DisplayName = string.Format("Packet 0x{0:X2} - {1}", packetCommand, packetName);
            }
        }

        public void SetSelectedPacketContent(IPacketPart part)
        {
            SelectedPacketPart = part;
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
            TryClose();
        }

        public override void CanClose(System.Action<bool> callback)
        {
            callback(true);
        }
    }
}
