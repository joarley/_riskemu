namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Linq;
    using Caliburn.Micro;

    class PacketEmitterShellViewModel : Screen
    {
        object selectedItem;

        public ObservableCollection<Model.NetworkComunication.PacketEmitter> PacketEmitters { get; set; }
        public bool CanAddPacket { get { return SelectedItem is Model.NetworkComunication.PacketEmitter; } }
        public bool CanEditPacket { get { return SelectedItem is Model.Packet.Packet; } }
        public bool CanDeletePacket { get { return SelectedItem is Model.Packet.Packet; } }
        public bool CanAddPacketEmmiter { get { return true; } }
        public bool CanEditPacketEmmiter { get { return SelectedItem is Model.NetworkComunication.PacketEmitter; } }
        public bool CanDeletePacketEmmiter { get { return SelectedItem is Model.NetworkComunication.PacketEmitter; } }
        public IConductor ConductorParent;

        public object SelectedItem
        {
            get { return selectedItem; }
            set
            {
                selectedItem = value;
                NotifyOfPropertyChange(() => SelectedItem);
                NotifyOfPropertyChange(() => CanAddPacket);
                NotifyOfPropertyChange(() => CanEditPacket);
                NotifyOfPropertyChange(() => CanDeletePacket);
                NotifyOfPropertyChange(() => CanAddPacketEmmiter);
                NotifyOfPropertyChange(() => CanEditPacketEmmiter);
                NotifyOfPropertyChange(() => CanDeletePacketEmmiter);
            }
        }
        

        public PacketEmitterShellViewModel(ObservableCollection<Model.NetworkComunication.PacketEmitter> packetEmitters, IConductor parent)
        {
            PacketEmitters = packetEmitters;
            ConductorParent = parent;
        }

        public void SetSelectedItem(object item)
        {
            if (item == null ||
                (item is Model.NetworkComunication.PacketEmitter &&
                PacketEmitters.Contains(item as Model.NetworkComunication.PacketEmitter)) ||
                (item is Model.Packet.Packet && 
                    PacketEmitters.Count(x => x.EmitPackets.Contains(item as Model.Packet.Packet)) > 0))
            {
                SelectedItem = item;                
                return;
            }

            throw new Exception();
        }

        public void AddPacket()
        {
            var currrentEmitter = SelectedItem is Model.NetworkComunication.PacketEmitter ?
                SelectedItem as Model.NetworkComunication.PacketEmitter :
                PacketEmitters.First(x => x.EmitPackets.Contains(SelectedItem as Model.Packet.Packet));

            Model.Packet.Packet newPacket = new Model.Packet.Packet();

            var edit = new PacketEditViewModel(newPacket);
        }

        public void EditPacket()
        {
            var edit = new PacketEditViewModel(selectedItem as Model.Packet.Packet);
            ConductorParent.ActivateItem(new Infrastructure.ScreenWithCloseVisibilityViewModel(edit));
        }

        public void DeletePacket()
        {
            PacketEmitters.First(x => x.EmitPackets.Contains(SelectedItem as Model.Packet.Packet)).
                EmitPackets.Remove(SelectedItem as Model.Packet.Packet);
        }

        public void AddPacketEmmiter()
        {

        }

        public void EditPacketEmmiter()
        {

        }

        public void DeletePacketEmmiter()
        {
            
        }
    }
}
