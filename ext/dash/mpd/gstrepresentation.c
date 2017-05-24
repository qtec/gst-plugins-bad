/* GStreamer
 * Copyright (C) 2011 Flumotion S.L. <devteam@flumotion.com>
 * Copyright (C) 2011 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gstrepresentation.c:
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

#include "gstrepresentation.h"
#include "gstsegmentlist.h"
#include "gstxmlhelper.h"
#include "gstsegmenttemplate.h"

GstRepresentation *
gst_representation_new (gchar * id, StreamType stream_type,
    const gchar * mimeType, guint32 width,
    guint32 height, guint32 parx, guint32 pary, gdouble frameRate,
    gchar * channels, guint32 samplingRate, guint32 bitrate, const gchar * lang,
    gboolean use_ranges, guint fragment_duration, guint64 time_shift,
    const gchar * title)
{
  GstRepresentation *rep;
  GList *langs = NULL;

  rep = g_new0 (GstRepresentation, 1);

  gst_media_common_init (&rep->common);
  rep->common.stream_type = stream_type;
  g_free (rep->common.mimeType);
  rep->common.mimeType = g_strdup (mimeType);
  rep->common.width = width;
  rep->common.height = height;
  rep->common.parx = parx;
  rep->common.pary = pary;
  rep->common.frameRate = frameRate;
  rep->common.numberOfChannels =
      g_list_append (rep->common.numberOfChannels, g_strdup (channels));
  rep->common.samplingRate =
      g_list_append (rep->common.samplingRate, g_strdup_printf ("%d",
          samplingRate));
  langs = g_list_append (langs, g_strdup (lang));
  rep->common.lang = langs;

  rep->id = g_strdup (id);
  rep->bandwidth = bitrate;
  rep->startWithRAP = TRUE;
  rep->segment_list =
      gst_segment_list_new (fragment_duration, use_ranges, time_shift);
  rep->segment_template = gst_segment_template_new (fragment_duration);
  rep->title = g_strdup (title);
  return rep;
}

void
gst_representation_free (GstRepresentation * rep)
{
  g_return_if_fail (rep != NULL);

  g_free (rep->id);
  gst_media_common_free (&(rep->common));
  gst_segment_list_free (rep->segment_list);
  if (rep->title)
    g_free (rep->title);
  g_free (rep);
}

void
gst_representation_add_media_segment (GstRepresentation * rep,
    GstMediaSegment * segment, GList ** removed_fragments)
{
  g_return_if_fail (rep != NULL);
  g_return_if_fail (segment != NULL);

  gst_segment_list_add_media_segment (rep->segment_list, segment,
      removed_fragments);
  if (rep->bandwidth == 0)
    rep->bandwidth = gst_segment_list_get_average_bitrate (rep->segment_list);
}

void
gst_representation_set_init_segment (GstRepresentation * rep,
    GstMediaSegment * segment)
{
  gchar *str;
  g_return_if_fail (rep != NULL);
  g_return_if_fail (segment != NULL);

  gst_segment_list_set_init_segment (rep->segment_list, segment);

  str = g_strdup_printf ("$RepresentationID$/%s.m4s", rep->title);
  gst_segment_template_set_init_template (rep->segment_template, str);
  g_free (str);

  str =
      g_strdup_printf ("$RepresentationID$/%s_fragment_$Number$.m4s",
      rep->title);
  gst_segment_template_set_media_template (rep->segment_template, str);
  g_free (str);
}

void
gst_representation_add_base_url (GstRepresentation * rep, gchar * url)
{
  g_return_if_fail (rep != NULL);
  g_return_if_fail (url != NULL);

  gst_segment_list_add_base_url (rep->segment_list, url);
}

guint64
gst_representation_get_duration (GstRepresentation * rep)
{
  g_return_val_if_fail (rep != NULL, 0);

  return rep->segment_list->duration;
}

gboolean
gst_representation_render (GstRepresentation * rep, xmlTextWriterPtr writer)
{

  if (!gst_segment_template_render (rep->segment_template, writer))
    return FALSE;

  /* Start Representation */
  if (!gst_media_presentation_start_element (writer, "Representation"))
    return FALSE;

  if (!gst_media_presentation_write_uint32_attribute (writer, "bandwidth",
          rep->bandwidth))
    return FALSE;
  if (!gst_media_presentation_write_string_attribute (writer, "id", rep->id))
    return FALSE;
  if (!gst_media_presentation_write_uint32_attribute (writer, "startWithSAP",
          1))
    return FALSE;
  if (!gst_media_common_render (&rep->common, writer))
    return FALSE;

  if (rep->common.stream_type == STREAM_TYPE_VIDEO) {
    if (!gst_media_presentation_write_string_attribute (writer,
            "codecs", "avc1.640028"))
      return FALSE;
  } else if (rep->common.stream_type == STREAM_TYPE_AUDIO) {
    if (!gst_media_presentation_write_string_attribute (writer,
            "codecs", "mp4a.40.2"))
      return FALSE;
  }

  /* write SegmentList */
#if 0
  if (!gst_segment_list_render (rep->segment_list, writer))
    return FALSE;
#endif

  if (!gst_media_presentation_end_element (writer))
    return FALSE;

  return TRUE;
}
