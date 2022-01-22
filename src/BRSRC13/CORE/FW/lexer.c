#include "lexer.h"
#include "CORE/FW/brprintf.h"
#include "CORE/FW/file.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/fixed.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

// IDA: void __cdecl lexerError(br_lexer *l, char *string)
void lexerError(br_lexer* l, char* string) {
    LOG_TRACE("(%p, \"%s\")", l, string);
}

// IDA: br_lexer* __cdecl BrLexerAllocate(br_lexer_keyword *keywords, int nkeywords)
br_lexer* BrLexerAllocate(br_lexer_keyword* keywords, int nkeywords) {
    br_lexer* l;
    LOG_TRACE("(%p, %d)", keywords, nkeywords);

    l = BrResAllocate(fw.res, sizeof(br_lexer), BR_MEMORY_LEXER);
    if (l == NULL) {
        return NULL;
    }
    l->keywords = keywords;
    l->nkeywords = nkeywords;
    l->advance = lexerAdvance;
    l->source = NULL;
    l->string_buffer = BrResAllocate(l, 256, BR_MEMORY_LEXER);
    l->string_buffer_size = 256;
    l->error = lexerError;
    return l;
}

// IDA: char __cdecl BrLexerCommentSet(br_lexer *l, char eol_comment)
char BrLexerCommentSet(br_lexer* l, char eol_comment) {
    char old;
    LOG_TRACE("(%p, %d)", l, eol_comment);

    old = l->eol_comment;
    l->eol_comment = eol_comment;
    return old;
}

// IDA: br_lexer_error_cbfn* __cdecl BrLexerErrorSet(br_lexer *l, br_lexer_error_cbfn *error)
br_lexer_error_cbfn* BrLexerErrorSet(br_lexer* l, br_lexer_error_cbfn* error) {
    br_lexer_error_cbfn* old;
    LOG_TRACE("(%p, %p)", l, error);

    old = l->error;
    if (error == NULL) {
        l->error = lexerError;
    } else {
        l->error = error;
    }
    return old == lexerError ? NULL : old;
}

// IDA: void __cdecl BrLexerFree(br_lexer *l)
void BrLexerFree(br_lexer* l) {
    LOG_TRACE("(%p)", l);

    BrResFree(l);
}

// IDA: void __cdecl fileGetchar(br_lexer_source *source)
void fileGetchar(br_lexer_source* source) {
    LOG_TRACE("(%p)", source);

    source->next = BrFileGetChar(source->ptr);
}

// IDA: br_error __cdecl BrLexerPushFile(br_lexer *l, char *file)
br_error BrLexerPushFile(br_lexer* l, char* file) {
    br_lexer_source* s;
    void* f;
    int mode;
    LOG_TRACE("(%p, \"%s\")", l, file);

    mode = BR_FS_MODE_TEXT;
    f = BrFileOpenRead(file, 0, NULL, &mode);
    if (f == NULL) {
        return 0x1002;
    }
    s = BrResAllocate(l, sizeof(br_lexer_source), BR_MEMORY_LEXER);
    if (s == NULL) {
        return 0x1003;
    }
    BrResAdd(s, f);
    s->name = file;
    s->line = 1;
    s->getchar = fileGetchar;
    s->ptr = f;
    s->getchar(s);
    s->prev = l->source;
    l->source = s;
    return 0;
}

// IDA: void __cdecl stringGetchar(br_lexer_source *source)
void stringGetchar(br_lexer_source* source) {
    char* cp;
    LOG_TRACE9("(%p)", source);

    cp = source->ptr;
    if (cp != NULL && *cp != '\0') {
        source->next = *cp;
        source->ptr = cp + 1;
    } else {
        source->next = -1;
    }
}

// IDA: br_error __cdecl BrLexerPushString(br_lexer *l, char *string, char *name)
br_error BrLexerPushString(br_lexer* l, char* string, char* name) {
    br_lexer_source* s;
    LOG_TRACE("(%p, \"%s\", \"%s\")", l, string, name);

    s = BrResAllocate(l, sizeof(br_lexer_source), BR_MEMORY_LEXER);
    if (s == NULL) {
        return 0x1003;
    }
    s->name = name;
    s->line = 1;
    s->getchar = stringGetchar;
    s->ptr = string;
    s->getchar(s);
    s->prev = l->source;
    l->source = s;
    return 0;
}

