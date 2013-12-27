namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;

    public class PacketEmitter : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid id;
        string name;
        SourceAddress source;
        ObservableCollection<Packet.Packet> emitPackets;

        public Guid Id
        {
            get { return id; }
            set { id = value; OnPropertyChanged("Id"); }
        }
        public string Name
        {
            get { return name; }
            set { name = value; OnPropertyChanged("Name"); }
        }
        public SourceAddress Source
        {
            get { return source; }
            set { source = value; OnPropertyChanged("Source"); }
        }
        public ObservableCollection<Packet.Packet> EmitPackets
        {
            get { return emitPackets; }
            set { emitPackets = value; OnPropertyChanged("EmitPackets"); }
        }

        public PacketEmitter()
        {
            Id = Guid.NewGuid();
            EmitPackets = new ObservableCollection<Packet.Packet>();
            Name = "New Emitter";
        }

        private void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
