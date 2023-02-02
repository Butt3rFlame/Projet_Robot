#include "pilot.h"
#include "robot.h"
/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  

typedef int bool;

typedef struct{
    PilotState state;
    VelocityVector vel;
} Pilot;

typedef enum{
    IDLE=0,
    RUNNING
} MachineState;

typedef enum{
    EVENT_SET_VELOCITY,
    EVENT_CHECK,
} Event;

static MachineState machineState;
static Event event;
static Pilot *pilot;


static void run(Event event, VelocityVector vel);

static void sendMvt(VelocityVector vel);

static bool hasBumped();


extern void Pilot_start()
{
    Robot_start();
    machineState = IDLE;
}

extern void Pilot_stop()
{
    Robot_stop();
    machineState = IDLE;
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
    event = EVENT_SET_VELOCITY;
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
    event = EVENT_CHECK;
    run(event, pilot->vel);
}

static void run(Event event, VelocityVector vel)
{
    switch (machineState){
        default:
        case IDLE:
            if(event == EVENT_SET_VELOCITY){
                if(vel.dir == STOP || pilot->state.collision){
                    Pilot_stop();
                }
                else{
                    machineState = RUNNING;
                }
            }
        case RUNNING:
            if(event == EVENT_SET_VELOCITY){
                if(vel.dir == STOP){
                    machineState = IDLE;
                }
                else{
                    sendMvt(vel);
                }
            }
            else if(event == EVENT_CHECK){
                if(pilot->state.collision){
                    Pilot_stop();
                    machineState = IDLE;
                }
            }
    }
}

void sendMvt(VelocityVector vel)
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

        default:
            Pilot_stop();
            break;
    }
}

static bool hasBumped()
{
    return (Robot_getSensorState().collision == BUMPED);
}
