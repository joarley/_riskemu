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
                packetContent.Add(Packet.Content);
            SetSelectedPacketContent(PacketContent.FirstOrDefault());
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
                                    (SelectedPacketPart as IContainerPart).Content.Add(edit.Part);
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
                FindParentPart((PacketContent.First() as IContainerPart), SelectedPacketPart).
                    Content.Remove(SelectedPacketPart);
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
    }
}
