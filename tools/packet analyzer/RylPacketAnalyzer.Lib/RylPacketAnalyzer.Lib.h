#pragma once

#include <crypt/CryptEngine.h>

using namespace System;

namespace RylPacketAnalyzer {
	namespace Lib {
		public ref class RylPacketAnalyzerLib
		{
		public:
			static void XorDecrypt(array<Byte> ^bytes);
		};
	}
}
