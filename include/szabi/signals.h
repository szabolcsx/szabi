#ifndef SZABI_SIGNALS_H_INCLUDED
#define SZABI_SIGNALS_H_INCLUDED

#include <functional>
#include <vector>
#include <list>
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

	/*
	Forward declaration of signal used by signals::auto_disconnect
	*/
	template<typename... Args>
	class void_signal;

	template<typename... Args>
	class void_slot_impl : public slot_base
	{
		template <typename...>
		friend class void_signal;
	public:
		using slot_type = std::function<void(Args...)>;
		using iterator_t = typename std::list<std::shared_ptr<void_slot_impl<Args...>>>::iterator;
		using disconnector_type = std::function<void(iterator_t)>;

		void_slot_impl(slot_type&& slot, disconnector_type&& disconnector) :
			slot(std::move(slot)),
			disconnector(std::move(disconnector))
		{}

		/**
		Calling this method will disconnect the underlying slot
		*/
		void disconnect()
		{
			this->disconnector(this->it);
		}

		/**
		The signal was emited, calling the slot
		*/
		void emit(Args&&... args)
		{
			this->slot(std::forward<Args>(args)...);
		}

	private:
		slot_type slot;
		disconnector_type disconnector;
		iterator_t it;

		void set_iterator(iterator_t it)
		{
			this->it = it;
		}
	};

	namespace signals
	{
		class connection;

		/**
		This class is used to disconnect all slots when the object is destroyed.
		All classes which have slots must inherit from this
		*/
		class auto_disconnect
		{
			// A signal has access to add_connection function
			template<typename... Args> friend
			class szabi::void_signal;

			friend class member_connection;

		public:
			using iterator_t = std::list<connection>::iterator;
			/**
			When the class is destructed, all slots will be disconnected
			*/
			virtual ~auto_disconnect();

		protected:
			/**
			Adds a connection to the list
			*/
			iterator_t add_connection(const connection& conn);

			void remove_connection(iterator_t it);

		private:
			std::list<connection> connections;
			mutable std::mutex mutex;
		};

		class connection
		{
		public:

			connection(const std::weak_ptr<slot_base>& slot);

			virtual void disconnect() const;

			/**
			Returns true if the slot is connected
			*/
			bool connected() const;

		protected:
			std::weak_ptr<slot_base> slot;
		};

		// This class manages a connection between a signal and slot
		class member_connection : public connection
		{
		public:
			using iterator_t = std::list<connection>::iterator;

			member_connection(connection& conn, auto_disconnect* instance, iterator_t iterator);

			void disconnect() const override;

		private:
			auto_disconnect* instance;
			iterator_t iterator;
		};
	}

	template<typename... Args>
	class void_signal : public non_copyable
	{
	public:
		void_signal()
		{}

		virtual ~void_signal()
		{
			for (auto& conn : this->member_connections)
				conn.disconnect();
		}

		using slot_type = void_slot_impl<Args...>;
		using slot_container_type = std::list<std::shared_ptr<slot_type>>;
		using connection_container_t = std::vector<signals::member_connection>;
		/**
		Used for lambdas, global functions
		*/
		template<typename Slot>
		signals::connection connect(Slot&& slot)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			this->slots.push_back(
				std::shared_ptr<slot_type>{
				new slot_type{
					std::forward<Slot>(slot),
					[=](auto it)
				{
					// This lambda function is the disconnector which is called on sock_impl::disconnect()
					std::lock_guard<std::mutex> lock{ this->mutex };
					this->slots.erase(it);
				}}
			});

			auto it = this->slots.end();
			std::advance(it, -1);
			this->slots.back()->set_iterator(it);

			// Constructing a new connection from last slot
			return{ this->slots.back() };
		}

		/**
		Used for slots which are member functions
		*/
		template<typename Slot, typename T>
		signals::member_connection connect(Slot&& slot, T* instance)
		{
			static_assert(std::is_base_of<signals::auto_disconnect, T>::value, "T must inherit auto_disconnect");
			auto conn = this->connect(
				[=](Args&&... args)
			{
				// Using a lambda function to call a slot which is a member function
				(instance->*slot)(std::forward<Args>(args)...);
			});

			std::lock_guard<std::mutex> lock{ this->mutex };

			this->member_connections.emplace_back(conn, instance, instance->add_connection(conn));

			return{ this->member_connections.back() };
		}

		/**
		Used for slots which are member functions
		*/
		template<typename Slot, typename T>
		signals::member_connection connect(Slot&& slot, T& instance)
		{
			return this->connect(slot, std::addressof(instance));
		}

		/**
		Calling the slots
		*/
		void emit(Args&&... args)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			for (auto const &slot : this->slots)
			{
				if (slot)
				{
					slot->emit(std::forward<Args>(args)...);
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
		connection_container_t member_connections;
		mutable std::mutex mutex;
	};
}

#endif /* SZABI_SIGNALS_H_INCLUDED */