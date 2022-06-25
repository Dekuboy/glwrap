#include "GBuffer.h"

namespace glwrap
{
	GBuffer::GBuffer()
	{

	}

	GBuffer::GBuffer(int _width, int _height) : RenderTexture(_width, _height, false)
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);

		glGenTextures(1, &m_nId);
		glBindTexture(GL_TEXTURE_2D, m_nId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_nId, 0);

		glGenTextures(1, &m_asId);
		glBindTexture(GL_TEXTURE_2D, m_asId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_asId, 0);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		GLuint rbo = 0;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.x, m_size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GBuffer::~GBuffer()
	{
	}

	GLuint GBuffer::getNId()
	{
		return m_nId;
	}

	GLuint GBuffer::getAsId()
	{
		return m_asId;
	}
}