// IDA: br_lexer_source* __cdecl BrLexerPop(br_lexer *l)
br_lexer_source* BrLexerPop(br_lexer* l) {
    br_lexer_source* s;
    LOG_TRACE("(%p)", l);

    s = l->source;
    if (s != NULL) {
        l->source = s->prev;
        BrResFree(s);
    }
    return s;
}

// IDA: void __usercall lexerAdvance(br_lexer *l@<EAX>)
void lexerAdvance(br_lexer* l) {
    int n;
    br_boolean got_point;
    LOG_TRACE("(%p)", l);

    while (1) {
        if (l->source == NULL) {
            l->current.id = T_EOF;
            return;
        }
        if (l->source->next == -1) {
            BrLexerPop(l);
            continue;
        }
        if (l->eol_comment != '\0' && l->eol_comment == l->source->next) {
            l->source->getchar(l->source);
            while (l->source->next != '\n' && l->source->next != -1) {
                l->source->getchar(l->source);
            }
            continue;
        }
        if (l->source->next == '\n') {
            l->source->line++;
            l->source->getchar(l->source);
            continue;
        }
        if (l->source->next == '\"') {
            for (n = 0; n < l->string_buffer_size; n++) {
                l->source->getchar(l->source);
                l->string_buffer[n] = l->source->next;
                if ((l->source->next == '\"') || (l->source->next == '\n') || (l->source->next == -1)) {
                    break;
                }
            }
            if (l->source->next != '\"') {
                l->current.id = T_ERROR;
                l->current.v.string = "Unterminated string";
            } else {
                l->string_buffer[n] = '\0';
                l->current.id = T_STRING;
                l->current.v.string = l->string_buffer;
            }
            l->source->getchar(l->source);
            return;
        }
        if (BrIsSpace(l->source->next) || l->source->next == '\x1a') {
            l->source->getchar(l->source);
            continue;
        }
        if (BrIsAlpha(l->source->next) != 0 || l->source->next == '_') {
            l->string_buffer[0] = l->source->next;
            for (n = 1; n < l->string_buffer_size - 1; n++) {
                l->source->getchar(l->source);
                if (BrIsAlpha(l->source->next) == 0 && BrIsDigit(l->source->next) == 0 && l->source->next != '_') {
                    break;
                }
                l->string_buffer[n] = l->source->next;
            }
            l->string_buffer[n] = '\0';
            l->current.v.string = l->string_buffer;
            for (n = 0; n < l->nkeywords; n++) {
                if ((l->keywords[n].name[0] == l->string_buffer[0]) && (BrStrCmp(l->keywords[n].name, l->string_buffer) == 0)) {
                    l->current.id = l->keywords[n].id;
                    return;
                }
            }
            l->current.id = T_IDENT;
            return;
        }
        if (BrIsDigit(l->source->next) != 0) {
            got_point = 0;
            l->string_buffer[0] = l->source->next;
            for (n = 1; n < l->string_buffer_size - 1; n++) {
                l->source->getchar(l->source);
                if (l->source->next == '.') {
                    got_point = 1;
                } else if (!(('0' <= l->source->next && l->source->next <= '9')
                    || ('A' <= l->source->next && l->source->next <= 'F') || (l->source->next == 'X')
                    || ('a' <= l->source->next && l->source->next <= 'f') || (l->source->next == 'x'))) {
                    break;
                }
                l->string_buffer[n] = l->source->next;
            }
            l->string_buffer[n] = '\0';
            if (got_point != 0) {
                l->current.v.real = BrStrToF(l->string_buffer, NULL);
                l->current.id = T_REAL;
            } else {
                l->current.v.integer = BrStrToL(l->string_buffer, NULL, 0);
                l->current.id = T_INTEGER;
            }
            return;
        }
        if (BrIsPrint(l->source->next) != 0) {
            l->current.id = l->source->next;
            l->source->getchar(l->source);
            return;
        }
        l->current.id = T_ERROR;
        l->current.v.string = "Unexpected character";
        l->source->getchar(l->source);
        return;
    }
}

