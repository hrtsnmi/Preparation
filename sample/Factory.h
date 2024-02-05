#pragma once
class Factory
{
public:
	template<typename type>
	static void* create();

	using pFunc = void* (*)();

	template<typename type = void>
	static type* Create(const std::string& sName);

	template<typename type>
	static bool Register(const std::string& sName);

protected:
	static std::map<std::string, pFunc>& get();
};

template<typename type>
inline void* Factory::create()
{
	return new type();
}

template<typename type>
inline type* Factory::Create(const std::string& sName)
{
	std::map<std::string, pFunc>::const_iterator itFind = get().find(sName);

	if (itFind == get().end())
	{
		return nullptr;
	}

	return (type*)itFind->second();
}

template<typename type>
inline bool Factory::Register(const std::string& sName)
{
	//std::map<std::string, pFunc>::const_iterator itFind = get().find(sName);
	std::map<std::string, pFunc>::const_iterator itFind = get().find(sName);
	if (itFind != get().end())
	{
		return false;
	}

	get()[sName] = &create<type>;

	return true;
}

#define STRINGS1(x) STRINGS (x)
#define STRINGS(x) #x

#define STRINGS(TYPE) \
namespace\
{\
	static bool registerName = Factory::Register<TYPE>(STRINGS1(TYPE));\
}