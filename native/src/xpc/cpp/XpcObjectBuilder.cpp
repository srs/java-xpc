#include "XpcObjectBuilder.h"

XpcObjectBuilder::XpcObjectBuilder(JNIEnv* env)
{
    this->env = env;
    this->clz = env->FindClass("com/moowork/xpc/object/XpcObjectBuilder");

    jmethodID method = this->env->GetMethodID(this->clz, "<init>", "()V");
    this->builder = env->NewObject(this->clz, method);
};

void XpcObjectBuilder::add(xpc_object_t source) {
    xpc_type_t type = xpc_get_type(source);

    if (type == XPC_TYPE_INT64) {
        this->longValue(xpc_int64_get_value(source));
    } else if (type == XPC_TYPE_UINT64) {
        this->longValue(xpc_uint64_get_value(source));
    } else if (type == XPC_TYPE_STRING) {
        this->stringValue(xpc_string_get_string_ptr(source));
    } else if (type == XPC_TYPE_DICTIONARY) {
        this->dictionary(source);
    } else if (type == XPC_TYPE_ARRAY) {
        this->array(source);
    } else if (type == XPC_TYPE_DATA) {
        this->dataValue((int8_t*)xpc_data_get_bytes_ptr(source), xpc_data_get_length(source));
    } else if (type == XPC_TYPE_UUID) {
        this->uuidValue((int8_t*)xpc_uuid_get_bytes(source), sizeof(uuid_t));
    } else if (type == XPC_TYPE_BOOL) {
        this->booleanValue(xpc_bool_get_value(source));
    }
};

jobject XpcObjectBuilder::build() {
    jmethodID method = this->env->GetMethodID(this->clz, "build", "()Lcom/moowork/xpc/object/XpcObject;");
    return this->env->CallObjectMethod(builder, method);
};

void XpcObjectBuilder::key(const char* value) {
    jstring key = this->env->NewStringUTF(value);
    jmethodID method = this->env->GetMethodID(this->clz, "key", "(Ljava/lang/String;)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallObjectMethod(builder, method, key);
};

void XpcObjectBuilder::end() {
    jmethodID method = this->env->GetMethodID(this->clz, "end", "()Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallObjectMethod(builder, method);
};

void XpcObjectBuilder::longValue(long value) {
    jmethodID method = this->env->GetMethodID(this->clz, "longValue", "(J)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallObjectMethod(builder, method, value);
};

void XpcObjectBuilder::stringValue(const char* value) {
    jstring str = this->env->NewStringUTF(value);
    jmethodID method = this->env->GetMethodID(this->clz, "stringValue", "(Ljava/lang/String;)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallVoidMethod(builder, method, str);
};

void XpcObjectBuilder::booleanValue(bool value) {
    jmethodID method = this->env->GetMethodID(this->clz, "booleanValue", "(Z)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallVoidMethod(builder, method, value);
};

void XpcObjectBuilder::dataValue(const int8_t *value, int len) {
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, value);

    jmethodID method = this->env->GetMethodID(this->clz, "dataValue", "([B)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallVoidMethod(builder, method, array);
};

void XpcObjectBuilder::uuidValue(const int8_t *value, int len) {
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, value);

    jmethodID method = this->env->GetMethodID(this->clz, "uuidValue", "([B)Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallVoidMethod(builder, method, array);
};

void XpcObjectBuilder::dictionary(xpc_object_t source) {
    jmethodID method = this->env->GetMethodID(this->clz, "dictionary", "()Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallObjectMethod(builder, method);

    xpc_dictionary_apply(source, ^bool(const char *key, xpc_object_t value) {
        this->key(key);
        this->add(value);
        return true;
    });

    this->end();
};

void XpcObjectBuilder::array(xpc_object_t source) {
    jmethodID method = this->env->GetMethodID(this->clz, "array", "()Lcom/moowork/xpc/object/XpcObjectBuilder;");
    this->env->CallObjectMethod(builder, method);

    xpc_array_apply(source, ^bool(size_t index, xpc_object_t value) {
        this->add(value);
        return true;
    });

    this->end();
};
