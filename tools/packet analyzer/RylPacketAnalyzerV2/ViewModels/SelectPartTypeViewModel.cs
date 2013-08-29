namespace RylPacketAnalyzerV2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using Caliburn.Micro;
    using RylPacketAnalyzerV2.Model.Packet.Parts;

    class SelectPartTypeViewModel : Screen
    {
        public Type SelectedType { get; set; }
        public bool Selected { get; private set; }
        public Dictionary<string, Type> Types { get; private set; }

        public SelectPartTypeViewModel()
        {
            Types = new Dictionary<string, Type>();
            Types["Float"] = typeof(FloatPart);
            Types["For"] = typeof(ForPart);
            Types["Integer"] = typeof(IntegerPart);
            Types["String"] = typeof(StringPart);
            Types["Struct"] = typeof(StructPart);
        }

        public void Ok()
        {
            Selected = SelectedType != null;
            TryClose();
        }

        public void Cancel()
        {
            SelectedType = null;
            TryClose();
        }        
    }
}
