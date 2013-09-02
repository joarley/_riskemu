namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;

    public class ForPart : IContainerPart
    {
        Guid id;
        int order;
        string name;
        string notes;
        ObservableCollection<IPacketPart> content;
        Guid? intergerPartSizeId;
        int? size;

        public Guid Id
        {
            get { return id; }
            set { id = value; OnPropertyChanged("Id"); }
        }
        public int Order
        {
            get { return order; }
            set { order = value; OnPropertyChanged("Order"); }
        }
        public string Name
        {
            get { return name; }
            set { name = value; OnPropertyChanged("Name"); }
        }
        public string Notes
        {
            get { return notes; }
            set { notes = value; OnPropertyChanged("Notes"); }
        }

        public ObservableCollection<IPacketPart> Content
        {
            get { return content; }
            set { content = value; OnPropertyChanged("Content"); }
        }

        public Guid? IntergerPartSizeId
        {
            get { return intergerPartSizeId; }
            set { intergerPartSizeId = value; OnPropertyChanged("IntergerPartSizeId"); }
        }

        public int? Size
        {
            get { return size; }
            set { size = value; OnPropertyChanged("Size"); }
        }

        public ForPart()
        {
            Name = "New For Part";
            Id = Guid.NewGuid();
            Content = new ObservableCollection<IPacketPart>();
        }

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
