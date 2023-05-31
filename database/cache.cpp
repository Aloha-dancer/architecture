
#include "cache.h"
#include "../config/config.h"

#include <exception>
#include <mutex>

#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>
#include <redis-cpp/value.h>
#include <redis-cpp/detail/stream.hpp>
#include <redis-cpp/resp/serialization.h>
#include <redis-cpp/detail/config.h>
#include <redis-cpp/resp/deserialization.h>

namespace database
{
    std::mutex mutex;
    Cache::Cache()
    {
        std::string host = Config::get().get_cache_server().substr(0,
                                                                    Config::get().get_cache_server().find(':'));
        std::string port = Config::get().get_cache_server().substr(Config::get().get_cache_server().find(':') + 1);

        std::cout << "cache host:" << host <<" port:" << port << std::endl;
        stream = rediscpp::make_stream(host, port);
    }

    Cache Cache::get_instance()
    {
        static Cache instance;
        return instance;
    }

    void Cache::put([[maybe_unused]] long id, [[maybe_unused]] const std::string &value)
    {
        std::lock_guard<std::mutex> lck(mutex);
        rediscpp::value response = rediscpp::execute(
            *stream, "set", std::to_string(id), value, "ex", "60"
        );
    }

    bool Cache::get([[maybe_unused]] long id, [[maybe_unused]] std::string &value)
    {
        std::lock_guard<std::mutex> lck(mutex);
        rediscpp::value response = rediscpp::execute(*stream, "get", std::to_string(id));

        if (response.is_error_message() || response.empty()) return false;
        std::string str = response.as_string().data();
        std::cout << "Redis response: " + str << std::endl;

        value = response.as<std::string>();
        return true;
    }
}