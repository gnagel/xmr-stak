#pragma once

#include <string>

#include <unistd.h>
#include <pwd.h>
#include <cstdlib>

namespace
{
	inline std::string get_home()
	{
		const char *home = ".";

		if ((home = getenv("HOME")) == nullptr)
			home = getpwuid(getuid())->pw_dir;

		return home;
	}
} // namespace anonymous
