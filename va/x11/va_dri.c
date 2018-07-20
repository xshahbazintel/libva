/* $XFree86: xc/lib/GL/dri/XF86dri.c,v 1.13 2002/10/30 12:51:25 alanh Exp $ */
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright 2000 VA Linux Systems, Inc.
Copyright 2007 Intel Corporation
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors:
 *   Kevin E. Martin <martin@valinux.com>
 *   Jens Owen <jens@tungstengraphics.com>
 *   Rickard E. (Rik) Faith <faith@valinux.com>
 *
 */

/* THIS IS NOT AN X CONSORTIUM STANDARD */

#define NEED_REPLIES
#include <X11/Xlibint.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/extutil.h>
#include "va_dristr.h"

#define PUBLIC

static XExtensionInfo _va_dri_info_data;
static XExtensionInfo *va_dri_info = &_va_dri_info_data;
static char va_dri_extension_name[] = VA_DRINAME;

#define VA_DRICheckExtension(dpy,i,val) \
  XextCheckExtension (dpy, i, va_dri_extension_name, val)

/*****************************************************************************
 *                                                                           *
 *			   private utility routines                          *
 *                                                                           *
 *****************************************************************************/

static int close_display(Display *dpy, XExtCodes *extCodes);
static /* const */ XExtensionHooks va_dri_extension_hooks = {
    NULL,				/* create_gc */
    NULL,				/* copy_gc */
    NULL,				/* flush_gc */
    NULL,				/* free_gc */
    NULL,				/* create_font */
    NULL,				/* free_font */
    close_display,			/* close_display */
    NULL,				/* wire_to_event */
    NULL,				/* event_to_wire */
    NULL,				/* error */
    NULL,				/* error_string */
};

static XEXT_GENERATE_FIND_DISPLAY (find_display, va_dri_info, 
				   va_dri_extension_name, 
				   &va_dri_extension_hooks, 
				   0, NULL)

static XEXT_GENERATE_CLOSE_DISPLAY (close_display, va_dri_info)


/*****************************************************************************
 *                                                                           *
 *		    public XFree86-DRI Extension routines                    *
 *                                                                           *
 *****************************************************************************/

#if 0
#include <stdio.h>
#define TRACE(msg)  fprintf(stderr,"XF86DRI%s\n", msg);
#else
#define TRACE(msg)
#endif


PUBLIC Bool VA_DRIQueryExtension (dpy, event_basep, error_basep)
    Display *dpy;
    int *event_basep, *error_basep;
{
    XExtDisplayInfo *info = find_display (dpy);

    TRACE("QueryExtension...");
    if (XextHasExtension(info)) {
	*event_basep = info->codes->first_event;
	*error_basep = info->codes->first_error;
        TRACE("QueryExtension... return True");
	return True;
    } else {
        TRACE("QueryExtension... return False");
	return False;
    }
}

PUBLIC Bool VA_DRIQueryVersion(dpy, majorVersion, minorVersion, patchVersion)
    Display* dpy;
    int* majorVersion; 
    int* minorVersion;
    int* patchVersion;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIQueryVersionReply rep;
    xVA_DRIQueryVersionReq *req;

    TRACE("QueryVersion...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIQueryVersion, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIQueryVersion;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("QueryVersion... return False");
	return False;
    }
    *majorVersion = rep.majorVersion;
    *minorVersion = rep.minorVersion;
    *patchVersion = rep.patchVersion;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("QueryVersion... return True");
    return True;
}

PUBLIC Bool VA_DRIQueryDirectRenderingCapable(dpy, screen, isCapable)
    Display* dpy;
    int screen;
    Bool* isCapable;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIQueryDirectRenderingCapableReply rep;
    xVA_DRIQueryDirectRenderingCapableReq *req;

    TRACE("QueryDirectRenderingCapable...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIQueryDirectRenderingCapable, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIQueryDirectRenderingCapable;
    req->screen = screen;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("QueryDirectRenderingCapable... return False");
	return False;
    }
    *isCapable = rep.isCapable;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("QueryDirectRenderingCapable... return True");
    return True;
}

PUBLIC Bool VA_DRIOpenConnection(dpy, screen, hSAREA, busIdString)
    Display* dpy;
    int screen;
    drm_handle_t * hSAREA;
    char **busIdString;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIOpenConnectionReply rep;
    xVA_DRIOpenConnectionReq *req;

    TRACE("OpenConnection...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIOpenConnection, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIOpenConnection;
    req->screen = screen;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("OpenConnection... return False");
	return False;
    }

    *hSAREA = rep.hSAREALow;
    if (sizeof(drm_handle_t) == 8) {
       int shift = 32; /* var to prevent warning on next line */
       *hSAREA |= ((drm_handle_t) rep.hSAREAHigh) << shift;
    }

    if (rep.length) {
        if (!(*busIdString = (char *)Xcalloc(rep.busIdStringLength + 1, 1))) {
            _XEatData(dpy, ((rep.busIdStringLength+3) & ~3));
            UnlockDisplay(dpy);
            SyncHandle();
            TRACE("OpenConnection... return False");
            return False;
        }
	_XReadPad(dpy, *busIdString, rep.busIdStringLength);
    } else {
        *busIdString = NULL;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("OpenConnection... return True");
    return True;
}

