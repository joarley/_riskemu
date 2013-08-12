// This is the main DLL file.

#include "stdafx.h"

#include "RylPacketAnalyzer.Lib.h"

namespace RylPacketAnalyzer {
	namespace Lib {
		void RylPacketAnalyzerLib::XorDecrypt(array<Byte> ^bytes)
		{
			pin_ptr<Byte> pb = &bytes[0];
			Byte *b = pb;
			Buffer_ptr buff(new ::Buffer((byte*)b, bytes->Length));
			CryptEngine::XorDecrypt(buff);
			*buff >> ::Buffer::FromPosition(::Buffer::Bytes((byte*)b, bytes->Length), 0);
		}
	}
}