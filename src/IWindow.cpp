#include "IWindow.h"

namespace IWindow {
	static Version ver = "";

	bool DefaultErrorCallback(const Error&) { return false; }

	static ErrorCallback errCallback = DefaultErrorCallback;

	void Initialize(const Version& version) {
		ver = version;
	}

	ErrorCallback SetErrorCallback(ErrorCallback errorCallback) {
		ErrorCallback oldCallback = errorCallback;
		errCallback = errorCallback;
		return oldCallback;
	}

	const ErrorCallback& GetErrorCallback() { return errCallback; }

	const Version& GetVersion() { return ver; }

	// Shutdown does not do anything currently
	void Shutdown() { }
}
