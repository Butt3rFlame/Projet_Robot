/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  

#include "adminui.h"
#include "pilot.h"
#include "stdio.h"
#include <termios.h>
#include <unistd.h>
   
#define FORWARD_KEY 'z'

#define BACKWARD_KEY 's'

#define LEFT_KEY 'q'

#define RIGHT_KEY 'd'

#define QUIT_KEY 'a'

#define STOP_KEY ' '

#define SHOW_LOG_KEY 'r'

#define ERASE_LOG_KEY 'e'

#define DEFAULT_POWER 100

static int received_char = ERASE_LOG_KEY;

/**
 * @brief fonction qui affiche les commandes
*/
static void display(void);

/**
 * @brief fonction qui récupère la commande rentrer par l'utilisateur
*/
static void captureChoice(void);

/**
 * @brief fonction pour lancer l'application
*/
static void run(void);

/**
 * @brief fonction pour quitter l'application
*/
static void quit(void);

/**
 * @brief fonction qui transforme l'ordre de direction en vecteur de déplacement
 * 
 * @param dir ordre de direction
*/
VelocityVector translate(Direction dir);

/**
 * @brief fonction qui envoie l'ordre de déplacement au pilote
*/
static void askMvt(const Direction dir);

/**
 * @brief affiche l'etat du robot
*/
static void ask4Log(void);

/**
 * @brief demande l'effacement du texte du terminal
*/
static void askClearLog(void);

/**
 * @brief efface le texte du terminal
*/
static void eraseLog(void);

static void display(void)
{
    printf("Bienvenue sur Robot V1 \n");
    printf("Vous pouvez faire les actions suivantes : \n");
    printf("%c : Aller à gauche \n", LEFT_KEY);
    printf("%c : Aller à droite \n", RIGHT_KEY);
    printf("%c : Avancer \n", FORWARD_KEY);
    printf("%c : Reculer \n", BACKWARD_KEY);
    printf("%c : Stopper \n", STOP_KEY);
    printf("%c : Effacer les log \n", ERASE_LOG_KEY);
    printf("%c : Afficher l'etat du robot \n", SHOW_LOG_KEY);
    printf("%c : Quitter \n", QUIT_KEY);
}

static void captureChoice(void)
{
    switch(received_char){
        
    case FORWARD_KEY:
        askMvt(FORWARD);
        break;

    case LEFT_KEY:
        askMvt(LEFT);
        break;
    
    case RIGHT_KEY:
        askMvt(RIGHT);
        break;
    
    case BACKWARD_KEY:
        askMvt(BACKWARD);
        break;

    case STOP_KEY:
        Pilot_stop();
        break;
        
    case ERASE_LOG_KEY:
        askClearLog();
        break;
    
    case SHOW_LOG_KEY:
        ask4Log();
        break;
    
    case QUIT_KEY:
        quit();
        break;
    
    default:
        break;
    }
}

static void quit(void)
{
    Pilot_stop();
}

VelocityVector translate(Direction direction)
{
    VelocityVector vector = {direction, DEFAULT_POWER};
    return vector;
}

static void askMvt(const Direction dir)
{
    Pilot_setVelocity(translate(dir));
}

static void ask4Log(void)
{
    askClearLog();
    Pilot_check();
    PilotState state = Pilot_getState();
    printf("Etat Robot: Vitesse = %d, Collision = %s", state.speed, state.collision ? "Oui\n":"Non\n");
}

static void askClearLog(void)
{
    eraseLog();
}

static void eraseLog(void)
{
    printf("\033[H\033[J");
    display();
}

static void run(void)
{
    display();
    do
    {
        struct termios oldt, newt;
        
        tcgetattr(STDIN_FILENO, &oldt);
        
        newt = oldt;
        
        newt.c_lflag &= ~(ICANON | ECHO);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        received_char = getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        
        captureChoice();

    } while (received_char != QUIT_KEY);
}


extern void AdminUI_new()
{
    Pilot_new();
}

extern void AdminUI_free()
{
    Pilot_free();
}

extern void AdminUI_start()
{
    Pilot_start();
    run();
}

extern void AdminUI_stop()
{
    Pilot_stop();
}