/**
 * Copyright (C) 2015-2018 Think Silicon S.A. (https://think-silicon.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public v3
 * License as published by the Free Software Foundation;
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

/**
 *  @file       eglConfig.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Configuration handler module
 *
 */

#include "eglConfig.h"

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#include "eglConfig_android.h"
#else
#include "eglConfig_linux.h"
#endif

void
InitConfig(EGLConfig_t *conf, EGLDisplay *dpy, EGLint id)
{
    FUN_ENTRY(DEBUG_DEPTH);

    memset(conf, 0, sizeof(*conf));

    conf->Display = dpy;

    /* some attributes take non-zero default values */
    conf->ConfigID         = id;
    conf->ConfigCaveat     = EGL_NONE;
    conf->TransparentType  = EGL_NONE;
    conf->NativeVisualType = EGL_NONE;
    conf->ColorBufferType  = EGL_RGB_BUFFER;
}

enum {
    /* types */
    ATTRIB_TYPE_INTEGER,
    ATTRIB_TYPE_BOOLEAN,
    ATTRIB_TYPE_BITMASK,
    ATTRIB_TYPE_ENUM,
    ATTRIB_TYPE_PSEUDO, /* non-queryable */
    ATTRIB_TYPE_PLATFORM, /* platform-dependent */
    /* criteria */
    ATTRIB_CRITERION_EXACT,
    ATTRIB_CRITERION_ATLEAST,
    ATTRIB_CRITERION_MASK,
    ATTRIB_CRITERION_SPECIAL,
    ATTRIB_CRITERION_IGNORE
};

/* EGL spec Table 3.1 and 3.4 */
static const struct {
   EGLint attr;
   EGLint type;
   EGLint criterion;
   EGLint default_value;
} validationTable[] =
{
   /* core */
   { EGL_BUFFER_SIZE,                   ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_RED_SIZE,                      ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_GREEN_SIZE,                    ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_BLUE_SIZE,                     ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_LUMINANCE_SIZE,                ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_ALPHA_SIZE,                    ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_ALPHA_MASK_SIZE,               ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_BIND_TO_TEXTURE_RGB,           ATTRIB_TYPE_BOOLEAN,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_BIND_TO_TEXTURE_RGBA,          ATTRIB_TYPE_BOOLEAN,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_COLOR_BUFFER_TYPE,             ATTRIB_TYPE_ENUM,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_RGB_BUFFER },
   { EGL_CONFIG_CAVEAT,                 ATTRIB_TYPE_ENUM,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_CONFIG_ID,                     ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_CONFORMANT,                    ATTRIB_TYPE_BITMASK,
                                        ATTRIB_CRITERION_MASK,
                                        0 },
   { EGL_DEPTH_SIZE,                    ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_LEVEL,                         ATTRIB_TYPE_PLATFORM,
                                        ATTRIB_CRITERION_EXACT,
                                        0 },
   { EGL_MAX_PBUFFER_WIDTH,             ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_IGNORE,
                                        0 },
   { EGL_MAX_PBUFFER_HEIGHT,            ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_IGNORE,
                                        0 },
   { EGL_MAX_PBUFFER_PIXELS,            ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_IGNORE,
                                        0 },
   { EGL_MAX_SWAP_INTERVAL,             ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_MIN_SWAP_INTERVAL,             ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_NATIVE_RENDERABLE,             ATTRIB_TYPE_BOOLEAN,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_NATIVE_VISUAL_ID,              ATTRIB_TYPE_PLATFORM,
                                        ATTRIB_CRITERION_IGNORE,
                                        0 },
   { EGL_NATIVE_VISUAL_TYPE,            ATTRIB_TYPE_PLATFORM,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_RENDERABLE_TYPE,               ATTRIB_TYPE_BITMASK,
                                        ATTRIB_CRITERION_MASK,
                                        EGL_OPENGL_ES2_BIT },
   { EGL_SAMPLE_BUFFERS,                ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_SAMPLES,                       ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_STENCIL_SIZE,                  ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_ATLEAST,
                                        0 },
   { EGL_SURFACE_TYPE,                  ATTRIB_TYPE_BITMASK,
                                        ATTRIB_CRITERION_MASK,
                                        EGL_WINDOW_BIT },
   { EGL_TRANSPARENT_TYPE,              ATTRIB_TYPE_ENUM,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_NONE },
   { EGL_TRANSPARENT_RED_VALUE,         ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_TRANSPARENT_GREEN_VALUE,       ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_TRANSPARENT_BLUE_VALUE,        ATTRIB_TYPE_INTEGER,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_DONT_CARE },
   { EGL_MATCH_NATIVE_PIXMAP,           ATTRIB_TYPE_PSEUDO,
                                        ATTRIB_CRITERION_SPECIAL,
                                        EGL_NONE },
   { EGL_RECORDABLE_ANDROID,            ATTRIB_TYPE_BOOLEAN,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_FALSE },
   { EGL_FRAMEBUFFER_TARGET_ANDROID,    ATTRIB_TYPE_BOOLEAN,
                                        ATTRIB_CRITERION_EXACT,
                                        EGL_FALSE }
};
static void
SwapConfigs(const EGLConfig_t **conf1, const EGLConfig_t **conf2)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    const EGLConfig_t *tmp = *conf1;
    *conf1 = *conf2;
    *conf2 = tmp;
}


