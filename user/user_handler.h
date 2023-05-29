#ifndef USER_HANDLER_H
#define USER_HANDLER_H
#include <typeinfo>
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
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/JSON/Parser.h"
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

#include "../entities/user/user.h"
#include "../helper.h"

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

class UserHandler : public HTTPRequestHandler
{
private:
    bool check_name(const std::string &name, std::string &reason)
    {
        if (name.length() < 3)
        {
            reason = "Name must be at leas 3 signs";
            return false;
        }

        if (name.find(' ') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        if (name.find('\t') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        return true;
    };

    bool check_email(const std::string &email, std::string &reason)
    {
        if (email.find('@') == std::string::npos)
        {
            reason = "Email must contain @";
            return false;
        }

        if (email.find(' ') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        if (email.find('\t') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        return true;
    };

public:
    UserHandler(const std::string &format) : _format(format) {}

    Poco::JSON::Object::Ptr remove_password(Poco::JSON::Object::Ptr src)
    {
        if (src->has("password"))
            src->set("password", "*******");
        return src;
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");

        const std::string method = request.getMethod();
        Poco::URI uri = Poco::URI(request.getURI());
        std::string path = uri.getPath();

        std::istream& stream = request.stream();
        std::ostringstream oss;
        Poco::StreamCopier::copyStream(stream, oss);
        std::string requestBody = oss.str();
        HTMLForm form(request, stream);
        try
        {
            if (method == Poco::Net::HTTPRequest::HTTP_GET)
            {
                if (hasSubstr(path, "/searchID"))
                {
                    long id = stol(form.get("id", "0"));
                    std::cout << id << std::endl;
                    std::cout << typeid(id).name() << std::endl;
                    std::optional<database::User> result = database::User::get_by_id(id);
                    std::cout << 3;
                    if (result)
                    {   
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root->set("type", "/errors/not_found");
                        root->set("title", "Internal exception");
                        root->set("uri_1", uri);
                        root->set("idd", atol(form.get("id").c_str()));
                        root->set("ids", form.get("id"));
                        root->set("status", "404");
                        root->set("detail", "user not found");
                        root->set("instance", "/user");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                        return;
                    }
                }
                if (hasSubstr(path, "/auth"))
                {
                    std::string scheme;
                    std::string info;
                    request.getCredentials(scheme, info);
                    std::cout << "scheme: " << scheme << " identity: " << info << std::endl;
                    std::string login, password;
                    long type = static_cast<int>(form.has("login"));
                    if (scheme == "Basic")
                    {
                        get_identity(info, login, password);
                        if (auto id = database::User::auth(login, password, type))
                        {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            std::ostream &ostr = response.send();
                            ostr << "{ \"id\" : \"" << *id << "\"}" << std::endl;
                            return;
                        }
                    }
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/unauthorized");
                    root->set("title", "Internal exception");
                    root->set("status", "401");
                    root->set("detail", "not authorized");
                    root->set("instance", "/auth");
                    std::ostream& ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
                if (hasSubstr(path, "/searchFL"))
                {
                    std::string fn = form.get("first_name", "");
                    std::string ln = form.get("last_name", "");
                    std::vector<database::User> results = database::User::get_by_name_mask(fn, ln);
                    Poco::JSON::Array arr;
                    for (auto s : results)
                        arr.add(remove_password(s.toJSON()));
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    std::ostream& ostr = response.send();
                    Poco::JSON::Stringifier::stringify(arr, ostr);

                    return;
                }
                if (hasSubstr(path, "/searchLog"))
                {
                    std::string login = form.get("login");
                    std::optional<database::User> result = database::User::get_by_login(login);
                    if(result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root->set("type", "/errors/not_found");
                        root->set("title", "Internal exception");
                        root->set("status", "404");
                        root->set("detail", "user not found");
                        root->set("instance", "/user");
                        std::ostream& ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                        return;
                    }
                }
                if (hasSubstr(path, "/user"))
                {
                    std::vector<database::User> result = database::User::get_all();
                    Poco::JSON::Array array;
                    for(std::vector<database::User>::iterator iter = result.begin();
                        iter != result.end();
                        ++iter)
                    {
                        array.add(*iter->toJSON());
                    }
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    std::ostream& ostr = response.send();
                    Poco::JSON::Stringifier::stringify(array, ostr);
                    return;
                }
            }
            if (method == Poco::Net::HTTPRequest::HTTP_POST)
            {
                if (hasSubstr(path, "/auth"))
                {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream& stream = response.send();
                        std::cout << "I am here" << std::endl;
                        database::User user;
                        std::string login = "";
                        Poco::Nullable<std::string> firstname;
                        Poco::Nullable<std::string> lastname;
                        std::string password = "";
                        std::string email = "";
                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();
                        std::cout <<" Correct" << std::endl;
                        login = obj->getValue<std::string>("login");
                        email = obj->getValue<std::string>("email");
                        password = obj->getValue<std::string>("password");
                        firstname = obj->getNullableValue<std::string>("first_name");
                        lastname = obj->getNullableValue<std::string>("last_name");
                    }
                    catch (std::exception &e) 
                    {
                            stream << "{\"error_message\":\"" + (std::string)e.what() + "\"}";
                            return;
                    }
                    
                    if(!firstname.isNull())user.set_fn(firstname.value());
                    if(!lastname.isNull())user.set_ln(lastname.value());
                    user.set_em(email);
                    user.set_log(login);
                    user.set_pwd(password);

                    user.save_to_mysql();
                    std::cout << "Perfekt" << std::endl;
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << user.get_id();
                    return;
                }
            }
        }
        catch (Poco::Exception& e)
        {
            throw;
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("form", form.get("id"));
        root->set("path", request.getURI());
        root->set("cond", hasSubstr(path, "/searchID"));
        root->set("detail", "request not found");
        root->set("instance", "/user");
        root->set("method", method);
        std::ostream& ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif