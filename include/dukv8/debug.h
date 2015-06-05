//
// Created by Jiang Lu on 6/3/15.
//

#ifndef DUKV8_DEBUG_H
#define DUKV8_DEBUG_H

#include <stdint.h>
#include "base.h"
#include "handle.h"

namespace v8 {

enum DebugEvent {
    Break = 1,
    Exception = 2,
    NewFunction = 3,
    BeforeCompile = 4,
    AfterCompile = 5,
    ScriptCollected = 6,
    BreakForCommand = 7
};

class V8EXPORT Debug {
public:
    class ClientData {
    public:
        virtual ~ClientData() { }
    };

    class Message {
    public:
        virtual bool IsEvent() const = 0;
        virtual bool IsResponse() const = 0;
        virtual DebugEvent GetEvent() const = 0;
        virtual bool WillStartRunning() const = 0;
        virtual Handle<Object> GetExecutionState() const = 0;
        virtual Handle<Object> GetEventData() const = 0;
        virtual Handle<String> GetJSON() const = 0;
        virtual Handle<Context> GetEventContext() const = 0;
        virtual ClientData *GetClientData() const = 0;
        virtual ~Message() { }
    };

    class EventDetails {
    public:
        virtual DebugEvent GetEvent() const = 0;
        virtual Handle<Object> GetExecutionState() const = 0;
        virtual Handle<Object> GetEventData() const = 0;
        virtual Handle<Context> GetEventContext() const = 0;
        virtual Handle<Value> GetCallbackData() const = 0;
        virtual ClientData *GetClientData() const = 0;
        virtual ~EventDetails() { }
    };

    typedef void (*EventCallback)(DebugEvent event, Handle<Object> exec_state, Handle<Object> event_data,
                                  Handle<Value> data);
    typedef void (*EventCallback2)(const EventDetails &event_details);
    typedef void (*MessageHandler)(const uint16_t *message, int length, ClientData *client_data);
    typedef void (*MessageHandler2)(const Message &message);
    typedef void (*HostDispatchHandler)();
    typedef void (*DebugMessageDispatchHandler)();
    static bool SetDebugEventListener(EventCallback that, Handle<Value> data = Handle<Value>());
    static bool SetDebugEventListener2(EventCallback2 that, Handle<Value> data = Handle<Value>());
    static bool SetDebugEventListener(v8::Handle<v8::Object> that, Handle<Value> data = Handle<Value>());
    static void DebugBreak(Isolate *isolate = NULL);
    static void CancelDebugBreak(Isolate *isolate = NULL);
    static void DebugBreakForCommand(ClientData *data = NULL, Isolate *isolate = NULL);
    static void SetMessageHandler(MessageHandler handler, bool message_handler_thread = false);
    static void SetMessageHandler2(MessageHandler2 handler);
    static void SendCommand(const uint16_t *command, int length, ClientData *client_data = NULL,
                            Isolate *isolate = NULL);
    static void SetHostDispatchHandler(HostDispatchHandler handler, int period = 100);
    static void SetDebugMessageDispatchHandler(DebugMessageDispatchHandler handler, bool provide_locker = false);
    static Local<Value> Call(v8::Handle<v8::Function> fun, Handle<Value> data = Handle<Value>());
    static Local<Value> GetMirror(v8::Handle<v8::Value> obj);
    static bool EnableAgent(const char *name, int port, bool wait_for_connection = false);
    static void DisableAgent();
    static void ProcessDebugMessages();
    static Local<Context> GetDebugContext();
    static void SetLiveEditEnabled(bool enable, Isolate *isolate = NULL);
};

}

#endif //DUKV8_DEBUG_H
