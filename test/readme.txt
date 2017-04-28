<<<<<<< HEAD
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
=======
<<<<<<< HEAD
=======

>>>>>>> origin/master

>>>>>>> origin/master
// Enable and select radio type attached
//#define MY_RADIO_NRF24

#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RF69_868MHZ
#define MY_RFM69_NETWORKID 13
#define MY_RFM69_ENABLE_ENCRYPTION
#define MY_IS_RFM69HW


-----------------------


void 	_begin (void)
 	Node initialisation.

void 	_process (void)
 	Main framework process.

bool 	_processInternalMessages (void)
 	Processes internal messages. More...

void 	_infiniteLoop (void)
 	Puts node to a infinite loop if unrecoverable situation detected.

void 	_registerNode (void)
 	Handles registration request.

bool 	_sendRoute (MyMessage &message)
 	Sends message according to routing table. More...

void 	receive (const MyMessage &message) __attribute__((weak))
 	Callback for incoming messages. More...

void 	receiveTime (unsigned long) __attribute__((weak))
 	Callback for incoming time messages.

void 	presentation (void) __attribute__((weak))
 	Node presenation.

void 	before (void) __attribute__((weak))
 	Called before node initialises.

void 	preHwInit (void) __attribute__((weak))
 	Called before any hwInitialization is done.

void 	setup (void) __attribute__((weak))
 	Called after node initialises but before main loop. More...

void 	loop (void) __attribute__((weak))
 	Main loop. More...
