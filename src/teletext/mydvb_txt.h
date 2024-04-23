#ifndef __MYDVB_TXT__
#define __MYDVB_TXT__

typedef struct {
	int x, y;		/* position of the character */
	unsigned char c;/* character or mosaic */
	int g;			/* character set */
	int dh; 		/* double height */
	int fgcolor; 	/* foreground color */
	int bgcolor; 	/* background color */
} MYDVB_TXT_CELL;

typedef struct {
	int erase;
	int newflash;
	int subtitle;
	int no_header;
	int update;
	int no_sequence;
	int no_display;

	int serial;

	unsigned char data[40];
	

} MYDVB_TELETEXT_PAGE_HEADER;

typedef struct {
	int magazine;
	int page_number;
	int sub_page;
	int ready;

	unsigned char active_lines[24];
	unsigned char data[24][40];

} MYDVB_TELETEXT_PAGE;

typedef struct _mydvb_teletext {
	MYDVB_TELETEXT_PAGE_HEADER header;

	int paginas_len;
	MYDVB_TELETEXT_PAGE **paginas;

	MYDVB_TELETEXT_PAGE *current_page;

	void (*page_ready_callback)(MYDVB_TELETEXT_PAGE *page, void *data);
	void *data_page_ready;

	void (*first_line_callback)(struct _mydvb_teletext *txt, void *data);
	void *data_first_line;

	/* render data */
	MYDVB_TXT_CELL canvas[24][40];
	
} MYDVB_TELETEXT;

unsigned char mydvb_txt_get_char (unsigned char c, int lang);

void mydvb_txt_init (MYDVB_TELETEXT *txt);
void mydvb_txt_end (MYDVB_TELETEXT *txt);
MYDVB_TELETEXT_PAGE *mydvb_txt_find (MYDVB_TELETEXT *txt, int number);
void mydvb_txt_add_page (MYDVB_TELETEXT *txt, MYDVB_TELETEXT_PAGE *page);

MYDVB_TELETEXT_PAGE *mydvb_txt_page_new ();
void mydvb_txt_page_free (MYDVB_TELETEXT_PAGE *page);
void mydvb_txt_page_init (MYDVB_TELETEXT_PAGE *page, int magazine, int page_number);

void mydvb_decode_teletext (MYDVB_TELETEXT *txt, unsigned char *buf, int requested_page);
void mydvb_txt_dec_reverse (unsigned char *buf, size_t n);


void mydvb_txt_erase_page (MYDVB_TELETEXT_PAGE *page);
void mydvb_txt_print_page (MYDVB_TELETEXT_PAGE *page);
void mydvb_txt_print_row (unsigned char *buf);


#endif
