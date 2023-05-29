#include "user.h"
#include "../../database/database.h"
#include "../../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void User::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::JSON::Object::NameList hints = database::Database::get_all_hints();
            for(Poco::JSON::Object::NameList::iterator hint = hints.begin();
                hint != hints.end();
                ++hint)
            {

            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `user` (`id` BIGINT AUTO_INCREMENT,"
                        << "`first_name` VARCHAR(256) NOT NULL,"
                        << "`last_name` VARCHAR(256) NOT NULL,"
                        << "`login` VARCHAR(256) NOT NULL,"
                        << "`password` VARCHAR(256) NOT NULL,"
                        << "`email` VARCHAR(256) NULL,"
                        << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`)"
                        << ",KEY `KI_login` (`login`));"
                        << *hint,   
                        now;
            }
        }

        catch (const Poco::Data::MySQL::ConnectionException& e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (const Poco::Data::MySQL::StatementException& e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    User User::fromJSON(const std::string &str)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.id() = object->getValue<long>("id");
        user.first_name() = object->getValue<std::string>("first_name");
        user.last_name() = object->getValue<std::string>("last_name");
        user.email() = object->getValue<std::string>("email");
        user.login() = object->getValue<std::string>("login");
        user.password() = object->getValue<std::string>("password");

        return user;
    }

    std::optional<long> User::auth(std::string &login, std::string &password, long& type)
    {
        std::string selectstat = ((type == 1) ? " SELECT id FROM User where login=? and password=? " :
                                    " SELECT id FROM User where email=? and password=? ");
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
            for(size_t i = 0; i < database::Database::get_max_shard(); ++i)
            {
                select << selectstat,
                    into(id),
                    use(login),
                    use(password),
                    range(0, 1); //  iterate over result set one row at a time

                select.execute();
                Poco::Data::RecordSet rs(select);
                if (rs.moveFirst()) return id;
                std::string shard = " -- sharding:1 ";
                selectstat += shard;
            }
        }

        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }
    std::optional<User> User::get_by_id(long id)
    {
        std::cout << 2;
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::cout << 1;
            std::string selector = " SELECT id, first_name, last_name, email, login, password FROM user where id=? ";
            User a;
            for (size_t i = 0; i < database::Database::get_max_shard(); ++i)
            {
            Poco::Data::Statement select(session);
            select << selector,
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                use(id),
                range(0, 1);
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
            }
            std::string shard = "-- sharding:1";
            selector += shard;
        }

        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    std::vector<User> User::get_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            std::vector<User> result;
            User a;
            std::string sqlText = " SELECT id, first_name, last_name, email, login, password FROM user ";
            for(size_t i = 0; i < database::Database::get_max_shard(); ++i)
            {
                Poco::Data::Statement select(session);
                select << sqlText,
                    into(a._id),
                    into(a._first_name),
                    into(a._last_name),
                    into(a._email),
                    into(a._login),
                    into(a._password),
                    range(0, 1); //  iterate over result set one row at a time

                while (!select.done())
                {
                    if (select.execute())
                        result.push_back(a);
                }
                std::string shard = " -- sharding:1 ";
                sqlText += shard;
            }
            return result;
        }

        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<User> User::get_by_name_mask(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            std::vector<User> result;
            User a;
            first_name += "%";
            first_name = "%" + first_name;
            last_name += "%";
            last_name = "%" + last_name;
            std::string sqlText = " SELECT id, first_name, last_name, email, login, password FROM user where first_name LIKE ? and last_name LIKE ? ";
            for(size_t i = 0; i < database::Database::get_max_shard(); ++i)
            {
                Statement select(session);
                select << sqlText,
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                use(first_name),
                use(last_name),
                range(0, 1); //  iterate over result set one row at a time

                while (!select.done())
                {
                    if (select.execute())
                        result.push_back(a);
                }
                std::string shard = " -- sharding:1 ";
                sqlText += shard;
            }
            return result;
       }

        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    std::optional<User> User::get_by_login(std::string& login)
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            User a;
            std::string tmp = login + "%";
            login = "%" + tmp; 
            std::string sqlText = " select id, first_name, last_name, email, login, password from serv_store.user where login like ? ";
            for(size_t i = 0; i < database::Database::get_max_shard(); ++i)
            {
                Poco::Data::Statement query(session);
                query << sqlText ,
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                use(login),
                range(0, 1);
                query.execute();
                Poco::Data::RecordSet res(query);
                if(res.moveFirst()){return a;}
                std::string test = " -- sharding:1 ";
                sqlText += test;
            }
        }
        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    void User::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO user (first_name,last_name,email,login,password) VALUES(?, ?, ?, ?, ?)",
                use(_first_name),
                use(_last_name),
                use(_email),
                use(_login),
                use(_password);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (const Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (const Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    const std::string& User::get_login() const
    {
        return _login;
    }

    const std::string& User::get_password() const
    {
        return _password;
    }

    std::string& User::login()
    {
        return _login;
    }

    std::string& User::password()
    {
        return _password;
    }

    const long& User::get_id() const
    {
        return _id;
    }

    const std::string& User::get_first_name() const
    {
        return _first_name;
    }

    const std::string& User::get_last_name() const
    {
        return _last_name;
    }

    const std::string& User::get_email() const
    {
        return _email;
    }

    long& User::id()
    {
        return _id;
    }

    std::string& User::first_name()
    {
        return _first_name;
    }

    std::string& User::last_name()
    {
        return _last_name;
    }

    std::string& User::email()
    {
        return _email;
    }

    void User::set_fn(std::string name)
    {
        _first_name = name;
    }

    void User::set_ln(std::string ln)
    {
        _last_name = ln;
    }

    void User::set_em(std::string email)
    {
        _email = email;
    }

    void User::set_log(std::string login)
    {
        _login = login;
    }

    void User::set_pwd(std::string pwd)
    {
        _password = pwd;
    }
}