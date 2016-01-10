#ifndef SZABI_CONFIG_H_INCLUDED
#define SZABI_CONFIG_H_INCLUDED

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <exception>
#include <sstream>
#include <map>

namespace szabi
{
	template <typename E>
	constexpr auto to_underlying(E e) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(e);
	}

	class config
	{
		enum class scope
		{
			global,
			inside_group,
		};

		enum class token_type
		{
			none,
			string,
			group_begin,
			group_end,
			equals
		};

		enum class delimiter : char
		{
			no,
			group_begin = '{',
			group_end = '}',
			equals = '=',
			quote = '\"'
		};

		friend bool operator == (const std::string& lhs, const delimiter& rhs);
		friend bool operator != (const std::string& lhs, const delimiter& rhs);

	public:
		using stream_t = std::fstream;
		using token_t = std::string;
		using value_t = std::string;

		config(stream_t& stream);

		void parse();

		value_t lookup_value(const std::string& group, const std::string& key, value_t default_value = "");

	private:
		std::map<std::string, std::map<std::string, value_t>> groups;

		stream_t& stream;

		scope last_scope;
		token_type last_token_type;
		std::string last_token;
		bool got_last_value;

		std::string last_group;
		std::string last_key;

		void process_token(const token_t& token, std::size_t line_number, std::size_t pos, bool check_empty = true);
		std::string format_error(std::size_t line_number, std::size_t pos, const std::string& message);
		delimiter is_delimiter(const token_t& token);
	};

	inline bool operator == (const std::string& lhs, const config::delimiter& rhs)
	{
		std::string delim;
		delim += static_cast<char>(rhs);

		return{ lhs == delim };
	}

	inline bool operator != (const std::string& lhs, const config::delimiter& rhs)
	{
		return{ !(lhs == rhs) };
	}
}

#endif /* SZABI_CONFIG_H_INCLUDED */