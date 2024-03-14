// AraEngine Debugger
// (c) 2024 Jerome Lavoie

#ifdef DEBUG

#include <MemoryFree.h>
#include <Arduino.h>
#define debug_init(baud_rt) {Serial.begin(2000000);}

#define debug_raw(str) {Serial.print(F(str));}
#define debug_raw_nl(str) {Serial.println(F(str));}
#define debug_msg(msg) {debug_raw("Message: "); debug_raw_nl(msg);}
#define debug_freemem() {debug_raw("Free memory: "); Serial.println(freeMemory());}

#define debug_assert(var, condition) { \
    if (!condition) {                  \
        debug_raw("Assert failed: ");  \
        debug_raw(#condition);         \
        debug_raw(" -> (");            \
        debug_raw(#var);               \
        debug_raw(" = ");              \
        Serial.print(var);             \
        debug_raw_nl(")");             \
    }                                  \
}                                      \

#else

#define debug_init(baud_rt)
#define debug_raw(str)
#define debug_raw_nl(str)
#define debug_msg(msg)
#define debug_assert(var, condition)
#define debug_freemem()

#endif