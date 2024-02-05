#include "pch.h"
#include "Factory.h"

std::map<std::string, Factory::pFunc>& Factory::get()
{
	static std::map<std::string, Factory::pFunc> _kTypes;

	return _kTypes;
}
