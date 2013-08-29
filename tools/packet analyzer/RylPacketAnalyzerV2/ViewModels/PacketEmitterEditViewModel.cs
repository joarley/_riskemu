namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.ComponentModel;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.NetworkComunication;

    public class PacketEmitterEditViewModel : Screen
    {
        PacketEmitter packetEmitter;

        Guid packetEmitterId;
        string packetEmitterName;
        int packetEmitterPort;
        SourceAddressType packetEmitterType;

        public bool Saved { get; set; }

        public Guid PacketEmitterId
        {
            get { return packetEmitterId; }
            set { packetEmitterId = value; NotifyOfPropertyChange(() => PacketEmitterId); }
        }
        public string PacketEmitterName
        {
            get { return packetEmitterName; }
            set { packetEmitterName = value; NotifyOfPropertyChange(() => PacketEmitterName); }
        }

        public int PacketEmitterSourcePort
        {
            get { return packetEmitterPort; }
            set { packetEmitterPort = value; NotifyOfPropertyChange(() => PacketEmitterSourcePort); }
        }

        public SourceAddressType PacketEmitterSourceType
        {
            get { return packetEmitterType; }
            set { packetEmitterType = value; NotifyOfPropertyChange(() => PacketEmitterSourceType); }
        }

        public PacketEmitter PacketEmitter
        {
            get { return packetEmitter; }
            set { packetEmitter = value; NotifyOfPropertyChange(() => PacketEmitter); }
        }

        public PacketEmitterEditViewModel(PacketEmitter packetEmitter)
        {
            Load(packetEmitter);
        }

        private void Load(PacketEmitter packetEmitter)
        {
            PropertyChanged += PacketEmitterEditViewModel_PropertyChanged;
            PacketEmitter = packetEmitter;

            PacketEmitterId = PacketEmitter.Id;
            PacketEmitterName = PacketEmitter.Name;
            PacketEmitterSourcePort = PacketEmitter.Source.Port;
            PacketEmitterSourceType = PacketEmitter.Source.Type;
        }

        void PacketEmitterEditViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "PacketEmitterName")
            {
                DisplayName = string.Format("Packet Emiter - {0}", packetEmitterName);
            }
        }

        public void Create()
        {
            Load(new PacketEmitter());
        }

        public void Save()
        {
            PacketEmitter.Id = PacketEmitterId;
            PacketEmitter.Name = PacketEmitterName;
            PacketEmitter.Source = new SourceAddress()
            {
                Port = PacketEmitterSourcePort,
                Type = PacketEmitterSourceType
            };

            Saved = true;
            TryClose();
        }

        public void Cancel()
        {
            Saved = false;
            TryClose();
        }
    }
}