// IDA: void __usercall lexerAdvanceDump(br_lexer *l@<EAX>)
void lexerAdvanceDump(br_lexer* l) {
    char* tname;
    char* tvalue;
    char val[40];
    char tmp[256];
    LOG_TRACE("(%p)", l);

    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrLexerDumpSet(br_lexer *l, br_putline_cbfn *putline, void *putline_arg)
br_error BrLexerDumpSet(br_lexer* l, br_putline_cbfn* putline, void* putline_arg) {
    LOG_TRACE("(%p, %p, %p)", l, putline, putline_arg);

    return 0;
}

// IDA: void __cdecl BrLexerTokenError(br_lexer *l, br_lexer_token_id t)
void BrLexerTokenError(br_lexer* l, br_lexer_token_id t) {
    int i;
    char tmp[256];
    LOG_TRACE("(%p, %d)", l, t);

    if (l->current.id != T_ERROR) {
        switch (t) {
        case T_EOF:
            l->error(l, "expected end of file");
            break;
        case T_ERROR:
            l->error(l, l->current.v.string);
            break;
        case T_IDENT:
            l->error(l, "expected an identifier");
            break;
        case T_STRING:
            l->error(l, "expected a string");
            break;
        case T_INTEGER:
            l->error(l, "expected an integer");
            break;
        case T_REAL:
            l->error(l, "expected a real number");
            break;
        default:
            if (t < 0x80) {
                if (BrIsPrint(t) != 0) {
                    BrSprintfN(tmp, sizeof(tmp), "expected '%c'", t);
                } else {
                    BrSprintfN(tmp, sizeof(tmp), "expected unknown token %d", t);
                }
            } else {
                for (i = 0; i < l->nkeywords; i++) {
                    if (l->keywords[i].id == t) {
                        break;
                    }
                }
                if (i < l->nkeywords) {
                    BrSprintfN(tmp, sizeof(tmp), "expected '%s'", l->keywords[i].name);
                } else {
                    BrSprintfN(tmp, sizeof(tmp), "expected unknown keyword %d", t);
                }
            }
            l->error(l, tmp);
            break;
        }
    }

    while (l->current.id != t && l->current.id != T_EOF) {
        l->advance(l);
    }
}

// IDA: void __cdecl BrLexerPosition(br_lexer *l, char *buf, br_size_t buf_size)
void BrLexerPosition(br_lexer* l, char* buf, br_size_t buf_size) {
    LOG_TRACE("(%p, \"%s\", %d)", l, buf, buf_size);

    if ((l->source != NULL) && (l->source->name != NULL)) {
        BrSprintfN(buf, buf_size, "%s:%d ", l->source->name, l->source->line);
    } else {
        *buf = '\0';
    }
}

// IDA: br_fixed_ls __cdecl BrParseFixed(br_lexer *l)
br_fixed_ls BrParseFixed(br_lexer* l) {
    br_boolean neg;
    br_fixed_ls x;
    LOG_TRACE("(%p)", l);

    if (l->current.id == T_PLUS) {
        l->advance(l);
    }
    neg = l->current.id == T_DASH;
    if (neg) {
        l->advance(l);
    }
    if (l->current.id == T_REAL) {
        x = BrFloatToFixed(l->current.v.real);
        l->advance(l);
    } else if (l->current.id == T_INTEGER) {
        x = BrIntToFixed(l->current.v.integer);
        l->advance(l);
    } else {
        l->error(l, "expecting a fixed");
        l->advance(l);
        x = 0;
    }
    if (neg) {
        x = -x;
    }
    return x;
}

// IDA: br_float __cdecl BrParseFloat(br_lexer *l)
br_float BrParseFloat(br_lexer* l) {
    br_boolean neg;
    br_float f;
    LOG_TRACE("(%p)", l);

    if (l->current.id == T_PLUS) {
        l->advance(l);
    }
    neg = l->current.id == T_DASH;
    if (neg) {
        l->advance(l);
    }
    if (l->current.id == T_REAL) {
        f = l->current.v.real;
        l->advance(l);
    } else if (l->current.id == T_INTEGER) {
        f = (float)l->current.v.integer;
        l->advance(l);
    } else {
        l->error(l, "expecting a float");
        l->advance(l);
        f = 0.f;
    }
    if (neg) {
        f = -f;
    }
    return f;
}

// IDA: br_int_32 __cdecl BrParseInteger(br_lexer *l)
br_int_32 BrParseInteger(br_lexer* l) {
    br_boolean neg;
    br_int_32 i;
    LOG_TRACE("(%p)", l);

    if (l->current.id == T_PLUS) {
        l->advance(l);
    }
    neg = l->current.id == T_DASH;
    if (neg) {
        l->advance(l);
    }
    if (l->current.id == T_INTEGER) {
        i = l->current.v.integer;
        l->advance(l);
    } else {
        l->error(l, "expecting an integer");
        l->advance(l);
        i = 0;
    }
    if (neg) {
        i = -i;
    }
    return i;
}

// IDA: br_int_32 __cdecl BrParseVectorFixed(br_lexer *l, br_fixed_ls *v, br_int_32 max)
br_int_32 BrParseVectorFixed(br_lexer* l, br_fixed_ls* v, br_int_32 max) {
    int n;
    LOG_TRACE("(%p, %p, %d)", l, v, max);

    if (l->current.id == T_LSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_LSQUARE);
    }
    for (n = 0; n < max; n++) {
#ifdef BRENDER_FIX_BUGS
        if (l->current.id == T_RSQUARE) {
            break;
        }
#endif
        *v = BrParseFixed(l);
        v++;
        if (l->current.id == T_RSQUARE) {
            n++;
            break;
        }
        if (n != max - 1) {
            if (l->current.id == T_COMMA) {
                l->advance(l);
            } else {
                BrLexerTokenError(l, T_COMMA);
            }
        }
    }
    if (l->current.id == T_RSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_RSQUARE);
    }
    return n;
}

