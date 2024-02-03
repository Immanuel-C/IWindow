#pragma once

#include <string>
#include <functional>
#include "IWindowCore.h"

namespace IWindow {
	struct Error;
	typedef std::string Version;
	typedef std::function<bool(const Error&)> ErrorCallback;

	const Version VersionMajor = "1";
	const Version VersionMinor = "0";
	const Version VersionRevision = "0";
	const Version CurrentVersion = "1.0.0";
	const Version V100 = "1.0.0";

	void IWINDOW_API Initialize(const Version& version);
	ErrorCallback IWINDOW_API SetErrorCallback(ErrorCallback errorCallback);
	const ErrorCallback& IWINDOW_API GetErrorCallback();
	const Version& IWINDOW_API GetVersion();
	void IWINDOW_API Shutdown();
}

#include "IWindowWindow.h"