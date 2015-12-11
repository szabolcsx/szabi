#ifndef SZABI_SERVER_H_INCLUDED
#define SZABI_SERVER_H_INCLUDED

#include <vector>
#include <memory>

#include <szabi\extensible\version.h>

namespace szabi
{
	namespace extensible
	{
		class manager;
		class iextension;

		class server_base
		{
			friend class manager;

		protected:
			virtual void attach(iextension*) = 0;
		};

		template<typename I>
		class server : public server_base
		{
		public:
			using interface_t = I;
			using interface_p = I*;
			 
			using impl_p = std::shared_ptr<I>;
			using container_t = std::vector<impl_p>;
			using iterator_t = typename container_t::iterator;
			using const_iterator_t = typename container_t::const_iterator;

			virtual void attached(interface_p) = 0;

			iterator_t begin()
			{
				return this->extensions.begin();
			}

			iterator_t end()
			{
				return this->extensions.end();
			}

			const_iterator_t begin() const
			{
				return this->extensions.begin();
			}

			const_iterator_t end() const
			{
				return this->extensions.end();
			}

		private:
			container_t extensions;

		protected:
			void attach(iextension* object)
			{
				I* implementation = static_cast<I*>(object);
				this->extensions.emplace_back(implementation);
				this->attached(implementation);
			}
		};
	}
}

#endif /* SZABI_SERVER_H_INCLUDED */