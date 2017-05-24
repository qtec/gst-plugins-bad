
#ifndef ____gst_adaptive_marshal_MARSHAL_H__
#define ____gst_adaptive_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:STRING,STRING (gstadaptive-marshal.list:1) */
extern void __gst_adaptive_marshal_VOID__STRING_STRING (GClosure     *closure,
                                                        GValue       *return_value,
                                                        guint         n_param_values,
                                                        const GValue *param_values,
                                                        gpointer      invocation_hint,
                                                        gpointer      marshal_data);

/* VOID:BOXED (gstadaptive-marshal.list:2) */
#define __gst_adaptive_marshal_VOID__BOXED	g_cclosure_marshal_VOID__BOXED

G_END_DECLS

#endif /* ____gst_adaptive_marshal_MARSHAL_H__ */