PUBLIC Bool VA_DRIAuthConnection(dpy, screen, magic)
    Display* dpy;
    int screen;
    drm_magic_t magic;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIAuthConnectionReq *req;
    xVA_DRIAuthConnectionReply rep;

    TRACE("AuthConnection...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIAuthConnection, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIAuthConnection;
    req->screen = screen;
    req->magic = magic;
    rep.authenticated = 0;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse) || !rep.authenticated) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("AuthConnection... return False");
	return False;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("AuthConnection... return True");
    return True;
}

PUBLIC Bool VA_DRICloseConnection(dpy, screen)
    Display* dpy;
    int screen;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRICloseConnectionReq *req;

    TRACE("CloseConnection...");

    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRICloseConnection, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRICloseConnection;
    req->screen = screen;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("CloseConnection... return True");
    return True;
}

PUBLIC Bool VA_DRIGetClientDriverName(dpy, screen, ddxDriverMajorVersion, 
	ddxDriverMinorVersion, ddxDriverPatchVersion, clientDriverName)
    Display* dpy;
    int screen;
    int* ddxDriverMajorVersion;
    int* ddxDriverMinorVersion;
    int* ddxDriverPatchVersion;
    char** clientDriverName;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIGetClientDriverNameReply rep;
    xVA_DRIGetClientDriverNameReq *req;

    TRACE("GetClientDriverName...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIGetClientDriverName, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIGetClientDriverName;
    req->screen = screen;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("GetClientDriverName... return False");
	return False;
    }

    *ddxDriverMajorVersion = rep.ddxDriverMajorVersion;
    *ddxDriverMinorVersion = rep.ddxDriverMinorVersion;
    *ddxDriverPatchVersion = rep.ddxDriverPatchVersion;

    if (rep.length) {
        if (!(*clientDriverName = (char *)Xcalloc(rep.clientDriverNameLength + 1, 1))) {
            _XEatData(dpy, ((rep.clientDriverNameLength+3) & ~3));
            UnlockDisplay(dpy);
            SyncHandle();
            TRACE("GetClientDriverName... return False");
            return False;
        }
	_XReadPad(dpy, *clientDriverName, rep.clientDriverNameLength);
    } else {
        *clientDriverName = NULL;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("GetClientDriverName... return True");
    return True;
}

PUBLIC Bool VA_DRICreateContextWithConfig(dpy, screen, configID, context,
	hHWContext)
    Display* dpy;
    int screen;
    int configID;
    XID* context;
    drm_context_t * hHWContext;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRICreateContextReply rep;
    xVA_DRICreateContextReq *req;

    TRACE("CreateContext...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRICreateContext, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRICreateContext;
    req->visual = configID;
    req->screen = screen;
    *context = XAllocID(dpy);
    req->context = *context;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("CreateContext... return False");
	return False;
    }
    *hHWContext = rep.hHWContext;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("CreateContext... return True");
    return True;
}

PUBLIC Bool VA_DRICreateContext(dpy, screen, visual, context, hHWContext)
    Display* dpy;
    int screen;
    Visual* visual;
    XID* context;
    drm_context_t * hHWContext;
{
    return VA_DRICreateContextWithConfig( dpy, screen, visual->visualid,
					   context, hHWContext );
}

