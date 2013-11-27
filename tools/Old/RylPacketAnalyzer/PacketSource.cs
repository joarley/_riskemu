using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows.Media.Imaging;

namespace RylPacketAnalyzer
{
    public class PacketSource : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid id;
        int port;
        string name;
        PacketSourceType type;

        public Guid Id { get { return id; } set { SetField(ref id, value, "Id"); } }
        public int Port { get { return port; } set { SetField(ref port, value, "Port"); } }
        public string Name { get { return name; } set { SetField(ref name, value, "Name"); } }
        public PacketSourceType Type { get { return type; } set { SetField(ref type, value, "Type"); } }

        public PacketSource()
        {
            id = Guid.NewGuid();
        }

        void OnPropertyChanged(string pn)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(pn));
            }
        }

        void SetField<T>(ref T field, T value, string propertyName)
        {
            if (EqualityComparer<T>.Default.Equals(field, value)) return;
            field = value;
            OnPropertyChanged(propertyName);
        }
    }
}
