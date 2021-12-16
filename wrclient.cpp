#include <iostream>

#include <stdlib.h>
#inlcude "wrclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include <unistd.h>

using namespace jsonrpc;
using namespace std;

int main(){
    HttpClient httpclient("http://localhost:8300");
    wrClient myClient(httpclient, JSONRPC_CLIENT_V2);
    Json::Value my_json;
    Json::Value result;

    my_json["img-url"] = "https://i.pinimg.com/originals/14/21/1c/14211c1aed46ff1b2fe51eb4ca69b5c0.png";
    my_json["config"] = "you can change the icon";
    my_json["myVsID"] = "1234567890";

    int unfinished =1 ;
    while (unfinished)
    {
        fprintf(stderr, "reload the PostJson here (myFile2JSON in Post.cpp)\n");
        try{
            result = myClient.WebReport("web report", (my_json.toStyledString()).c_str());
            cout << result.toStyledString() << endl;
            unfinished = 0;
        } catch (JsonRpcException &e) {
            cerr << e.what() << endl;
            int code = e.GetCode();
            fprintf(stderr, "code = %d\n", code);
            if(code == -32003)
            {
                fprintf(stderr, "save the PostJson here (myJSON2File in Post.cpp)\n");
                unfinished = 1;
            }
            else
            {
                unfinished = 0;
            }
        }
        sleep(5);
    }
    return 0;
    }
