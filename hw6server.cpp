// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>
#include <ctype.h>

// for JsonRPCCPP
#include <iostream>
#include "hw6server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

#include "Post.h"

std::map<std::string, Post *> Post_Map;
std::map<std::string, JvTime *> RT_Map;

extern const char *hw5error[];

using namespace jsonrpc;
using namespace std;

class Myhw6Server : public hw6Server
{
    public:
    Myhw6Server(AbstractServerConnector &connector, serverVersion_t type);
    virtual Json::Value update(const std::string& updating_json);
    virtual Json::Value search(const std::string& search_clause);
};

Myhw6Server::Myhw6Server(AbstractServerConnector &connector, serverVersion_t type) : hw6Server(connector, type)
{
    std::cout << "Myhw6Server Object created" << std::endl;
}

int checkTimeInterval(std::string arg_id)
{
    time_t ticks;
    ticks = time(NULL);
    struct std::tm * my_tm_ptr = gmtime(&ticks);
    JvTime * now_time_ptr = new JvTime();
    int rc = now_time_ptr->setStdTM(my_tm_ptr);
    if(rc != HW5_ERROR_NORMAL)
        {
            std::cout << "error: failed to set time!" << std::endl;
            delete now_time_ptr;
            return HW5_ERROR_SET_STD_TM;
        }
    return HW5_ERROR_NORMAL;
}

Json::Value Myhw6Server::update(const std::string& updating_json)
{
    Json::Value result_json;
    char fname_buf[1024];
    FILE * post_f_ptr;
    int rc = HW5_ERROR_NORMAL;
    Post * post_ptr = NULL;

#ifdef _ECS36B_DEBUG_
    std::cout << "update" << " " << updating_json << std::endl;
#endif /*_ECS36B_DEBUG_*/

    myPrintLog("{\"location\":\"p0\"}","hw6server_update.log");

    try 
    {
        if((updating_json.c_str())[0] != '{')
            throw HW5_ERROR_JSON_PARSING;
        myPrintLog(updating_json, "hw6server_update.log");

        Json::Value myv_message;

        rc = checkBigID(updating_json);
        if(rc!=HW5_ERROR_NORMAL) throw rc;
        rc= myParseJSON(updating_json, &myv_message);
        if(rc!=HW5_ERROR_NORMAL) throw rc;

        if (((myv_message["id"]).isNull() == true) && ((myv_message["id"]).isString() == false) && (checkPostID((myv_message["id"]).asString())!=0))
            {
                throw HW5_ERROR_JSON_POST_NO_ID;
            }
        rc = checkTimeInterval((myv_message["id"]).asString());
        if (rc != HW5_ERROR_NORMAL) throw rc;

        bzero(fname_buf, 1024);
        sprintf(fname_buf, "./posts/post_%s.json", ((myv_message["id"]).asString()).c_str());
        std::cout << "name: " << fname_buf << std::endl;

        {
            char idstr[1024];
            bzero(idstr, 1024);
            sprintf(idstr, "%", ((myv_message["id"]).asString()).c_str());

            char c_prof_buf[256];
            char c_post_buf[256];
            bzero(c_prof_buf, 256);
            bzero(c_post_buf, 256);
            scanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
            std::string s_vsID {c_prof_buf};
            
#ifdef _REAL_SERVER_
        if(vsID_Map.find(s_vsID)== vsID_Map.end())
        {
            throw HW5_ERROR_VSID_POST_ID;
        }
#endif /*_REAL_SERVER_*/
        }

        myPrintLog("{\"location\":\"entering 3 cases\"}", "hw6server_update.log");

        if(Post_Map.find((myv_message["id"]).asString())!=Post_Map.end())
        {
            myPrintLog("{\"location\":\"case 1\"}", "hw6server_update.log");
            post_ptr = Post_Map[(myv_message["id"]).asString()];

            myPrintLog("{\"location\":\"before Json Post Merging\"}", "hw6server_update.log");

            bool result_flag = post_ptr->Jdump(&myv_message);
            if(result_flag == false)
            {
                throw HW5_ERROR_JSON_POST_MERGING;
            }
            myPrintLog("{\"location\":\"after JSON Post Merging\"}", "hw6server_update.log");
        }
        else
        {
            Json::Value pjf_v;
            rc = myFile2JSON(fname_buf, &pjf_v);
            if (rc == HW5_ERROR_NORMAL)
                {
                    myPrintLog("{\"location\":\"case 2\"}", "hw6server_update.log");
                    post_ptr = new Post((Person *) NULL, (Message *) NULL);
                    myPrintLog("{\"location\":\"before Json 2 Post\"}", "hw6server_update.log");

                    bool result_flag = post_ptr->Jdump(&pjf_v);
                    myPrintLog("{\"location\":\"after Json 2 Post\"}", "hw6server_update.log");
                    if(result_flag == false)
                    {
                        delete post_ptr;
                        throw HW5_ERROR_JSON2_POST;
                    }
                    else
                    {
                        std::cout << "update p2 case 2s\n";
                        Post_Map[(myv_message["id"]).asString()] = post_ptr;
                        myPrintLog("{\"location\":\"before Json Post Merging\"}", "hw6server_update.log");

                        bool result_flag = post_ptr->Jdump(&myv_message);
                        myPrintLog("{\"location\":\"after Json Post Merging\"}", "hw6server_update.log");

                        if (result_flag == false)
                            {
                                throw HW5_ERROR_JSON_POST_MERGING;
                            }
                    }
                }
                else
                {
                    myPrintLog("{\"location\":\"case 3\"}", "hw6server_update.log");

                    post_ptr = new Post((Person *) NULL, (Message *) NULL);
                    myPrintLog("{\"location\":\"before Json_2_Post\"}", "hw6server_update.log");
                    bool result_flag = post_ptr->Jdump(&myv_message);
                    myPrintLog("{\"location\":\"after Json 2 Post\"}", "hw6server_update.log");
                    if(result_flat == false)
                    {
                        delete post_ptr;
                        throw HW5_ERROR_JSON_2_POST;
                    }
                    else
                        Post_Map[(myv_message["id"]).asString*()] = post_ptr;
                }
        }
    }    

    catch (int error_code)
    {
        myPrintLog("{\"location\":\"catch\"}", "hw6server_update.log");
        int erc = produceErrorJSON(error_code, "hw6server_update.log", &result_json, 0);
        if(erc != HW5_ERROR_NORMAL)
            myPrintLog("{\"location\":\"catch more\"}", "hw6server_update.log");
        return result_json;
    }
    myPrintLog("{\"location\":\"before write back\"}", "hw6server_update.log");
    if (post_ptr==NULL){
        myPrintLog("{\"location\":\"before write back POST_PTR null\"}", "hw6server_update.log");
    }
    Json::Value **jj = post_ptr->dumpJ();
    myPrintLog("{\"location\":\"after dumpJ\"}", "hw6server_update.log");
    rc = myJSON2File(fname_buf, jj);
    myPrintLog("{\"location\":\"after myJSON2File\"}", "hw6server_update.log");

    if (rc != HW5_ERROR_NORMAL)
    {
        int erc = produceErrorJSON(rc, "hw6server_update.log", &result_json, 0);
        if(erc != HW5_ERROR_NORMAL)
            myPrintLog("{\"location\":\"wrtie back double errors\"}", "hw6server_update.log");
        return result_json;
    }

    myPrintLog("{\"location\":\"after wrtie back \"}", "hw6server_update.log");
    std::cout << Post_Map.size() << std::endl;
    Json::Value *rj=ptr = post_ptr->dumpJ();
    myPrintLog("{\"location\":\"after dumpJ \"}", "hw6server_update.log");
    produceErrorJSON(HW5_ERROR_NORMAL, "hw6server_update.log", &result_json, 0);
    return (*rj_ptr);
}

