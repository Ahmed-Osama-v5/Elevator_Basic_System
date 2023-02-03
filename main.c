#include <stdio.h>
#include <stdint.h>

//#define DEBUG_

#define MAX_FLOORS  8

#define ARRAY_NOT_EMPTY     0
#define ARRAY_EMPTY         1

typedef enum
{
    IDLE,
    MOVING
}Elevator_State_t;

typedef enum
{
    MOVING_UP,
    MOVING_DOWN
}Elevator_Direction_t;

typedef enum
{
    UP_COLLECTIVE,
    DOWN_COLLECTIVE
}Elevator_Collection_Direction_t;

typedef enum
{
    NO_CALL,
    INTERNAL_CALL,
    EXTERNAL_CALL
}Call_t;

typedef struct
{
    uint8_t                             current_floor;
    uint8_t                             destination_floor;
    Elevator_Direction_t                direction;
    Elevator_State_t                    state;
    Elevator_Collection_Direction_t     collectionDirection;
    uint8_t                             floors[MAX_FLOORS];
    Call_t                              callType[MAX_FLOORS];
    uint8_t                             safety_flag;
}Elevator_str;

Elevator_str elevator;

void elevator_init(Elevator_str *elevator);
void elevator_call(Elevator_str *elevator, uint8_t floor, Call_t callType);
void elevator_move(Elevator_str *elevator);
void elevator_check_safety(Elevator_str *elevator);
uint8_t isEmpty(uint8_t arr[], uint8_t size);
void traverseArray(uint8_t arr[], uint8_t size);

const char* direction_ToStr(Elevator_Direction_t direction);
const char* collectionDirection_ToStr(Elevator_Collection_Direction_t collectionDirection);

int main(void)
{
    elevator_init(&elevator);

    /*************************************/
    /************* Test cases ************/
    printf("Test cases:\n\n");
    uint8_t callFloor = 0;
    
    /***** TC1: handle internal call *****/
    printf("TC1: handle internal call>\n");
    /* Initial conditions */
    elevator.collectionDirection = DOWN_COLLECTIVE;
    elevator.current_floor = 2;
    callFloor = 5;
    elevator.state = IDLE;

    elevator_call(&elevator, callFloor, INTERNAL_CALL);
    printf("Current floor: %d, Destination: %d\n", elevator.current_floor, elevator.destination_floor);
    elevator_move(&elevator);
    while(elevator.state != IDLE)
    {
        elevator_move(&elevator);
    }
    printf("Result: ");
    if(elevator.current_floor == callFloor)
        printf("Pass\n");
    else
        printf("Fail\n");
    printf("TC1: Done\n\n");
    /*************************************/
    
    /***** TC2: handle external call *****/
    printf("TC2: handle external call>\n");
    /* Initial conditions */
    elevator.collectionDirection = DOWN_COLLECTIVE;
    elevator.current_floor = 2;
    callFloor = 7;
    elevator.state = IDLE;

    elevator_call(&elevator, callFloor, EXTERNAL_CALL);
    printf("Current floor: %d, Destination: %d\n", elevator.current_floor, elevator.destination_floor);
    elevator_move(&elevator);
    while(elevator.state != IDLE)
    {
        elevator_move(&elevator);
    }
    printf("Result: ");
    if(elevator.current_floor == callFloor)
        printf("Pass\n");
    else
        printf("Fail\n");
    printf("TC2: Done\n\n");
    /*************************************/
    
    /***** TC3: handle external with internal calls *****/
    printf("TC3: handle external with internal calls>\n");
    /* Initial conditions */
    elevator.collectionDirection = DOWN_COLLECTIVE;
    elevator.current_floor = 2;
    callFloor = 7;
    elevator.state = IDLE;

    elevator_call(&elevator, callFloor, EXTERNAL_CALL);
    printf("Current floor: %d, Destination: %d\n", elevator.current_floor, elevator.destination_floor);
    elevator_move(&elevator);
    while(elevator.state != IDLE)
    {
        if(elevator.current_floor == 7)
        { 
            callFloor = 4;
            elevator_call(&elevator, callFloor, INTERNAL_CALL);
        }
        else if(elevator.current_floor == 3)
        { 
            callFloor = 5;
            elevator_call(&elevator, callFloor, INTERNAL_CALL);
            callFloor = 6;
            elevator_call(&elevator, callFloor, INTERNAL_CALL);
        }
        else
        {
            /* Do nothing */
        }
        elevator_move(&elevator);
    }
    printf("Result: ");
    if(elevator.current_floor == callFloor)
        printf("Pass\n");
    else
        printf("Fail\n");
    printf("TC3: Done\n\n");
    /*************************************/

    /*************************************/


    return 0;
}

