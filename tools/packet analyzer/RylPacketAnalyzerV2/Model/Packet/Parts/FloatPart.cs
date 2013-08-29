﻿namespace RylPacketAnalyzerV2.Model.Packet.Parts
{
    using System;
    using System.ComponentModel;

    public class FloatPart : IPacketPart
    {
        Guid id;
        int order;
        string name;
        string notes;

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

        public FloatPartPrecision Precision { get; set; }

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