// IDA: br_int_32 __cdecl BrParseVectorFloat(br_lexer *l, br_float *v, br_int_32 max)
br_int_32 BrParseVectorFloat(br_lexer* l, br_float* v, br_int_32 max) {
    int n;
    LOG_TRACE("(%p, %p, %d)", l, v, max);

    if (l->current.id == T_LSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_LSQUARE);
    }
    for (n = 0; n < max; n++) {
#ifdef BRENDER_FIX_BUGS
        if (l->current.id == T_RSQUARE) {
            break;
        }
#endif
        *v = BrParseFloat(l);
        v++;
        if (l->current.id == T_RSQUARE) {
            n++;
            break;
        }
        if (n != max - 1) {
            if (l->current.id == T_COMMA) {
                l->advance(l);
            } else {
                BrLexerTokenError(l, T_COMMA);
            }
        }
    }
    if (l->current.id == T_RSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_RSQUARE);
    }
    return n;
}

// IDA: br_int_32 __cdecl BrParseMatrixFixed(br_lexer *l, br_fixed_ls *m, br_int_32 width, br_int_32 max_h)
br_int_32 BrParseMatrixFixed(br_lexer* l, br_fixed_ls* m, br_int_32 width, br_int_32 max_h) {
    int n;
    LOG_TRACE("(%p, %p, %d, %d)", l, m, width, max_h);

    if (l->current.id == T_LSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_LSQUARE);
    }
    for (n = 0; n < max_h; n++, m = m + width) {
#ifdef BRENDER_FIX_BUGS
        if (l->current.id == T_RSQUARE) {
            break;
        }
#endif
        BrParseVectorFixed(l, m, width);
        if (l->current.id == T_RSQUARE) {
            n++;
            break;
        }
        if (n != max_h - 1) {
            if (l->current.id == T_COMMA) {
                l->advance(l);
            } else {
                BrLexerTokenError(l, T_COMMA);
            }
        }
    }
    if (l->current.id == T_RSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_RSQUARE);
    }
    return n;
}

// IDA: br_int_32 __cdecl BrParseMatrixFloat(br_lexer *l, br_float *m, br_int_32 width, br_int_32 max_h)
br_int_32 BrParseMatrixFloat(br_lexer* l, br_float* m, br_int_32 width, br_int_32 max_h) {
    int n;
    LOG_TRACE("(%p, %p, %d, %d)", l, m, width, max_h);

    if (l->current.id == T_LSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_LSQUARE);
    }
    for (n = 0; n < max_h; n++, m = m + width) {
#ifdef BRENDER_FIX_BUGS
        if (l->current.id == T_RSQUARE) {
            break;
        }
#endif
        BrParseVectorFloat(l, m, width);
        if (l->current.id == T_RSQUARE) {
            n++;
            break;
        }
        if (n != max_h - 1) {
            if (l->current.id == T_COMMA) {
                l->advance(l);
            } else {
                BrLexerTokenError(l, T_COMMA);
            }
        }
    }
    if (l->current.id == T_RSQUARE) {
        l->advance(l);
    } else {
        BrLexerTokenError(l, T_RSQUARE);
    }
    return n;
}
