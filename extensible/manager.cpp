#include <szabi\extensible\manager.h>

#include <algorithm>

namespace szabi
{
	namespace extensible
	{
		manager::manager() {}
		manager::~manager() {}

		void manager::load_extension(const std::string& path)
		{
			auto pos = std::find_if(this->objects.begin(), this->objects.end(),
				[path](const szabi::shared_object& object) -> bool
			{
				return{ object.path() == path };
			});

			if (pos == this->objects.end())
			{
				this->objects.emplace_back(path);

				shared_object &object = this->objects.back();

				auto attach = object.template get_symbol<void(manager&)>("attach");

				if (attach)
				{
					attach(*this);
				}
			}
		}
	}
}