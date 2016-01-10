#include <szabi\config_file.h>

#include <fstream>
#include <regex>
#include <cctype>
#include <stdexcept>

namespace szabi
{
	config_file::config_file() {}

	config_file::config_file(const std::string& file_path)
	{
		this->open(file_path);
	}

	config_file::~config_file() {}

	void config_file::open(const std::string& file_path)
	{
		std::ifstream in(file_path);
		std::string line;
		std::size_t line_no = 0;
		std::regex pattern(".+\\s+.+\\s+[[:print:]]+");

		while (std::getline(in, line))
		{
			line_no++;

			if (line.empty())
				continue;

			if (std::regex_match(line, pattern))
			{
				std::vector<std::string> token;
				bool inside_quotes = false;
				std::string buffer;
				for (std::size_t i = 0; i < line.size(); i++)
				{
					char ch = line[i];

					if (!std::isspace(ch))
					{
						buffer += ch;
					}
					else if (ch == '"')
					{
						inside_quotes = !inside_quotes;
					}
					else if (std::isspace(ch) && !inside_quotes)
					{
						token.push_back(buffer);
						buffer.clear();
					}
				}

				token.push_back(buffer);

				if (token.size() > 3)
					throw std::runtime_error(file_path + ":" + std::to_string(line_no) + " Too many arguments");
				if (inside_quotes)
					throw std::runtime_error(file_path + ":" + std::to_string(line_no) + " Open quotes");

				this->keys.push_back({ token[0], token[1], token[2] });
			}
			else
			{
				throw std::runtime_error(file_path + ":" + std::to_string(line_no) + " Syntax error");
			}
		}
	}

	config_file::group_container_t config_file::lookup(const std::string& group) const
	{
		config_file::group_container_t result;

		for (const auto& p : this->keys)
			if (p.group == group)
				result.push_back(p);

		return{ result };
	}

	config_file::key_t config_file::lookup(const std::string& group, const std::string& name) const
	{
		config_file::key_t result;

		for (const auto& p : this->keys)
		{
			if ((p.group == group) && (p.name == name))
			{
				result.group = p.group;
				result.name = p.name;
				result.value = p.value;
			}
		}

		return{ result };
	}
}