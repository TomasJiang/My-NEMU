#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

WP *head_wp()
{
	return head;
}

WP *new_wp()
{
	Assert(free_, "No free watchpoint!");
	WP *wp = free_;
	free_ = wp->next;
	wp->next = head;
	head = wp;
	return wp;
}

void free_wp(WP *wp)
{
	WP *p = head;
	if(p == wp)
		head = wp->next;
	while(p)
	{
		if(p->next == wp) {
			p->next = wp->next;
			break;
		}
		p = p->next;
	}
	wp->next = free_;
	free_ = wp;
}