/**
 * Quick sort an array of configs.  This differs from the standard
 * qsort() in that the compare function accepts an additional
 * argument.
 */
static void
SortConfigs(const EGLConfig_t **configs, EGLint count, EGLint(*compare)(const EGLConfig_t *, const EGLConfig_t *, void *), void *priv_data)
{
    FUN_ENTRY(DEBUG_DEPTH);

    const EGLint pivot = 0;
    EGLint i, j;

    if(count <= 1) {
        return;
    }

    SwapConfigs(&configs[pivot], &configs[count / 2]);
    i = 1;
    j = count - 1;
    do {
        while(i < count && compare(configs[i], configs[pivot], priv_data) < 0) {
            i++;
        }
        while(compare(configs[j], configs[pivot], priv_data) > 0) {
            j--;
        }
        if(i < j) {
            SwapConfigs(&configs[i], &configs[j]);
            i++;
            j--;
        } else if(i == j) {
            i++;
            j--;
            break;
        }
    } while(i <= j);

    SwapConfigs(&configs[pivot], &configs[j]);
    SortConfigs(configs, j, compare, priv_data);
    SortConfigs(configs + i, count - i, compare, priv_data);
}

static EGLBoolean
IsConfigAttribValid(EGLConfig_t *conf, EGLint attr)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(OffsetOfConfig(attr) < 0) {
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

/**
 * Return true if a config is valid.  When for_matching is true,
 * EGL_DONT_CARE is accepted as a valid attribute value, and checks
 * for conflicting attribute values are skipped.
 *
 * Note that some attributes are platform-dependent and are not
 * checked.
 */
EGLBoolean
ValidateConfig(const EGLConfig_t *conf, EGLBoolean for_matching)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLint i, attr, val;
    EGLBoolean valid = EGL_TRUE;

    /* check attributes by their types */
    for(i = 0; i < ARRAY_SIZE(validationTable); i++) {
        EGLint mask;

        attr = validationTable[i].attr;
        val = GetConfigKey(conf, attr);

        switch(validationTable[i].type) {
        case ATTRIB_TYPE_INTEGER:
            switch(attr) {
            case EGL_CONFIG_ID:
                /* config id must be positive */
                if(val <= 0) {
                    valid = EGL_FALSE;
                }
                break;
            case EGL_SAMPLE_BUFFERS:
                /* there can be at most 1 sample buffer */
                if(val > 1 || val < 0) {
                    valid = EGL_FALSE;
                }
                break;
            default:
                if(val < 0) {
                    valid = EGL_FALSE;
                }
                break;
            }
            break;
        case ATTRIB_TYPE_BOOLEAN:
            if(val != EGL_TRUE && val != EGL_FALSE) {
                valid = EGL_FALSE;
            }
            break;
        case ATTRIB_TYPE_ENUM:
            switch(attr) {
            case EGL_CONFIG_CAVEAT:
                if(val != EGL_NONE && val != EGL_SLOW_CONFIG &&
                        val != EGL_NON_CONFORMANT_CONFIG) {
                    valid = EGL_FALSE;
                }
                break;
            case EGL_TRANSPARENT_TYPE:
                if(val != EGL_NONE && val != EGL_TRANSPARENT_RGB) {
                    valid = EGL_FALSE;
                }
                break;
            case EGL_COLOR_BUFFER_TYPE:
                if(val != EGL_RGB_BUFFER && val != EGL_LUMINANCE_BUFFER) {
                    valid = EGL_FALSE;
                }
                break;
            default:
                assert(0);
                break;
            }
            break;
        case ATTRIB_TYPE_BITMASK:
            switch(attr) {
            case EGL_SURFACE_TYPE:
                mask = EGL_PBUFFER_BIT |
                       EGL_PIXMAP_BIT |
                       EGL_WINDOW_BIT |
                       EGL_VG_COLORSPACE_LINEAR_BIT |
                       EGL_VG_ALPHA_FORMAT_PRE_BIT |
                       EGL_MULTISAMPLE_RESOLVE_BOX_BIT |
                       EGL_SWAP_BEHAVIOR_PRESERVED_BIT;
                break;
            case EGL_RENDERABLE_TYPE:
            case EGL_CONFORMANT:
                mask = EGL_OPENGL_ES_BIT |
                       EGL_OPENVG_BIT |
                       EGL_OPENGL_ES2_BIT |
                       /*EGL_OPENGL_ES3_BIT_KHR |*/
                       EGL_OPENGL_BIT;
                break;
            default:
                assert(0);
                mask = 0;
                break;
            }

            if(val & ~mask) {
                valid = EGL_FALSE;
            }

            break;
        case ATTRIB_TYPE_PLATFORM:
            /* unable to check platform-dependent attributes here */
            break;
        case ATTRIB_TYPE_PSEUDO:
            /* pseudo attributes should not be set */
            if(val != 0) {
                valid = EGL_FALSE;
            }
            break;
        default:
            assert(0);
            break;
        }

        if(!valid && for_matching) {
            /* accept EGL_DONT_CARE as a valid value */
            if(val == EGL_DONT_CARE) {
                valid = EGL_TRUE;
            }
            if(validationTable[i].criterion == ATTRIB_CRITERION_SPECIAL) {
                valid = EGL_TRUE;
            }
        }

        if(!valid) {
            // attribute has an invalid value
            break;
        }
    }

    /* any invalid attribute value should have been catched */
    if(!valid || for_matching) {
        return valid;
    }

    /* now check for conflicting attribute values */

    switch(conf->ColorBufferType) {
    case EGL_RGB_BUFFER:
        if(conf->LuminanceSize) {
            valid = EGL_FALSE;
        }
        if(conf->RedSize + conf->GreenSize +
                conf->BlueSize + conf->AlphaSize != conf->BufferSize) {
            valid = EGL_FALSE;
        }
        break;
    case EGL_LUMINANCE_BUFFER:
        if(conf->RedSize || conf->GreenSize || conf->BlueSize) {
            valid = EGL_FALSE;
        }
        if(conf->LuminanceSize + conf->AlphaSize != conf->BufferSize) {
            valid = EGL_FALSE;
        }
        break;
    }

    if(!valid) {
        // conflicting color buffer type and channel sizes
        return EGL_FALSE;
    }

    if(!conf->SampleBuffers && conf->Samples) {
        valid = EGL_FALSE;
    }

    if(!valid) {
        // conflicting samples and sample buffers
        return EGL_FALSE;
    }

    if(!(conf->SurfaceType & EGL_WINDOW_BIT)) {
        if(conf->NativeVisualID != 0 || conf->NativeVisualType != EGL_NONE) {
            valid = EGL_FALSE;
        }
    }

    if(!(conf->SurfaceType & EGL_PBUFFER_BIT)) {
        if(conf->BindToTextureRGB || conf->BindToTextureRGBA) {
            valid = EGL_FALSE;
        }
    }

    if(!valid) {
        // conflicting surface type and native visual/texture binding
        return EGL_FALSE;
    }

    return valid;
}

