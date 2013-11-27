namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.ComponentModel;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using RylPacketAnalyzerV2.Model.Packet.Parts;

    class PacketPartEditViewModel : Screen
    {
        IPacketPart part;
        IPacketPart originalPart;
        SizeType partSizeType;
        int? sizeFixedValue;
        IntegerPart sizeIntegerPart;

        public bool Saved { get; set; }
        public Type Type { get { return Part.GetType(); } }

        public int? SizeFixedValue
        {
            get { return sizeFixedValue; }
            set { sizeFixedValue = value; NotifyOfPropertyChange(() => SizeFixedValue); }
        }

        public IntegerPart SizeIntegerPart
        {
            get { return sizeIntegerPart; }
            set { sizeIntegerPart = value; NotifyOfPropertyChange(() => SizeIntegerPart); }
        }

        SizeType PartSizeType
        {
            get { return partSizeType; }
            set
            {
                partSizeType = value;
                if (partSizeType == SizeType.Fixed) SizeFixedValue = null;
                else if (partSizeType == SizeType.IntegerType) SizeIntegerPart = null;
                else { SizeFixedValue = null; SizeIntegerPart = null; }
                NotifyOfPropertyChange(() => PartSizeType);
            }
        }

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

            if (Part is StringPart)
            {
                var strPart = Part as StringPart;

                SizeFixedValue = strPart.Size;

                if (strPart.IntergerPartSizeId != null) PartSizeType = SizeType.IntegerType;
                else if (strPart.Size != null) PartSizeType = SizeType.Fixed;
                else PartSizeType = SizeType.Unknown;
            }
            else if (Part is ForPart)
            {
                var forPart = Part as ForPart;
                if (forPart.IntergerPartSizeId != null)
                {
                    PartSizeType = SizeType.IntegerType;
                }
                else if (forPart.Size != null)
                {
                    PartSizeType = SizeType.Fixed;
                }
                else
                {
                    PartSizeType = SizeType.Unknown;
                }
            }
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

        public enum SizeType
        {
            Fixed, IntegerType, Unknown
        }
    }
}
