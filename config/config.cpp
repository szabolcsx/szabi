#include <szabi\config.h>

namespace szabi
{
	config::config(config::stream_t& stream) :
		stream{ stream },
		last_scope{ scope::global },
		last_token_type{ token_type::none },
		got_last_value{ true },
		last_group { "global" }
	{

	}

	void config::parse()
	{
		std::string line;
		std::string token;
		std::size_t line_number = 0;
		std::size_t i;

		bool inside_brackets = false;

		while (std::getline(this->stream, line))
		{
			line_number++;
			line += " ";
			bool inside_quotes = false;
			for (i = 0; i < line.length(); i++)
			{
				switch (line[i])
				{
				case to_underlying(delimiter::group_begin):
					if (inside_brackets)
					{
						throw std::domain_error(this->format_error(line_number, i, "Unexpected {"));
					}

					this->process_token(token, line_number, i);
					token.clear();
					this->process_token({ '{' }, line_number, i, false);
					inside_brackets = true;
					break;

				case to_underlying(delimiter::group_end):
					if (!inside_brackets)
					{
						throw std::domain_error(this->format_error(line_number, i, "Unexpected }"));
					}

					this->process_token(token, line_number, i);
					token.clear();
					this->process_token({ '}' }, line_number, i, false);
					inside_brackets = false;
					break;

				case to_underlying(delimiter::equals):
					this->process_token(token, line_number, i);
					token.clear();
					this->process_token({ '=' }, line_number, i, false);
					break;

				case to_underlying(delimiter::quote):

					inside_quotes = !inside_quotes;
					break;

				case ' ':
				case '\t':
				case '\r':
					if (inside_quotes)
					{
						token += line[i];
					}
					else
					{
						this->process_token(token, line_number, i);
						token.clear();
					}
					break;

				case '\n':
					this->process_token(token, line_number, i);
					token.clear();
					break;

				default:
					token += line[i];
				}
			}

			if (inside_quotes)
			{
				throw std::domain_error(this->format_error(line_number, i, "Unclosed quotes, expected \""));
			}
		}

		if (inside_brackets)
		{
			throw std::domain_error(this->format_error(line_number, i, "Unclosed group, expected }"));
		}

		this->process_token(token, line_number, i);
	}

	config::value_t config::lookup_value(const std::string& group, const std::string& key, value_t default_value)
	{
		value_t value = this->groups[group][key];

		if (!value.empty())
		{
			return{ value };
		}

		return default_value;
	}

	void config::process_token(const token_t& token, std::size_t line_number, std::size_t pos, bool check_empty)
	{
		if (check_empty && token.empty())
		{
			return;
		}

		switch (this->last_scope)
		{
		case scope::global:
			if (token == delimiter::group_begin)
			{
				if (this->last_token_type != token_type::string)
				{
					throw std::domain_error(this->format_error(line_number, pos, "Group name expected"));
				}

				this->last_group = this->last_token;
				this->last_scope = scope::inside_group;
				this->last_token_type = token_type::group_begin;
			}
			break;

		case scope::inside_group:
			if (token == delimiter::group_end)
			{
				this->last_group = "global";
				this->last_scope = scope::global;
				this->last_token_type = token_type::group_end;
			}
			break;
		}

		switch (this->is_delimiter(token))
		{
		case delimiter::no:
			if (this->last_token_type != token_type::equals)
			{
				this->last_key = token;
				this->got_last_value = false;
			}
			else if (this->last_token_type == token_type::equals)
			{
				this->groups[this->last_group][this->last_key] = token;
				this->got_last_value = true;
			}

			this->last_token_type = token_type::string;
			break;

		case delimiter::equals:
			if (this->got_last_value &&
				this->last_token_type == token_type::string)
			{
				throw std::domain_error(this->format_error(line_number, pos, "Syntax error"));
			}
			this->last_token_type = token_type::equals;
			break;
		}

		this->last_token = token;
	}

	std::string config::format_error(std::size_t line_number, std::size_t pos, const std::string& message)
	{
		std::stringstream what;

		what << "Syntax error <L";
		what << line_number;
		what << ":C";
		what << pos + 1;
		what << ">: ";
		what << message;

		return{ what.str() };
	}

	config::delimiter config::is_delimiter(const token_t& token)
	{
		if (token == delimiter::group_begin)
		{
			return{ delimiter::group_begin };
		}
		else if (token == delimiter::group_end)
		{
			return{ delimiter::group_end };
		}
		else if (token == delimiter::equals)
		{
			return{ delimiter::equals };
		}
		else if (token == delimiter::quote)
		{
			return{ delimiter::quote };
		}

		return{ delimiter::no };
	}
}