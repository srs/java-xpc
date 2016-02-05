#ifndef ___XPC_OBJECT_BUILDER_H___
#define ___XPC_OBJECT_BUILDER_H___

#include <jni.h>
#include <xpc/xpc.h>

class XpcObjectBuilder {
  private:
    JNIEnv* env;
    jobject builder;
    jclass clz;
    
    void key(const char*);
    void longValue(long);
    void stringValue(const char*);
    void dataValue(const int8_t*, int len);
    void uuidValue(const int8_t*, int len);
    void booleanValue(bool);
    void dictionary(xpc_object_t);
    void array(xpc_object_t);
    void end();

  public:
    XpcObjectBuilder(JNIEnv* env);
    void add(xpc_object_t);
    jobject build();
};

#endif
