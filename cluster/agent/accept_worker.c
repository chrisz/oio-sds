/*
OpenIO SDS cluster
Copyright (C) 2014 Worldine, original work as part of Redcurrant
Copyright (C) 2015 OpenIO, modified as part of OpenIO Software Defined Storage

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <metautils/lib/metautils.h>

#include "./agent.h"
#include "./accept_worker.h"
#include "./gridagent.h"
#include "./message.h"
#include "./io_scheduler.h"
#include "./read_message_worker.h"

int
accept_worker(worker_t *worker, GError **error)
{
	int fd;
	struct sockaddr_un remote;
	socklen_t remote_len = 0;
	worker_t *mes_worker = NULL;

	(void) error;

	memset(&remote, 0, sizeof(struct sockaddr_un));
	remote_len = sizeof(remote);

	fd = accept_nonblock(worker->data.fd, (struct sockaddr *)&remote, &remote_len);
	if (fd < 0) {
		ERROR("Failed to accept on socket %d : %s", worker->data.fd, strerror(errno));
		return 1;
	}

	DEBUG("Accepting new connection on sock %d", worker->data.fd);

	/* Create worker */
	mes_worker = g_malloc0(sizeof(worker_t));
	mes_worker->func = read_message_size_worker;
	mes_worker->clean = NULL;
	mes_worker->timeout.activity = worker->data.sock_timeout;
	mes_worker->data.fd = fd;
	mes_worker->data.sock_timeout = worker->data.sock_timeout;
	mes_worker->data.session = NULL;

	GError *e = NULL;
	if (!add_fd_to_io_scheduler(mes_worker, EPOLLIN, &e)) {
		ERROR("Failed to add fd to io_scheduler : %s", e->message);
		g_clear_error(&e);
		g_free(mes_worker);
	}

	return(1);
}

