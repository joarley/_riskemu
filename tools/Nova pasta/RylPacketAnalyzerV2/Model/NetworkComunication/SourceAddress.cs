namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    using System.ComponentModel;

    public struct SourceAddress : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        int port;
        SourceAddressType type;

        public int Port
        {
            get { return port; }
            set { port = value; OnPropertyChanged("Port"); }
        }

        public SourceAddressType Type
        {
            get { return type; }
            set { type = value; OnPropertyChanged("Type"); }
        }

        public static bool operator ==(SourceAddress a, SourceAddress b)
        {
            if (a == null && b == null) return true;
            if (a == null || b == null) return false;
            return a.Port == b.Port &&
                a.type == b.type;
        }


        public static bool operator !=(SourceAddress a, SourceAddress b)
        {
            return !(a == b);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj);
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