#ifndef __SOCKET_HTTP_H__
#define __SOCKET_HTTP_H__

#include <vector>

#include "SocketOperate.h"

class SocketHTTP : public SocketOperate
{
public:
    string Response(string strRequest);

private:
    string GetMethod(string strURL);
    string PostMethod(string strPageName, string strRequest);

    string Router(string& strPageName, vector<string>& vKeys,
                  vector<string>& vValues);

    int ParameterGet(string& strParms, vector<string>& vKeys,
                      vector<string>& vValues);

    string ServeFile(const char* filename);

    string Cat(FILE* resource);
    string Headers();
    string BadRequest();
    string NotFound();
    string Unimplemented();

    char m_pBuf[1024];
};

#endif


