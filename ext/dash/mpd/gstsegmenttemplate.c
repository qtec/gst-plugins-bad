/* GStreamer
 * Copyright (C) 2011 Flumotion S.L. <devteam@flumotion.com>
 * Copyright (C) 2011 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gstsegmentinfo.c:
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

#include "gstsegmenttemplate.h"
#include "gstxmlhelper.h"

GstSegmentTemplate *
gst_segment_template_new (guint fragment_duration)
{
  GstSegmentTemplate *info;

  info = g_new0 (GstSegmentTemplate, 1);
  info->fragment_duration = fragment_duration;
  info->start_index = 0;
  info->init_template = NULL;
  info->media_template = NULL;

  return info;
}

void
gst_segment_template_free (GstSegmentTemplate * info)
{
  g_return_if_fail (info != NULL);

  if (info->init_template != NULL)
    g_free (info->init_template);

  if (info->media_template != NULL)
    g_free (info->media_template);

  g_free (info);
}

void
gst_segment_template_set_media_template (GstSegmentTemplate * ele,
    const gchar * template)
{
  g_return_if_fail (ele != NULL);
  g_return_if_fail (template != NULL);

  if (ele->media_template != NULL)
    g_free (ele->media_template);

  ele->media_template = g_strdup (template);
}

void
gst_segment_template_set_init_template (GstSegmentTemplate * ele,
    const gchar * template)
{
  g_return_if_fail (ele != NULL);
  g_return_if_fail (template != NULL);

  if (ele->init_template != NULL)
    g_free (ele->init_template);

  ele->init_template = g_strdup (template);
}

gint32
gst_segment_template_get_average_bitrate (GstSegmentTemplate * info)
{
  return 0;
}

gboolean
gst_segment_template_render (GstSegmentTemplate * info, xmlTextWriterPtr writer)
{
  /* Start SegmentTemplate */
  if (!gst_media_presentation_start_element (writer, "SegmentTemplate"))
    return FALSE;
  if (!gst_media_presentation_write_uint32_attribute (writer, "timescale",
          1000))
    return FALSE;
  if (!gst_media_presentation_write_uint32_attribute (writer, "duration", 3000))
    //info->fragment_duration / GST_MSECOND))
    return FALSE;
  if (!gst_media_presentation_write_uint32_attribute (writer, "startNumber",
          info->start_index))
    return FALSE;

  /* Write initialization segment */
  if (!gst_media_presentation_write_string_attribute (writer, "initialization",
          info->init_template))
    return FALSE;

  if (!gst_media_presentation_write_string_attribute (writer, "media",
          info->media_template))
    return FALSE;

  /* End SegmentTemplate */
  if (!gst_media_presentation_end_element (writer))
    return FALSE;

  return TRUE;
}
