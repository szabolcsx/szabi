#ifndef SZABI_CONFIG_FILE_H_INCLUDED
#define SZABI_CONFIG_FILE_H_INCLUDED

#include <string>
#include <tuple>
#include <vector>

namespace szabi
{
	class config_file
	{
		struct key
		{
			std::string group;
			std::string name;
			std::string value;

			bool operator == (const std::string& rhs) const
			{
				return{ this->name == rhs };
			}

			bool operator != (const std::string& rhs) const
			{
				return{ !(*this == rhs) };
			}

			bool operator == (const key& rhs) const
			{
				return{ std::tie(this->name, this->value) == std::tie(rhs.name, rhs.value) };
			}

			bool operator != (const key& rhs) const
			{
				return{ !(*this == rhs) };
			}

			operator std::string() const
			{
				return{ this->value };
			}
		};

	public:
		using key_t = key;
		using group_container_t = std::vector<key_t>;

		config_file();
		config_file(const std::string& file_path);
		virtual ~config_file();

		void open(const std::string& file_path);

		group_container_t lookup(const std::string& group) const;
		key_t lookup(const std::string& group, const std::string& name) const;

	private:
		group_container_t keys;
	};
}

#endif /* SZABI_CONFIG_FILE_H_INCLUDED */