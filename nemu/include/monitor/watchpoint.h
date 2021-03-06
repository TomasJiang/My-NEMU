#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	char str[256];
	uint32_t oldvalue;
} WP;

WP *head_wp();
WP *new_wp();
void free_wp(WP *wp);

#endif
