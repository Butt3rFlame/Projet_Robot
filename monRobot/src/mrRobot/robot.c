/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
  

#include "robot.h"
#include "mrpiz.h"
#include <stdio.h>

#define LIMIT_SENSOR_VALUE 20

typedef struct{
    SensorState sensorState;
    int robotSpeed;
} Robot;

static Robot defaultRobot = {{NO_BUMP}, 0};
static Robot *robot = &defaultRobot;

extern void Robot_start()
{
    if(mrpiz_init() != 0){
        printf("Problème d'initialisation du simulateur MrPiz \n");
    }
}

extern void Robot_stop()
{
    Robot_setWheelsVelocity(0, 0);
}

extern void Robot_new()
{
    robot = (Robot*) malloc(sizeof(Robot));
}

extern void Robot_free()
{
    mrpiz_close();
    free(robot);
}

extern int Robot_getRobotSpeed()
{
    return robot->robotSpeed;
}

extern SensorState Robot_getSensorState()
{
    if(mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT) < 0){
        printf("Problème de communication avec le capteur gauche \n");
    }
    else if(mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER) < 0){
        printf("Problème de communication avec le capteur central \n");
    }
    else if(mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT) < 0){
        printf("Problème de communication avec le capteur droit \n");
    }
    else if((0 <= mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT) && mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT) < LIMIT_SENSOR_VALUE) 
            || (0 <= mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER) && mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER)< LIMIT_SENSOR_VALUE)
            || (0 <= mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT) && mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT) < LIMIT_SENSOR_VALUE))
    {
        robot->sensorState.collision = BUMPED;
    }
    else{
        robot->sensorState.collision  = NO_BUMP;
    }
    return robot->sensorState;
}

extern void Robot_setWheelsVelocity(int mr, int ml)
{
    if(mrpiz_motor_set(MRPIZ_MOTOR_LEFT, ml) != 0){
        printf("Problème de commande du moteur gauche \n");
    }
    if(mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, mr) != 0){
        printf("Problème de commande du moteur droit \n");
    }
    robot->robotSpeed = (mr + ml)/2;
}
