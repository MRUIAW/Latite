#pragma once
#include "sdk/Util.h"
#include "sdk/Version.h"
#include "api/memory/memory.h"
#include "RakPeer.h"

#include <string>

namespace SDK {
	class RakNetConnector {
		inline static RakNetConnector* instance = nullptr;
	public:
		static void setInstance(RakNetConnector* inst) {
			instance = inst;
		}

		// please check for null
		static RakNetConnector* get() {
			return instance;
		}

		RakNetConnector() = delete;

		MVCLASS_FIELD(std::string, ipAddress, 0x428, 0x3D0, 0x438);
		MVCLASS_FIELD(std::string, dns, 0x448, 0x3F0, 0x458);
		MVCLASS_FIELD(uint16_t, port, 0x468, 0x410, 0x478);
		MVCLASS_FIELD(std::string, featuredServer, 0x4F8, 0x4C8, 0x530);

		class RakPeer* getPeer() {
			if (internalVers > V1_18_12) {
				return memory::callVirtual<RakPeer*>(this, MV_DETAIL_GETOFFSET(0x19, 0, 0x17));
			}
			return util::directAccess<RakPeer*>(this, 0x230);
		}
	};
}
