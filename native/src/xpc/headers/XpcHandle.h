#ifndef ___XPC_HANDLE_H___
#define ___XPC_HANDLE_H___

#include <queue>
#include <xpc/xpc.h>

class XpcHandle {
  private:
    xpc_connection_t conn;
    std::queue<xpc_object_t> queue;
  
  public:
    void connect(const char *name);
    void disconnect();
    void send(xpc_object_t message);
    xpc_object_t nextEvent();
};

typedef XpcHandle* XpcHandle_t;

#endif
