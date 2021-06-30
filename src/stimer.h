/******************************************************************************/
// File : stimer.h
// Function : Interface for software timer
//            We use a real hardware timer to manage multiple software timer
/******************************************************************************/
// Version : 1.0    Date : 18/06/2021    Author : tartifP
// Modification :
//     - Original
/******************************************************************************/

#ifndef _STIMER_H
#define	_STIMER_H

/******************************************************************************/
/*                                 Includes                                   */
/******************************************************************************/

#include <stdint.h>     //uint
#include <stdbool.h>    // boolean

/******************************************************************************/
/*                              Data definition                               */
/******************************************************************************/

// Timer usage mode
typedef enum {
    STIMER_MODE_ONESHOT,    // Timer mode one-shot
    STIMER_MODE_PERIODIC,   // Timer mode periodic
} STIMER_MODE;

// Structure of timer handle
typedef struct s_STIMER_HANDLE STIMER_HANDLE;
struct s_STIMER_HANDLE {
    STIMER_MODE     mode;               // Timer mode
    uint32_t        tmr_cnt;            // Counter for real timer callback 
    uint32_t        tmr_need;           // real timer callback count need to trig software timer
    void            (*callback)(void);  // Callback function when software timer timeout
    STIMER_HANDLE*  next;               // Next timer handler in the list
};

// Data for STIMER task
typedef struct {
    uint32_t        callbackTime;   // Real timer callback time
    STIMER_HANDLE*  timerList;      // List of software timer handle
} STIMER_DATA;

/******************************************************************************/
/*                            Function prototype                              */
/******************************************************************************/

// Task manage
void STIMER_Initialize(uint32_t _callbackTime);  // Task initialisation
void STIMER_Tasks(void);                         // Task state machine

// Real timer interrupt handler
void STIMER_InterruptHandler(void);

// Create and remove a timer
bool STIMER_Create(STIMER_HANDLE* _handle, STIMER_MODE _mode, uint32_t _delayms, void (*_callback)(void));
bool STIMER_Remove(STIMER_HANDLE* _handle);

#endif	/* _STIMER_H */

