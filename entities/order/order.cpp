#include "order.h"
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

namespace database
{
    const long& Order::get_id() const
    {
        return id_;
    }

    const long& Order::get_usr_id() const
    {
        return id_user_;
    }

    const long& Order::get_serv_id() const
    {
        return id_service_;
    }

    const Poco::Data::Date& Order::get_ord_date() const
    {
        return order_date_;
    }

    const std::string& Order::get_status() const
    {
        return status_;
    }

    long& Order::id()
    {
        return id_;
    }

    long& Order::id_service()
    {
        return id_service_;
    }

    long& Order::id_user()
    {
        return id_user_;
    }

    Poco::Data::Date& Order::order_date()
    {
        return order_date_;
    }

    std::string& Order::status()
    {
        return status_;
    }

    Order Order::fromJSON(const std::string& data)
    {
        Order ord;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(data);
        Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();

        ord.id() = obj -> getValue<long>("id");
        ord.order_date() = obj -> getValue<std::string>("order_date");
        ord.id_service() = obj -> getValue<long>("id_service");
        ord.id_user() = obj -> getValue<long>("id_user");
        ord.status() = obj -> getValue<std::string>("status");

        return ord;
    }

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root -> set("id", id_);
        root -> set("order_date", order_date_);
        root -> set("id_service", id_service_);
        root -> set("id_user", id_user_);
        root -> set("status", status_);

        return root;
    }

    void Order::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);

            query << " create table if not exists `order` ("
	              <<" `id` bigint auto_increment, "
                  <<" `id_user` bigint not null references `user`(id), " 
	              <<" `id_service` bigint not null references `service`(id), "
	              <<" `order_date` varchar(256) not null, "
	              <<" `status` varchar(256) not null, "
	              <<" primary key (`id`, `id_user`, `id_service`), "
                  <<" key(`order_date`));",
                  Poco::Data::Keywords::now;

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

    bool Order::check_service_id(long id_service, long id, long id_user)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            Order obj;

            query << " select id, id_user, id_service, order_date, status "
                  << " from `order` where id_service=? and id=? and id_user=?",
                  Poco::Data::Keywords::into(obj.id_),
                  Poco::Data::Keywords::into(obj.id_user_),
                  Poco::Data::Keywords::into(obj.id_service_),
                  Poco::Data::Keywords::into(obj.order_date_),
                  Poco::Data::Keywords::into(obj.status_),
                  Poco::Data::Keywords::use(id_service),
                  Poco::Data::Keywords::use(id),
                  Poco::Data::Keywords::use(id_user),
                  Poco::Data::Keywords::range(0, 1);

            query.execute();
            Poco::Data::RecordSet rs(query);
            return rs.moveFirst();
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
        return {};
    }

    std::vector<Order> Order::get_by_user(long id_user)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            Order obj;
            std::vector<Order> result;
            query << " select id, id_user, id_service, order_date, status from `order` " 
                  << " where id_user=?",
                  Poco::Data::Keywords::into(obj.id_),
                  Poco::Data::Keywords::into(obj.id_user_),
                  Poco::Data::Keywords::into(obj.id_service_),
                  Poco::Data::Keywords::into(obj.order_date_),
                  Poco::Data::Keywords::into(obj.status_),
                  Poco::Data::Keywords::use(id_user),
                  Poco::Data::Keywords::range(0,1);

            while(!query.done())
                {
                    if(query.execute())
                    {
                        result.push_back(obj);
                    }
                }
                return result;
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

    std::vector<Order> Order::get_all()
    {
        try
        {  

            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            Order obj;
            std::vector<Order> res;

            query << " select id, id_user, id_service, order_date, status "
                  << " from `order` ",
                  Poco::Data::Keywords::into(obj.id_),
                  Poco::Data::Keywords::into(obj.id_user_),
                  Poco::Data::Keywords::into(obj.id_service_),
                  Poco::Data::Keywords::into(obj.order_date_),
                  Poco::Data::Keywords::into(obj.status_),
                  Poco::Data::Keywords::range(0, 1);

            while(!query.done())
            {
                if(query.execute())
                {
                    res.push_back(obj);
                }
            }
            return res;
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

    std::vector<Order> Order::get_by_order(long id)
    {
        try
        {
        Poco::Data::Session session = database::Database::get().create_session();
        Poco::Data::Statement query(session);
        Order obj;
        std::vector<Order> result;

        query << " select id, id_user, id_service, order_date, status from `order` " 
              << " where id=?",
              Poco::Data::Keywords::into(obj.id_),
              Poco::Data::Keywords::into(obj.id_user_),
              Poco::Data::Keywords::into(obj.id_service_),
              Poco::Data::Keywords::into(obj.order_date_),
              Poco::Data::Keywords::into(obj.status_),
              Poco::Data::Keywords::use(id),
              Poco::Data::Keywords::range(0, 1);
        
        while(!query.done())
        {
            if(query.execute())
            {
                result.push_back(obj);
            }
        }
        
        return result;
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

    void Order::save_to_mysql()
    {
         try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement query(session);
            
            query << " insert into `order`(id_user, id_service, order_date, status) values ( ?, ?, ?, ?) ",
                Poco::Data::Keywords::use(id_user_),
                Poco::Data::Keywords::use(id_service_),
                Poco::Data::Keywords::use(order_date_),
                Poco::Data::Keywords::use(status_);

            query.execute();

            Poco::Data::Statement query_id(session);
            query_id << " select last_insert_id() ",
                Poco::Data::Keywords::into(id_),
                Poco::Data::Keywords::range(0, 1);
            
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
}

