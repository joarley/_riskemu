using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RylPacketAnalyzer
{
    class PacketCaptured
    {
        public int Command { get; private set; }
        public int Length { get; private set; }
        public Destination Destination { get; private set; }
        public byte[] OriginalBytes { get; private set; }
        public byte[] ParsedBytes { get; private set; }

        public PacketCaptured(byte[] bytes, Destination destination)
        {
            this.Destination = destination;
            this.OriginalBytes = new byte[bytes.Length];
            bytes.CopyTo(this.OriginalBytes, 0);
            Lib.RylPacketAnalyzerLib.XorDecrypt(bytes);
            this.ParsedBytes = bytes;

            
        }        
    }
}
