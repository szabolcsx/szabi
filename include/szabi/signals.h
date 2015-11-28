#ifndef SZABI_SIGNALS_H_INCLUDED
#define SZABI_SIGNALS_H_INCLUDED

#include <functional>
#include <vector>
#include <mutex>
#include <memory>

#include <szabi\non_copyable.h>

namespace szabi
{
	/**
	Helper struct used when dealing with overloaded slots
	*/
	template<typename... Args>
	struct overload
	{
		/**
		Returns the selected overload

		Example: overload<int>::of(&foo::bar)
		*/
		template<typename F, typename R>
		static constexpr auto of(R(F::*ptr)(Args...)) -> decltype(ptr)
		{
			return ptr;
		}

		template<typename F, typename R>
		static constexpr auto of(R(F::*ptr)(const Args&...)) -> decltype(ptr)
		{
			return ptr;
		}

		template<typename R>
		static constexpr auto of(R(*ptr)(Args...)) -> decltype(ptr)
		{
			return ptr;
		}

		template<typename R>
		static constexpr auto of(R(*ptr)(const Args&...)) -> decltype(ptr)
		{
			return ptr;
		}
	};

	/**
	Base class for slot_impl, also used to store slots in connection class
	*/
	class slot_base
	{
	public:
		/*
		This is required for connection class which only has to disconnect slots
		*/
		virtual void disconnect() = 0;
	};

	template<typename... Args>
	class slot_impl : public slot_base
	{
	public:
		using slot_type = std::function<void(Args...)>;
		using disconnector_type = std::function<void()>;

		slot_impl(slot_type&& slot, disconnector_type&& disconnector) :
			slot(std::move(slot)),
			disconnector(std::move(disconnector)) { }

		/**
		Calling this method will disconnect the underlying slot
		*/
		void disconnect()
		{
			this->disconnector();
		}

		/**
		The signal was emited, calling the slot
		*/
		void emit(Args... args)
		{
			this->slot(args...);
		}

	private:
		slot_type slot;
		disconnector_type disconnector;
	};

	/*
	Forward declaration of signal used by signals::auto_disconnect
	*/
	template<typename... Args>
	class signal;

	namespace signals
	{
		// This class manages a connection between a signal and slot
		class connection
		{
		public:
			connection(const std::weak_ptr<slot_base>& slot) : slot(slot) {}

			~connection() {}

			void disconnect() const
			{
				/*
				Since the slot is a weak pointer to the main slot_impl object, when it's deleted this->slot.lock() will return false
				*/
				if (auto temp = this->slot.lock())
				{
					temp->disconnect();
				}
			}

			/**
			Returns true if the slot is connected
			*/
			bool connected() const
			{
				/*
				If the weak pointer is not expired then the slot is still connected
				*/
				return !this->slot.expired();
			}

		private:
			std::weak_ptr<slot_base> slot;
		};

		/**
		This class is used to disconnect all slots when the object is destroyed.
		All classes which have slots must inherit from this
		*/
		class auto_disconnect
		{
			// A signal has access to add_connection function
			template<typename... Args> friend
			class szabi::signal;

		public:
			/**
			When the class is destructed, all slots will be disconnected
			*/
			virtual ~auto_disconnect()
			{
				std::lock_guard<std::mutex> lock(this->mutex);
				for (const auto& conn : this->connections)
				{
					conn.disconnect();
				}
			}

		protected:
			/**
			Adds a connection to the list
			*/
			void add_connection(const connection& conn)
			{
				std::lock_guard<std::mutex> lock(this->mutex);
				this->connections.push_back(conn);
			}

		private:
			std::vector<connection> connections;
			mutable std::mutex mutex;
		};
	}

	template<typename... Args>
	class signal : public non_copyable
	{
	public:
		signal() {}

		using slot_type = slot_impl<Args...>;
		using slot_container_type = std::vector<std::shared_ptr<slot_type>>;
		using slot_iterator_type = typename slot_container_type::iterator;

		/**
		Used for lambdas, global functions
		*/
		template<typename S>
		signals::connection connect(S&& slot)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			this->slots.push_back(
				std::shared_ptr<slot_type>(
					new slot_type(std::forward<S>(slot), [&]()
			{
				// This lambda function is the disconnector which is called on sock_impl::disconnect()
				std::lock_guard<std::mutex> lock(this->mutex);
				slot_iterator_type it = std::prev(this->slots.end());
				this->slots.erase(it);
			})));

			// Constructing a new connection from last slot
			return{ this->slots.back() };
		}

		/**
		Used for slots which are member functions
		*/
		template<typename S, typename T>
		signals::connection connect(S&& slot, T* instance)
		{
			static_assert(std::is_base_of<signals::auto_disconnect, T>::value, "T must inherit auto_disconnect");
			auto conn = this->connect(
				[&](Args... args)
			{
				// Using a lambda function to call a slot which is a member function
				(instance->*slot)(std::forward<Args>(args)...);
			});

			instance->add_connection(conn);

			return conn;
		}

		/**
		Used for slots which are member functions
		*/
		template<typename S, typename T>
		signals::connection connect(S&& slot, T& instance)
		{
			return this->connect(slot, std::addressof(instance));
		}

		/**
		Calling the slots
		*/
		void emit(Args... args)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			for (auto const &slot : this->slots)
			{
				if (slot)
				{
					slot->emit(args...);
				}
			}
		}


		/**
		Return the number of connected slots
		*/
		std::size_t slot_count() const
		{
			return this->slots.size();
		}

	private:
		slot_container_type slots;
		mutable std::mutex mutex;
	};
}

#endif /* SZABI_SIGNALS_H_INCLUDED */