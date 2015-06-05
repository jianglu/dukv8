//
// Created by Jiang Lu on 6/3/15.
//

#include "dukv8/debug.h"
#include "dukv8/local.h"
#include "dukv8/value.h"
#include "dukv8/integer.h"
#include "dukv8/object.h"
#include "dukv8/context.h"
#include "dukv8/function.h"

namespace v8 {

/*static*/
bool Debug::SetDebugEventListener(EventCallback that, Handle<Value> data) {
    TODO();
    return false;
}

/*static*/
bool Debug::SetDebugEventListener2(EventCallback2 that, Handle<Value> data) {
    TODO();
    return false;
}

/*static*/
bool Debug::SetDebugEventListener(v8::Handle<v8::Object> that, Handle<Value> data) {
    TODO();
    return false;
}

/*static*/
void Debug::DebugBreak(Isolate *isolate) { TODO(); }

/*static*/
void Debug::CancelDebugBreak(Isolate *isolate) { TODO(); }

/*static*/
void Debug::DebugBreakForCommand(ClientData *data, Isolate *isolate) { TODO(); }

/*static*/
void Debug::SetMessageHandler(MessageHandler handler, bool message_handler_thread) { TODO(); }

/*static*/
void Debug::SetMessageHandler2(MessageHandler2 handler) { TODO(); }

/*static*/
void Debug::SendCommand(const uint16_t *command, int length, ClientData *client_data,
                                   Isolate *isolate) { TODO(); }

/*static*/
void Debug::SetHostDispatchHandler(HostDispatchHandler handler, int period) { TODO(); }

/*static*/
void Debug::SetDebugMessageDispatchHandler(DebugMessageDispatchHandler handler,
                                                      bool provide_locker) { TODO(); }

/*static*/
Local<Value> Debug::Call(v8::Handle<v8::Function> fun, Handle<Value> data) {
    TODO();
    return Local<Value>();
}

/*static*/
Local<Value> Debug::GetMirror(v8::Handle<v8::Value> obj) {
    TODO();
    return Local<Value>();
}

/*static*/
bool Debug::EnableAgent(const char *name, int port, bool wait_for_connection) {
    TODO();
    return false;
}

/*static*/
void Debug::DisableAgent() { TODO(); }

/*static*/
void Debug::ProcessDebugMessages() { TODO(); }

/*static*/
Local<Context> Debug::GetDebugContext() {
    TODO();
    return Local<Context>();
}

/*static*/
void Debug::SetLiveEditEnabled(bool enable, Isolate *isolate) { TODO(); }
}