/**
 * Return true if a config matches the criteria.  This and
 * _eglParseConfigAttribList together implement the algorithm
 * described in "Selection of EGLConfigs".
 *
 * Note that attributes that are special (currently, only
 * EGL_MATCH_NATIVE_PIXMAP) are ignored.
 */
EGLBoolean
MatchConfig(const EGLConfig_t *conf, const EGLConfig_t *criteria)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLint attr, val, i;
    EGLBoolean matched = EGL_TRUE;

    for(i = 0; i < ARRAY_SIZE(validationTable); i++) {
        EGLint cmp;
        if(validationTable[i].criterion == ATTRIB_CRITERION_IGNORE) {
            continue;
        }

        attr = validationTable[i].attr;
        cmp = GetConfigKey(criteria, attr);
        if(cmp == EGL_DONT_CARE) {
            continue;
        }

        val = GetConfigKey(conf, attr);
        switch(validationTable[i].criterion) {
        case ATTRIB_CRITERION_EXACT:
            if(val != cmp) {
                matched = EGL_FALSE;
            }
            break;
        case ATTRIB_CRITERION_ATLEAST:
            if(val < cmp) {
                matched = EGL_FALSE;
            }
            break;
        case ATTRIB_CRITERION_MASK:
            if((val & cmp) != cmp) {
                matched = EGL_FALSE;
            }
            break;
        case ATTRIB_CRITERION_SPECIAL:
            /* ignored here */
            break;
        default:
            assert(0);
            break;
        }

        if(!matched) {
            break;
        }
    }

    return matched;
}

