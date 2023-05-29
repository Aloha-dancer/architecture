#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Service
    {
        private:
            long id_;
            std::string caption_;
            std::string category_;
            std::string description_;
            long price_;

        public:

            static Service fromJSON(const std::string& data);

            const long& get_id() const;
            const std::string& get_caption() const;
            const std::string& get_category() const;
            const std::string& get_desctiption() const;
            const long& get_price() const;

            long& id();
            std::string& caption();
            std::string& category();
            std::string& description();
            long& price();

            static void init();
            static std::optional<Service> get_by_id(long id);
            static std::vector<Service> get_all();
            static std::optional<Service> get_by_caption(std::string data);
            void save_to_mysql();
            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif