namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Windows.Controls;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using RylPacketAnalyzerV2.Model.Packet.Parts;

    class PacketPartEditViewModel : Screen
    {
        IPacketPart part;
        IPacketPart originalPart;

        public bool Saved { get; set; }
        public Type Type { get { return Part.GetType(); } }
        public IPacketPart Part
        {
            get { return part; }
            set
            {
                part = value;
                NotifyOfPropertyChange(() => Part);
                Part.PropertyChanged += PacketPartEditViewModel_PropertyChanged;
            }
        }

        public PacketPartEditViewModel(IPacketPart part)
        {
            Load(part);
        }

        private void Load(IPacketPart part)
        {
            PropertyChanged += PacketPartEditViewModel_PropertyChanged;

            Part = (IPacketPart)Activator.CreateInstance(part.GetType());
            originalPart = part;

            CopyPart(part, Part);
        }

        private void CopyPart(IPacketPart from, IPacketPart to)
        {
            to.Id = from.Id;
            to.Name = from.Name;
            to.Notes = from.Notes;
            to.Order = from.Order;

            dynamic dto = to;
            dynamic dfrom = from;

            if (from is IContainerPart)
                dto.Content = dfrom.Content;

            if (from is FloatPart)
                dto.Precision = dfrom.Precision;
            else if (from is ForPart)
            {
                dto.IntergerPartSizeId = dfrom.IntergerPartSizeId;
                dto.Size = dfrom.Size;                
            }
            else if (from is IntegerPart)
            {
                dto.IsUnsigned = dfrom.IsUnsigned;
                dto.Precision = dfrom.Precision;
            }
            else if (from is StringPart)
            {
                dto.IntergerPartSizeId = dfrom.IntergerPartSizeId;
                dto.Size = dfrom.Size;
            }
            else if (from is StructPart) { } 
        }

        void PacketPartEditViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name" || e.PropertyName == "Part")
            {
                DisplayName = string.Format("Packet Part - {0}", Part.Name);
            }
        }

        public void Save()
        {
            CopyPart(Part, originalPart);
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
