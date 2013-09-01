namespace RylPacketAnalyzerV2.ViewModels
{
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Collections.Generic;
    using RylPacketAnalyzerV2.Model.Packet.Parts;
    using System.Linq;

    class PacketEditViewModel : Screen
    {
        Guid packetId;
        byte packetCommand;
        string packetName;
        ObservableCollection<IPacketPart> packetContent;
        Infrastructure.IShell shell;
        IPacketPart selectedPacketPart;

        public Packet Packet { get; set; }
        public bool Saved { get; private set; }
        public IPacketPart SelectedPacketPart
        {
            get
            {
                return selectedPacketPart;
            }
            private set
            {
                selectedPacketPart = value;
                NotifyOfPropertyChange(() => SelectedPacketPart);
                NotifyOfPropertyChange(() => CanAddPacketPart);
                NotifyOfPropertyChange(() => CanEditPacketPart);
                NotifyOfPropertyChange(() => CanDeletePacketPart);
                NotifyOfPropertyChange(() => CanOrderUpSelectedPacketPart);
                NotifyOfPropertyChange(() => CanOrderDownSelectedPacketPart);
            }
        }
        public Guid PacketId
        {
            get { return packetId; }
            set { packetId = value; NotifyOfPropertyChange(() => PacketId); }
        }
        public byte PacketCommand
        {
            get { return packetCommand; }
            set { packetCommand = value; NotifyOfPropertyChange(() => PacketCommand); }
        }
        public string PacketName
        {
            get { return packetName; }
            set { packetName = value; NotifyOfPropertyChange(() => PacketName); }
        }
        public ObservableCollection<IPacketPart> PacketContent
        {
            get { return packetContent; }
            set { packetContent = value; NotifyOfPropertyChange(() => PacketContent); }
        }

        public PacketEditViewModel(Infrastructure.IShell shell, Packet packet)
        {
            this.shell = shell;
            Load(packet);
        }

        public void Create()
        {
            Load(new Packet());
        }

        private void Load(Packet packet)
        {
            PropertyChanged += PacketEditViewModel_PropertyChanged;

            Packet = packet;
            PacketId = Packet.Id;
            PacketCommand = Packet.Command;
            PacketName = Packet.Name;
            PacketContent = new ObservableCollection<IPacketPart>();
            if (Packet.Content != null)
            {
                var n = (IPacketPart)Activator.CreateInstance(Packet.Content.GetType());
                packetContent.Add(n);
                CopyPart(Packet.Content, n);
            }
            SetSelectedPacketContent(PacketContent.FirstOrDefault());
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
            {
                foreach (var f in dfrom.Content)
                {
                    var t = Activator.CreateInstance(f.GetType());
                    dto.Content.Add(t);
                    CopyPart(f, t);
                }

                FixOrder(dto.Content);
            }

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

        void PacketEditViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "PacketCommand" || e.PropertyName == "PacketName")
            {
                DisplayName = string.Format("Packet 0x{0:X2} - {1}", packetCommand, packetName);
            }
        }

        public void SetSelectedPacketContent(IPacketPart part)
        {
            SelectedPacketPart = part;
        }

        public void Save()
        {
            Packet.Id = PacketId;
            Packet.Command = PacketCommand;
            Packet.Name = PacketName;
            Packet.Content = PacketContent.FirstOrDefault();

            Saved = true;
            TryClose();
        }

        public void Cancel()
        {
            Saved = false;
            TryClose();
        }


        public bool CanEditPacketPart { get { return SelectedPacketPart != null; } }
        public IEnumerable<IResult> EditPacketPart()
        {
            yield return shell.ShowEditScreen(new PacketPartEditViewModel(SelectedPacketPart));
        }

        public bool CanAddPacketPart { get { return packetContent.Count == 0 || SelectedPacketPart is IContainerPart; } }
        public IEnumerable<IResult> AddPacketPart()
        {
            var sp = new SelectPartTypeViewModel();
            shell.ShowModalScreen(sp);
            if (!sp.Selected)
                yield break;
            else
            {
                IPacketPart newPart;
                newPart = (IPacketPart)Activator.CreateInstance(sp.SelectedType);

                PacketPartEditViewModel edit = new PacketPartEditViewModel(newPart);
                var res = shell.ShowEditScreen(edit);
                res.Completed += (sender, e) =>
                    {
                        if (!e.WasCancelled && e.Error == null)
                        {
                            if (edit.Saved)
                                if (PacketContent.Count == 0)
                                    PacketContent.Add(newPart);
                                else
                                {
                                    IContainerPart parent = (SelectedPacketPart as IContainerPart);
                                    edit.Part.Order = parent.Content.Count == 0 ? 0 : parent.Content.Max(x => x.Order) + 1;
                                    parent.Content.Add(edit.Part);
                                }
                        }
                    };

                yield return res;
            }
        }

        public bool CanDeletePacketPart { get { return SelectedPacketPart != null; } }
        public void DeletePacketPart()
        {
            if (SelectedPacketPart == PacketContent.FirstOrDefault())
                PacketContent.Clear();
            else
            {
                var parent = FindParentPart((PacketContent.First() as IContainerPart), SelectedPacketPart);
                parent.Content.Remove(SelectedPacketPart);
                parent.Content = new ObservableCollection<IPacketPart>(FixOrder(parent.Content));
            }
        }

        private IEnumerable<IPacketPart> FixOrder(IEnumerable<IPacketPart> parts)
        {
            int order = 0;
            var p = parts.OrderBy(x => x.Order);
            foreach (var part in p)
            {
                part.Order = order++;
            }
            return p;
        }

        private IContainerPart FindParentPart(IContainerPart source, IPacketPart finded)
        {
            if (source.Content.Contains(finded))
                return source;

            foreach (var ps in source.Content.Where(x => x is IContainerPart).Cast<IContainerPart>())
            {
                var ret = FindParentPart(ps, finded);
                if (ret != null) return ret;
            }

            return null;
        }

        public bool CanOrderUpSelectedPacketPart { get { return SelectedPacketPart != null && SelectedPacketPart.Order > 0; } }
        public void OrderUpSelectedPacketPart()
        {
            var parent = FindParentPart(PacketContent.First() as IContainerPart, SelectedPacketPart);
            var up = parent.Content.First(x => x.Order == SelectedPacketPart.Order - 1);

            up.Order++;
            SelectedPacketPart.Order--;

            parent.Content = new ObservableCollection<IPacketPart>(parent.Content.OrderBy(x => x.Order));
        }

        public bool CanOrderDownSelectedPacketPart
        {
            get
            {
                return SelectedPacketPart != null &&
                    SelectedPacketPart != PacketContent.First() &&
                    selectedPacketPart.Order <
                        FindParentPart(PacketContent.First() as IContainerPart, SelectedPacketPart).
                        Content.Max(x => x.Order);
            }
        }

        public void OrderDownSelectedPacketPart()
        {
            var parent = FindParentPart(PacketContent.First() as IContainerPart, SelectedPacketPart);
            var down = parent.Content.First(x => x.Order == SelectedPacketPart.Order + 1);

            down.Order--;
            SelectedPacketPart.Order++;

            parent.Content = new ObservableCollection<IPacketPart>(parent.Content.OrderBy(x => x.Order));
        }
    }
}
