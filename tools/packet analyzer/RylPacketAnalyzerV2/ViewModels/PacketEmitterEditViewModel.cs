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
        SourceAddress packetEmitterSource;

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
        public SourceAddress PacketEmitterSource
        {
            get { return packetEmitterSource; }
            set { packetEmitterSource = value; NotifyOfPropertyChange(() => PacketEmitterSource); }
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
            PacketEmitterSource = PacketEmitter.Source;
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
            PacketEmitter.Source = PacketEmitterSource;

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
