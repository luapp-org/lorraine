#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>

namespace lorraine::utils
{
    class timer final
    {
       public:
        void start()
        {
            start_time = std::chrono::steady_clock::now();
            is_running = true;
        }

        void stop()
        {
            end_time = std::chrono::steady_clock::now();
            is_running = false;
        }

        template<typename T>
        double elapsed()
        {
            std::chrono::time_point< std::chrono::steady_clock > endTime;

            if ( is_running )
            {
                endTime = std::chrono::steady_clock::now();
            }
            else
            {
                endTime = end_time;
            }

            return std::chrono::duration_cast< T >( endTime - start_time )
                .count();
        }

       private:
        std::chrono::time_point< std::chrono::steady_clock > start_time, end_time;
        bool is_running = false;
    };
}  // namespace lorraine::utils