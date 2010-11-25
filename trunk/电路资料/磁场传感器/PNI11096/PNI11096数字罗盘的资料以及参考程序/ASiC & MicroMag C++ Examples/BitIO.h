#pragma once

#define BIT(x) (1UL << x)
#define BITRANGE(x,y) (0xFFFFFFFF >> (x) << (x) << (31-(y)) >> (31-(y)))
#define BITHIGH( set, bit ) ( (bool)(set & BIT(bit)) )
#define BITLOW( set, bit ) ( !((bool)(set & BIT(bit))) )
#define SETBIT( set, bit ) ( set |= BIT(bit))
#define CLRBIT( set, bit ) ( set &= ~BIT(bit))
#define SETBITFLAG( set, bit, flag) if(flag) SETBIT(set,bit); else CLRBIT(set,bit);
#define SETMASK( set, map ) ( set |= map )
#define CLRMASK( set, map ) ( set &= ~map )
#define MASKHIGH( set, map ) ( (bool)(set & map) )
#define MASKLOW( set, map ) ( !((bool)(set & map)) )
#define INVERTMASK( set, map ) ( set ^= map )