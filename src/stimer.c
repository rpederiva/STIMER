/******************************************************************************/
// File : stimer.c
// Function : Interface for software timer
//            We use a real hardware timer to manage multiple software timer
/******************************************************************************/
// Version : 1.0    Date : 18/06/2021    Author : tartifP
// Modification :
//     - Original
/******************************************************************************/

/******************************************************************************/
/*                                 Includes                                   */
/******************************************************************************/

#include "stimer.h" // Header
#include <stddef.h> // NULL

/******************************************************************************/
/*                              Global variables                              */
/******************************************************************************/

static STIMER_DATA timerData;  // Task data

/******************************************************************************/
/*                        Public function implementation                      */
/******************************************************************************/

/**
 * Initialize software timer module
 * We ne to know the real timeout periode of the hardware timer
 * @param _callbackTime : callback time of hardware timer (in micro second)
 */
void STIMER_Initialize(uint32_t _callbackTime)
{
    timerData.callbackTime  = _callbackTime;
    timerData.timerList     = NULL;             // List init
}

/**
 * Software time task
 * Check if any software timer elapsed and call callback if need
 */
void STIMER_Tasks(void)
{
    STIMER_HANDLE* timer = timerData.timerList;
    
    // Check all timer of the list
    while(timer != NULL)
    {
        // If counter reach 0 -> callback
        if(timer->tmr_cnt == 0)
        {
            timer->callback();
            timer->tmr_cnt = timer->tmr_need;
            
            // If its a one shot timer, remove it from the list
            if(timer->mode == STIMER_MODE_ONESHOT)
            {
                STIMER_Remove(timer);
            }
        }
    }
}

/**
 * Real timer interrupt handler
 * Just update software timer counter
 */
void STIMER_InterruptHandler(void)
{
    STIMER_HANDLE* timer = timerData.timerList;
    while(timer != NULL)
    {
        if(timer->tmr_cnt)
            timer->tmr_cnt--;
        timer = timer->next;
    }
}

/**
 * Add a timer
 * @param _handle : Handle of the software timer
 * @param _mode : Timer mode
 * @param _delays_ms : Delais between callback timer
 * @param _callback : callback function
 * @return true if ok
 */
bool STIMER_Create(STIMER_HANDLE* _handle, STIMER_MODE _mode, uint32_t _delays_ms, void (*_callback)(void))
{
    // Configure timer handle
    _handle->callback = _callback;
    _handle->mode = _mode;
    _handle->tmr_need = _delays_ms / (timerData.callbackTime/1000);
    _handle->tmr_cnt = _handle->tmr_need;
    
    // Add it to the list
    STIMER_HANDLE* timer = timerData.timerList;
    
    // if list empty
    if(timer == NULL)
        timerData.timerList = _handle;
    
    // Else find last place
    else
    {
        while(timer->next != NULL)
            timer = timer->next;
        timer->next = _handle;
    }
    
    return true;
}

/**
 * Find a timer and remove it from the list
 * @param _handle : handle of the timer to remove
 * @return true if remove, else any other case (not find)
 */
bool STIMER_Remove(STIMER_HANDLE* _handle)
{
    // Add it to the list
    STIMER_HANDLE* timer = timerData.timerList;
    
    // if list empty
    if(timer == NULL)
    {
        return false;
    }
    
    // Else find timer
    else
    {
        while((timer->next == _handle) || (timer->next != NULL))
            timer = timer->next;
        
        // if we found id
        if(timer->next != NULL)
        {
            // if last timer of the list
            if(_handle->next == NULL)
                timer->next = NULL;
            else
                timer->next = _handle->next;
        }
        return true;        
    }
}