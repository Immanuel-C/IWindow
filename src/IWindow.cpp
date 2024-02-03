#include "IWindow.h"

namespace IWindow {
	static Version ver = "";

	void Initialize(const Version& version) {
		ver = version;
	}

	const Version& GetVersion() { return ver; }

	// Shutdown does not do anything currently
	void Shutdown() { }
}
