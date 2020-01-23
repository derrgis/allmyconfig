//
//  p1.h
//  
//
//  Created by Yves BAZIN on 27/07/18.
//

#ifndef p1_h
#define p1_h
#define PORT_MASK 0b100011110000111100001101010110100
#define PL (PORT_MASK & 0xFFFF)
#define PH (PORT_MASK & 0xFFFF0000)
#define M1 (( ( PORT_MASK&(PORT_MASK-1) ) ^  PORT_MASK))
#define T1 (  PORT_MASK&(PORT_MASK-1) )
#define T2 (  T1&(T1-1) )
#define T3 (  T2&(T2-1) )
#define T4 (  T3&(T3-1) )
#define T5 (  T4&(T4-1) )
#define T6 (  T5&(T5-1) )
#define T7 (  T6&(T6-1) )
#define T8 (  T7&(T7-1) )
#define T9 (  T8&(T8-1) )
#define T10 (  T9&(T9-1) )
#define T11 (  T10&(T10-1) )
#define T12 (  T11&(T11-1) )
#define T13 (  T12&(T12-1) )
#define T14 (  T13&(T13-1) )
#define T15 (  T14&(T14-1) )
#define T16 (  T15&(T15-1) )


#define M2 (( ( T1&(T1-1) ) ^  T1))
#define M3 (( ( T2&(T2-1) ) ^  T2))
#define M4 (( ( T3&(T3-1) ) ^  T3))
#define M5 (( ( T4&(T4-1) ) ^  T4))
#define M6 (( ( T5&(T5-1) ) ^  T5))
#define M7 (( ( T6&(T6-1) ) ^  T6))
#define M8 (( ( T7&(T7-1) ) ^  T7))
#define M9 (( ( T8&(T8-1) ) ^  T8))
#define M10 (( ( T9&(T9-1) ) ^  T9))
#define M11 (( ( T10&(T10-1) ) ^  T10))
#define M12 (( ( T11&(T11-1) ) ^  T11))
#define M13 (( ( T12&(T12-1) ) ^  T12))
#define M14 (( ( T13&(T13-1) ) ^  T13))
#define M15 (( ( T14&(T14-1) ) ^  T14))
#define M16 (( ( T15&(T15-1) ) ^  T15))
#endif /* p1_h */
