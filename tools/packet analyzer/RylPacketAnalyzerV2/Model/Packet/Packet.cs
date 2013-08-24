namespace RylPacketAnalyzerV2.Model.Packet
{
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;

    public class Packet : INotifyPropertyChanged
    {
        Guid id;
        byte command;
        string name;
        ObservableCollection<IPacketPart> content;

        public Guid Id
        {
            get { return id; }
            set { id = value; OnPropertyChanged("Id"); }
        }
        public byte Command
        {
            get { return command; }
            set { command = value; OnPropertyChanged("Command"); }
        }
        public string Name
        {
            get { return name; }
            set { name = value; OnPropertyChanged("Name"); }
        }
        public ObservableCollection<IPacketPart> Content
        {
            get { return content; }
            set { content = value; OnPropertyChanged("Content"); }
        }

        public Packet()
        {
            id = Guid.NewGuid();
            content = new ObservableCollection<IPacketPart>();
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
