#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include "Poco/Data/Date.h"
#include <optional>

namespace database{
    class Order
    {
        private:
            long id_user_;
            long id_;
            long id_service_;
            Poco::Data::Date order_date_;
            std::string status_;
        
        public:

            const long& get_id() const;
            const long& get_usr_id() const;
            const long& get_serv_id() const;
            const Poco::Data::Date& get_ord_date() const;
            const std::string& get_status() const;
            long& id();
            long& id_user();
            long& id_service();
            Poco::Data::Date& order_date();
            std::string& status();

            static Order fromJSON(const std::string& data);
            Poco::JSON::Object::Ptr toJSON() const;

            static void init();
            void save_to_mysql();
            static std::vector<Order> get_all();
            static bool check_service_id(long id_service,
                                         long id,
                                         long id_user);
            static std::vector<Order> get_by_order(long id);
            static std::vector<Order> get_by_user(long id_user);
    };
}

#endif