/**
 * Decide the ordering of conf1 and conf2, under the given criteria.
 * When compare_id is true, this implements the algorithm described
 * in "Sorting of EGLConfigs".  When compare_id is false,
 * EGL_CONFIG_ID is not compared.
 *
 * It returns a negative integer if conf1 is considered to come
 * before conf2;  a positive integer if conf2 is considered to come
 * before conf1;  zero if the ordering cannot be decided.
 *
 * Note that EGL_NATIVE_VISUAL_TYPE is platform-dependent and is
 * ignored here.
 */
EGLint
CompareConfigs(const EGLConfig_t *conf1, const EGLConfig_t *conf2,
               const EGLConfig_t *criteria, EGLBoolean compare_id)
{
    FUN_ENTRY(DEBUG_DEPTH);

    const EGLint compare_attribs[] = {
        EGL_BUFFER_SIZE,
        EGL_SAMPLE_BUFFERS,
        EGL_SAMPLES,
        EGL_DEPTH_SIZE,
        EGL_STENCIL_SIZE,
        EGL_ALPHA_MASK_SIZE,
    };
    EGLint val1, val2;
    EGLint i;

    if(conf1 == conf2) {
        return 0;
    }

    /* the enum values have the desired ordering */
    assert(EGL_NONE < EGL_SLOW_CONFIG);
    assert(EGL_SLOW_CONFIG < EGL_NON_CONFORMANT_CONFIG);
    val1 = conf1->ConfigCaveat - conf2->ConfigCaveat;
    if(val1) {
        return val1;
    }

    /* the enum values have the desired ordering */
    assert(EGL_RGB_BUFFER < EGL_LUMINANCE_BUFFER);
    val1 = conf1->ColorBufferType - conf2->ColorBufferType;
    if(val1) {
        return val1;
    }

    if(criteria) {
        val1 = val2 = 0;
        if(conf1->ColorBufferType == EGL_RGB_BUFFER) {
            if(criteria->RedSize > 0) {
                val1 += conf1->RedSize;
                val2 += conf2->RedSize;
            }
            if(criteria->GreenSize > 0) {
                val1 += conf1->GreenSize;
                val2 += conf2->GreenSize;
            }
            if(criteria->BlueSize > 0) {
                val1 += conf1->BlueSize;
                val2 += conf2->BlueSize;
            }
        } else {
            if(criteria->LuminanceSize > 0) {
                val1 += conf1->LuminanceSize;
                val2 += conf2->LuminanceSize;
            }
        }
        if(criteria->AlphaSize > 0) {
            val1 += conf1->AlphaSize;
            val2 += conf2->AlphaSize;
        }
    } else {
        /* assume the default criteria, which gives no specific ordering */
        val1 = val2 = 0;
    }

    /* for color bits, larger one is preferred */
    if(val1 != val2) {
        return (val2 - val1);
    }

    for(i = 0; i < ARRAY_SIZE(compare_attribs); i++) {
        val1 = GetConfigKey(conf1, compare_attribs[i]);
        val2 = GetConfigKey(conf2, compare_attribs[i]);
        if(val1 != val2) {
            return (val1 - val2);
        }
    }

    /* EGL_NATIVE_VISUAL_TYPE cannot be compared here */

    return (compare_id) ? (conf1->ConfigID - conf2->ConfigID) : 0;
}

