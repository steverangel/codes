#ifndef CONGESTION_CONTROLLER_CORE_H
#define CONGESTION_CONTROLLER_CORE_H

/**
 * congestion-controller.h -- Organizing state and behavior for congestion management
 * Neil McGlohon
 * 
 * Copyright (c) 2019 Rensselaer Polytechnic Institute
 */
#include <ross.h>
#define MAX_PATTERN_LEN 16
#define MAX_PORT_COUNT 256

#ifdef __cplusplus
extern "C" {
#endif

// Defines congestion (aggregate of stall)
typedef enum congestion_status
{
    UNCONGESTED = 0,
    CONGESTED = 1
} congestion_status;

typedef enum congestion_change
{
    TO_DECONGESTION = 0,
    TO_CONGESTION = 1
} congestion_change;

// Like congestion but on a per port or NIC basis
typedef enum stall_status
{
    NOT_STALLED = 0,
    STALLED = 1
} stalled_status;

typedef enum controller_type
{
    CC_SUPERVISOR = 0,
    CC_ROUTER = 1,
    CC_TERMINAL = 2
} controller_type;

/* Enumeration of types of events sent between congestion controllers */
typedef enum cc_event_t
{
    CC_SC_HEARTBEAT = 1001,
    // CC_SC_PERF_REQUEST,
    CC_SC_SIGNAL_NORMAL,
    CC_SC_SIGNAL_ABATE,
    CC_RLC_HEARTBEAT,
    CC_TLC_HEARTBEAT,
    CC_R_PERF_REPORT,
    CC_N_PERF_REPORT,
    CC_WORKLOAD_RANK_COMPLETE,
} cc_event_t;

// Enum for how to determine if the 
// NICS across the network are considered congested
typedef enum nic_congestion_criterion
{
    NIC_CONGESTION_ALPHA = 1
} nic_congestion_criterion; 

// Enum for how to determine if the 
// PORTS across the network are considered congested
typedef enum port_congestion_criterion
{
    PORT_CONGESTION_ALPHA = 1
} port_congestion_criterion;

// Enum for how to determine if the 
// NICS on a terminal are considered stalled
typedef enum nic_stall_criterion
{
    NIC_STALL_ALPHA = 1
} nic_stall_criterion;

// Enum for how to determine if the 
// PORTS on a terminal are considered stalled
typedef enum port_stall_criterion
{
    PORT_STALL_ALPHA = 1
} port_stall_criterion;

typedef struct congestion_control_message
{
    short type; //type of event
    tw_lpid sender_lpid; //lpid of the sender
    unsigned int stalled_count; //used by both routers and terminals, if router then is is the number of port stalled, if terminal nonzero implies congestion
    unsigned long long current_epoch; //the measurement period that these numbers apply to
    unsigned long long rc_value; //rc value storage - dependent on context
    void *rc_ptr; //pointer to dynamic data - dependent on context - NOT FOR USE OUTSIDE OF LP THAT ALLOC'D IT, free'd in either RC or Commit
    void *rc_ptr2;
    int check_sum;
    int rc_network_router_congested;
    int rc_network_terminal_congested;
    tw_stime rc_time;
    unsigned long * stalled_chunks_at_last_epoch;
    unsigned long * total_chunks_at_last_epoch;
} congestion_control_message;

extern const tw_lptype* sc_get_lp_type();
extern void congestion_control_register_lp_type();


// /**
//  * @class RouterLocalController
//  * 
//  * @brief This class organizes state that is ON a router LP that this controller monitors.
//  * No non-pointer-to-LP-state properties that change throughout the simulation should be considered
//  * RC safe. These should be considered invalid upon a context change and are thus named as
//  * private values with a preceeding underscore. This is simply because this state doesn't 
//  * generally need to be accessed except immediately after being set and this is a way to simplify
//  * router RC.
//  * 
//  * 
//  */
// class RouterLocalController {
//     unsigned long* *port_stall_counter; //pointer to 2d array of unsigned longs representing "stalled_chunks", this is a pointer to avoid RC hassle

//     map < unsigned int, bool > _nic_stall_map; //temporary storage for results of generate_nic_stall_count(); 

// public:
//     void stall_detector();

//     void generate_nic_stall_count();

//     bool check_nic_stall_criterion();
// }

// class NodeLocalController {
//     unsigned long nic_stall_counter;
//     unsigned long ejection_counter;

// public:
//     void check_nic_stall_criterion();

//     void congestion_control_abatement();

//     void send_ejection_count();
// }


#ifdef __cplusplus
}
#endif

#endif /* end of include guard */