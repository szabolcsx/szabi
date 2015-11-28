#include <szabi\extensible\shared_object.h>

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>

namespace szabi
{
	shared_object::shared_object(const std::string& path) :
		object((void*)LoadLibraryA(path.c_str()),
			[](void* object)
	{
		if (object)
		{
			FreeLibrary((HMODULE)object);
		}
	})
	{
		if (!this->object)
		{
			std::cout << GetLastError();
			throw std::runtime_error("Unable to open library.");
		}

		this->object_path = path;
	}

	shared_object::~shared_object() {}

	std::string shared_object::path() const
	{
		return this->object_path;
	}

	void* shared_object::get_raw_symbol(const std::string& name)
	{
		void* address = nullptr;

		if (!this->object)
		{
			throw std::runtime_error("Object not loaded.");
		}

		if (name.empty())
		{
			throw std::runtime_error("Symbol name can't be empty.");
		}

		address = (void*)GetProcAddress((HMODULE)this->object.get(), name.c_str());

		if (address == nullptr)
		{
			throw std::runtime_error("Unable to get symbol.");
		}

		return address;
	}
}
