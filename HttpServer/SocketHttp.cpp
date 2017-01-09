#include "SocketHttp.h"

#include <stdio.h>
#include <string.h>

#include "json/json.h"

#define SERVER_STRING "Server: jdbHttpServer/0.1.0\r\n"

string& Trim(string& str)
{
    if(str.empty())
    {
        return str;
    }

    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);

    return str;
}

int Split(string& str, string delim, vector<string>* pvRet)
{
    pvRet->clear();

    size_t last = 0;
    size_t index = str.find_first_of(delim, last);

    while (index != string::npos)
    {
        pvRet->push_back(str.substr(last, index - last));
        last = index + 1;
        index = str.find_first_of(delim, last);
    }

    if (index - last > 0)
    {
        pvRet->push_back(str.substr(last, index - last));
    }

    return pvRet->size();
}

string SocketHTTP::Response(string strRequest)
{
    string strRet;

    const char* buf = strRequest.c_str();

    printf("---->Request : %s\n", buf);

    int find;
    if((find = strRequest.find("\r\n")) == string::npos)
    {
        return this->Unimplemented();
    }

    string strFirstLine = strRequest.substr(0, find);
    printf("First Line : %s\n", strFirstLine.c_str());

    vector<string> vStr;
    Split(strFirstLine, "/", &vStr);

    string strMethod = Trim(vStr[0]); // Method
    string strURL = vStr[1]; // URL
    // Remove "HTTP"
    strURL = Trim(strURL.erase(strURL.find_last_of(' ')));

    printf("Method : %s\n", strMethod.c_str());
    printf("URL : %s\n", strURL.c_str());

    if (strMethod == "GET")
    {
        strRet = GetMethod(strURL);
    }
    else if (strMethod == "POST")
    {
        strRet = PostMethod(strURL, strRequest);
    }
    else
    {
        return Unimplemented();
    }

    return strRet;
}

// strURL : home?abc=1&bcd=2
string SocketHTTP::GetMethod(string strURL)
{
    printf("--->GetMethod()\n");

    string strPageName = "";
    vector<string> vKeys;
    vector<string> vValues;

    if (strURL.length() != 0)
    {
       int find = strURL.find('?');
       if (find == string::npos)
       {
            strPageName = strURL;
       }
       else
       {
            strPageName = strURL.substr(0, find);

            string str = strURL.substr(find + 1);
            ParameterGet(str, vKeys, vValues);
       }
    }

    return Router(strPageName, vKeys, vValues);
}

string SocketHTTP::PostMethod(string strPageName, string strRequest)
{
    printf("--->PostMethod()\n");

    if (strPageName.empty())
    {
        return this->NotFound();
    }

    vector<string> vKeys;
    vector<string> vValues;

    int find = strRequest.find_last_of('\n');
    if (find == string::npos)
    {
        return this->NotFound();
    }

    string strURL = strRequest.substr(find + 1);

    printf("Post Para : %s\n", strURL.c_str());

    ParameterGet(strURL, vKeys, vValues);

    return Router(strPageName, vKeys, vValues);
}

// strParms : abc=123&bcd=456
int SocketHTTP::ParameterGet(string& strParms, vector<string>& vKeys,
                  vector<string>& vValues)
{
    vKeys.clear();
    vValues.clear();

    printf("ParmeterGet() - %s\n", strParms.c_str());
    Split(strParms, "&", &vKeys);

    int find;
    string str = "";
    for (int i = 0; i < vKeys.size(); i++)
    {
        str = vKeys[i];

        find = str.find('=');
        vKeys[i] = str.substr(0, find);
        vValues.push_back(str.substr(find + 1));
    }

    return vKeys.size();
}

string SocketHTTP::Router(string& strPageName,
                          vector<string>& vKeys, vector<string>& vValues)
{
    string strFilePath = "html/";
    /*默认情况为 index.html */
    if (strPageName == "")
    {
        strFilePath.append("index.html");
        return ServeFile(strFilePath.c_str());
    }

    /* Operate the parameters yourself */
    Json::Value root;
    Json::FastWriter writer;
    Json::Value node;

    for (int i = 0; i < vKeys.size(); i++)
    {
        node[vKeys[i]] = vValues[i];
    }

    root.append(node);

    string strJson = writer.write(root);

    string strRet = Headers();

    strRet.append(strJson);

    return strRet;
}


string SocketHTTP::ServeFile(const char* filename)
{
    string strRet;

    printf("---> Start serve_file()\n");
    printf("filename : %s\n", filename);

    /*打开 sever 的文件*/
    FILE* resource = fopen(filename, "r");
    if (resource == NULL)
    {
        printf("File Not Found\n");
        strRet = NotFound();
    }
    else
    {
        /*写 HTTP header */
        strRet = Headers();
        /*复制文件*/
        strRet.append(Cat(resource));

        fclose(resource);
    }


    return strRet;
}

string SocketHTTP::Cat(FILE* resource)
{
    string strRet;

    /*读取文件中的所有数据写到 socket */
    fgets(m_pBuf, sizeof(m_pBuf), resource);
    while (!feof(resource))
    {
        strRet.append(m_pBuf);
        fgets(m_pBuf, sizeof(m_pBuf), resource);
    }

    return strRet;
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
string SocketHTTP::Headers()
{
    /*正常的 HTTP header */
    string strRet = "HTTP/1.0 200 OK\r\n";

    /*服务器信息*/
    strRet.append(SERVER_STRING);
    strRet.append("Content-Type: text/html\r\n");
    strRet.append("\r\n");

    return strRet;
}


/**********************************************************************/
/* Inform the client that a request it has made has a problem.
 * Parameters: client socket */
/**********************************************************************/
string SocketHTTP::BadRequest()
{
    /*回应客户端错误的 HTTP 请求 */
    string strRet = "HTTP/1.0 400 BAD REQUEST\r\n";
    strRet.append("Content-type: text/html\r\n");
    strRet.append("\r\n");
    strRet.append("<P>Your browser sent a bad request, ");
    strRet.append("such as a POST without a Content-Length.\r\n");

    return strRet;
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
string SocketHTTP::NotFound()
{
    /* 404 页面 */
    string strRet = "HTTP/1.0 404 NOT FOUND\r\n";

    /*服务器信息*/
    strRet.append(SERVER_STRING);
    strRet.append("Content-Type: text/html\r\n");
    strRet.append("\r\n");
    strRet.append("<HTML><TITLE>Not Found</TITLE>\r\n");
    strRet.append("<BODY><P>The server could not fulfill\r\n");
    strRet.append("your request because the resource specified\r\n");
    strRet.append("is unavailable or nonexistent.\r\n");
    strRet.append("</BODY></HTML>\r\n");

    return strRet;
}
/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
string SocketHTTP::Unimplemented()
{
    /* HTTP method 不被支持*/
    string strRet = "HTTP/1.0 501 Method Not Implemented\r\n";

    /*服务器信息*/
    strRet.append(SERVER_STRING);
    strRet.append("Content-Type: text/html\r\n");
    strRet.append("\r\n");
    strRet.append("<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    strRet.append("</TITLE></HEAD>\r\n");
    strRet.append("<BODY><P>HTTP request method not supported.\r\n");
    strRet.append("<BR/>From : HTTP Server\r\n");
    strRet.append("</BODY></HTML>\r\n");

    return strRet;
}
