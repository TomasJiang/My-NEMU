#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmova-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmova-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmova_r2rm)


#define DATA_BYTE 2
#include "cmovo-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovo-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmovo_r2rm)

#define DATA_BYTE 2
#include "cmovs-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovs-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmovs_r2rm)

#define DATA_BYTE 2
#include "cmovns-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovns-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmovns_r2rm)

#define DATA_BYTE 2
#include "cmove-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmove-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmove_r2rm)

#define DATA_BYTE 2
#include "cmovge-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovge-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmovge_r2rm)
