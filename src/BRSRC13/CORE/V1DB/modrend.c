#include "modrend.h"
#include "dbsetup.h"
#include "harness_hooks.h"
#include "harness_trace.h"

br_colour bounds_colours[12];
fmt_vertex bounds_vertices[8];
v11face bounds_faces[12];
void (*RenderStyleCalls[])(
    br_actor* actor,
    br_model* model,
    br_material* material,
    void* render_data,
    br_uint_8 style,
    int on_screen)
    = {
          renderFaces, /* BR_RSTYLE_DEFAULT			*/
          nullRender, /* BR_RSTYLE_NONE				*/
          renderPoints, /* BR_RSTYLE_POINTS				*/
          renderEdges, /* BR_RSTYLE_EDGES				*/
          renderFaces, /* BR_RSTYLE_FACES				*/
          boundingBoxRenderPoints, /* BR_RSTYLE_BOUNDING_POINTS	*/
          boundingBoxRenderEdges, /* BR_RSTYLE_BOUNDING_EDGES		*/
          boundingBoxRenderFaces, /* BR_RSTYLE_BOUNDING_FACES		*/
      };

v11model bounds_prepared;
br_model bounds_model;
v11group bounds_face_groups[1];
char rscid[49];

// IDA: void __usercall renderFaces(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE9("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    Harness_Hook_renderFaces(model, material, BRT_TRIANGLE);
}

// IDA: void __usercall renderEdges(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall renderPoints(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall nullRender(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void nullRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: br_model* __usercall makeMeshFromBounds@<EAX>(br_bounds *b@<EAX>)
br_model* makeMeshFromBounds(br_bounds* b) {
    int i;
    LOG_TRACE("(%p)", b);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall boundingBoxRenderPoints(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall boundingBoxRenderEdges(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall boundingBoxRenderFaces(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}
