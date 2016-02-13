#include <szabi\signals.h>

namespace szabi
{
	namespace signals
	{
		connection::connection(const std::weak_ptr<slot_base>& slot) :
			slot{ slot }
		{}

		void connection::disconnect() const
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
		bool connection::connected() const
		{
			/*
			If the weak pointer is not expired then the slot is still connected
			*/
			return !this->slot.expired();
		}

		member_connection::member_connection(connection& conn, auto_disconnect* instance, iterator_t iterator) :
			connection{ conn },
			instance{ instance },
			iterator{ iterator }
		{}

		void member_connection::disconnect() const
		{
			if (auto temp = this->slot.lock())
			{
				temp->disconnect();
				this->instance->remove_connection(this->iterator);
			}
		}


		auto_disconnect::~auto_disconnect()
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			for (const auto& conn : this->connections)
			{
				conn.disconnect();
			}
		}

		auto_disconnect::iterator_t auto_disconnect::add_connection(const connection& conn)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			this->connections.push_back(conn);
			iterator_t it = this->connections.end();
			std::advance(it, -1);
			return it;
		}

		void auto_disconnect::remove_connection(iterator_t it)
		{
			this->connections.erase(it);
		}
	}
}