#ifndef __RUN_PROCESS_H__
#define __RUN_PROCESS_H__

#include "SocketOperate.h"

class RunProcess : public SocketOperate
{
public:
    string Response(string strRequest);

private:
    string Run();
    string Error();

    char m_pBuf[100];

};

#endif


