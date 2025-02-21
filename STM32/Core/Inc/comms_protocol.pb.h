/* Automatically generated nanopb header */
/* Generated by nanopb-1.0.0-dev */

#ifndef PB_COMMS_PROTOCOL_PROTO_COMMS_PROTOCOL_PB_H_INCLUDED
#define PB_COMMS_PROTOCOL_PROTO_COMMS_PROTOCOL_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _comms_protocol_Device {
    comms_protocol_Device_RPI = 0,
    comms_protocol_Device_STM32 = 1
} comms_protocol_Device;

/* Enum for message types */
typedef enum _comms_protocol_MessageType {
    comms_protocol_MessageType_UNKNOWN = 0,
    comms_protocol_MessageType_COMMAND = 1,
    comms_protocol_MessageType_RESPONSE = 2,
    comms_protocol_MessageType_STATUS = 3
} comms_protocol_MessageType;

typedef enum _comms_protocol_CommandType {
    comms_protocol_CommandType_FLIP_PAGE = 0,
    comms_protocol_CommandType_RESET = 1
} comms_protocol_CommandType;

typedef enum _comms_protocol_DeviceState {
    comms_protocol_DeviceState_INIT = 0,
    comms_protocol_DeviceState_RUNNING = 1,
    comms_protocol_DeviceState_STOPPED = 2
} comms_protocol_DeviceState;

/* Enum for response codes */
typedef enum _comms_protocol_ResponseCode {
    comms_protocol_ResponseCode_SUCCESS = 0,
    comms_protocol_ResponseCode_ERROR = 1,
    comms_protocol_ResponseCode_INVALID_COMMAND = 2,
    comms_protocol_ResponseCode_TIMEOUT = 3
} comms_protocol_ResponseCode;

/* Struct definitions */
typedef struct _comms_protocol_Status {
    comms_protocol_DeviceState state;
    uint32_t ticks;
} comms_protocol_Status;

typedef struct _comms_protocol_Command {
    comms_protocol_CommandType command_type; /* Type of command */
    pb_size_t which_payload;
    union _comms_protocol_Command_payload {
        pb_callback_t defaultpayload; /*  */
    } payload;
} comms_protocol_Command;

/* Response message to return results or acknowledgments */
typedef struct _comms_protocol_Response {
    comms_protocol_ResponseCode response_code; /* Status of the command */
    pb_callback_t payload; /* Data returned from the command */
} comms_protocol_Response;

typedef struct _comms_protocol_CommsMessage {
    uint32_t sequence_number;
    comms_protocol_MessageType type;
    comms_protocol_Device device;
    pb_size_t which_msg;
    union _comms_protocol_CommsMessage_msg {
        comms_protocol_Command command;
        comms_protocol_Response response;
        comms_protocol_Status status;
    } msg;
} comms_protocol_CommsMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _comms_protocol_Device_MIN comms_protocol_Device_RPI
#define _comms_protocol_Device_MAX comms_protocol_Device_STM32
#define _comms_protocol_Device_ARRAYSIZE ((comms_protocol_Device)(comms_protocol_Device_STM32+1))

#define _comms_protocol_MessageType_MIN comms_protocol_MessageType_UNKNOWN
#define _comms_protocol_MessageType_MAX comms_protocol_MessageType_STATUS
#define _comms_protocol_MessageType_ARRAYSIZE ((comms_protocol_MessageType)(comms_protocol_MessageType_STATUS+1))

#define _comms_protocol_CommandType_MIN comms_protocol_CommandType_FLIP_PAGE
#define _comms_protocol_CommandType_MAX comms_protocol_CommandType_RESET
#define _comms_protocol_CommandType_ARRAYSIZE ((comms_protocol_CommandType)(comms_protocol_CommandType_RESET+1))

#define _comms_protocol_DeviceState_MIN comms_protocol_DeviceState_INIT
#define _comms_protocol_DeviceState_MAX comms_protocol_DeviceState_STOPPED
#define _comms_protocol_DeviceState_ARRAYSIZE ((comms_protocol_DeviceState)(comms_protocol_DeviceState_STOPPED+1))

#define _comms_protocol_ResponseCode_MIN comms_protocol_ResponseCode_SUCCESS
#define _comms_protocol_ResponseCode_MAX comms_protocol_ResponseCode_TIMEOUT
#define _comms_protocol_ResponseCode_ARRAYSIZE ((comms_protocol_ResponseCode)(comms_protocol_ResponseCode_TIMEOUT+1))

#define comms_protocol_CommsMessage_type_ENUMTYPE comms_protocol_MessageType
#define comms_protocol_CommsMessage_device_ENUMTYPE comms_protocol_Device

