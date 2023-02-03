#include "pilot.h"
#include "robot.h"
/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  

typedef int bool;

typedef struct{
    PilotState state;
    VelocityVector vel;
} Pilot;

typedef enum{
    S_NONE = 0,
    S_IDLE,
    S_RUNNING,
    NB_STATE
} State;

typedef enum{
    E_SET_VELOCITY=0,
    E_CHECK,
    E_STOP,
    NB_EVENT
} Event;

typedef enum{
    A_NONE,
    A_SEND_MVT,
    A_CHECK
} Action;

typedef struct{
    State nextState;
    Action action;
} Transition;

static State currentState;
static Event event;
static Pilot *pilot;

static Transition stateMachine[NB_STATE][NB_EVENT] = {
    [S_IDLE][E_STOP] = {S_IDLE, A_SEND_MVT},
    [S_IDLE][E_SET_VELOCITY] = {S_RUNNING, A_SEND_MVT},

    [S_RUNNING][E_SET_VELOCITY] = {S_RUNNING, A_SEND_MVT},
    [S_RUNNING][E_CHECK] = {S_RUNNING, A_CHECK}
};

static void run(Event event, VelocityVector vel);

static void performAction(Action action);

static void sendMvt(VelocityVector vel);

static bool hasBumped();


extern void Pilot_start()
{
    Robot_start();
    currentState = S_IDLE;
}

extern void Pilot_stop()
{
    Robot_stop();
    currentState = S_IDLE;
}

extern void Pilot_new()
{
    pilot = (Pilot*) malloc(sizeof(Pilot));
    Robot_new();
}

extern void Pilot_free()
{
    free(pilot);
    Robot_free();
}

extern void Pilot_setVelocity(VelocityVector vel)
{
    event = E_SET_VELOCITY;
    pilot->vel = vel;
    run(event, vel);
}

PilotState Pilot_getState()
{
    return pilot->state;
}

void Pilot_check()
{
    pilot->state.collision = hasBumped();
    pilot->state.speed = Robot_getRobotSpeed();
    if (hasBumped()){
        pilot->vel.dir = STOP;
        pilot->vel.power = 0;
        currentState = S_IDLE;
        run(E_SET_VELOCITY, pilot->vel);
    }
}

static void run(Event event, VelocityVector vel)
{
    Action action;
    action = stateMachine[currentState][event].action;
    State nextState = stateMachine[currentState][event].nextState;

    if(nextState != S_NONE){
        currentState = nextState;
        performAction(action);
    }
}

static void performAction(Action action)
{
    switch(action){
        case A_NONE:
            break;
        case A_CHECK:
            Pilot_check();
            break;
        case A_SEND_MVT:
            sendMvt(pilot->vel);
            break;
    }
}

static void sendMvt(VelocityVector vel)
{
    switch(vel.dir){

        case FORWARD:
            Robot_setWheelsVelocity(vel.power, vel.power);
            break;
        
        case LEFT:
            Robot_setWheelsVelocity(vel.power/2, -(vel.power/2));
            break;
        
        case RIGHT:
            Robot_setWheelsVelocity(-(vel.power/2), vel.power/2);
            break;

        case BACKWARD:
            Robot_setWheelsVelocity(-vel.power, -vel.power);
            break;

        case STOP:
            Pilot_stop();
            currentState = S_IDLE;
            break;
    }
}

static bool hasBumped()
{
    return (Robot_getSensorState().collision == BUMPED);
}
