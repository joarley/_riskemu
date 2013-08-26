namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    using System;
    using System.ComponentModel;

    public class SourceAddress : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid id;
        int port;
        SourceAddressType type;

        public Guid Id
        {
            get { return id; }
            set { id = value; OnPropertyChanged("Id"); }
        }
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

        public SourceAddress() : this(new Guid()) { }

        public SourceAddress(Guid id)
        {
            this.Id = id;
        }

        public static bool operator ==(SourceAddress a, SourceAddress b)
        {
            if (a == null && b == null) return true;
            if (a == null || b == null) return false;
            return a.Id == b.Id;
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