/**
 * Initialize a criteria config from the given attribute list.
 * Return EGL_FALSE if any of the attribute is invalid.
 */
EGLBoolean
ParseConfigAttribList(EGLConfig_t *conf, EGLDisplay *dpy, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLint attr, val, i;

    InitConfig(conf, dpy, EGL_DONT_CARE);

    /* reset to default values */
    for(i = 0; i < ARRAY_SIZE(validationTable); i++) {
        attr = validationTable[i].attr;
        val = validationTable[i].default_value;
        SetConfigKey(conf, attr, val);
    }

    /* parse the list */
    for(i = 0; attrib_list && attrib_list[i] != EGL_NONE; i += 2) {
        attr = attrib_list[i];
        val = attrib_list[i + 1];

        if(!IsConfigAttribValid(conf, attr)) {
            return EGL_FALSE;
        }

        SetConfigKey(conf, attr, val);
    }

    if(!ValidateConfig(conf, EGL_TRUE)) {
        return EGL_FALSE;
    }

    /* EGL_LEVEL and EGL_MATCH_NATIVE_PIXMAP cannot be EGL_DONT_CARE */
    if(conf->Level == EGL_DONT_CARE || conf->MatchNativePixmap == EGL_DONT_CARE) {
        return EGL_FALSE;
    }

    /* ignore other attributes when EGL_CONFIG_ID is given */
    if(conf->ConfigID != EGL_DONT_CARE) {
        for(i = 0; i < ARRAY_SIZE(validationTable); i++) {
            attr = validationTable[i].attr;
            if(attr != EGL_CONFIG_ID) {
                SetConfigKey(conf, attr, EGL_DONT_CARE);
            }
        }
    } else {
        if(!(conf->SurfaceType & EGL_WINDOW_BIT)) {
            conf->NativeVisualType = EGL_DONT_CARE;
        }

        if(conf->TransparentType == EGL_NONE) {
            conf->TransparentRedValue = EGL_DONT_CARE;
            conf->TransparentGreenValue = EGL_DONT_CARE;
            conf->TransparentBlueValue = EGL_DONT_CARE;
        }
    }

    return EGL_TRUE;
}

EGLBoolean
FilterConfigArray(EGLConfig_t **configs, EGLint config_size, EGLint *num_config, const EGLConfig_t *criteria)
{
    FUN_ENTRY(DEBUG_DEPTH);

    int i, j, count = 0;

    for(i = 0; i < ARRAY_SIZE(EglConfigs); ++i) {
        if(MatchConfig(&EglConfigs[i], criteria)) {
            ++count;
        }
    }

    if(num_config) {
        *num_config = count;
    }

    count = count > config_size ? config_size : count;
    i = 0;
    j = 0;
    while(j < count && configs) {
        if(MatchConfig(&EglConfigs[i], criteria)) {
            /// NOTE: User now has write access to EglConfigs array
            configs[j] = (EGLConfig_t *)&EglConfigs[i];
            ++j;
        }
        ++i;
    }

    return EGL_TRUE;
}