PUBLIC Bool VA_DRIDestroyContext( __DRInativeDisplay * ndpy, int screen, 
    __DRIid context )
{
    Display * const dpy = (Display *) ndpy;
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIDestroyContextReq *req;

    TRACE("DestroyContext...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIDestroyContext, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIDestroyContext;
    req->screen = screen;
    req->context = context;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("DestroyContext... return True");
    return True;
}

PUBLIC Bool VA_DRICreateDrawable( __DRInativeDisplay * ndpy, int screen, 
    __DRIid drawable, drm_drawable_t * hHWDrawable )
{
    Display * const dpy = (Display *) ndpy;
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRICreateDrawableReply rep;
    xVA_DRICreateDrawableReq *req;

    TRACE("CreateDrawable...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRICreateDrawable, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRICreateDrawable;
    req->screen = screen;
    req->drawable = drawable;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("CreateDrawable... return False");
	return False;
    }
    *hHWDrawable = rep.hHWDrawable;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("CreateDrawable... return True");
    return True;
}

PUBLIC Bool VA_DRIDestroyDrawable( __DRInativeDisplay * ndpy, int screen,
    __DRIid drawable )
{
    Display * const dpy = (Display *) ndpy;
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIDestroyDrawableReq *req;

    TRACE("DestroyDrawable...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIDestroyDrawable, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIDestroyDrawable;
    req->screen = screen;
    req->drawable = drawable;
    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("DestroyDrawable... return True");
    return True;
}

PUBLIC Bool VA_DRIGetDrawableInfo(Display* dpy, int screen, Drawable drawable,
    unsigned int* index, unsigned int* stamp,
    int* X, int* Y, int* W, int* H,
    int* numClipRects, drm_clip_rect_t ** pClipRects,
    int* backX, int* backY,
    int* numBackClipRects, drm_clip_rect_t ** pBackClipRects )
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIGetDrawableInfoReply rep;
    xVA_DRIGetDrawableInfoReq *req;
    int total_rects;

    TRACE("GetDrawableInfo...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIGetDrawableInfo, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIGetDrawableInfo;
    req->screen = screen;
    req->drawable = drawable;

    if (!_XReply(dpy, (xReply *)&rep, 1, xFalse)) 
    {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("GetDrawableInfo... return False");
	return False;
    }
    *index = rep.drawableTableIndex;
    *stamp = rep.drawableTableStamp;
    *X = (int)rep.drawableX;
    *Y = (int)rep.drawableY;
    *W = (int)rep.drawableWidth;
    *H = (int)rep.drawableHeight;
    *numClipRects = rep.numClipRects;
    total_rects = *numClipRects;

    *backX = rep.backX;
    *backY = rep.backY;
    *numBackClipRects = rep.numBackClipRects;
    total_rects += *numBackClipRects;

#if 0
    /* Because of the fix in Xserver/GL/dri/xf86dri.c, this check breaks
     * backwards compatibility (Because of the >> 2 shift) but the fix
     * enables multi-threaded apps to work.
     */
    if (rep.length !=  ((((SIZEOF(xVA_DRIGetDrawableInfoReply) - 
		       SIZEOF(xGenericReply) + 
		       total_rects * sizeof(drm_clip_rect_t)) + 3) & ~3) >> 2)) {
        _XEatData(dpy, rep.length);
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("GetDrawableInfo... return False");
        return False;
    }
#endif

    if (*numClipRects) {
       int len = sizeof(drm_clip_rect_t) * (*numClipRects);

       *pClipRects = (drm_clip_rect_t *)Xcalloc(len, 1);
       if (*pClipRects) 
	  _XRead(dpy, (char*)*pClipRects, len);
    } else {
        *pClipRects = NULL;
    }

    if (*numBackClipRects) {
       int len = sizeof(drm_clip_rect_t) * (*numBackClipRects);

       *pBackClipRects = (drm_clip_rect_t *)Xcalloc(len, 1);
       if (*pBackClipRects) 
	  _XRead(dpy, (char*)*pBackClipRects, len);
    } else {
        *pBackClipRects = NULL;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("GetDrawableInfo... return True");
    return True;
}

PUBLIC Bool VA_DRIGetDeviceInfo(dpy, screen, hFrameBuffer, 
	fbOrigin, fbSize, fbStride, devPrivateSize, pDevPrivate)
    Display* dpy;
    int screen;
    drm_handle_t * hFrameBuffer;
    int* fbOrigin;
    int* fbSize;
    int* fbStride;
    int* devPrivateSize;
    void** pDevPrivate;
{
    XExtDisplayInfo *info = find_display (dpy);
    xVA_DRIGetDeviceInfoReply rep;
    xVA_DRIGetDeviceInfoReq *req;

    TRACE("GetDeviceInfo...");
    VA_DRICheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(VA_DRIGetDeviceInfo, req);
    req->reqType = info->codes->major_opcode;
    req->driReqType = X_VA_DRIGetDeviceInfo;
    req->screen = screen;
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
        TRACE("GetDeviceInfo... return False");
	return False;
    }

    *hFrameBuffer = rep.hFrameBufferLow;
    if (sizeof(drm_handle_t) == 8) {
       int shift = 32; /* var to prevent warning on next line */
       *hFrameBuffer |= ((drm_handle_t) rep.hFrameBufferHigh) << shift;
    }

    *fbOrigin = rep.framebufferOrigin;
    *fbSize = rep.framebufferSize;
    *fbStride = rep.framebufferStride;
    *devPrivateSize = rep.devPrivateSize;

    if (rep.length) {
        if (!(*pDevPrivate = (void *)Xcalloc(rep.devPrivateSize, 1))) {
            _XEatData(dpy, ((rep.devPrivateSize+3) & ~3));
            UnlockDisplay(dpy);
            SyncHandle();
            TRACE("GetDeviceInfo... return False");
            return False;
        }
	_XRead(dpy, (char*)*pDevPrivate, rep.devPrivateSize);
    } else {
        *pDevPrivate = NULL;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    TRACE("GetDeviceInfo... return True");
    return True;
}

PUBLIC Bool VA_DRIOpenFullScreen(dpy, screen, drawable)
    Display* dpy;
    int screen;
    Drawable drawable;
{
    /* This function and the underlying X protocol are deprecated.
     */
    (void) dpy;
    (void) screen;
    (void) drawable;
    return False;
}

PUBLIC Bool VA_DRICloseFullScreen(dpy, screen, drawable)
    Display* dpy;
    int screen;
    Drawable drawable;
{
    /* This function and the underlying X protocol are deprecated.
     */
    (void) dpy;
    (void) screen;
    (void) drawable;
    return True;
}

#undef TRACE

