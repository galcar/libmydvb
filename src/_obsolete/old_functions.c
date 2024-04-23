void mydvb_dvr_redirect (MYDVB *mydvb);
void mydvb_dvr_simple_redirect (MYDVB *mydvb);

off_t mydvb_read (MYDVB *mydvb, char *buf, off_t len);

void mydvb_dvr_redirect (MYDVB *mydvb) {
	MYDVB_PROGRAM *selected_program;
	MYDVB_PROGRAM *p;
	MYDVB_STREAM *s;
	int i,j;

	struct dmx_pes_filter_params dmx_p;

	// we are going to redirect any audio/video stream related to the current vpid
	// first, search program for vpid

	selected_program = NULL;

	for (i=0; i < mydvb->pat->num_programs; i++) {
		p = &mydvb->pat->programs[i];
		for (j=0; j < p->num_streams; j++) {
			s = p->streams[j];
			if (s->pid==mydvb->current_vpid) {
				selected_program = p;
				break;
			}
		}
		if (selected_program != NULL) {
			break;
		}
	}
	if (selected_program == NULL) {
		return;
	}

	for (j=0; j < selected_program->num_streams; j++) {
		s = selected_program->streams[j];
		switch (s->type) {
			case 0x01:
			case 0x02: // video stream
				if (mydvb->fd_filters[DVBE_FILTER_VIDEO]!=-1) {
					// send video to dvr device
					dmx_p.pid = s->pid;
					dmx_p.input = DMX_IN_FRONTEND;
					dmx_p.output = DMX_OUT_TS_TAP;
					dmx_p.pes_type = DMX_PES_VIDEO;
					dmx_p.flags = DMX_IMMEDIATE_START;
					ioctl (mydvb->fd_filters[DVBE_FILTER_VIDEO], DMX_SET_PES_FILTER, &dmx_p);
					printf ("Agregado stream video: %d\n", s->pid);
				}
				break;
			case 0x03:
			case 0x04: // audio stream
				if (mydvb->fd_filters[DVBE_FILTER_AUDIO]!=-1) {
					// send audio to dvr device
					dmx_p.pid = s->pid;
					dmx_p.input = DMX_IN_FRONTEND;
					dmx_p.output = DMX_OUT_TS_TAP;
					dmx_p.pes_type = DMX_PES_AUDIO;
					dmx_p.flags = DMX_IMMEDIATE_START;

					ioctl (mydvb->fd_filters[DVBE_FILTER_AUDIO], DMX_SET_PES_FILTER, &dmx_p);
					printf ("Agregado stream audio: %d\n", s->pid);
				}
				break;
			default:
				// do nothing
		}
	}

}

void mydvb_dvr_simple_redirect (MYDVB *mydvb) {

	struct dmx_pes_filter_params dmx_p;
	struct dmx_sct_filter_params dmx_s;

	// add pat table
	/*
	memset(&dmx_s, 0, sizeof(dmx_s));
	dmx_s.pid = 0; //PAT pid
	dmx_s.filter.filter[0] = 0;
	dmx_s.filter.mask[0] = 0xff;
	dmx_s.timeout = 0;
	dmx_s.flags = DMX_IMMEDIATE_START;

	if (ioctl(mydvb->fd_filters[DVBE_FILTER_INT], DMX_SET_FILTER, &dmx_s)==-1) {
		return -1;
	}
	*/



	if (mydvb->fd_filters[DVBE_FILTER_VIDEO]!=-1 && mydvb->current_vpid!=0) {
		// send video to dvr device
		dmx_p.pid = mydvb->current_vpid;
		dmx_p.input = DMX_IN_FRONTEND;
		dmx_p.output = DMX_OUT_TS_TAP;
		dmx_p.pes_type = DMX_PES_VIDEO;
		dmx_p.flags = DMX_IMMEDIATE_START;

		ioctl (mydvb->fd_filters[DVBE_FILTER_VIDEO], DMX_SET_PES_FILTER, &dmx_p);
	}

	if (mydvb->fd_filters[DVBE_FILTER_AUDIO]!=-1 && mydvb->current_apid!=0) {
		// send audio to dvr device
		dmx_p.pid = mydvb->current_apid;
		dmx_p.input = DMX_IN_FRONTEND;
		dmx_p.output = DMX_OUT_TS_TAP;
		dmx_p.pes_type = DMX_PES_AUDIO;
		dmx_p.flags = DMX_IMMEDIATE_START;

		ioctl (mydvb->fd_filters[DVBE_FILTER_AUDIO], DMX_SET_PES_FILTER, &dmx_p);
	}

}


off_t mydvb_read (MYDVB *mydvb, char *buf, off_t len) {

	struct pollfd pfd[2];
	off_t n = 0;
	unsigned char *aux_buf;
	int r;

	if (mydvb->fd_dvr == -1) {
		return 0;
	}

	// poll for data stream
	pfd[0].fd = mydvb->fd_filters[DVBE_FILTER_INT];
	pfd[0].events = POLLPRI | POLLIN | POLLERR;
	pfd[0].revents = 0;

	// poll for video/audio stream
	pfd[1].fd = mydvb->fd_dvr;
	pfd[1].events = POLLIN | POLLPRI | POLLERR;
	pfd[1].revents = 0;


	r = poll (pfd, 1, 1500);

	if (r > 0) {
		if (pfd[1].revents & POLLPRI || pfd[1].revents & POLLIN) {
			// datos disponibles
			//n = read (mydvb->fd_dvr, mydvb->realtime_buf, MYDVB_BUFFY);
			n = read (mydvb->fd_dvr, mydvb->realtime_buf, len);
			if (n > 0) {
				if (mydvb->ts_status==MYDVB_TIME_SHIFT_RECORDING) {
					// write to fileb
					fileb_write (&mydvb->ts_fb, mydvb->realtime_buf, n);
					aux_buf = mydvb->realtime_buf;
				} else if (mydvb->ts_status==MYDVB_TIME_SHIFT_VIEWING) {
					// read from fileb 
					fileb_read (&mydvb->ts_fb, mydvb->delayed_buf, n);
					aux_buf = mydvb->delayed_buf;
					//write to fileb
					fileb_write (&mydvb->ts_fb, mydvb->realtime_buf, n);
				} else {
					aux_buf = mydvb->realtime_buf;
				}
				// write to buffer
				memcpy (buf, aux_buf, n);
			
				if (mydvb->recording) {
					write (mydvb->fd_record, mydvb->realtime_buf, n);
				}
			}
		} else if (pfd[0].revents & POLLPRI || pfd[0].revents & POLLIN) {
			// pat data
		}
	} else if (r == 0) {
		//time out
		printf ("input_dvbe: timeout getting data\n");
	} else {
		// error
		printf ("input_dvbe: error getting data, %d\n", errno);
	}

	return n;
}

