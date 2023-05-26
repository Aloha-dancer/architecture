#ifndef ORDER_HANDLER_H
#define ORDER_HANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/URI.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../entities/order/order.h"
#include "../../helper.h"

static bool hasSubstr(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

class OrderHandler: public HTTPRequestHandler
{
    public:

        OrderHandler (const std::string& format): _format(format) {}

        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response)
        {
                Poco::Net::HTMLForm form(request, request.stream());
            try
            {
                if(request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    if(form.has("id") &&
                       form.has("id_user") &&
                       form.has("id_service"))
                    {
                        database::Order order;
                        order.id() = atol(form.get("id").c_str());
                        order.id_user() = atol(form.get("id_user").c_str());
                        order.id_service() = atol(form.get("id_service").c_str());

                        bool flag = true;
                        std::string mes;
                        std::string reas;

                        if(database::Order::check_service_id(order.get_serv_id(),
                                                             order.get_id()))
                        {
                            flag = false;
                            reas += "This service have already been included to order";
                            reas += "<br>";

                        }

                    if(flag)
                    {
                        order.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream& ostr = response.send();
                        ostr << " Success ";
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        mes+= reas;
                        ostr << mes;
                        response.send();
                        return;
                    }
                    }
                }
                else  if (hasSubstr(request.getURI(), "/search"))
                {
                    const long id_user = atol(form.get("order_name").c_str());
                    std::vector<database::Order> results = database::Order::search_by_user(id_user);
                    Poco::JSON::Array arr;
                    for (std::vector<database::Order>::iterator iter = results.begin();
                         iter != results.end();
                         ++iter)
                        arr.add(*iter -> toJSON());
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(arr, ostr);
                    return;
                }
            }
            catch(const std::exception& e)
            {
                throw;
            }
            


        }


    private:
        std::string _format;

};

#endif