uint8_t isEmpty(uint8_t arr[], uint8_t size)
{
    for(uint8_t i=0;i<size;i++)
    {
        if(arr[i] == 1)
        {
            #ifdef DEBUG_
            printf("ARRAY_NOT_EMPTY()\n");
            #endif
            return ARRAY_NOT_EMPTY;
        }
        else
        {
            /* Do nothing */
        }
    }
    #ifdef DEBUG_
    printf("ARRAY_EMPTY()\n");
    #endif
    return ARRAY_EMPTY;
}

void traverseArray(uint8_t arr[], uint8_t size)
{
    printf("traverseArray()\n");
    for(uint8_t i=0;i<size;i++)
    {
        printf("arr[%d]: %d\n", i, arr[i]);
    }
}

uint8_t getDestination(Elevator_str *elevator)
{
    #ifdef DEBUG_
    printf("getDestination()\n");
    #endif
    if(elevator->direction == MOVING_UP)
    {
        #ifdef DEBUG_
        printf("MOVING_UP()\n");
        #endif
        for(uint8_t i=elevator->current_floor;i<MAX_FLOORS;i++)
        {
            #ifdef DEBUG_
            printf("current: %d, i: %d, floors: %d\n", elevator->current_floor, i, elevator->floors[i]);
            #endif
            if((i > elevator->current_floor) && (elevator->floors[i] == 1))
            {
                //elevator->direction = MOVING_UP;
                #ifdef DEBUG_
                printf("Dest: %d\n", i);
                #endif
                return i;
            }
        }

        #ifdef DEBUG_
        printf("MOVING_DOWN()\n");
        #endif
        /* Check if destination is down */
        for(uint8_t i=elevator->current_floor;i>0;i--)
        {
            #ifdef DEBUG_
            printf("current: %d, i: %d\n", elevator->current_floor, i);
            #endif
            if((i < elevator->current_floor) && (elevator->floors[i] == 1))
            {
                elevator->direction = MOVING_DOWN;
                #ifdef DEBUG_
                printf("Dest: %d\n", i);
                #endif
                return i;
            }
        }
        if((0 < elevator->current_floor) && (elevator->floors[0] == 1))
        {
            elevator->direction = MOVING_DOWN;
            #ifdef DEBUG_
            printf("Dest: %d\n", 0);
            #endif
            return 0;
        }
    }
    else
    {
        /* Check if destination is down */
        for(uint8_t i=elevator->current_floor;i>0;i--)
        {
            #ifdef DEBUG_
            printf("current: %d, i: %d\n", elevator->current_floor, i);
            #endif
            if((i < elevator->current_floor) && (elevator->floors[i] == 1))
            {
                //elevator->direction = MOVING_DOWN;
                return i;
            }
        }
        if((0 < elevator->current_floor) && (elevator->floors[0] == 1))
        {
            elevator->direction = MOVING_DOWN;
            #ifdef DEBUG_
            printf("Dest: %d\n", 0);
            #endif
            return 0;
        }

        for(uint8_t i=elevator->current_floor;i<MAX_FLOORS;i++)
        {
            #ifdef DEBUG_
            printf("current: %d, i: %d\n", elevator->current_floor, i);
            #endif
            if((i > elevator->current_floor) && (elevator->floors[i] == 1))
            {
                elevator->direction = MOVING_UP;
                return i;
            }
        }
    }
    return 0xFF;
}

void elevator_init(Elevator_str *elevator)
{
    printf("elevator_init()\n");
    elevator->current_floor = 4;
    elevator->collectionDirection = DOWN_COLLECTIVE;
    elevator->state = IDLE;
    elevator->direction = MOVING_UP;
    elevator->destination_floor = 0;
    elevator->safety_flag = 0;
    for (uint8_t i = 0; i < MAX_FLOORS; i++) {
        elevator->floors[i] = 0;
        elevator->callType[i] = NO_CALL;
    }
}