Json::Value
Myhw6Server::search(const std::string& search_clause)
{
    Json::Value result_json;
    int rc = HW5_ERROR_NORMAL;

#ifdef _ECS36B_DEBUG_
    std::cout << "search" << " " << search_clause << std::endl;
#endif /*_ECS36B_DEBUG_*/

    myPrintLog("{\"location\":\"p0\"}", "hw6server_update.log");
    try
    {
        if((search_clause.c_str())[0] != '{')
            throw HW5_ERROR_JSON_PARSING;
        myPrintLog(search_clause, "hw6server_update.log");

        Json::Value myv_message;
        rc = myParseJSON(search_clause, &myv_message);
        if( rc != HW5_ERROR_NORMAL) throw rc;
        if(myv_message.isObject() != true)
            {
                throw HW5_ERROR_BAD_KEYWORD_FORMAT;
            }
        
        Json::Value json_data;
        if ((myv_message["keyword"]).isNull() == true)
        {
            throw HW5_ERROR_NO_SEARCH_KEYWORD;
        }
        if ((myv_message["keyword"]).isString() == false)
        {
            throw HW5_ERROR_BAD_KEYWORD_FORMAT;
        }
        myPrintLog("{\"location\":\"p1\"}", "hw6server_update.log");

        char fn_buf[256];
        bzero(fn_buf, 256);

        char test_str[1024];
        bzero(test_str, 1024);
        sprintf(test_str, "%s", ((myv_message["keyword"]).asString()).c_str());

        char cbuf[1024];
        bzero(cbuf, 1024);

        int ii;
        for (ii=0, ii<strlen(test_str); ii++)
        {
            if ((test_str[ii] >= 'A' && (test_str[ii]) <= 'Z'))
                cbuf[ii] = tolower(test_str[ii]);
            else
                cbuf[ii] = test_str[ii];
        }

        sprintf(fn_buf, "./okeys/okey_%s.list", cbuf);

        std::string s = fn_buf;
        myPrintLog(s, "hw6server_update.log");

        FILE *okey_f=fopen(fn_buf, "r");
        if (okey_f==NULL)
        {
            throw HW5_ERROR_NO_OKEY;
        }

        char post_id_buf[1024];
        int okey_flag = 1;
        int post_count = 0;
        int i = 0;
        Json::Value *dumpjv_ptr;
       
        myPrintLog("{\"location\":\"p2\"}", "hw6server_update.log");
        while (okey_flag)
        {
            bzero(post_id_buf, 1024);
            int f_rc = fscanf(okeyf, "%[^\n]", post_id_buf);
            std::cout << post_id_buf << std::endl;
            std::string post_id_string = post_id_buf;

            if(f_rc == EOF)
            {
                okey_flag = 0;
            }
            else
            {
                if (checkPostID(post_id_string) == 0)
                {
                    char pfn_buf[1024];
                    bzero(pfn_buf, 1024);
                    sprintf(pfn_buf, "./posts/post_%s.json", post_id_buf);
                    std::cout << pfn_buf << std::endl;

                    myPrintLog("{\"location\":\"p3\"}", "hw6server_update.log");
                    Json::Value pjf_v;
                    rc = myFile2JSON(pfn_buf, %pjf_v);
                    if(rc!= HW5_ERROR_NORMAL)
                    {
                        std::string s {  "{\"id 123\"}:{" };
                        s += pfn_buf
                        s += "\"}";
                        myPrintLog(s, "hw6server_update.log");
                    }
                    else
                    {
                        Post * post_ptr = new Post((Person *) NULL, (Message *) NULL);
                        myPrintLog("{\"location\":\"before Json_2_Post\"}", "hw6server_update.log");
                        bool result_flag = post->Jdumnp(&pjf_v);
                        myPrintLog("{\"location\":\"after Json 2 Post\"}", "hw6server_update.log");

                        if (result_flag == flase)
                        {
                            std::string s {  "{\"id 114\"}:{" };
                            s += pfn_buf
                            s += "\"}";
                            myPrintLog(s, "hw6server_update.log");
                        }
                        else
                        {
                            dumpjv_ptr = post_ptr->dumpJ();
                            json_data[i] = (*dumpjv_ptr);
                            i++;
                            delete dumpjv_ptr;
                        }
                    }
                }
            }
            fgetc(okey_f);
            
        }
        fclose(okey_f);
        result_json["data"] = json_data;
        result_json["count"] = ((int) i);
    }
    catch (int error_code)
    {
        myPrintLog("{\"location\":\"catch\"}", "hw6server_update.log");
        int erc = produceErrorJSON(error_code,"hw6server_update.log", &result_json,0);
        if (erc != HW5_ERROR_NORMAL)
            myPrintLog("{\"location\":\"catch more\"}", "hw6server_update.log");
        return result_json;
    }

    myPrintLog("{\"location\":\"before the end\"}", "hw6server_update.log");
    Json::Value r2;
    produceErrorJSON(HW5_ERROR_NORMAL, "hw6server_update.log", &r2, 0);
    return result_json;
}
int main(int argc, char *argvp[])
{
    if (argc != 1) exit(-1);

#ifdef _REAL_SERVER_
    if (argc != 2) exit(-1);
    FILE *vsID_f = fopen(argv[1], "r");
    if (vsID_f == NULL) return -1;
#endif /*_REAL_SERVER_*/

    char buf[1024];
    int flag1;

#ifdef _REAL_SERVER_
    flag1=1;
    while(flag1)
    {
        bzero(buf, 1024);
        int rc = fscanf(vsID_f, "%[^\n]", buf);

        if(rc == EOF)
        {
            flag1 = 0;
        }
        else
        {
            char *buf_ptr = buf;
            char sID_buf[256];
            char vsID_buf[256];
            char avatar_buf[256];

            bzero(sID_buf, 256);
            bzero(vsID_buf, 256);
            bzero(avatar_buf, 256);

            sscanf(buf_ptr, "%[^\t]", sID_buf);
            buf_ptr += (strlen(sID_buf) + strlen ("\t"));
            sscanf(buf_ptr, "%[^\t]", vsID_buf);
            buf_ptr += (strlen(vsID_buf) + strlen ("\t"));
            sscanf(buf_ptr, "%[^\t]", avatar_buf);

            std::string str_vsID { vsID_buf};
            std::string str_avatar {avatar_buf};

            Person *p_ptr = new Person(str_avatar, str_vsID);
            vsID_Map[str_vsID] = p_ptr;

            fgetc(vsID_f);
        }

    }
    fclose(vsID_f);
#endif /*_REAL_SERVER_*/
    HttpServer httpserver(55407);
    Myhw6Server s(httpserver, JSONRPC_SERVER_V1V2);
    s.StarListening();
    std::cout<< "Hit enter to stop the server" << endl;
    getchar();

    s.StopListening();
}