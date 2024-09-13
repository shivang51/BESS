#pragma once
#include "fwd.hpp"
#include "gl_wrapper.h"
#include "renderer/gl/texture.h"
#include <memory>
#include <vector>

namespace Bess::Gl {
    enum FBAttachmentType {
        R32I_REDI,
        RGB_RGB,
        RGBA_RGBA,
        // non-color attachments
        DEPTH24_STENCIL8 = 11,
        DEPTH32F_STENCIL8,
    };

    class FBAttachment {
      public:
        FBAttachment(FBAttachmentType type, float width, float height, GLint internalFormat, GLenum format, GLenum index);
        void resize(float width, float height);

        GLuint getTextureId() const;

        GLenum getIndex() const;

        FBAttachmentType getType() const;
        GLuint getIntenalFormat() const;
        GLenum getFormat() const;

      private:
        std::shared_ptr<Texture> m_texture;
        GLuint m_internalFormat;
        GLenum m_format, m_index;
        FBAttachmentType m_type;
    };
} // namespace Bess::Gl