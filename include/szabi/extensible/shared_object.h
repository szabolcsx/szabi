#ifndef SZABI_SHARED_OBJECT_H_INCLUDED
#define SZABI_SHARED_OBJECT_H_INCLUDED

#include <string>
#include <memory>

namespace szabi
{
	class shared_object
	{
	public:
		shared_object(const std::string&);

		virtual ~shared_object();

		template<typename S>
		S* get_symbol(const std::string& name)
		{
			return reinterpret_cast<S*>(this->get_raw_symbol(name));
		}

		std::string path() const;

	private:
		void* get_raw_symbol(const std::string&);

		std::shared_ptr<void> object;
		std::string object_path;
	};
}

#endif /* SZABI_SHARED_OBJECT_H_INCLUDED */