void elevator_call(Elevator_str *elevator, uint8_t floor, Call_t callType)
{
    printf("Called floor: %d\n", floor);
    elevator->state = MOVING;
    #ifdef DEBUG_
    printf("elevator_call()\n");
    traverseArray(elevator->floors, MAX_FLOORS);
    #endif
    /* Check if array is empty */
    if(isEmpty(elevator->floors, MAX_FLOORS) == ARRAY_EMPTY)
    {
        #ifdef DEBUG_
        printf("elevator_call() ARRAY_EMPTY\n");
        #endif
        elevator->floors[floor] = 1;
        elevator->callType[floor] = callType;
        elevator->destination_floor = floor;
        if(elevator->destination_floor > elevator->current_floor)
        {
            elevator->direction = MOVING_UP;
        }
        else
        {
            elevator->direction = MOVING_DOWN;
        }
    }
    else
    {
        #ifdef DEBUG_
        printf("elevator_call() ARRAY_NOT_EMPTY\n");
        printf("Floor: %d\n", floor);
        #endif
        elevator->floors[floor] = 1;
        if(elevator->direction == MOVING_UP)
        {
            if((floor > elevator->current_floor) && (floor < elevator->destination_floor))
            {
                if(elevator->collectionDirection == UP_COLLECTIVE)
                {
                    elevator->destination_floor = floor;
                }
                else
                {
                    if(elevator->callType[floor] == INTERNAL_CALL)
                    {
                        elevator->destination_floor = floor;
                    }
                    #ifdef DEBUG_
                    printf("Down_Collective_Moving_UP()\n");
                    printf("Floor: %d\n", floor);
                    traverseArray(elevator->floors, MAX_FLOORS);
                    #endif
                }
            }
            else
            {
                #ifdef DEBUG_
                printf("Down_Collective_Moving_UP()\n");
                printf("Floor: %d\n", floor);
                traverseArray(elevator->floors, MAX_FLOORS);
                #endif
            }
        }
        else
        {
            if((floor < elevator->current_floor) && (floor > elevator->destination_floor))
            {
                if(elevator->collectionDirection == DOWN_COLLECTIVE)
                {
                    elevator->destination_floor = floor;
                }
                else
                {
                    if(elevator->callType[floor] == INTERNAL_CALL)
                    {
                        elevator->destination_floor = floor;
                    }
                }
            }
            else
            {
                /* Do nothing */
            }
        }
    }
}

void elevator_move(Elevator_str *elevator)
{
    #ifdef DEBUG_
    printf("elevator_move()\n");
    #endif
    if(elevator != NULL)
    {
        if(elevator->current_floor != elevator->destination_floor)
        {
            printf("Direction: %s\n", direction_ToStr(elevator->direction));
            printf("current_Floor: %d, Destination: %d\n", elevator->current_floor, elevator->destination_floor);
            if(elevator->direction == MOVING_UP)
            {
                elevator->current_floor++;
            }
            else
            {
                elevator->current_floor--;
            }
        }
        else
        {
            printf("current_Floor: %d, Destination: %d\n", elevator->current_floor, elevator->destination_floor);
            printf("Stopped at floor# %d\n", elevator->current_floor);
            elevator->floors[elevator->current_floor] = 0;
            uint8_t destination = getDestination(elevator);
            if(destination == 0xFF)
            {
                elevator->state = IDLE;
                printf("State: IDLE\n");
            }
            else
            {
                elevator->destination_floor = destination;
                printf("Direction: %s\n", direction_ToStr(elevator->direction));
                printf("current_Floor: %d, Destination: %d\n", elevator->current_floor, elevator->destination_floor);
            }
        }
    }
    else
    {
        printf("elevator_move: Null ptr!\n");
    }
}

void elevator_check_safety(Elevator_str *elevator)
{
    if (elevator->safety_flag == 1) {
        elevator->state = IDLE;
        elevator->destination_floor = elevator->current_floor;
    }
}


const char* direction_ToStr(Elevator_Direction_t direction)
{
    switch (direction)
    {
    case MOVING_UP:
        return "MOVING_UP";
    case MOVING_DOWN:
        return "MOVING_DOWN";
    default:
        return "DEFAULT";
    }
}

const char* collectionDirection_ToStr(Elevator_Collection_Direction_t collectionDirection)
{
    switch (collectionDirection)
    {
    case UP_COLLECTIVE:
        return "UP_COLLECTIVE";
    case DOWN_COLLECTIVE:
        return "DOWN_COLLECTIVE";
    default:
        return "DEFAULT";
    }
}