#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class User{
        private:
            long _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _login;
            std::string _password;

        public:

            static User fromJSON(const std::string & str);

            const long        &get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_email() const;
            const std::string &get_login() const;
            const std::string &get_password() const;

            void set_id(long );
            void  set_fn(std::string );
            void  set_ln(std::string );
            void  set_em(std::string );
            void  set_log(std::string );
            void  set_pwd(std::string );

            long &id();
            std::string &first_name();
            std::string &last_name();
            std::string &email();
            std::string &login();
            std::string &password();
            
            static void init();
            static std::optional<User> get_by_id(long );
            static std::optional<long> auth(std::string& , std::string &, long& );
            static std::vector<User> get_all();
            static std::vector<User> get_by_name_mask(std::string ,std::string );
            static std::optional<User> get_by_login(std::string& );
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif