namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet;
    using RylPacketAnalyzerV2.Model.Packet.Parts;

    class PacketEditViewModel : Screen
    {
        public Packet Packet;

        public bool Saved { get; private set; }

        public byte Command { get; set; }
        public string Name { get; set; }
        public ObservableCollection<PacketPart> Content { get; set; }

        public PacketEditViewModel(Packet packet)
        {
            Packet = packet;
            Command = packet.Command;
            Name = packet.Name;
            Content = new ObservableCollection<PacketPart>();
            foreach (var psrc in packet.Content)
            {
                PacketPart pdest = (PacketPart)Activator.CreateInstance(psrc.GetType());
                psrc.CopyTo(ref pdest);
                Content.Add(psrc);
            }
        }

        public void Save()
        {
            Packet.Command = Command;
            Packet.Name = Name;
            Packet.Content = Content;
            Saved = true;
            TryClose();
        }

        public void Cancel()
        {

            Saved = false;
            TryClose();
        }

        public override void CanClose(System.Action<bool> callback)
        {
            callback(true);
        }
    }
}
