#include "service.h"
#include "database.h"
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
    void Service::init()
    {
        try
            {
                Poco::Data::Session session = database::Database::get().create_session();
                Poco::Data::Statement query(session);
                query << " create table if not exists `Service` (`id` bigint not null auto_increment, "
                      << " caption varchar(100) unique not null, "
                      << " category varchar(100) unique not null, "
                      << " description text not null, "
                      << " price long not null, "
                      << " constraint PK_service primary key (id) );", now;
            }

        catch (Poco::Data::MySQL::ConnectionException &e)
            {
                std::cout << "Connection :" << e.what() << std::endl;
                throw;
            }

        catch (Poco::Data::MySQL::StatementException &e)
            {
                std::cout << "Statement: " << e.what() << std::endl;
                throw;
            }
    }

    Poco::JSON::Object::Ptr Service::toJSON() const 
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root -> set("id", id);
        root -> set("caption", caption);
        root -> set("category", category);
        root -> set("description", description);
        root -> set("price", price);

        return root; 
    }

    Service Service::fromJSON(const std::string& data)
    {
        Service service;
        Poco::JSON::Parser parseJSON;
        Poco::Dynamic::Var result = parseJSON.parse(data);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        service.id() = object -> getValue<long>("id");
        service.caption() = object -> getValue<std::string>("caption");
        service.category() = object -> getValue<std::string>("category");
        service.description() = object -> getValue<std::string>("description");
        service.price() = object -> getValue<long>("price");

        return service;
    }

    std::optional<Service> Service::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            Service a;
            query << "  select id, caption, category, description, price from service where id=? ",
            into(a.id_),
            into(a.caption_),
            into(a.description_),
            into(a.category_),
            into(a.price_),
            use(id),
            range(0, 1);

            query.execute();
            Poco::Data::RecordSet rec(query);
            if(rec.moveFirst()) return a;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    std::vector<Service> Service::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            std::vector<Service> res;
            Service obj;
            query << "Select id, caption, category, description, price from service",
            into(obj.caption_),
            into(obj.category_),
            into(obj.id_),
            into(obj.description_),
            into(obj.price_),
            range(0, 1);
            while(!query.done())
            {
                if(query.execute())
                {
                    res.push_back(obj);
                }
            }
            return res;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::optional<Service> Service::search_by_caption(const std::string& data)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            Service obj;
            query << " select id, caption, category, description, price from service "
                  << " where caption=? ",
                  into(obj.id_),
                  into(obj.caption_),
                  into(obj.category_),
                  into(obj.description_),
                  into(obj.price_),
                  use(data),
                  range(0,1);
            query.execute();
            Poco::Data::RecordSet rec(query);
            if(rec.moveFirst()) return obj;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    void Service::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            
            query << " insert into serv_store.service(caption, category, description, price) values (?, ?, ?, ?, ?) ",
                use(caption_),
                use(category_),
                use(description_),
                use(price_);

            query.execute();

            Poco::Data::Statement query_id(session);
            query_id << " select last_insert_id() ",
                into(id_),
                range(0, 1);
            
            if(!query_id.done())
            {
                query_id.execute();
            }
            std::cout << "inserted: " << id_ << std::endl;
        }
         catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    const long& Service::get_id() const
    {
        return id_;
    }

    const std::string& Service::get_caption() const
    {
        return caption_;
    }

    const std::string& Service::get_category() const
    {
        return category_;
    }

    const std::string& Service::get_desctiption() const
    {
        return description_;
    }

    const long& Service::get_price() const
    {
        return price_;
    }

    std::string& Service::caption()
    {
        return caption_;
    }

    std::string& Service::category()
    {
        return category_;
    }

    std::string& Service::description()
    {
        return description_;
    }

    long& Service::id()
    {
        return id_;
    }

    long& Service::price()
    {
        return price_;
    }
}
