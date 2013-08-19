using System;
namespace RylPacketAnalyzerV2.Model.NetworkComunication
{
    public class SourceAddress
    {
        public Guid Id { get; set; }
        public int Port { get; set; }
        public SourceAddressType Type { get; set; }

        public SourceAddress() : this(new Guid()) { }

        public SourceAddress(Guid id)
        {
            this.Id = id;
        }

        public static bool operator ==(SourceAddress a, SourceAddress b)
        {
            if (a == null && b == null) return true;
            if (a == null || b == null) return false;
            return a.Id == b.Id;
        }


        public static bool operator !=(SourceAddress a, SourceAddress b)
        {
            return !(a == b);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj);
        }
    }
}