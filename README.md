# STIMER
Software timer implementation for embedded devices.

## Introduction
On embedded devices (STM32, PIC, etc.) you need lot of timer.
Sometime, you dont have enought timer or you want a simpler way to deal with them.

One solution is to use a software timer interface. It use only one hardware timer and propose you to create multiple software timer.

## Usage
What you need:
- 1 Hardware timer
- 1 main loop (while(1))
- enought memory space for variable allocation

### Initialisation
First of all you must call this function. It take only one parameter who is the real callback time of the hardware timer (typical recommended : 1ms)

```
STIMER_Initialize(uint32_t _callbackTime); // Call this function before main loop
```

You also need to configure a real hardware timer callback. For exemple on PIC18 with MCC Timer library :

```
// We use TMR0
// Call this function before timer start and while(1)
TMR0_SetInterruptHandler(STIMER_InterruptHandler);
```

Finaly, start the hardware timer if is not already running.

### Running the software timer
In the main loop, call this funcion at every loop to check the software timer status and call their callback if need
```
while(1)
{
  ...
  // Process software timer
  STIMER_Tasks();
  ...
}
```

### Create and delete a software timer

#### Create
First of all, you need to define a callback function for your timer. For example a simple handle for toggle led :

```
void MyInterruptHandle(void)
{
  LED_Toggle();
}
```

In a second time, register your timer by creating a variable and register it. For example, if I want a perdiodic timer of 1ms I write :

```
...
STIMER_HANDLE stimerHandle;
STIMER_Create(&stimerHandle, STIMER_MODE_PERIODIC, 1000, MyInterruptHandle);
...
```

#### Delete

To delete a timer (just need for perdiodic one) juste write :
```
...
STIMER_Remove(&stimerHandle);
...
```


