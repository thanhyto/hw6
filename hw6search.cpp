#include <iostream>

#include <stdlib.h>
#include "hw6client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"

using namespace std;
using namespace jsonrpc;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char arg_string[] = "<search_clause_json>";
        std::cout << "usage: " << argv[0] << arg_string << std::endl;
        return -1;
    }
    Json::Value jv;
    int rc = myFile2JSON(argv[1], &jv);

    HttpClient httpclient("http:/127.0.0.1:54407");
    hw6Client myClient(httpclient, JSONRPC_CLIENT_V2);

    Json::Value result_json;
    OKey * ok_ptr;

    if ((jv["keyword"].isNull()!= true) && (jv["keyword"].isString()!= true)  && (jv["keyword"].asString() > 0))
    {
        try {
            ok_ptr = new OKey(jv["keyword"].asString());
            std::string search_str;
            search_str += "{\"keyword\" : \"";
            search_str += ok_ptr->okey;
            search_str += "\"}";
            std::cout << search_str << std::endl;
            result_json = myClient.search(search_str);
            std::cout << result_json.toStyledString() << endl;

        } catch (JsonRpcException &e){
            cerr <<e.what() << endl;
        }
    }

    if ((result_json["data"].isNull() != true) && (result_json["data"].isArray()== true))
    {
        for (int i = 0; i < (result_json["data"]).size(); i++)
        {
            if ((result_json["data"][i]).isObject() == true)
            {
                try
                {
                    char fname_buf[1024];
                    bzero(fname_buf, 1024);

                    sprintf(fname_buf, "post_%s.json", (result_json["data"][i]["id"]).asString().c_str());

                    Post * post_ptr = new Post((Person *) NULL, (Message *) NULL);

                    bool result_flag = post_ptr->Jdump(&(result_json["data"][i]));
                    if(result_flag == false)
                    {
                        throw HW5_ERROR_JSON_2_POST;
                    }
                    rc = myJSON2File(fname_buf, post_ptr->dumpJ());
                    if(rc!= HW5_ERROR_NORMAL) throw pc;
                }
                catch(int error_code)
                {
                    Json::Value err_jv;
                    int erc = produceErrorJSON(error_code, "hw7client_search.log", &err_jv, 0);
                    if(erc != HW5_ERROR_NORMAL) std::cerr << "more failure\n";
                }
                
            }
        }
    }
    return 0;
}