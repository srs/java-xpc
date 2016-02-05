#include <queue>
#include <xpc/xpc.h>
#include "XpcHandle.h"

void XpcHandle::connect(const char *name) {
  this->conn = xpc_connection_create_mach_service(name, dispatch_get_main_queue(), XPC_CONNECTION_MACH_SERVICE_PRIVILEGED);

  xpc_connection_set_event_handler(this->conn, ^(xpc_object_t event) {
    xpc_retain(event);
    this->queue.push(event);
  });
    
  xpc_connection_resume(this->conn);
};

void XpcHandle::disconnect() {
  xpc_connection_suspend(this->conn);
};

void XpcHandle::send(xpc_object_t message) {
  xpc_connection_send_message(this->conn, message);
};

xpc_object_t XpcHandle::nextEvent() {
  if (this->queue.empty()) {
    return NULL; 
  }
  
  xpc_object_t event = this->queue.front();
  this->queue.pop();

  return event;
};
