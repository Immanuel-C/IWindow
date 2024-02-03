#pragma once

#include <string>
#include "IWindowCore.h"

namespace IWindow {
	typedef std::string Version;

	const Version VersionMajor = "1";
	const Version VersionMinor = "0";
	const Version VersionRevision = "0";
	const Version CurrentVersion = "1.0.0";
	const Version V100 = "1.0.0";

	void IWINDOW_API Initialize(const Version& version);
	const Version& IWINDOW_API GetVersion();
	void IWINDOW_API Shutdown();
}

#include "IWindowWindow.h"