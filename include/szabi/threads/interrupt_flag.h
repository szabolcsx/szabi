#ifndef SZABI_THREADS_INTERRUPT_FLAG_H_
#define SZABI_THREADS_INTERRUPT_FLAG_H_

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace szabi
{
	namespace threads
	{
		class interrupt_flag
		{
		public:
			interrupt_flag() :
				flag_set{ false }
			{}

			void set()
			{
				std::unique_lock<std::mutex> lock(this->mutex);
				this->flag_set = true;
				this->cond.notify_all();
			}

			void reset()
			{
				std::unique_lock<std::mutex> lock(this->mutex);
				this->flag_set = false;
			}

			bool check()
			{
				bool flag_set;
				{
					std::unique_lock<std::mutex> lock(this->mutex);
					flag_set = this->flag_set;
				}

				return !flag_set;
			}

			template<typename T>
			bool check(const T& duration)
			{
				std::unique_lock<std::mutex> lock(this->mutex);
				if (this->flag_set ||
					this->cond.wait_for(lock, duration) == std::cv_status::no_timeout)
				{
					return false;
				}

				return true;
			}

		private:
			bool flag_set;
			std::mutex mutex;
			std::condition_variable cond;
		};
	}
}

#endif /* SZABI_THREADS_INTERRUPT_FLAG_H_ */