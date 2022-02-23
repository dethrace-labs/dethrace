#include "image.h"
#include "CORE/FW/loader.h"
#include "CORE/FW/brlists.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/pattern.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/scratch.h"
#include "CORE/HOST/himage.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

// IDA: br_boolean __cdecl BrImageAdd(br_image *img)
br_boolean BrImageAdd(br_image* img) {
    LOG_TRACE("(%p)", img);

    BrAddHead(&fw.images, &img->node);
}

// IDA: br_boolean __cdecl BrImageRemove(br_image *img)
br_boolean BrImageRemove(br_image* img) {
    LOG_TRACE("(%p)", img);

    BrRemove(&img->node);
}

// IDA: br_image* __cdecl BrImageFind(char *pattern)
br_image* BrImageFind(char* pattern) {
    char* c;
    br_image* img;
    LOG_TRACE("(\"%s\")", pattern);

    c = BrStrRChr(pattern, '.');
    if (c != NULL && (BrStrCmp(c, ".dll") == 0 || BrStrCmp(c, ".bdd") == 0|| BrStrCmp(c, "bed"))) {
        *c = '\0';
    }
    for (img = (br_image*)fw.images.head; img->node.next != NULL; img = (br_image*)img->node.next) {
        if (BrNamePatternMatch(pattern, img->identifier) != 0) {
            return img;
        }
    }
    return NULL;
}

// IDA: br_image* __usercall imageLoadHost@<EAX>(char *name@<EAX>)
br_image* imageLoadHost(char* name) {
    br_image* img;
    void* host_image;
    LOG_TRACE("(\"%s\")", name);
    NOT_IMPLEMENTED();

    host_image = HostImageLoad(name);
    if (host_image != NULL) {
        img = BrResAllocate(NULL, sizeof(br_image), BR_MEMORY_IMAGE);
        img->identifier = BrResStrDup(img, name);
        img->type = 3;
        img->type_pointer = host_image;
    }
    return img;
}

// IDA: br_image* __cdecl BrImageReference(char *name)
br_image* BrImageReference(char* name) {
    char* suffix;
    char* scratch;
    br_image* img;
    LOG_TRACE("(\"%s\")", name);

    scratch = BrScratchString();
    img = BrImageFind(name);
    if (img != NULL) {
        img->ref_count++;
        return img;
    }
    for (suffix = name; *suffix != '\0' && *suffix != '.'; suffix++) {
    }
    if (*suffix == '\0') {
        if (img == NULL) {
            BrStrCpy(scratch, name);
            BrStrCat(scratch, ".BDD");
            img = ImageLoad(name);
        }
        if (img == NULL) {
            img = imageLoadHost(scratch);
        }
        if (img == NULL) {
            BrStrCpy(scratch, name);
            BrStrCat(scratch, ".DLL");
            img = ImageLoad(scratch);
        }
    } else if (BrStrCmp(suffix, ".bdd") == 0 || BrStrCmp(suffix, ".bed") == 0) {
        if (img == NULL) {
            img = ImageLoad(name);
        }
        if (img == NULL) {
            img = imageLoadHost(name);
        }
    } else {
        if (img == NULL) {
            img = imageLoadHost(name);
        }
        if (img == NULL) {
            img = ImageLoad(name);
        }
    }

    if (img != NULL) {
        BrResAdd(fw.res, img);
        BrAddHead(&fw.images, &img->node);
    }
    return img;
}

// IDA: void* __usercall imageLookupName@<EAX>(br_image *img@<EAX>, char *name@<EDX>, br_uint_32 hint@<EBX>)
void* imageLookupName(br_image* img, char* name, br_uint_32 hint) {
    int c;
    int limit;
    int base;
    LOG_TRACE("(%p, \"%s\", %d)", img, name, hint);

    if (hint < img->n_names && BrStrCmp(name, img->names[hint]) == 0) {
        return img->functions[img->name_ordinals[hint]];
    }
    base = 0;
    limit = img->n_names;
    while (1) {
        if (limit == 0) {
            return NULL;
        }
        c = BrStrCmp(name, img->names[base + limit / 2]);
        if (c == 0) {
            return img->functions[img->name_ordinals[base + limit / 2]];
        } else if (c < 0) {
            continue;
        } else {
            base += limit / 2 + 1;
            limit = limit - (limit / 2 + 1);
        }
    }
}

// IDA: void* __cdecl BrImageLookupName(br_image *img, char *name, br_uint_32 hint)
void* BrImageLookupName(br_image* img, char* name, br_uint_32 hint) {
    char* scratch;
    void* p;
    LOG_TRACE("(%p, \"%s\", %d)", img, name, hint);

    scratch = BrScratchString();
    if (img->type == 3) {
        return HostImageLookupName(img->type_pointer, name, hint);
    }
    p = imageLookupName(img, name, hint);
    if (p != NULL) {
        return p;
    }
    if (*name == '_') {
        p = imageLookupName(img, &name[1], hint);
        if (p != NULL) {
            return p;
        }
    }
    *scratch = '_';
    BrStrCpy(&scratch[1], name);
    return imageLookupName(img, scratch, hint);
}

// IDA: void* __cdecl BrImageLookupOrdinal(br_image *img, br_uint_32 ordinal)
void* BrImageLookupOrdinal(br_image* img, br_uint_32 ordinal) {
    LOG_TRACE("(%p, %d)", img, ordinal);

    if (img->type == 3) {
        return HostImageLookupOrdinal(img->type_pointer, ordinal);
    }
    if (img->n_functions < (ordinal - img->ordinal_base)) {
        return NULL;
    }
    return img->functions[ordinal - img->ordinal_base];
}

// IDA: void __cdecl BrImageDereference(br_image *image)
void BrImageDereference(br_image* image) {
    LOG_TRACE("(%p)", image);

    image->ref_count--;

    if (image->ref_count <= 0) {
        switch (image->type) {
        case 2:
            break;
        case 3:
            HostImageUnload(image->type_pointer);
            // fall through
        default:
            BrRemove(&image->node);
            BrResFree(image);
        }

    }
}

// IDA: void __cdecl BrImageFree(br_image *image)
void BrImageFree(br_image* image) {
    int i;
    LOG_TRACE("(%p)", image);

    for (i = 0; i < image->n_imports; i++) {
        BrImageDereference(image->imports[i]);
    }
}

// IDA: void __cdecl _BrImageFree(void *res, br_uint_8 res_class, br_size_t size)
void _BrImageFree(void* res, br_uint_8 res_class, br_size_t size) {
    LOG_TRACE("(%p, %d, %d)", res, res_class, size);

    BrImageFree(res);
}
