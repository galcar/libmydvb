#ifndef _MHP_TABLES_H_
#define _MHP_TABLES_H_

#include <mydvb_mhp_tables.h>

MYDVB_AIT *mydvb_new_ait ();
MYDVB_AIT *mydvb_new_ait_if_changed (const unsigned char *buffer, size_t len, MYDVB_AIT *ait);
void mydvb_delete_ait (MYDVB_AIT *ait);

#endif