#define comms_protocol_Status_state_ENUMTYPE comms_protocol_DeviceState

#define comms_protocol_Command_command_type_ENUMTYPE comms_protocol_CommandType

#define comms_protocol_Response_response_code_ENUMTYPE comms_protocol_ResponseCode


/* Initializer values for message structs */
#define comms_protocol_CommsMessage_init_default {0, _comms_protocol_MessageType_MIN, _comms_protocol_Device_MIN, 0, {comms_protocol_Command_init_default}}
#define comms_protocol_Status_init_default       {_comms_protocol_DeviceState_MIN, 0}
#define comms_protocol_Command_init_default      {_comms_protocol_CommandType_MIN, 0, {{{NULL}, NULL}}}
#define comms_protocol_Response_init_default     {_comms_protocol_ResponseCode_MIN, {{NULL}, NULL}}
#define comms_protocol_CommsMessage_init_zero    {0, _comms_protocol_MessageType_MIN, _comms_protocol_Device_MIN, 0, {comms_protocol_Command_init_zero}}
#define comms_protocol_Status_init_zero          {_comms_protocol_DeviceState_MIN, 0}
#define comms_protocol_Command_init_zero         {_comms_protocol_CommandType_MIN, 0, {{{NULL}, NULL}}}
#define comms_protocol_Response_init_zero        {_comms_protocol_ResponseCode_MIN, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define comms_protocol_Status_state_tag          2
#define comms_protocol_Status_ticks_tag          3
#define comms_protocol_Command_command_type_tag  1
#define comms_protocol_Command_defaultpayload_tag 2
#define comms_protocol_Response_response_code_tag 1
#define comms_protocol_Response_payload_tag      2
#define comms_protocol_CommsMessage_sequence_number_tag 1
#define comms_protocol_CommsMessage_type_tag     2
#define comms_protocol_CommsMessage_device_tag   3
#define comms_protocol_CommsMessage_command_tag  4
#define comms_protocol_CommsMessage_response_tag 5
#define comms_protocol_CommsMessage_status_tag   6

/* Struct field encoding specification for nanopb */
#define comms_protocol_CommsMessage_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   sequence_number,   1) \
X(a, STATIC,   SINGULAR, UENUM,    type,              2) \
X(a, STATIC,   SINGULAR, UENUM,    device,            3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (msg,command,msg.command),   4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (msg,response,msg.response),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (msg,status,msg.status),   6)
#define comms_protocol_CommsMessage_CALLBACK NULL
#define comms_protocol_CommsMessage_DEFAULT NULL
#define comms_protocol_CommsMessage_msg_command_MSGTYPE comms_protocol_Command
#define comms_protocol_CommsMessage_msg_response_MSGTYPE comms_protocol_Response
#define comms_protocol_CommsMessage_msg_status_MSGTYPE comms_protocol_Status

#define comms_protocol_Status_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    state,             2) \
X(a, STATIC,   SINGULAR, UINT32,   ticks,             3)
#define comms_protocol_Status_CALLBACK NULL
#define comms_protocol_Status_DEFAULT NULL

#define comms_protocol_Command_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    command_type,      1) \
X(a, CALLBACK, ONEOF,    BYTES,    (payload,defaultpayload,payload.defaultpayload),   2)
#define comms_protocol_Command_CALLBACK pb_default_field_callback
#define comms_protocol_Command_DEFAULT NULL

#define comms_protocol_Response_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    response_code,     1) \
X(a, CALLBACK, SINGULAR, BYTES,    payload,           2)
#define comms_protocol_Response_CALLBACK pb_default_field_callback
#define comms_protocol_Response_DEFAULT NULL

extern const pb_msgdesc_t comms_protocol_CommsMessage_msg;
extern const pb_msgdesc_t comms_protocol_Status_msg;
extern const pb_msgdesc_t comms_protocol_Command_msg;
extern const pb_msgdesc_t comms_protocol_Response_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define comms_protocol_CommsMessage_fields &comms_protocol_CommsMessage_msg
#define comms_protocol_Status_fields &comms_protocol_Status_msg
#define comms_protocol_Command_fields &comms_protocol_Command_msg
#define comms_protocol_Response_fields &comms_protocol_Response_msg

/* Maximum encoded size of messages (where known) */
/* comms_protocol_CommsMessage_size depends on runtime parameters */
/* comms_protocol_Command_size depends on runtime parameters */
/* comms_protocol_Response_size depends on runtime parameters */
#define COMMS_PROTOCOL_PROTO_COMMS_PROTOCOL_PB_H_MAX_SIZE comms_protocol_Status_size
#define comms_protocol_Status_size               8

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
