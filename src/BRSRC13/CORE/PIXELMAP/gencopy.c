#include "gencopy.h"
#include "harness_trace.h"


// IDA: br_error __usercall GeneralRectangleCopy@<EAX>(br_device_pixelmap *dest@<EAX>, br_point *p@<EDX>, br_device_pixelmap *src@<EBX>, br_rectangle *r@<ECX>)
br_error GeneralRectangleCopy(br_device_pixelmap* dest, br_point* p, br_device_pixelmap* src, br_rectangle* r) {
    LOG_TRACE("(%p, %p, %p, %p)", dest, p, src, r);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall GeneralCopy@<EAX>(br_device_pixelmap *dst@<EAX>, br_device_pixelmap *src@<EDX>)
br_error GeneralCopy(br_device_pixelmap* dst, br_device_pixelmap* src) {
    LOG_TRACE("(%p, %p)", dst, src);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall GeneralRectangleStretchCopy@<EAX>(br_device_pixelmap *dst@<EAX>, br_rectangle *d@<EDX>, br_device_pixelmap *src@<EBX>, br_rectangle *s@<ECX>)
br_error GeneralRectangleStretchCopy(br_device_pixelmap* dst, br_rectangle* d, br_device_pixelmap* src, br_rectangle* s) {
    LOG_TRACE("(%p, %p, %p, %p)", dst, d, src, s);
    NOT_IMPLEMENTED();
}
