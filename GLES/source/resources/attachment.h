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
 *  @file       attachment.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Framebuffer Attachment Functionality in GLOVE
 *
 */

#ifndef __ATTACHMENT_H__
#define __ATTACHMENT_H__

#include "texture.h"

class Attachment {

private:
    GLenum                  mType;
    uint32_t                mName;
    GLint                   mLevel;
    GLenum                  mLayer;
    Texture *               mTexture;

public:
    Attachment(Texture *tex = nullptr);
    ~Attachment();

// Get Functions
    inline GLenum           GetType(void)                               const   { FUN_ENTRY(GL_LOG_TRACE); return mType;     }
    inline uint32_t         GetName(void)                               const   { FUN_ENTRY(GL_LOG_TRACE); return mName;     }
    inline GLint            GetLevel(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mLevel;    }
    inline GLenum           GetLayer(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mLayer;    }
    inline Texture *        GetTexture(void)                            const   { FUN_ENTRY(GL_LOG_TRACE); return mTexture;  }

// Set Functions
    inline void             SetType(GLenum type)                                { FUN_ENTRY(GL_LOG_TRACE); mType    = type;  }
    inline void             SetName(uint32_t name)                              { FUN_ENTRY(GL_LOG_TRACE); mName    = name;  }
    inline void             SetLevel(GLint level)                               { FUN_ENTRY(GL_LOG_TRACE); mLevel   = level; }
    inline void             SetLayer(GLenum layer)                              { FUN_ENTRY(GL_LOG_TRACE); mLayer   = layer; }
    inline void             SetTexture(Texture *tex)                            { FUN_ENTRY(GL_LOG_TRACE); mTexture = tex;   }
};

#endif // __ATTACHMENT_H__
