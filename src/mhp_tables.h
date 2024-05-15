/*
 * This file is part of the libmydvb distribution (https://github.com/galcar/libmydvb).
 * Copyright (c) 2024 G. Alcaraz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _MHP_TABLES_H_
#define _MHP_TABLES_H_

#include <mydvb_mhp_tables.h>

MYDVB_AIT *mydvb_new_ait ();
MYDVB_AIT *mydvb_new_ait_if_changed (const unsigned char *buffer, size_t len, MYDVB_AIT *ait);
void mydvb_delete_ait (MYDVB_AIT *ait);

#endif
