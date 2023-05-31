#include <string>
#include <iostream>
#include <fstream>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "../entities/user/user.h"
#include "../database/database.h"

using Poco::Data::MySQL::Connector;
using Poco::Data::MySQL::MySQLException;

int main()
{

    Poco::Data::MySQL::Connector::registerConnector();

    std::string db_connection_string;
    db_connection_string += "host=127.0.0.1;";
    db_connection_string += "port=6033;";
    db_connection_string += "user=myyuser;";
    db_connection_string += "password=123;";
    db_connection_string += "db=serv_store";
    std::cout << "Database connection string:" << db_connection_string << std::endl;

    std::vector<std::string> shards;
    for(int i = 0; i <= 1; i++)
    {
        std::string shard_name = "-- sharding:";
        shard_name += std::to_string(i);
        shards.push_back(shard_name);
    }


    Poco::Data::Session session(
        Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, db_connection_string)
    );
    std::cout << "Session created" << std::endl;

    try
    {
        for ([[maybe_unused]] std::string &shard : shards) {
            session << "drop table `user`";
            Poco::Data::Statement create_stmt(session);
            create_stmt << "create table if not exists `user`"
                        << "("
                        << "`id` int not null auto_increment,"
                        << "`first_name` varchar(256) not null,"
                        << "`last_name` varchar(256) not null,"
                        << "`ogin` varchar(256) not null,"
                        << "`password` varchar(64) not null,"
                        << "`email` varchar(256) null,"
                        << "primary key (`id`), key `K1` (`first_name`), key `K2` (`last_name`)"
                        << ",KEY `KI_login` (`login`))";
            create_stmt.execute();
        }
        std::cout << "User table created" << std::endl;

        Poco::Data::Statement truncate_stmt(session);
        truncate_stmt << "set foreign_key_checks=0;";
        truncate_stmt.execute();
        truncate_stmt << "truncate table `user`;";
        truncate_stmt.execute();
        truncate_stmt << "SET FOREIGN_KEY_CHECKS = 1;";
        truncate_stmt.execute();
        std::string json;
        std::ifstream is("cache_testing/data.json");
        std::istream_iterator<char> eos;
        std::istream_iterator<char> iit(is);
        std::cout << *iit << std::endl;
        int count = 0;
        while (iit != eos)
        {
            json.push_back(*(iit++));
            ++count;
        }
        std::cout << count << std::endl;
        is.close();

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();

        for (int i = 0; i < (int) arr->size(); i++)
        {
            Poco::JSON::Object::Ptr object = arr->getObject(i);
            std::string first_name = object->getValue<std::string>("_first_name");
            std::string last_name = object->getValue<std::string>("_last_name");
            std::string email = object->getValue<std::string>("email");
            std::string login = object->getValue<std::string>("login");
            std::string password = object->getValue<std::string>("password");

            std::string key;

            std::string select_str = "insert into `user` (first_name, last_name, login, password, email) values(?, ?, ?, ?, ?)";
            std::cout << select_str << std::endl;

            Poco::Data::Statement insert(session);
            insert << select_str,
                    Poco::Data::Keywords::use(first_name),
                    Poco::Data::Keywords::use(last_name),
                    Poco::Data::Keywords::use(login),
                    Poco::Data::Keywords::use(password),
                    Poco::Data::Keywords::use(email),

            insert.execute();

            long inserted_id;
            Poco::Data::Statement select(session);
            select << "select last_insert_id()",
                    Poco::Data::Keywords::into(inserted_id),
                    Poco::Data::Keywords::range(0, 1);

            if (!select.done()) select.execute();

            std::cout << "Inserted 1 new user with id = " << inserted_id << std::endl;
        }

        std::cout << "Inserted " << arr->size() << " records" << std::endl;

    }
    catch (Poco::Data::MySQL::ConnectionException &e)
    {
        std::cout << "Connection exception: " << e.message() << std::endl;
    }
    catch (Poco::Data::MySQL::StatementException &e)
    {

        std::cout << "Statement exception: " << e.message() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 1;
}