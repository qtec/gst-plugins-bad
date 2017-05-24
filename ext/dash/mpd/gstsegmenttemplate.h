/* GStreamer
 * Copyright (C) 2011 Flumotion S.L. <devteam@flumotion.com>
 * Copyright (C) 2011 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gstsegmentinfo.h:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_SEGMENT_TEMPLATE_H__
#define __GST_SEGMENT_TEMPLATE_H__

#include <glib.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

typedef struct _GstSegmentTemplate GstSegmentTemplate;

struct _GstSegmentTemplate
{
  guint64 fragment_duration;
  gint32 start_index;
  gchar *init_template;
  gchar *media_template;
};

GstSegmentTemplate *gst_segment_template_new           (guint fragment_duration);

void gst_segment_template_free                     (GstSegmentTemplate *segment_template);

void gst_segment_template_set_media_template        (GstSegmentTemplate *segment_template,
                                                const gchar* template);

void gst_segment_template_set_init_template         (GstSegmentTemplate *segmenttemplate,
                                                 const gchar* template);

gboolean gst_segment_template_render               (GstSegmentTemplate * segment_template,
                                                xmlTextWriterPtr writer);

gint32 gst_segment_template_get_average_bitrate    (GstSegmentTemplate * info);

#endif
