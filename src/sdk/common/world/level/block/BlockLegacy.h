#pragma once
#include "sdk/deps/Core/StringUtils.h"
#include "sdk/Util.h"

namespace SDK {
	class BlockLegacy {
	public:
		CLASS_FIELD(FNVString, translateName, 0x28); // tile.deepslate_diamond_ore
		CLASS_FIELD(std::string, name, 0x50); // deepslate_diamond_ore
		CLASS_FIELD(FNVString, Namespace, 0x78); // minecraft
		CLASS_FIELD(std::string, namespacedId, 0xA0); // minecraft:deepslate_diamond_ore
		CLASS_FIELD(std::string, itemGroup, 0x140); // itemGroup.name.ore
	private:
		virtual ~BlockLegacy() = 0;
	};
}