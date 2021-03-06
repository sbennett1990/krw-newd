/*	$OpenBSD$	*/

/*
 * Copyright (c) 2004 Esben Norby <norby@openbsd.org>
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define CONF_FILE		"/etc/newd.conf"
#define	NEWD_SOCKET		"/var/run/newd.sock"
#define NEWD_USER		"_newd"

#define OPT_VERBOSE	0x00000001
#define OPT_VERBOSE2	0x00000002
#define OPT_NOACTION	0x00000004

#define NEWD_MAXTEXT		256
#define NEWD_MAXGROUPNAME	16

enum {
	PROC_MAIN,
	PROC_ENGINE,
	PROC_FRONTEND
} newd_process;

static const char * const log_procnames[] = {
	"main",
	"engine",
	"frontend"
};

struct imsgev {
	struct imsgbuf	 ibuf;
	void		(*handler)(int, short, void *);
	struct event	 ev;
	short		 events;
};

enum imsg_type {
	IMSG_NONE,
	IMSG_CTL_LOG_VERBOSE,
	IMSG_CTL_RELOAD,
	IMSG_CTL_SHOW_ENGINE_INFO,
	IMSG_CTL_SHOW_FRONTEND_INFO,
	IMSG_CTL_SHOW_MAIN_INFO,
	IMSG_CTL_END,
	IMSG_RECONF_CONF,
	IMSG_RECONF_GROUP,
	IMSG_RECONF_END,
	IMSG_SOCKET_IPC
};

struct group {
	LIST_ENTRY(group)	 entry;
	char		name[NEWD_MAXGROUPNAME];
	int		yesno;
	int		integer;
	int		group_v4_bits;
	int		group_v6_bits;
	struct in_addr	group_v4address;
	struct in6_addr	group_v6address;
};

struct newd_conf {
	int		yesno;
	int		integer;
	char		global_text[NEWD_MAXTEXT];
	LIST_HEAD(, group)	group_list;
};

struct ctl_frontend_info {
	int		yesno;
	int		integer;
	char		global_text[NEWD_MAXTEXT];
};

struct ctl_engine_info {
	char		name[NEWD_MAXGROUPNAME];
	int		yesno;
	int		integer;
	int		group_v4_bits;
	int		group_v6_bits;
	struct in_addr	group_v4address;
	struct in6_addr	group_v6address;
};

struct ctl_main_info {
	char		text[NEWD_MAXTEXT];
};

extern uint32_t	 cmd_opts;

/* newd.c */
void	main_imsg_compose_frontend(int, pid_t, void *, uint16_t);
void	main_imsg_compose_engine(int, pid_t, void *, uint16_t);
void	merge_config(struct newd_conf *, struct newd_conf *);
void	imsg_event_add(struct imsgev *);
int	imsg_compose_event(struct imsgev *, uint16_t, uint32_t, pid_t,
	    int, void *, uint16_t);

struct newd_conf       *config_new_empty(void);
void			config_clear(struct newd_conf *);

/* printconf.c */
void	print_config(struct newd_conf *);

/* parse.y */
struct newd_conf	*parse_config(char *);
int			 cmdline_symset(char *);
