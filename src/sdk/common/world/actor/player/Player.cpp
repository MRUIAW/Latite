#include "Player.h"
#include "util/Util.h"
#include "api/memory/memory.h"
#include "sdk/Version.h"
#include "sdk/signature/storage.h"

void SDK::Player::displayClientMessage(std::string const& message) {
	int index = 0;
	switch (SDK::internalVers) {
	case V1_18_12:
		index = 0x184;
		break;
	case V1_19_41:
		index = 0x18C;
		break;
	case V1_19_51:
		index = 0x18A;
		break;
	default:
		index = 0x165;
		break;
	}
	memory::callVirtual<void>(this, index, message);
}

SDK::MoveInputComponent* SDK::Player::getMoveInputComponent() {
	// tryGetMoveInputHandler
	if (SDK::internalVers == V1_19_51) {
		return memory::callVirtual<SDK::MoveInputComponent*>(this, 0x1B7);
	}

	if (SDK::internalVers == V1_19_41) {
		return memory::callVirtual<SDK::MoveInputComponent*>(this, 0x1B9);
	}

	if (SDK::internalVers == V1_18_12) {
		return memory::callVirtual<SDK::MoveInputComponent*>(this, 0x1B1);
	}

	using try_get_t = SDK::MoveInputComponent* (__fastcall*)(uintptr_t a1, uint32_t* a2);
	static auto func = reinterpret_cast<try_get_t>(Signatures::Components::moveInputComponent.result);
	return func(entityContext.registry->basicRegistry, &entityContext.id);
}