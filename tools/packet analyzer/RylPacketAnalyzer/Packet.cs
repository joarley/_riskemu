using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace RylPacketAnalyzer
{
    public class Packet : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid id;
        PacketSource source;
        List<PktPart> content;
        string name;
        byte command;

        public Guid Id { get { return id; } set { SetField(ref id, value, "Id"); } }
        public PacketSource Source { get { return source; } set { SetField(ref source, value, "Source"); } }
        public List<PktPart> Content { get { return content; } set { SetField(ref content, value, "Content"); } }
        public string Name { get { return name; } set { SetField(ref name, value, "Name"); } }
        public byte Command { get { return command; } set { SetField(ref command, value, "Command"); } }

        public Packet()
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
