/*
OpenIO SDS sqlx
Copyright (C) 2015 OpenIO, original work as part of OpenIO Software Defined Storage

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

#ifndef OIO_SDS__sqlx__sqlx_client_h
# define OIO_SDS__sqlx__sqlx_client_h 1

# include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

struct oio_sqlx_output_ctx_s
{
	gint64 changes;
	gint64 total_changes;
	gint64 last_rowid;
};

struct oio_sqlx_batch_s;

void oio_sqlx_batch__destroy (struct oio_sqlx_batch_s *self);

void oio_sqlx_batch__add (struct oio_sqlx_batch_s *self,
		const char *stmt, gchar **params);


struct oio_sqlx_batch_result_s;

void oio_sqlx_batch_result__destroy (struct oio_sqlx_batch_result_s *self);

guint oio_sqlx_batch_result__count_statements (
		struct oio_sqlx_batch_result_s *self);

/* Collect information about the given statement in the result set.
 * @param i_stmt identifies the i-th statement in the set
 * @param pcount will store the number of lines
 * @param out_ctx will be filled with
 */
GError* oio_sqlx_batch_result__get_statement (
		struct oio_sqlx_batch_result_s *self, guint i_stmt,
		guint *out_count, struct oio_sqlx_output_ctx_s *out_ctx);

/* Get the given line in the given statement in the result set
 * @param i_stmt the index of the statement
 * @param i_line the index of the row
 * @return a NULL-erminated array of strings, one for each field in the given
 *         row. All the fields are currently mapped to their textual
 *         representation. BLOBs are not supported yet. */
gchar** oio_sqlx_batch_result__get_row (struct oio_sqlx_batch_result_s *self,
		guint i_stmt, guint i_row);

/* -------------------------------------------------------------------------- */

struct oio_sqlx_client_s;

void oio_sqlx_client__destroy (struct oio_sqlx_client_s *self);

GError * oio_sqlx_client__execute_statement (struct oio_sqlx_client_s *self,
		const char *in_stmt, gchar **in_params,
		struct oio_sqlx_output_ctx_s *out_ctx, gchar ***out_lines);

GError * oio_sqlx_client__execute_batch (struct oio_sqlx_client_s *self,
		struct oio_sqlx_batch_s *batch,
		struct oio_sqlx_batch_result_s **out_result);

/* -------------------------------------------------------------------------- */

struct oio_url_s;

struct oio_sqlx_client_factory_s;

void oio_sqlx_client_factory__destroy (struct oio_sqlx_client_factory_s *self);

GError * oio_sqlx_client_factory__open (struct oio_sqlx_client_factory_s *self,
		struct oio_url_s *u, struct oio_sqlx_client_s **out);

GError * oio_sqlx_client_factory__batch (struct oio_sqlx_client_factory_s *self,
		struct oio_sqlx_batch_s **out);

#ifdef __cplusplus
}
#endif

#endif /*OIO_SDS__sqlx__sqlx_client_h*/
