#ifndef __TABLES_H__
#define __TABLES_H__

#include <mydvb_tables.h>

#include <stdio.h>

MYDVB_PAT *mydvb_new_pat (unsigned char *buffer, size_t len);
MYDVB_PAT *mydvb_new_pat_if_changed (unsigned char *buffer, size_t len, MYDVB_PAT *c_pat);
void mydvb_delete_pat (MYDVB_PAT *pat);

int mydvb_edit_nit (MYDVB_NIT *nit, const unsigned char *buffer, size_t len);
void mydvb_delete_nit (MYDVB_NIT *nit);

int mydvb_edit_program (MYDVB_PROGRAM *pmt, const unsigned char *buffer, size_t len);
void mydvb_delete_program (MYDVB_PROGRAM *pmt);

MYDVB_SDT *mydvb_new_sdt_if_changed (const unsigned char *buffer, size_t len, MYDVB_SDT *c_sdt);
void mydvb_delete_sdt (MYDVB_SDT *sdt);
void free_service_description (void *obj);

void mydvb_new_eit_if_changed (MYDVB_PAT *pat, const unsigned char *buffer, size_t len);

MYDVB_CAT *mydvb_new_cat_if_changed (const unsigned char *buffer, size_t len, MYDVB_CAT *cat);
void mydvb_delete_cat (MYDVB_CAT *cat);

void mydvb_tdt (const unsigned char *buffer, size_t len, MYDVB_TDT *tdt);

void mydvb_save_nit (FILE *f, MYDVB_NIT *nit);

void mydvb_save_program (FILE *f, MYDVB_PROGRAM *program);

void mydvb_save_stream (FILE *f, MYDVB_STREAM *stream);

#endif
