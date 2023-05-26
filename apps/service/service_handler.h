#ifndef SERVICE_HANDLER_H
#define SERVICE_HANDLER_H

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

#include "../../entities/service/service.h"
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

class ServiceHandler: public HTTPRequestHandler
{
    public:
        ServiceHandler(const std::string& format): _format(format) {}

        void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
        {
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::URI uri = Poco::URI(request.getURI());
            std::string path = uri.getPath();

            HTMLForm form(request, request.stream());
            try
            {
                if(form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
                {
                    const long id = atol(form.get("id").c_str());
                    std::optional<database::Service> result = database::Service::read_by_id(id);

                    if(result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root -> set("type", "/errors/not_found");
                        root->set("title", "Internal exception");
                        root->set("status", "404");
                        root->set("detail", "service not found");
                        root->set("instance", "/service");
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                    }
                }
                else if(hasSubstr(request.getURI(), "/search"))
                {
                    const std::string caption = form.get("caption");
                    std::optional<database::Service> result = database::Service::search_by_caption(caption);
                    if(result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result, ostr);
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root -> set("type", "/errors/not_found");
                        root -> set("title", "Internal exception");
                        root -> set("status", "404");
                        root -> set("detail", "service not found");
                        root -> set("instance", "/service");
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                    }
                }
                else if (hasSubstr(request.getURI(), "/list"))
                {
                    std::vector<database::Service> result = database::Service::read_all();
                    Poco::JSON::Array array;
                    for (std::vector<database::Service>::iterator iter = result.begin();
                         iter != result.end();
                         ++iter)
                    {
                        array.add(*iter->toJSON());
                    }
                }
                else if(request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    if (form.has("caption") &&
                        form.has("description") &&
                        form.has("category") &&
                        form.has("price"))
                    {
                        database::Service service_object;
                        service_object.caption() = form.get("caption");
                        service_object.description() = form.get("description");
                        service_object.category() = form.get("category");
                        service_object.price() = atol(form.get("price").c_str());
                        
                        bool flag = true;
                        std::string mes;
                        std::string reas;

                        if(service_object.get_caption().length())
                        {
                            flag = false;
                            reas += "Null value in `caption` field";
                            reas += "<br>";
                        }

                        if(service_object.get_desctiption().length())
                        {
                            flag = false;
                            reas += "Null value in `description` field";
                            reas += "<br>";
                        }

                        if(service_object.get_category().length())
                        {
                            flag = false;
                            reas += "Null value in `category` field";
                            reas += "<br>";
                        }

                        if(service_object.get_price())
                        {
                            flag = false;
                            reas += "Null value in `price` field";
                            reas += "<br>";
                        }
                        if(flag)
                        {  
                            service_object.save_to_mysql();
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            response.setChunkedTransferEncoding(true);
                            response.setContentType("application/json");
                            std::ostream& ostr = response.send();
                            ostr << service_object.get_id();
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