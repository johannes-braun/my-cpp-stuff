#pragma once

namespace mygl
{
    namespace { MYGL_DISPATCH_NAME static_dispatch; }
    MYGL_DISPATCH_NAME& get_static_dispatch() noexcept
    {
        return static_dispatch;
    }
}
void glActiveShaderProgram(std::uint32_t pipeline, std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().activeShaderProgram(pipeline, program); }
void glActiveTexture(GLenum texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().activeTexture(texture); }
void glAttachShader(std::uint32_t program, std::uint32_t shader) MYGL_NOEXCEPT { return mygl::get_static_dispatch().attachShader(program, shader); }
void glBeginConditionalRender(std::uint32_t id, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().beginConditionalRender(id, mode); }
void glBeginQuery(GLenum target, std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().beginQuery(target, id); }
void glBeginQueryIndexed(GLenum target, std::uint32_t index, std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().beginQueryIndexed(target, index, id); }
void glBeginTransformFeedback(GLenum primitiveMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().beginTransformFeedback(primitiveMode); }
void glBindAttribLocation(std::uint32_t program, std::uint32_t index, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindAttribLocation(program, index, name); }
void glBindBuffer(GLenum target, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindBuffer(target, buffer); }
void glBindBufferBase(GLenum target, std::uint32_t index, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindBufferBase(target, index, buffer); }
void glBindBufferRange(GLenum target, std::uint32_t index, std::uint32_t buffer, std::intptr_t offset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindBufferRange(target, index, buffer, offset, size); }
void glBindBuffersBase(GLenum target, std::uint32_t first, std::int32_t count, const std::uint32_t * buffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindBuffersBase(target, first, count, buffers); }
void glBindBuffersRange(GLenum target, std::uint32_t first, std::int32_t count, const std::uint32_t * buffers, const std::intptr_t * offsets, const std::int64_t * sizes) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindBuffersRange(target, first, count, buffers, offsets, sizes); }
void glBindFragDataLocation(std::uint32_t program, std::uint32_t color, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindFragDataLocation(program, color, name); }
void glBindFragDataLocationIndexed(std::uint32_t program, std::uint32_t colorNumber, std::uint32_t index, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindFragDataLocationIndexed(program, colorNumber, index, name); }
void glBindFramebuffer(GLenum target, std::uint32_t framebuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindFramebuffer(target, framebuffer); }
void glBindImageTexture(std::uint32_t unit, std::uint32_t texture, std::int32_t level, bool layered, std::int32_t layer, GLenum access, GLenum format) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindImageTexture(unit, texture, level, layered, layer, access, format); }
void glBindImageTextures(std::uint32_t first, std::int32_t count, const std::uint32_t * textures) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindImageTextures(first, count, textures); }
void glBindProgramPipeline(std::uint32_t pipeline) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindProgramPipeline(pipeline); }
void glBindRenderbuffer(GLenum target, std::uint32_t renderbuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindRenderbuffer(target, renderbuffer); }
void glBindSampler(std::uint32_t unit, std::uint32_t sampler) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindSampler(unit, sampler); }
void glBindSamplers(std::uint32_t first, std::int32_t count, const std::uint32_t * samplers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindSamplers(first, count, samplers); }
void glBindTexture(GLenum target, std::uint32_t texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindTexture(target, texture); }
void glBindTextureUnit(std::uint32_t unit, std::uint32_t texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindTextureUnit(unit, texture); }
void glBindTextures(std::uint32_t first, std::int32_t count, const std::uint32_t * textures) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindTextures(first, count, textures); }
void glBindTransformFeedback(GLenum target, std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindTransformFeedback(target, id); }
void glBindVertexArray(std::uint32_t array) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindVertexArray(array); }
void glBindVertexBuffer(std::uint32_t bindingindex, std::uint32_t buffer, std::intptr_t offset, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindVertexBuffer(bindingindex, buffer, offset, stride); }
void glBindVertexBuffers(std::uint32_t first, std::int32_t count, const std::uint32_t * buffers, const std::intptr_t * offsets, const std::int32_t * strides) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bindVertexBuffers(first, count, buffers, offsets, strides); }
void glBlendColor(float red, float green, float blue, float alpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendColor(red, green, blue, alpha); }
void glBlendEquation(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquation(mode); }
void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquationSeparate(modeRGB, modeAlpha); }
void glBlendEquationSeparatei(std::uint32_t buf, GLenum modeRGB, GLenum modeAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquationSeparatei(buf, modeRGB, modeAlpha); }
void glBlendEquationSeparateiARB(std::uint32_t buf, GLenum modeRGB, GLenum modeAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquationSeparateiARB(buf, modeRGB, modeAlpha); }
void glBlendEquationi(std::uint32_t buf, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquationi(buf, mode); }
void glBlendEquationiARB(std::uint32_t buf, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendEquationiARB(buf, mode); }
void glBlendFunc(GLenum sfactor, GLenum dfactor) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFunc(sfactor, dfactor); }
void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha); }
void glBlendFuncSeparatei(std::uint32_t buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha); }
void glBlendFuncSeparateiARB(std::uint32_t buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFuncSeparateiARB(buf, srcRGB, dstRGB, srcAlpha, dstAlpha); }
void glBlendFunci(std::uint32_t buf, GLenum src, GLenum dst) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFunci(buf, src, dst); }
void glBlendFunciARB(std::uint32_t buf, GLenum src, GLenum dst) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blendFunciARB(buf, src, dst); }
void glBlitFramebuffer(std::int32_t srcX0, std::int32_t srcY0, std::int32_t srcX1, std::int32_t srcY1, std::int32_t dstX0, std::int32_t dstY0, std::int32_t dstX1, std::int32_t dstY1, GLbitfield mask, GLenum filter) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter); }
void glBlitNamedFramebuffer(std::uint32_t readFramebuffer, std::uint32_t drawFramebuffer, std::int32_t srcX0, std::int32_t srcY0, std::int32_t srcX1, std::int32_t srcY1, std::int32_t dstX0, std::int32_t dstY0, std::int32_t dstX1, std::int32_t dstY1, GLbitfield mask, GLenum filter) MYGL_NOEXCEPT { return mygl::get_static_dispatch().blitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter); }
void glBufferAddressRangeNV(GLenum pname, std::uint32_t index, std::uint64_t address, std::int64_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bufferAddressRangeNV(pname, index, address, length); }
void glBufferData(GLenum target, std::int64_t size, const void * data, GLenum usage) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bufferData(target, size, data, usage); }
void glBufferStorage(GLenum target, std::int64_t size, const void * data, GLbitfield flags) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bufferStorage(target, size, data, flags); }
void glBufferStorageMemEXT(GLenum target, std::int64_t size, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bufferStorageMemEXT(target, size, memory, offset); }
void glBufferSubData(GLenum target, std::intptr_t offset, std::int64_t size, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().bufferSubData(target, offset, size, data); }
void glCallCommandListNV(std::uint32_t list) MYGL_NOEXCEPT { return mygl::get_static_dispatch().callCommandListNV(list); }
GLenum glCheckFramebufferStatus(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().checkFramebufferStatus(target); }
GLenum glCheckNamedFramebufferStatus(std::uint32_t framebuffer, GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().checkNamedFramebufferStatus(framebuffer, target); }
void glClampColor(GLenum target, GLenum clamp) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clampColor(target, clamp); }
void glClear(GLbitfield mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clear(mask); }
void glClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferData(target, internalformat, format, type, data); }
void glClearBufferSubData(GLenum target, GLenum internalformat, std::intptr_t offset, std::int64_t size, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferSubData(target, internalformat, offset, size, format, type, data); }
void glClearBufferfi(GLenum buffer, std::int32_t drawbuffer, float depth, std::int32_t stencil) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferfi(buffer, drawbuffer, depth, stencil); }
void glClearBufferfv(GLenum buffer, std::int32_t drawbuffer, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferfv(buffer, drawbuffer, value); }
void glClearBufferiv(GLenum buffer, std::int32_t drawbuffer, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferiv(buffer, drawbuffer, value); }
void glClearBufferuiv(GLenum buffer, std::int32_t drawbuffer, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearBufferuiv(buffer, drawbuffer, value); }
void glClearColor(float red, float green, float blue, float alpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearColor(red, green, blue, alpha); }
void glClearDepth(double depth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearDepth(depth); }
void glClearDepthf(float d) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearDepthf(d); }
void glClearNamedBufferData(std::uint32_t buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedBufferData(buffer, internalformat, format, type, data); }
void glClearNamedBufferSubData(std::uint32_t buffer, GLenum internalformat, std::intptr_t offset, std::int64_t size, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedBufferSubData(buffer, internalformat, offset, size, format, type, data); }
void glClearNamedFramebufferfi(std::uint32_t framebuffer, GLenum buffer, std::int32_t drawbuffer, float depth, std::int32_t stencil) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedFramebufferfi(framebuffer, buffer, drawbuffer, depth, stencil); }
void glClearNamedFramebufferfv(std::uint32_t framebuffer, GLenum buffer, std::int32_t drawbuffer, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value); }
void glClearNamedFramebufferiv(std::uint32_t framebuffer, GLenum buffer, std::int32_t drawbuffer, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value); }
void glClearNamedFramebufferuiv(std::uint32_t framebuffer, GLenum buffer, std::int32_t drawbuffer, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value); }
void glClearStencil(std::int32_t s) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearStencil(s); }
void glClearTexImage(std::uint32_t texture, std::int32_t level, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearTexImage(texture, level, format, type, data); }
void glClearTexSubImage(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, GLenum type, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clearTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data); }
GLenum glClientWaitSync(struct __GLsync * sync, GLbitfield flags, std::uint64_t timeout) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clientWaitSync(sync, flags, timeout); }
void glClipControl(GLenum origin, GLenum depth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().clipControl(origin, depth); }
void glColorFormatNV(std::int32_t size, GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorFormatNV(size, type, stride); }
void glColorMask(bool red, bool green, bool blue, bool alpha) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorMask(red, green, blue, alpha); }
void glColorMaski(std::uint32_t index, bool r, bool g, bool b, bool a) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorMaski(index, r, g, b, a); }
void glColorP3ui(GLenum type, std::uint32_t color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorP3ui(type, color); }
void glColorP3uiv(GLenum type, const std::uint32_t * color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorP3uiv(type, color); }
void glColorP4ui(GLenum type, std::uint32_t color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorP4ui(type, color); }
void glColorP4uiv(GLenum type, const std::uint32_t * color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().colorP4uiv(type, color); }
void glCommandListSegmentsNV(std::uint32_t list, std::uint32_t segments) MYGL_NOEXCEPT { return mygl::get_static_dispatch().commandListSegmentsNV(list, segments); }
void glCompileCommandListNV(std::uint32_t list) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compileCommandListNV(list); }
void glCompileShader(std::uint32_t shader) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compileShader(shader); }
void glCompressedTexImage1D(GLenum target, std::int32_t level, GLenum internalformat, std::int32_t width, std::int32_t border, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexImage1D(target, level, internalformat, width, border, imageSize, data); }
void glCompressedTexImage2D(GLenum target, std::int32_t level, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t border, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); }
void glCompressedTexImage3D(GLenum target, std::int32_t level, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth, std::int32_t border, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data); }
void glCompressedTexSubImage1D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t width, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data); }
void glCompressedTexSubImage2D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t width, std::int32_t height, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data); }
void glCompressedTexSubImage3D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); }
void glCompressedTextureSubImage1D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t width, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, data); }
void glCompressedTextureSubImage2D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t width, std::int32_t height, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, data); }
void glCompressedTextureSubImage3D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, std::int32_t imageSize, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().compressedTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); }
void glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, std::intptr_t readOffset, std::intptr_t writeOffset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size); }
void glCopyImageSubData(std::uint32_t srcName, GLenum srcTarget, std::int32_t srcLevel, std::int32_t srcX, std::int32_t srcY, std::int32_t srcZ, std::uint32_t dstName, GLenum dstTarget, std::int32_t dstLevel, std::int32_t dstX, std::int32_t dstY, std::int32_t dstZ, std::int32_t srcWidth, std::int32_t srcHeight, std::int32_t srcDepth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth); }
void glCopyNamedBufferSubData(std::uint32_t readBuffer, std::uint32_t writeBuffer, std::intptr_t readOffset, std::intptr_t writeOffset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size); }
void glCopyPathNV(std::uint32_t resultPath, std::uint32_t srcPath) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyPathNV(resultPath, srcPath); }
void glCopyTexImage1D(GLenum target, std::int32_t level, GLenum internalformat, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t border) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTexImage1D(target, level, internalformat, x, y, width, border); }
void glCopyTexImage2D(GLenum target, std::int32_t level, GLenum internalformat, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t border) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTexImage2D(target, level, internalformat, x, y, width, height, border); }
void glCopyTexSubImage1D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t x, std::int32_t y, std::int32_t width) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTexSubImage1D(target, level, xoffset, x, y, width); }
void glCopyTexSubImage2D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height); }
void glCopyTexSubImage3D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height); }
void glCopyTextureSubImage1D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t x, std::int32_t y, std::int32_t width) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTextureSubImage1D(texture, level, xoffset, x, y, width); }
void glCopyTextureSubImage2D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTextureSubImage2D(texture, level, xoffset, yoffset, x, y, width, height); }
void glCopyTextureSubImage3D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().copyTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, x, y, width, height); }
void glCoverFillPathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, GLenum coverMode, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().coverFillPathInstancedNV(numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues); }
void glCoverFillPathNV(std::uint32_t path, GLenum coverMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().coverFillPathNV(path, coverMode); }
void glCoverStrokePathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, GLenum coverMode, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().coverStrokePathInstancedNV(numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues); }
void glCoverStrokePathNV(std::uint32_t path, GLenum coverMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().coverStrokePathNV(path, coverMode); }
void glCreateBuffers(std::int32_t n, std::uint32_t * buffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createBuffers(n, buffers); }
void glCreateCommandListsNV(std::int32_t n, std::uint32_t * lists) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createCommandListsNV(n, lists); }
void glCreateFramebuffers(std::int32_t n, std::uint32_t * framebuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createFramebuffers(n, framebuffers); }
void glCreateMemoryObjectsEXT(std::int32_t n, std::uint32_t * memoryObjects) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createMemoryObjectsEXT(n, memoryObjects); }
std::uint32_t glCreateProgram() MYGL_NOEXCEPT { return mygl::get_static_dispatch().createProgram(); }
void glCreateProgramPipelines(std::int32_t n, std::uint32_t * pipelines) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createProgramPipelines(n, pipelines); }
void glCreateQueries(GLenum target, std::int32_t n, std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createQueries(target, n, ids); }
void glCreateRenderbuffers(std::int32_t n, std::uint32_t * renderbuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createRenderbuffers(n, renderbuffers); }
void glCreateSamplers(std::int32_t n, std::uint32_t * samplers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createSamplers(n, samplers); }
std::uint32_t glCreateShader(GLenum type) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createShader(type); }
std::uint32_t glCreateShaderProgramv(GLenum type, std::int32_t count, const char *const* strings) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createShaderProgramv(type, count, strings); }
void glCreateStatesNV(std::int32_t n, std::uint32_t * states) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createStatesNV(n, states); }
void glCreateTextures(GLenum target, std::int32_t n, std::uint32_t * textures) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createTextures(target, n, textures); }
void glCreateTransformFeedbacks(std::int32_t n, std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createTransformFeedbacks(n, ids); }
void glCreateVertexArrays(std::int32_t n, std::uint32_t * arrays) MYGL_NOEXCEPT { return mygl::get_static_dispatch().createVertexArrays(n, arrays); }
void glCullFace(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().cullFace(mode); }
void glDebugMessageCallback(GLDEBUGPROC callback, const void * userParam) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageCallback(callback, userParam); }
void glDebugMessageCallbackKHR(GLDEBUGPROCKHR callback, const void * userParam) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageCallbackKHR(callback, userParam); }
void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, std::int32_t count, const std::uint32_t * ids, bool enabled) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageControl(source, type, severity, count, ids, enabled); }
void glDebugMessageControlKHR(GLenum source, GLenum type, GLenum severity, std::int32_t count, const std::uint32_t * ids, bool enabled) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageControlKHR(source, type, severity, count, ids, enabled); }
void glDebugMessageInsert(GLenum source, GLenum type, std::uint32_t id, GLenum severity, std::int32_t length, const char * buf) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageInsert(source, type, id, severity, length, buf); }
void glDebugMessageInsertKHR(GLenum source, GLenum type, std::uint32_t id, GLenum severity, std::int32_t length, const char * buf) MYGL_NOEXCEPT { return mygl::get_static_dispatch().debugMessageInsertKHR(source, type, id, severity, length, buf); }
void glDeleteBuffers(std::int32_t n, const std::uint32_t * buffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteBuffers(n, buffers); }
void glDeleteCommandListsNV(std::int32_t n, const std::uint32_t * lists) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteCommandListsNV(n, lists); }
void glDeleteFramebuffers(std::int32_t n, const std::uint32_t * framebuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteFramebuffers(n, framebuffers); }
void glDeleteMemoryObjectsEXT(std::int32_t n, const std::uint32_t * memoryObjects) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteMemoryObjectsEXT(n, memoryObjects); }
void glDeletePathsNV(std::uint32_t path, std::int32_t range) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deletePathsNV(path, range); }
void glDeleteProgram(std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteProgram(program); }
void glDeleteProgramPipelines(std::int32_t n, const std::uint32_t * pipelines) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteProgramPipelines(n, pipelines); }
void glDeleteQueries(std::int32_t n, const std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteQueries(n, ids); }
void glDeleteRenderbuffers(std::int32_t n, const std::uint32_t * renderbuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteRenderbuffers(n, renderbuffers); }
void glDeleteSamplers(std::int32_t count, const std::uint32_t * samplers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteSamplers(count, samplers); }
void glDeleteSemaphoresEXT(std::int32_t n, const std::uint32_t * semaphores) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteSemaphoresEXT(n, semaphores); }
void glDeleteShader(std::uint32_t shader) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteShader(shader); }
void glDeleteStatesNV(std::int32_t n, const std::uint32_t * states) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteStatesNV(n, states); }
void glDeleteSync(struct __GLsync * sync) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteSync(sync); }
void glDeleteTextures(std::int32_t n, const std::uint32_t * textures) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteTextures(n, textures); }
void glDeleteTransformFeedbacks(std::int32_t n, const std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteTransformFeedbacks(n, ids); }
void glDeleteVertexArrays(std::int32_t n, const std::uint32_t * arrays) MYGL_NOEXCEPT { return mygl::get_static_dispatch().deleteVertexArrays(n, arrays); }
void glDepthBoundsEXT(double zmin, double zmax) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthBoundsEXT(zmin, zmax); }
void glDepthFunc(GLenum func) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthFunc(func); }
void glDepthMask(bool flag) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthMask(flag); }
void glDepthRange(double n, double f) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthRange(n, f); }
void glDepthRangeArrayv(std::uint32_t first, std::int32_t count, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthRangeArrayv(first, count, v); }
void glDepthRangeIndexed(std::uint32_t index, double n, double f) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthRangeIndexed(index, n, f); }
void glDepthRangef(float n, float f) MYGL_NOEXCEPT { return mygl::get_static_dispatch().depthRangef(n, f); }
void glDetachShader(std::uint32_t program, std::uint32_t shader) MYGL_NOEXCEPT { return mygl::get_static_dispatch().detachShader(program, shader); }
void glDisable(GLenum cap) MYGL_NOEXCEPT { return mygl::get_static_dispatch().disable(cap); }
void glDisableClientState(GLenum array) MYGL_NOEXCEPT { return mygl::get_static_dispatch().disableClientState(array); }
void glDisableVertexArrayAttrib(std::uint32_t vaobj, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().disableVertexArrayAttrib(vaobj, index); }
void glDisableVertexAttribArray(std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().disableVertexAttribArray(index); }
void glDisablei(GLenum target, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().disablei(target, index); }
void glDispatchCompute(std::uint32_t num_groups_x, std::uint32_t num_groups_y, std::uint32_t num_groups_z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().dispatchCompute(num_groups_x, num_groups_y, num_groups_z); }
void glDispatchComputeGroupSizeARB(std::uint32_t num_groups_x, std::uint32_t num_groups_y, std::uint32_t num_groups_z, std::uint32_t group_size_x, std::uint32_t group_size_y, std::uint32_t group_size_z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().dispatchComputeGroupSizeARB(num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z); }
void glDispatchComputeIndirect(std::intptr_t indirect) MYGL_NOEXCEPT { return mygl::get_static_dispatch().dispatchComputeIndirect(indirect); }
void glDrawArrays(GLenum mode, std::int32_t first, std::int32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawArrays(mode, first, count); }
void glDrawArraysIndirect(GLenum mode, const void * indirect) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawArraysIndirect(mode, indirect); }
void glDrawArraysInstanced(GLenum mode, std::int32_t first, std::int32_t count, std::int32_t instancecount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawArraysInstanced(mode, first, count, instancecount); }
void glDrawArraysInstancedBaseInstance(GLenum mode, std::int32_t first, std::int32_t count, std::int32_t instancecount, std::uint32_t baseinstance) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawArraysInstancedBaseInstance(mode, first, count, instancecount, baseinstance); }
void glDrawBuffer(GLenum buf) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawBuffer(buf); }
void glDrawBuffers(std::int32_t n, const GLenum * bufs) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawBuffers(n, bufs); }
void glDrawCommandsAddressNV(GLenum primitiveMode, const std::uint64_t * indirects, const std::int32_t * sizes, std::uint32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawCommandsAddressNV(primitiveMode, indirects, sizes, count); }
void glDrawCommandsNV(GLenum primitiveMode, std::uint32_t buffer, const std::intptr_t * indirects, const std::int32_t * sizes, std::uint32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawCommandsNV(primitiveMode, buffer, indirects, sizes, count); }
void glDrawCommandsStatesAddressNV(const std::uint64_t * indirects, const std::int32_t * sizes, const std::uint32_t * states, const std::uint32_t * fbos, std::uint32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawCommandsStatesAddressNV(indirects, sizes, states, fbos, count); }
void glDrawCommandsStatesNV(std::uint32_t buffer, const std::intptr_t * indirects, const std::int32_t * sizes, const std::uint32_t * states, const std::uint32_t * fbos, std::uint32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawCommandsStatesNV(buffer, indirects, sizes, states, fbos, count); }
void glDrawElements(GLenum mode, std::int32_t count, GLenum type, const void * indices) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElements(mode, count, type, indices); }
void glDrawElementsBaseVertex(GLenum mode, std::int32_t count, GLenum type, const void * indices, std::int32_t basevertex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsBaseVertex(mode, count, type, indices, basevertex); }
void glDrawElementsIndirect(GLenum mode, GLenum type, const void * indirect) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsIndirect(mode, type, indirect); }
void glDrawElementsInstanced(GLenum mode, std::int32_t count, GLenum type, const void * indices, std::int32_t instancecount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsInstanced(mode, count, type, indices, instancecount); }
void glDrawElementsInstancedBaseInstance(GLenum mode, std::int32_t count, GLenum type, const void * indices, std::int32_t instancecount, std::uint32_t baseinstance) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsInstancedBaseInstance(mode, count, type, indices, instancecount, baseinstance); }
void glDrawElementsInstancedBaseVertex(GLenum mode, std::int32_t count, GLenum type, const void * indices, std::int32_t instancecount, std::int32_t basevertex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex); }
void glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, std::int32_t count, GLenum type, const void * indices, std::int32_t instancecount, std::int32_t basevertex, std::uint32_t baseinstance) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instancecount, basevertex, baseinstance); }
void glDrawRangeElements(GLenum mode, std::uint32_t start, std::uint32_t end, std::int32_t count, GLenum type, const void * indices) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawRangeElements(mode, start, end, count, type, indices); }
void glDrawRangeElementsBaseVertex(GLenum mode, std::uint32_t start, std::uint32_t end, std::int32_t count, GLenum type, const void * indices, std::int32_t basevertex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex); }
void glDrawTextureNV(std::uint32_t texture, std::uint32_t sampler, float x0, float y0, float x1, float y1, float z, float s0, float t0, float s1, float t1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawTextureNV(texture, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1); }
void glDrawTransformFeedback(GLenum mode, std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawTransformFeedback(mode, id); }
void glDrawTransformFeedbackInstanced(GLenum mode, std::uint32_t id, std::int32_t instancecount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawTransformFeedbackInstanced(mode, id, instancecount); }
void glDrawTransformFeedbackStream(GLenum mode, std::uint32_t id, std::uint32_t stream) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawTransformFeedbackStream(mode, id, stream); }
void glDrawTransformFeedbackStreamInstanced(GLenum mode, std::uint32_t id, std::uint32_t stream, std::int32_t instancecount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().drawTransformFeedbackStreamInstanced(mode, id, stream, instancecount); }
void glEdgeFlagFormatNV(std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().edgeFlagFormatNV(stride); }
void glEnable(GLenum cap) MYGL_NOEXCEPT { return mygl::get_static_dispatch().enable(cap); }
void glEnableClientState(GLenum array) MYGL_NOEXCEPT { return mygl::get_static_dispatch().enableClientState(array); }
void glEnableVertexArrayAttrib(std::uint32_t vaobj, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().enableVertexArrayAttrib(vaobj, index); }
void glEnableVertexAttribArray(std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().enableVertexAttribArray(index); }
void glEnablei(GLenum target, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().enablei(target, index); }
void glEndConditionalRender() MYGL_NOEXCEPT { return mygl::get_static_dispatch().endConditionalRender(); }
void glEndQuery(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().endQuery(target); }
void glEndQueryIndexed(GLenum target, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().endQueryIndexed(target, index); }
void glEndTransformFeedback() MYGL_NOEXCEPT { return mygl::get_static_dispatch().endTransformFeedback(); }
struct __GLsync * glFenceSync(GLenum condition, GLbitfield flags) MYGL_NOEXCEPT { return mygl::get_static_dispatch().fenceSync(condition, flags); }
void glFinish() MYGL_NOEXCEPT { return mygl::get_static_dispatch().finish(); }
void glFlush() MYGL_NOEXCEPT { return mygl::get_static_dispatch().flush(); }
void glFlushMappedBufferRange(GLenum target, std::intptr_t offset, std::int64_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().flushMappedBufferRange(target, offset, length); }
void glFlushMappedNamedBufferRange(std::uint32_t buffer, std::intptr_t offset, std::int64_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().flushMappedNamedBufferRange(buffer, offset, length); }
void glFogCoordFormatNV(GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().fogCoordFormatNV(type, stride); }
void glFramebufferParameteri(GLenum target, GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferParameteri(target, pname, param); }
void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, std::uint32_t renderbuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); }
void glFramebufferTexture(GLenum target, GLenum attachment, std::uint32_t texture, std::int32_t level) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferTexture(target, attachment, texture, level); }
void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, std::uint32_t texture, std::int32_t level) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferTexture1D(target, attachment, textarget, texture, level); }
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, std::uint32_t texture, std::int32_t level) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferTexture2D(target, attachment, textarget, texture, level); }
void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, std::uint32_t texture, std::int32_t level, std::int32_t zoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferTexture3D(target, attachment, textarget, texture, level, zoffset); }
void glFramebufferTextureLayer(GLenum target, GLenum attachment, std::uint32_t texture, std::int32_t level, std::int32_t layer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().framebufferTextureLayer(target, attachment, texture, level, layer); }
void glFrontFace(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().frontFace(mode); }
void glGenBuffers(std::int32_t n, std::uint32_t * buffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genBuffers(n, buffers); }
void glGenFramebuffers(std::int32_t n, std::uint32_t * framebuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genFramebuffers(n, framebuffers); }
std::uint32_t glGenPathsNV(std::int32_t range) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genPathsNV(range); }
void glGenProgramPipelines(std::int32_t n, std::uint32_t * pipelines) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genProgramPipelines(n, pipelines); }
void glGenQueries(std::int32_t n, std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genQueries(n, ids); }
void glGenRenderbuffers(std::int32_t n, std::uint32_t * renderbuffers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genRenderbuffers(n, renderbuffers); }
void glGenSamplers(std::int32_t count, std::uint32_t * samplers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genSamplers(count, samplers); }
void glGenSemaphoresEXT(std::int32_t n, std::uint32_t * semaphores) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genSemaphoresEXT(n, semaphores); }
void glGenTextures(std::int32_t n, std::uint32_t * textures) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genTextures(n, textures); }
void glGenTransformFeedbacks(std::int32_t n, std::uint32_t * ids) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genTransformFeedbacks(n, ids); }
void glGenVertexArrays(std::int32_t n, std::uint32_t * arrays) MYGL_NOEXCEPT { return mygl::get_static_dispatch().genVertexArrays(n, arrays); }
void glGenerateMipmap(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().generateMipmap(target); }
void glGenerateTextureMipmap(std::uint32_t texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().generateTextureMipmap(texture); }
void glGetActiveAtomicCounterBufferiv(std::uint32_t program, std::uint32_t bufferIndex, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveAtomicCounterBufferiv(program, bufferIndex, pname, params); }
void glGetActiveAttrib(std::uint32_t program, std::uint32_t index, std::int32_t bufSize, std::int32_t * length, std::int32_t * size, GLenum * type, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveAttrib(program, index, bufSize, length, size, type, name); }
void glGetActiveSubroutineName(std::uint32_t program, GLenum shadertype, std::uint32_t index, std::int32_t bufsize, std::int32_t * length, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveSubroutineName(program, shadertype, index, bufsize, length, name); }
void glGetActiveSubroutineUniformName(std::uint32_t program, GLenum shadertype, std::uint32_t index, std::int32_t bufsize, std::int32_t * length, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveSubroutineUniformName(program, shadertype, index, bufsize, length, name); }
void glGetActiveSubroutineUniformiv(std::uint32_t program, GLenum shadertype, std::uint32_t index, GLenum pname, std::int32_t * values) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveSubroutineUniformiv(program, shadertype, index, pname, values); }
void glGetActiveUniform(std::uint32_t program, std::uint32_t index, std::int32_t bufSize, std::int32_t * length, std::int32_t * size, GLenum * type, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveUniform(program, index, bufSize, length, size, type, name); }
void glGetActiveUniformBlockName(std::uint32_t program, std::uint32_t uniformBlockIndex, std::int32_t bufSize, std::int32_t * length, char * uniformBlockName) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName); }
void glGetActiveUniformBlockiv(std::uint32_t program, std::uint32_t uniformBlockIndex, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveUniformBlockiv(program, uniformBlockIndex, pname, params); }
void glGetActiveUniformName(std::uint32_t program, std::uint32_t uniformIndex, std::int32_t bufSize, std::int32_t * length, char * uniformName) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveUniformName(program, uniformIndex, bufSize, length, uniformName); }
void glGetActiveUniformsiv(std::uint32_t program, std::int32_t uniformCount, const std::uint32_t * uniformIndices, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getActiveUniformsiv(program, uniformCount, uniformIndices, pname, params); }
void glGetAttachedShaders(std::uint32_t program, std::int32_t maxCount, std::int32_t * count, std::uint32_t * shaders) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getAttachedShaders(program, maxCount, count, shaders); }
std::int32_t glGetAttribLocation(std::uint32_t program, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getAttribLocation(program, name); }
void glGetBooleani_v(GLenum target, std::uint32_t index, bool * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBooleani_v(target, index, data); }
void glGetBooleanv(GLenum pname, bool * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBooleanv(pname, data); }
void glGetBufferParameteri64v(GLenum target, GLenum pname, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBufferParameteri64v(target, pname, params); }
void glGetBufferParameteriv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBufferParameteriv(target, pname, params); }
void glGetBufferParameterui64vNV(GLenum target, GLenum pname, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBufferParameterui64vNV(target, pname, params); }
void glGetBufferPointerv(GLenum target, GLenum pname, void ** params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBufferPointerv(target, pname, params); }
void glGetBufferSubData(GLenum target, std::intptr_t offset, std::int64_t size, void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getBufferSubData(target, offset, size, data); }
std::uint32_t glGetCommandHeaderNV(GLenum tokenID, std::uint32_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getCommandHeaderNV(tokenID, size); }
void glGetCompressedTexImage(GLenum target, std::int32_t level, void * img) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getCompressedTexImage(target, level, img); }
void glGetCompressedTextureImage(std::uint32_t texture, std::int32_t level, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getCompressedTextureImage(texture, level, bufSize, pixels); }
void glGetCompressedTextureSubImage(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getCompressedTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels); }
std::uint32_t glGetDebugMessageLog(std::uint32_t count, std::int32_t bufSize, GLenum * sources, GLenum * types, std::uint32_t * ids, GLenum * severities, std::int32_t * lengths, char * messageLog) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog); }
std::uint32_t glGetDebugMessageLogKHR(std::uint32_t count, std::int32_t bufSize, GLenum * sources, GLenum * types, std::uint32_t * ids, GLenum * severities, std::int32_t * lengths, char * messageLog) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getDebugMessageLogKHR(count, bufSize, sources, types, ids, severities, lengths, messageLog); }
void glGetDoublei_v(GLenum target, std::uint32_t index, double * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getDoublei_v(target, index, data); }
void glGetDoublev(GLenum pname, double * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getDoublev(pname, data); }
GLenum glGetError() MYGL_NOEXCEPT { return mygl::get_static_dispatch().getError(); }
void glGetFloati_v(GLenum target, std::uint32_t index, float * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFloati_v(target, index, data); }
void glGetFloatv(GLenum pname, float * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFloatv(pname, data); }
std::int32_t glGetFragDataIndex(std::uint32_t program, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFragDataIndex(program, name); }
std::int32_t glGetFragDataLocation(std::uint32_t program, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFragDataLocation(program, name); }
void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFramebufferAttachmentParameteriv(target, attachment, pname, params); }
void glGetFramebufferParameteriv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getFramebufferParameteriv(target, pname, params); }
GLenum glGetGraphicsResetStatus() MYGL_NOEXCEPT { return mygl::get_static_dispatch().getGraphicsResetStatus(); }
std::uint64_t glGetImageHandleARB(std::uint32_t texture, std::int32_t level, bool layered, std::int32_t layer, GLenum format) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getImageHandleARB(texture, level, layered, layer, format); }
void glGetInteger64i_v(GLenum target, std::uint32_t index, std::int64_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getInteger64i_v(target, index, data); }
void glGetInteger64v(GLenum pname, std::int64_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getInteger64v(pname, data); }
void glGetIntegeri_v(GLenum target, std::uint32_t index, std::int32_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getIntegeri_v(target, index, data); }
void glGetIntegerui64i_vNV(GLenum value, std::uint32_t index, std::uint64_t * result) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getIntegerui64i_vNV(value, index, result); }
void glGetIntegerui64vNV(GLenum value, std::uint64_t * result) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getIntegerui64vNV(value, result); }
void glGetIntegerv(GLenum pname, std::int32_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getIntegerv(pname, data); }
void glGetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, std::int32_t bufSize, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getInternalformati64v(target, internalformat, pname, bufSize, params); }
void glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, std::int32_t bufSize, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getInternalformativ(target, internalformat, pname, bufSize, params); }
void glGetMemoryObjectParameterivEXT(std::uint32_t memoryObject, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getMemoryObjectParameterivEXT(memoryObject, pname, params); }
void glGetMultisamplefv(GLenum pname, std::uint32_t index, float * val) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getMultisamplefv(pname, index, val); }
void glGetNamedBufferParameteri64v(std::uint32_t buffer, GLenum pname, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedBufferParameteri64v(buffer, pname, params); }
void glGetNamedBufferParameteriv(std::uint32_t buffer, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedBufferParameteriv(buffer, pname, params); }
void glGetNamedBufferParameterui64vNV(std::uint32_t buffer, GLenum pname, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedBufferParameterui64vNV(buffer, pname, params); }
void glGetNamedBufferPointerv(std::uint32_t buffer, GLenum pname, void ** params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedBufferPointerv(buffer, pname, params); }
void glGetNamedBufferSubData(std::uint32_t buffer, std::intptr_t offset, std::int64_t size, void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedBufferSubData(buffer, offset, size, data); }
void glGetNamedFramebufferAttachmentParameteriv(std::uint32_t framebuffer, GLenum attachment, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedFramebufferAttachmentParameteriv(framebuffer, attachment, pname, params); }
void glGetNamedFramebufferParameteriv(std::uint32_t framebuffer, GLenum pname, std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedFramebufferParameteriv(framebuffer, pname, param); }
void glGetNamedRenderbufferParameteriv(std::uint32_t renderbuffer, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getNamedRenderbufferParameteriv(renderbuffer, pname, params); }
void glGetObjectLabel(GLenum identifier, std::uint32_t name, std::int32_t bufSize, std::int32_t * length, char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getObjectLabel(identifier, name, bufSize, length, label); }
void glGetObjectLabelKHR(GLenum identifier, std::uint32_t name, std::int32_t bufSize, std::int32_t * length, char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getObjectLabelKHR(identifier, name, bufSize, length, label); }
void glGetObjectPtrLabel(const void * ptr, std::int32_t bufSize, std::int32_t * length, char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getObjectPtrLabel(ptr, bufSize, length, label); }
void glGetObjectPtrLabelKHR(const void * ptr, std::int32_t bufSize, std::int32_t * length, char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getObjectPtrLabelKHR(ptr, bufSize, length, label); }
void glGetPathColorGenfvNV(GLenum color, GLenum pname, float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathColorGenfvNV(color, pname, value); }
void glGetPathColorGenivNV(GLenum color, GLenum pname, std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathColorGenivNV(color, pname, value); }
void glGetPathCommandsNV(std::uint32_t path, std::uint8_t * commands) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathCommandsNV(path, commands); }
void glGetPathCoordsNV(std::uint32_t path, float * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathCoordsNV(path, coords); }
void glGetPathDashArrayNV(std::uint32_t path, float * dashArray) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathDashArrayNV(path, dashArray); }
float glGetPathLengthNV(std::uint32_t path, std::int32_t startSegment, std::int32_t numSegments) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathLengthNV(path, startSegment, numSegments); }
void glGetPathMetricRangeNV(GLbitfield metricQueryMask, std::uint32_t firstPathName, std::int32_t numPaths, std::int32_t stride, float * metrics) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathMetricRangeNV(metricQueryMask, firstPathName, numPaths, stride, metrics); }
void glGetPathMetricsNV(GLbitfield metricQueryMask, std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, std::int32_t stride, float * metrics) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathMetricsNV(metricQueryMask, numPaths, pathNameType, paths, pathBase, stride, metrics); }
void glGetPathParameterfvNV(std::uint32_t path, GLenum pname, float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathParameterfvNV(path, pname, value); }
void glGetPathParameterivNV(std::uint32_t path, GLenum pname, std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathParameterivNV(path, pname, value); }
void glGetPathSpacingNV(GLenum pathListMode, std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, float advanceScale, float kerningScale, GLenum transformType, float * returnedSpacing) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathSpacingNV(pathListMode, numPaths, pathNameType, paths, pathBase, advanceScale, kerningScale, transformType, returnedSpacing); }
void glGetPathTexGenfvNV(GLenum texCoordSet, GLenum pname, float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathTexGenfvNV(texCoordSet, pname, value); }
void glGetPathTexGenivNV(GLenum texCoordSet, GLenum pname, std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPathTexGenivNV(texCoordSet, pname, value); }
void glGetPointerv(GLenum pname, void ** params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPointerv(pname, params); }
void glGetPointervKHR(GLenum pname, void ** params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getPointervKHR(pname, params); }
void glGetProgramBinary(std::uint32_t program, std::int32_t bufSize, std::int32_t * length, GLenum * binaryFormat, void * binary) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramBinary(program, bufSize, length, binaryFormat, binary); }
void glGetProgramInfoLog(std::uint32_t program, std::int32_t bufSize, std::int32_t * length, char * infoLog) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramInfoLog(program, bufSize, length, infoLog); }
void glGetProgramInterfaceiv(std::uint32_t program, GLenum programInterface, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramInterfaceiv(program, programInterface, pname, params); }
void glGetProgramPipelineInfoLog(std::uint32_t pipeline, std::int32_t bufSize, std::int32_t * length, char * infoLog) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramPipelineInfoLog(pipeline, bufSize, length, infoLog); }
void glGetProgramPipelineiv(std::uint32_t pipeline, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramPipelineiv(pipeline, pname, params); }
std::uint32_t glGetProgramResourceIndex(std::uint32_t program, GLenum programInterface, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourceIndex(program, programInterface, name); }
std::int32_t glGetProgramResourceLocation(std::uint32_t program, GLenum programInterface, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourceLocation(program, programInterface, name); }
std::int32_t glGetProgramResourceLocationIndex(std::uint32_t program, GLenum programInterface, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourceLocationIndex(program, programInterface, name); }
void glGetProgramResourceName(std::uint32_t program, GLenum programInterface, std::uint32_t index, std::int32_t bufSize, std::int32_t * length, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourceName(program, programInterface, index, bufSize, length, name); }
void glGetProgramResourcefvNV(std::uint32_t program, GLenum programInterface, std::uint32_t index, std::int32_t propCount, const GLenum * props, std::int32_t bufSize, std::int32_t * length, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourcefvNV(program, programInterface, index, propCount, props, bufSize, length, params); }
void glGetProgramResourceiv(std::uint32_t program, GLenum programInterface, std::uint32_t index, std::int32_t propCount, const GLenum * props, std::int32_t bufSize, std::int32_t * length, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramResourceiv(program, programInterface, index, propCount, props, bufSize, length, params); }
void glGetProgramStageiv(std::uint32_t program, GLenum shadertype, GLenum pname, std::int32_t * values) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramStageiv(program, shadertype, pname, values); }
void glGetProgramSubroutineParameteruivNV(GLenum target, std::uint32_t index, std::uint32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramSubroutineParameteruivNV(target, index, param); }
void glGetProgramiv(std::uint32_t program, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getProgramiv(program, pname, params); }
void glGetQueryBufferObjecti64v(std::uint32_t id, std::uint32_t buffer, GLenum pname, std::intptr_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryBufferObjecti64v(id, buffer, pname, offset); }
void glGetQueryBufferObjectiv(std::uint32_t id, std::uint32_t buffer, GLenum pname, std::intptr_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryBufferObjectiv(id, buffer, pname, offset); }
void glGetQueryBufferObjectui64v(std::uint32_t id, std::uint32_t buffer, GLenum pname, std::intptr_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryBufferObjectui64v(id, buffer, pname, offset); }
void glGetQueryBufferObjectuiv(std::uint32_t id, std::uint32_t buffer, GLenum pname, std::intptr_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryBufferObjectuiv(id, buffer, pname, offset); }
void glGetQueryIndexediv(GLenum target, std::uint32_t index, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryIndexediv(target, index, pname, params); }
void glGetQueryObjecti64v(std::uint32_t id, GLenum pname, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryObjecti64v(id, pname, params); }
void glGetQueryObjectiv(std::uint32_t id, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryObjectiv(id, pname, params); }
void glGetQueryObjectui64v(std::uint32_t id, GLenum pname, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryObjectui64v(id, pname, params); }
void glGetQueryObjectuiv(std::uint32_t id, GLenum pname, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryObjectuiv(id, pname, params); }
void glGetQueryiv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getQueryiv(target, pname, params); }
void glGetRenderbufferParameteriv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getRenderbufferParameteriv(target, pname, params); }
void glGetSamplerParameterIiv(std::uint32_t sampler, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSamplerParameterIiv(sampler, pname, params); }
void glGetSamplerParameterIuiv(std::uint32_t sampler, GLenum pname, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSamplerParameterIuiv(sampler, pname, params); }
void glGetSamplerParameterfv(std::uint32_t sampler, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSamplerParameterfv(sampler, pname, params); }
void glGetSamplerParameteriv(std::uint32_t sampler, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSamplerParameteriv(sampler, pname, params); }
void glGetSemaphoreParameterui64vEXT(std::uint32_t semaphore, GLenum pname, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSemaphoreParameterui64vEXT(semaphore, pname, params); }
void glGetShaderInfoLog(std::uint32_t shader, std::int32_t bufSize, std::int32_t * length, char * infoLog) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getShaderInfoLog(shader, bufSize, length, infoLog); }
void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, std::int32_t * range, std::int32_t * precision) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getShaderPrecisionFormat(shadertype, precisiontype, range, precision); }
void glGetShaderSource(std::uint32_t shader, std::int32_t bufSize, std::int32_t * length, char * source) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getShaderSource(shader, bufSize, length, source); }
void glGetShaderiv(std::uint32_t shader, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getShaderiv(shader, pname, params); }
std::uint16_t glGetStageIndexNV(GLenum shadertype) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getStageIndexNV(shadertype); }
const std::uint8_t  *glGetString(GLenum name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getString(name); }
const std::uint8_t  *glGetStringi(GLenum name, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getStringi(name, index); }
std::uint32_t glGetSubroutineIndex(std::uint32_t program, GLenum shadertype, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSubroutineIndex(program, shadertype, name); }
std::int32_t glGetSubroutineUniformLocation(std::uint32_t program, GLenum shadertype, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSubroutineUniformLocation(program, shadertype, name); }
void glGetSynciv(struct __GLsync * sync, GLenum pname, std::int32_t bufSize, std::int32_t * length, std::int32_t * values) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getSynciv(sync, pname, bufSize, length, values); }
void glGetTexImage(GLenum target, std::int32_t level, GLenum format, GLenum type, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexImage(target, level, format, type, pixels); }
void glGetTexLevelParameterfv(GLenum target, std::int32_t level, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexLevelParameterfv(target, level, pname, params); }
void glGetTexLevelParameteriv(GLenum target, std::int32_t level, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexLevelParameteriv(target, level, pname, params); }
void glGetTexParameterIiv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexParameterIiv(target, pname, params); }
void glGetTexParameterIuiv(GLenum target, GLenum pname, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexParameterIuiv(target, pname, params); }
void glGetTexParameterfv(GLenum target, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexParameterfv(target, pname, params); }
void glGetTexParameteriv(GLenum target, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTexParameteriv(target, pname, params); }
std::uint64_t glGetTextureHandleARB(std::uint32_t texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureHandleARB(texture); }
void glGetTextureImage(std::uint32_t texture, std::int32_t level, GLenum format, GLenum type, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureImage(texture, level, format, type, bufSize, pixels); }
void glGetTextureLevelParameterfv(std::uint32_t texture, std::int32_t level, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureLevelParameterfv(texture, level, pname, params); }
void glGetTextureLevelParameteriv(std::uint32_t texture, std::int32_t level, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureLevelParameteriv(texture, level, pname, params); }
void glGetTextureParameterIiv(std::uint32_t texture, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureParameterIiv(texture, pname, params); }
void glGetTextureParameterIuiv(std::uint32_t texture, GLenum pname, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureParameterIuiv(texture, pname, params); }
void glGetTextureParameterfv(std::uint32_t texture, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureParameterfv(texture, pname, params); }
void glGetTextureParameteriv(std::uint32_t texture, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureParameteriv(texture, pname, params); }
std::uint64_t glGetTextureSamplerHandleARB(std::uint32_t texture, std::uint32_t sampler) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureSamplerHandleARB(texture, sampler); }
void glGetTextureSubImage(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, GLenum type, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels); }
void glGetTransformFeedbackVarying(std::uint32_t program, std::uint32_t index, std::int32_t bufSize, std::int32_t * length, std::int32_t * size, GLenum * type, char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTransformFeedbackVarying(program, index, bufSize, length, size, type, name); }
void glGetTransformFeedbacki64_v(std::uint32_t xfb, GLenum pname, std::uint32_t index, std::int64_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTransformFeedbacki64_v(xfb, pname, index, param); }
void glGetTransformFeedbacki_v(std::uint32_t xfb, GLenum pname, std::uint32_t index, std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTransformFeedbacki_v(xfb, pname, index, param); }
void glGetTransformFeedbackiv(std::uint32_t xfb, GLenum pname, std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getTransformFeedbackiv(xfb, pname, param); }
std::uint32_t glGetUniformBlockIndex(std::uint32_t program, const char * uniformBlockName) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformBlockIndex(program, uniformBlockName); }
void glGetUniformIndices(std::uint32_t program, std::int32_t uniformCount, const char *const* uniformNames, std::uint32_t * uniformIndices) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformIndices(program, uniformCount, uniformNames, uniformIndices); }
std::int32_t glGetUniformLocation(std::uint32_t program, const char * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformLocation(program, name); }
void glGetUniformSubroutineuiv(GLenum shadertype, std::int32_t location, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformSubroutineuiv(shadertype, location, params); }
void glGetUniformdv(std::uint32_t program, std::int32_t location, double * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformdv(program, location, params); }
void glGetUniformfv(std::uint32_t program, std::int32_t location, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformfv(program, location, params); }
void glGetUniformi64vARB(std::uint32_t program, std::int32_t location, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformi64vARB(program, location, params); }
void glGetUniformi64vNV(std::uint32_t program, std::int32_t location, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformi64vNV(program, location, params); }
void glGetUniformiv(std::uint32_t program, std::int32_t location, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformiv(program, location, params); }
void glGetUniformui64vARB(std::uint32_t program, std::int32_t location, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformui64vARB(program, location, params); }
void glGetUniformui64vNV(std::uint32_t program, std::int32_t location, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformui64vNV(program, location, params); }
void glGetUniformuiv(std::uint32_t program, std::int32_t location, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUniformuiv(program, location, params); }
void glGetUnsignedBytevEXT(GLenum pname, std::uint8_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUnsignedBytevEXT(pname, data); }
void glGetUnsignedBytei_vEXT(GLenum target, std::uint32_t index, std::uint8_t * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getUnsignedBytei_vEXT(target, index, data); }
void glGetVertexArrayIndexed64iv(std::uint32_t vaobj, std::uint32_t index, GLenum pname, std::int64_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexArrayIndexed64iv(vaobj, index, pname, param); }
void glGetVertexArrayIndexediv(std::uint32_t vaobj, std::uint32_t index, GLenum pname, std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexArrayIndexediv(vaobj, index, pname, param); }
void glGetVertexArrayiv(std::uint32_t vaobj, GLenum pname, std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexArrayiv(vaobj, pname, param); }
void glGetVertexAttribIiv(std::uint32_t index, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribIiv(index, pname, params); }
void glGetVertexAttribIuiv(std::uint32_t index, GLenum pname, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribIuiv(index, pname, params); }
void glGetVertexAttribLdv(std::uint32_t index, GLenum pname, double * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribLdv(index, pname, params); }
void glGetVertexAttribLui64vARB(std::uint32_t index, GLenum pname, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribLui64vARB(index, pname, params); }
void glGetVertexAttribPointerv(std::uint32_t index, GLenum pname, void ** pointer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribPointerv(index, pname, pointer); }
void glGetVertexAttribdv(std::uint32_t index, GLenum pname, double * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribdv(index, pname, params); }
void glGetVertexAttribfv(std::uint32_t index, GLenum pname, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribfv(index, pname, params); }
void glGetVertexAttribiv(std::uint32_t index, GLenum pname, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getVertexAttribiv(index, pname, params); }
void glGetnCompressedTexImage(GLenum target, std::int32_t lod, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnCompressedTexImage(target, lod, bufSize, pixels); }
void glGetnTexImage(GLenum target, std::int32_t level, GLenum format, GLenum type, std::int32_t bufSize, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnTexImage(target, level, format, type, bufSize, pixels); }
void glGetnUniformdv(std::uint32_t program, std::int32_t location, std::int32_t bufSize, double * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformdv(program, location, bufSize, params); }
void glGetnUniformfv(std::uint32_t program, std::int32_t location, std::int32_t bufSize, float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformfv(program, location, bufSize, params); }
void glGetnUniformi64vARB(std::uint32_t program, std::int32_t location, std::int32_t bufSize, std::int64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformi64vARB(program, location, bufSize, params); }
void glGetnUniformiv(std::uint32_t program, std::int32_t location, std::int32_t bufSize, std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformiv(program, location, bufSize, params); }
void glGetnUniformui64vARB(std::uint32_t program, std::int32_t location, std::int32_t bufSize, std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformui64vARB(program, location, bufSize, params); }
void glGetnUniformuiv(std::uint32_t program, std::int32_t location, std::int32_t bufSize, std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().getnUniformuiv(program, location, bufSize, params); }
void glHint(GLenum target, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().hint(target, mode); }
void glImportMemoryFdEXT(std::uint32_t memory, std::uint64_t size, GLenum handleType, std::int32_t fd) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importMemoryFdEXT(memory, size, handleType, fd); }
void glImportMemoryWin32HandleEXT(std::uint32_t memory, std::uint64_t size, GLenum handleType, void * handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importMemoryWin32HandleEXT(memory, size, handleType, handle); }
void glImportMemoryWin32NameEXT(std::uint32_t memory, std::uint64_t size, GLenum handleType, const void * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importMemoryWin32NameEXT(memory, size, handleType, name); }
void glImportSemaphoreFdEXT(std::uint32_t semaphore, GLenum handleType, std::int32_t fd) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importSemaphoreFdEXT(semaphore, handleType, fd); }
void glImportSemaphoreWin32HandleEXT(std::uint32_t semaphore, GLenum handleType, void * handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importSemaphoreWin32HandleEXT(semaphore, handleType, handle); }
void glImportSemaphoreWin32NameEXT(std::uint32_t semaphore, GLenum handleType, const void * name) MYGL_NOEXCEPT { return mygl::get_static_dispatch().importSemaphoreWin32NameEXT(semaphore, handleType, name); }
void glIndexFormatNV(GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().indexFormatNV(type, stride); }
void glInterpolatePathsNV(std::uint32_t resultPath, std::uint32_t pathA, std::uint32_t pathB, float weight) MYGL_NOEXCEPT { return mygl::get_static_dispatch().interpolatePathsNV(resultPath, pathA, pathB, weight); }
void glInvalidateBufferData(std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateBufferData(buffer); }
void glInvalidateBufferSubData(std::uint32_t buffer, std::intptr_t offset, std::int64_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateBufferSubData(buffer, offset, length); }
void glInvalidateFramebuffer(GLenum target, std::int32_t numAttachments, const GLenum * attachments) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateFramebuffer(target, numAttachments, attachments); }
void glInvalidateNamedFramebufferData(std::uint32_t framebuffer, std::int32_t numAttachments, const GLenum * attachments) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateNamedFramebufferData(framebuffer, numAttachments, attachments); }
void glInvalidateNamedFramebufferSubData(std::uint32_t framebuffer, std::int32_t numAttachments, const GLenum * attachments, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateNamedFramebufferSubData(framebuffer, numAttachments, attachments, x, y, width, height); }
void glInvalidateSubFramebuffer(GLenum target, std::int32_t numAttachments, const GLenum * attachments, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height); }
void glInvalidateTexImage(std::uint32_t texture, std::int32_t level) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateTexImage(texture, level); }
void glInvalidateTexSubImage(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().invalidateTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth); }
bool glIsBuffer(std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isBuffer(buffer); }
bool glIsBufferResidentNV(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isBufferResidentNV(target); }
bool glIsCommandListNV(std::uint32_t list) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isCommandListNV(list); }
bool glIsEnabled(GLenum cap) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isEnabled(cap); }
bool glIsEnabledi(GLenum target, std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isEnabledi(target, index); }
bool glIsFramebuffer(std::uint32_t framebuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isFramebuffer(framebuffer); }
bool glIsImageHandleResidentARB(std::uint64_t handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isImageHandleResidentARB(handle); }
bool glIsMemoryObjectEXT(std::uint32_t memoryObject) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isMemoryObjectEXT(memoryObject); }
bool glIsNamedBufferResidentNV(std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isNamedBufferResidentNV(buffer); }
bool glIsPathNV(std::uint32_t path) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isPathNV(path); }
bool glIsPointInFillPathNV(std::uint32_t path, std::uint32_t mask, float x, float y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isPointInFillPathNV(path, mask, x, y); }
bool glIsPointInStrokePathNV(std::uint32_t path, float x, float y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isPointInStrokePathNV(path, x, y); }
bool glIsProgram(std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isProgram(program); }
bool glIsProgramPipeline(std::uint32_t pipeline) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isProgramPipeline(pipeline); }
bool glIsQuery(std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isQuery(id); }
bool glIsRenderbuffer(std::uint32_t renderbuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isRenderbuffer(renderbuffer); }
bool glIsSemaphoreEXT(std::uint32_t semaphore) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isSemaphoreEXT(semaphore); }
bool glIsSampler(std::uint32_t sampler) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isSampler(sampler); }
bool glIsShader(std::uint32_t shader) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isShader(shader); }
bool glIsStateNV(std::uint32_t state) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isStateNV(state); }
bool glIsSync(struct __GLsync * sync) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isSync(sync); }
bool glIsTexture(std::uint32_t texture) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isTexture(texture); }
bool glIsTextureHandleResidentARB(std::uint64_t handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isTextureHandleResidentARB(handle); }
bool glIsTransformFeedback(std::uint32_t id) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isTransformFeedback(id); }
bool glIsVertexArray(std::uint32_t array) MYGL_NOEXCEPT { return mygl::get_static_dispatch().isVertexArray(array); }
void glLineWidth(float width) MYGL_NOEXCEPT { return mygl::get_static_dispatch().lineWidth(width); }
void glLinkProgram(std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().linkProgram(program); }
void glListDrawCommandsStatesClientNV(std::uint32_t list, std::uint32_t segment, const void ** indirects, const std::int32_t * sizes, const std::uint32_t * states, const std::uint32_t * fbos, std::uint32_t count) MYGL_NOEXCEPT { return mygl::get_static_dispatch().listDrawCommandsStatesClientNV(list, segment, indirects, sizes, states, fbos, count); }
void glLogicOp(GLenum opcode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().logicOp(opcode); }
void glMakeBufferNonResidentNV(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeBufferNonResidentNV(target); }
void glMakeBufferResidentNV(GLenum target, GLenum access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeBufferResidentNV(target, access); }
void glMakeImageHandleNonResidentARB(std::uint64_t handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeImageHandleNonResidentARB(handle); }
void glMakeImageHandleResidentARB(std::uint64_t handle, GLenum access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeImageHandleResidentARB(handle, access); }
void glMakeNamedBufferNonResidentNV(std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeNamedBufferNonResidentNV(buffer); }
void glMakeNamedBufferResidentNV(std::uint32_t buffer, GLenum access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeNamedBufferResidentNV(buffer, access); }
void glMakeTextureHandleNonResidentARB(std::uint64_t handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeTextureHandleNonResidentARB(handle); }
void glMakeTextureHandleResidentARB(std::uint64_t handle) MYGL_NOEXCEPT { return mygl::get_static_dispatch().makeTextureHandleResidentARB(handle); }
void *glMapBuffer(GLenum target, GLenum access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().mapBuffer(target, access); }
void *glMapBufferRange(GLenum target, std::intptr_t offset, std::int64_t length, GLbitfield access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().mapBufferRange(target, offset, length, access); }
void *glMapNamedBuffer(std::uint32_t buffer, GLenum access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().mapNamedBuffer(buffer, access); }
void *glMapNamedBufferRange(std::uint32_t buffer, std::intptr_t offset, std::int64_t length, GLbitfield access) MYGL_NOEXCEPT { return mygl::get_static_dispatch().mapNamedBufferRange(buffer, offset, length, access); }
void glMatrixFrustumEXT(GLenum mode, double left, double right, double bottom, double top, double zNear, double zFar) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixFrustumEXT(mode, left, right, bottom, top, zNear, zFar); }
void glMatrixLoad3x2fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoad3x2fNV(matrixMode, m); }
void glMatrixLoad3x3fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoad3x3fNV(matrixMode, m); }
void glMatrixLoadIdentityEXT(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoadIdentityEXT(mode); }
void glMatrixLoadTranspose3x3fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoadTranspose3x3fNV(matrixMode, m); }
void glMatrixLoadTransposedEXT(GLenum mode, const double * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoadTransposedEXT(mode, m); }
void glMatrixLoadTransposefEXT(GLenum mode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoadTransposefEXT(mode, m); }
void glMatrixLoaddEXT(GLenum mode, const double * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoaddEXT(mode, m); }
void glMatrixLoadfEXT(GLenum mode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixLoadfEXT(mode, m); }
void glMatrixMult3x2fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMult3x2fNV(matrixMode, m); }
void glMatrixMult3x3fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMult3x3fNV(matrixMode, m); }
void glMatrixMultTranspose3x3fNV(GLenum matrixMode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMultTranspose3x3fNV(matrixMode, m); }
void glMatrixMultTransposedEXT(GLenum mode, const double * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMultTransposedEXT(mode, m); }
void glMatrixMultTransposefEXT(GLenum mode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMultTransposefEXT(mode, m); }
void glMatrixMultdEXT(GLenum mode, const double * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMultdEXT(mode, m); }
void glMatrixMultfEXT(GLenum mode, const float * m) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixMultfEXT(mode, m); }
void glMatrixOrthoEXT(GLenum mode, double left, double right, double bottom, double top, double zNear, double zFar) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixOrthoEXT(mode, left, right, bottom, top, zNear, zFar); }
void glMatrixPopEXT(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixPopEXT(mode); }
void glMatrixPushEXT(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixPushEXT(mode); }
void glMatrixRotatedEXT(GLenum mode, double angle, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixRotatedEXT(mode, angle, x, y, z); }
void glMatrixRotatefEXT(GLenum mode, float angle, float x, float y, float z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixRotatefEXT(mode, angle, x, y, z); }
void glMatrixScaledEXT(GLenum mode, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixScaledEXT(mode, x, y, z); }
void glMatrixScalefEXT(GLenum mode, float x, float y, float z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixScalefEXT(mode, x, y, z); }
void glMatrixTranslatedEXT(GLenum mode, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixTranslatedEXT(mode, x, y, z); }
void glMatrixTranslatefEXT(GLenum mode, float x, float y, float z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().matrixTranslatefEXT(mode, x, y, z); }
void glMemoryBarrier(GLbitfield barriers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().memoryBarrier(barriers); }
void glMemoryBarrierByRegion(GLbitfield barriers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().memoryBarrierByRegion(barriers); }
void glMemoryObjectParameterivEXT(std::uint32_t memoryObject, GLenum pname, const std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().memoryObjectParameterivEXT(memoryObject, pname, params); }
void glMinSampleShading(float value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().minSampleShading(value); }
void glMultiDrawArrays(GLenum mode, const std::int32_t * first, const std::int32_t * count, std::int32_t drawcount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawArrays(mode, first, count, drawcount); }
void glMultiDrawArraysIndirect(GLenum mode, const void * indirect, std::int32_t drawcount, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawArraysIndirect(mode, indirect, drawcount, stride); }
void glMultiDrawArraysIndirectBindlessCountNV(GLenum mode, const void * indirect, std::int32_t drawCount, std::int32_t maxDrawCount, std::int32_t stride, std::int32_t vertexBufferCount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawArraysIndirectBindlessCountNV(mode, indirect, drawCount, maxDrawCount, stride, vertexBufferCount); }
void glMultiDrawArraysIndirectBindlessNV(GLenum mode, const void * indirect, std::int32_t drawCount, std::int32_t stride, std::int32_t vertexBufferCount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawArraysIndirectBindlessNV(mode, indirect, drawCount, stride, vertexBufferCount); }
void glMultiDrawArraysIndirectCount(GLenum mode, const void * indirect, std::intptr_t drawcount, std::int32_t maxdrawcount, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawArraysIndirectCount(mode, indirect, drawcount, maxdrawcount, stride); }
void glMultiDrawElements(GLenum mode, const std::int32_t * count, GLenum type, const void *const* indices, std::int32_t drawcount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElements(mode, count, type, indices, drawcount); }
void glMultiDrawElementsBaseVertex(GLenum mode, const std::int32_t * count, GLenum type, const void *const* indices, std::int32_t drawcount, const std::int32_t * basevertex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex); }
void glMultiDrawElementsIndirect(GLenum mode, GLenum type, const void * indirect, std::int32_t drawcount, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElementsIndirect(mode, type, indirect, drawcount, stride); }
void glMultiDrawElementsIndirectBindlessCountNV(GLenum mode, GLenum type, const void * indirect, std::int32_t drawCount, std::int32_t maxDrawCount, std::int32_t stride, std::int32_t vertexBufferCount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElementsIndirectBindlessCountNV(mode, type, indirect, drawCount, maxDrawCount, stride, vertexBufferCount); }
void glMultiDrawElementsIndirectBindlessNV(GLenum mode, GLenum type, const void * indirect, std::int32_t drawCount, std::int32_t stride, std::int32_t vertexBufferCount) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElementsIndirectBindlessNV(mode, type, indirect, drawCount, stride, vertexBufferCount); }
void glMultiDrawElementsIndirectCount(GLenum mode, GLenum type, const void * indirect, std::intptr_t drawcount, std::int32_t maxdrawcount, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiDrawElementsIndirectCount(mode, type, indirect, drawcount, maxdrawcount, stride); }
void glMultiTexCoordP1ui(GLenum texture, GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP1ui(texture, type, coords); }
void glMultiTexCoordP1uiv(GLenum texture, GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP1uiv(texture, type, coords); }
void glMultiTexCoordP2ui(GLenum texture, GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP2ui(texture, type, coords); }
void glMultiTexCoordP2uiv(GLenum texture, GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP2uiv(texture, type, coords); }
void glMultiTexCoordP3ui(GLenum texture, GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP3ui(texture, type, coords); }
void glMultiTexCoordP3uiv(GLenum texture, GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP3uiv(texture, type, coords); }
void glMultiTexCoordP4ui(GLenum texture, GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP4ui(texture, type, coords); }
void glMultiTexCoordP4uiv(GLenum texture, GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().multiTexCoordP4uiv(texture, type, coords); }
void glNamedBufferData(std::uint32_t buffer, std::int64_t size, const void * data, GLenum usage) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedBufferData(buffer, size, data, usage); }
void glNamedBufferStorage(std::uint32_t buffer, std::int64_t size, const void * data, GLbitfield flags) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedBufferStorage(buffer, size, data, flags); }
void glNamedBufferStorageMemEXT(std::uint32_t buffer, std::int64_t size, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedBufferStorageMemEXT(buffer, size, memory, offset); }
void glNamedBufferSubData(std::uint32_t buffer, std::intptr_t offset, std::int64_t size, const void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedBufferSubData(buffer, offset, size, data); }
void glNamedFramebufferDrawBuffer(std::uint32_t framebuffer, GLenum buf) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferDrawBuffer(framebuffer, buf); }
void glNamedFramebufferDrawBuffers(std::uint32_t framebuffer, std::int32_t n, const GLenum * bufs) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferDrawBuffers(framebuffer, n, bufs); }
void glNamedFramebufferParameteri(std::uint32_t framebuffer, GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferParameteri(framebuffer, pname, param); }
void glNamedFramebufferReadBuffer(std::uint32_t framebuffer, GLenum src) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferReadBuffer(framebuffer, src); }
void glNamedFramebufferRenderbuffer(std::uint32_t framebuffer, GLenum attachment, GLenum renderbuffertarget, std::uint32_t renderbuffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer); }
void glNamedFramebufferTexture(std::uint32_t framebuffer, GLenum attachment, std::uint32_t texture, std::int32_t level) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferTexture(framebuffer, attachment, texture, level); }
void glNamedFramebufferTextureLayer(std::uint32_t framebuffer, GLenum attachment, std::uint32_t texture, std::int32_t level, std::int32_t layer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer); }
void glNamedRenderbufferStorage(std::uint32_t renderbuffer, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedRenderbufferStorage(renderbuffer, internalformat, width, height); }
void glNamedRenderbufferStorageMultisample(std::uint32_t renderbuffer, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().namedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height); }
void glNormalFormatNV(GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().normalFormatNV(type, stride); }
void glNormalP3ui(GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().normalP3ui(type, coords); }
void glNormalP3uiv(GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().normalP3uiv(type, coords); }
void glObjectLabel(GLenum identifier, std::uint32_t name, std::int32_t length, const char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().objectLabel(identifier, name, length, label); }
void glObjectLabelKHR(GLenum identifier, std::uint32_t name, std::int32_t length, const char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().objectLabelKHR(identifier, name, length, label); }
void glObjectPtrLabel(const void * ptr, std::int32_t length, const char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().objectPtrLabel(ptr, length, label); }
void glObjectPtrLabelKHR(const void * ptr, std::int32_t length, const char * label) MYGL_NOEXCEPT { return mygl::get_static_dispatch().objectPtrLabelKHR(ptr, length, label); }
void glPatchParameterfv(GLenum pname, const float * values) MYGL_NOEXCEPT { return mygl::get_static_dispatch().patchParameterfv(pname, values); }
void glPatchParameteri(GLenum pname, std::int32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().patchParameteri(pname, value); }
void glPathColorGenNV(GLenum color, GLenum genMode, GLenum colorFormat, const float * coeffs) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathColorGenNV(color, genMode, colorFormat, coeffs); }
void glPathCommandsNV(std::uint32_t path, std::int32_t numCommands, const std::uint8_t * commands, std::int32_t numCoords, GLenum coordType, const void * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathCommandsNV(path, numCommands, commands, numCoords, coordType, coords); }
void glPathCoordsNV(std::uint32_t path, std::int32_t numCoords, GLenum coordType, const void * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathCoordsNV(path, numCoords, coordType, coords); }
void glPathCoverDepthFuncNV(GLenum func) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathCoverDepthFuncNV(func); }
void glPathDashArrayNV(std::uint32_t path, std::int32_t dashCount, const float * dashArray) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathDashArrayNV(path, dashCount, dashArray); }
void glPathFogGenNV(GLenum genMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathFogGenNV(genMode); }
GLenum glPathGlyphIndexArrayNV(std::uint32_t firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, std::uint32_t firstGlyphIndex, std::int32_t numGlyphs, std::uint32_t pathParameterTemplate, float emScale) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathGlyphIndexArrayNV(firstPathName, fontTarget, fontName, fontStyle, firstGlyphIndex, numGlyphs, pathParameterTemplate, emScale); }
GLenum glPathGlyphIndexRangeNV(GLenum fontTarget, const void * fontName, GLbitfield fontStyle, std::uint32_t pathParameterTemplate, float emScale, std::uint32_t baseAndCount[2]) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathGlyphIndexRangeNV(fontTarget, fontName, fontStyle, pathParameterTemplate, emScale, baseAndCount); }
void glPathGlyphRangeNV(std::uint32_t firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, std::uint32_t firstGlyph, std::int32_t numGlyphs, GLenum handleMissingGlyphs, std::uint32_t pathParameterTemplate, float emScale) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathGlyphRangeNV(firstPathName, fontTarget, fontName, fontStyle, firstGlyph, numGlyphs, handleMissingGlyphs, pathParameterTemplate, emScale); }
void glPathGlyphsNV(std::uint32_t firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, std::int32_t numGlyphs, GLenum type, const void * charcodes, GLenum handleMissingGlyphs, std::uint32_t pathParameterTemplate, float emScale) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathGlyphsNV(firstPathName, fontTarget, fontName, fontStyle, numGlyphs, type, charcodes, handleMissingGlyphs, pathParameterTemplate, emScale); }
GLenum glPathMemoryGlyphIndexArrayNV(std::uint32_t firstPathName, GLenum fontTarget, std::int64_t fontSize, const void * fontData, std::int32_t faceIndex, std::uint32_t firstGlyphIndex, std::int32_t numGlyphs, std::uint32_t pathParameterTemplate, float emScale) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathMemoryGlyphIndexArrayNV(firstPathName, fontTarget, fontSize, fontData, faceIndex, firstGlyphIndex, numGlyphs, pathParameterTemplate, emScale); }
void glPathParameterfNV(std::uint32_t path, GLenum pname, float value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathParameterfNV(path, pname, value); }
void glPathParameterfvNV(std::uint32_t path, GLenum pname, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathParameterfvNV(path, pname, value); }
void glPathParameteriNV(std::uint32_t path, GLenum pname, std::int32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathParameteriNV(path, pname, value); }
void glPathParameterivNV(std::uint32_t path, GLenum pname, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathParameterivNV(path, pname, value); }
void glPathStencilDepthOffsetNV(float factor, float units) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathStencilDepthOffsetNV(factor, units); }
void glPathStencilFuncNV(GLenum func, std::int32_t ref, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathStencilFuncNV(func, ref, mask); }
void glPathStringNV(std::uint32_t path, GLenum format, std::int32_t length, const void * pathString) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathStringNV(path, format, length, pathString); }
void glPathSubCommandsNV(std::uint32_t path, std::int32_t commandStart, std::int32_t commandsToDelete, std::int32_t numCommands, const std::uint8_t * commands, std::int32_t numCoords, GLenum coordType, const void * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathSubCommandsNV(path, commandStart, commandsToDelete, numCommands, commands, numCoords, coordType, coords); }
void glPathSubCoordsNV(std::uint32_t path, std::int32_t coordStart, std::int32_t numCoords, GLenum coordType, const void * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathSubCoordsNV(path, coordStart, numCoords, coordType, coords); }
void glPathTexGenNV(GLenum texCoordSet, GLenum genMode, std::int32_t components, const float * coeffs) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pathTexGenNV(texCoordSet, genMode, components, coeffs); }
void glPauseTransformFeedback() MYGL_NOEXCEPT { return mygl::get_static_dispatch().pauseTransformFeedback(); }
void glPixelStoref(GLenum pname, float param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pixelStoref(pname, param); }
void glPixelStorei(GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pixelStorei(pname, param); }
bool glPointAlongPathNV(std::uint32_t path, std::int32_t startSegment, std::int32_t numSegments, float distance, float * x, float * y, float * tangentX, float * tangentY) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointAlongPathNV(path, startSegment, numSegments, distance, x, y, tangentX, tangentY); }
void glPointParameterf(GLenum pname, float param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointParameterf(pname, param); }
void glPointParameterfv(GLenum pname, const float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointParameterfv(pname, params); }
void glPointParameteri(GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointParameteri(pname, param); }
void glPointParameteriv(GLenum pname, const std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointParameteriv(pname, params); }
void glPointSize(float size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pointSize(size); }
void glPolygonMode(GLenum face, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().polygonMode(face, mode); }
void glPolygonOffset(float factor, float units) MYGL_NOEXCEPT { return mygl::get_static_dispatch().polygonOffset(factor, units); }
void glPolygonOffsetClamp(float factor, float units, float clamp) MYGL_NOEXCEPT { return mygl::get_static_dispatch().polygonOffsetClamp(factor, units, clamp); }
void glPolygonOffsetClampEXT(float factor, float units, float clamp) MYGL_NOEXCEPT { return mygl::get_static_dispatch().polygonOffsetClampEXT(factor, units, clamp); }
void glPopDebugGroup() MYGL_NOEXCEPT { return mygl::get_static_dispatch().popDebugGroup(); }
void glPopDebugGroupKHR() MYGL_NOEXCEPT { return mygl::get_static_dispatch().popDebugGroupKHR(); }
void glPrimitiveRestartIndex(std::uint32_t index) MYGL_NOEXCEPT { return mygl::get_static_dispatch().primitiveRestartIndex(index); }
void glProgramBinary(std::uint32_t program, GLenum binaryFormat, const void * binary, std::int32_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programBinary(program, binaryFormat, binary, length); }
void glProgramParameteri(std::uint32_t program, GLenum pname, std::int32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programParameteri(program, pname, value); }
void glProgramPathFragmentInputGenNV(std::uint32_t program, std::int32_t location, GLenum genMode, std::int32_t components, const float * coeffs) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programPathFragmentInputGenNV(program, location, genMode, components, coeffs); }
void glProgramSubroutineParametersuivNV(GLenum target, std::int32_t count, const std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programSubroutineParametersuivNV(target, count, params); }
void glProgramUniform1d(std::uint32_t program, std::int32_t location, double v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1d(program, location, v0); }
void glProgramUniform1dv(std::uint32_t program, std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1dv(program, location, count, value); }
void glProgramUniform1f(std::uint32_t program, std::int32_t location, float v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1f(program, location, v0); }
void glProgramUniform1fv(std::uint32_t program, std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1fv(program, location, count, value); }
void glProgramUniform1i(std::uint32_t program, std::int32_t location, std::int32_t v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1i(program, location, v0); }
void glProgramUniform1i64ARB(std::uint32_t program, std::int32_t location, std::int64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1i64ARB(program, location, x); }
void glProgramUniform1i64NV(std::uint32_t program, std::int32_t location, std::int64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1i64NV(program, location, x); }
void glProgramUniform1i64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1i64vARB(program, location, count, value); }
void glProgramUniform1i64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1i64vNV(program, location, count, value); }
void glProgramUniform1iv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1iv(program, location, count, value); }
void glProgramUniform1ui(std::uint32_t program, std::int32_t location, std::uint32_t v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1ui(program, location, v0); }
void glProgramUniform1ui64ARB(std::uint32_t program, std::int32_t location, std::uint64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1ui64ARB(program, location, x); }
void glProgramUniform1ui64NV(std::uint32_t program, std::int32_t location, std::uint64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1ui64NV(program, location, x); }
void glProgramUniform1ui64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1ui64vARB(program, location, count, value); }
void glProgramUniform1ui64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1ui64vNV(program, location, count, value); }
void glProgramUniform1uiv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform1uiv(program, location, count, value); }
void glProgramUniform2d(std::uint32_t program, std::int32_t location, double v0, double v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2d(program, location, v0, v1); }
void glProgramUniform2dv(std::uint32_t program, std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2dv(program, location, count, value); }
void glProgramUniform2f(std::uint32_t program, std::int32_t location, float v0, float v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2f(program, location, v0, v1); }
void glProgramUniform2fv(std::uint32_t program, std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2fv(program, location, count, value); }
void glProgramUniform2i(std::uint32_t program, std::int32_t location, std::int32_t v0, std::int32_t v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2i(program, location, v0, v1); }
void glProgramUniform2i64ARB(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2i64ARB(program, location, x, y); }
void glProgramUniform2i64NV(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2i64NV(program, location, x, y); }
void glProgramUniform2i64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2i64vARB(program, location, count, value); }
void glProgramUniform2i64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2i64vNV(program, location, count, value); }
void glProgramUniform2iv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2iv(program, location, count, value); }
void glProgramUniform2ui(std::uint32_t program, std::int32_t location, std::uint32_t v0, std::uint32_t v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2ui(program, location, v0, v1); }
void glProgramUniform2ui64ARB(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2ui64ARB(program, location, x, y); }
void glProgramUniform2ui64NV(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2ui64NV(program, location, x, y); }
void glProgramUniform2ui64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2ui64vARB(program, location, count, value); }
void glProgramUniform2ui64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2ui64vNV(program, location, count, value); }
void glProgramUniform2uiv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform2uiv(program, location, count, value); }
void glProgramUniform3d(std::uint32_t program, std::int32_t location, double v0, double v1, double v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3d(program, location, v0, v1, v2); }
void glProgramUniform3dv(std::uint32_t program, std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3dv(program, location, count, value); }
void glProgramUniform3f(std::uint32_t program, std::int32_t location, float v0, float v1, float v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3f(program, location, v0, v1, v2); }
void glProgramUniform3fv(std::uint32_t program, std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3fv(program, location, count, value); }
void glProgramUniform3i(std::uint32_t program, std::int32_t location, std::int32_t v0, std::int32_t v1, std::int32_t v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3i(program, location, v0, v1, v2); }
void glProgramUniform3i64ARB(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3i64ARB(program, location, x, y, z); }
void glProgramUniform3i64NV(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3i64NV(program, location, x, y, z); }
void glProgramUniform3i64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3i64vARB(program, location, count, value); }
void glProgramUniform3i64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3i64vNV(program, location, count, value); }
void glProgramUniform3iv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3iv(program, location, count, value); }
void glProgramUniform3ui(std::uint32_t program, std::int32_t location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3ui(program, location, v0, v1, v2); }
void glProgramUniform3ui64ARB(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3ui64ARB(program, location, x, y, z); }
void glProgramUniform3ui64NV(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3ui64NV(program, location, x, y, z); }
void glProgramUniform3ui64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3ui64vARB(program, location, count, value); }
void glProgramUniform3ui64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3ui64vNV(program, location, count, value); }
void glProgramUniform3uiv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform3uiv(program, location, count, value); }
void glProgramUniform4d(std::uint32_t program, std::int32_t location, double v0, double v1, double v2, double v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4d(program, location, v0, v1, v2, v3); }
void glProgramUniform4dv(std::uint32_t program, std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4dv(program, location, count, value); }
void glProgramUniform4f(std::uint32_t program, std::int32_t location, float v0, float v1, float v2, float v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4f(program, location, v0, v1, v2, v3); }
void glProgramUniform4fv(std::uint32_t program, std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4fv(program, location, count, value); }
void glProgramUniform4i(std::uint32_t program, std::int32_t location, std::int32_t v0, std::int32_t v1, std::int32_t v2, std::int32_t v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4i(program, location, v0, v1, v2, v3); }
void glProgramUniform4i64ARB(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z, std::int64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4i64ARB(program, location, x, y, z, w); }
void glProgramUniform4i64NV(std::uint32_t program, std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z, std::int64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4i64NV(program, location, x, y, z, w); }
void glProgramUniform4i64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4i64vARB(program, location, count, value); }
void glProgramUniform4i64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4i64vNV(program, location, count, value); }
void glProgramUniform4iv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4iv(program, location, count, value); }
void glProgramUniform4ui(std::uint32_t program, std::int32_t location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::uint32_t v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4ui(program, location, v0, v1, v2, v3); }
void glProgramUniform4ui64ARB(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z, std::uint64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4ui64ARB(program, location, x, y, z, w); }
void glProgramUniform4ui64NV(std::uint32_t program, std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z, std::uint64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4ui64NV(program, location, x, y, z, w); }
void glProgramUniform4ui64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4ui64vARB(program, location, count, value); }
void glProgramUniform4ui64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4ui64vNV(program, location, count, value); }
void glProgramUniform4uiv(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniform4uiv(program, location, count, value); }
void glProgramUniformHandleui64ARB(std::uint32_t program, std::int32_t location, std::uint64_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformHandleui64ARB(program, location, value); }
void glProgramUniformHandleui64vARB(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * values) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformHandleui64vARB(program, location, count, values); }
void glProgramUniformMatrix2dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2dv(program, location, count, transpose, value); }
void glProgramUniformMatrix2fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2fv(program, location, count, transpose, value); }
void glProgramUniformMatrix2x3dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2x3dv(program, location, count, transpose, value); }
void glProgramUniformMatrix2x3fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2x3fv(program, location, count, transpose, value); }
void glProgramUniformMatrix2x4dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2x4dv(program, location, count, transpose, value); }
void glProgramUniformMatrix2x4fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix2x4fv(program, location, count, transpose, value); }
void glProgramUniformMatrix3dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3dv(program, location, count, transpose, value); }
void glProgramUniformMatrix3fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3fv(program, location, count, transpose, value); }
void glProgramUniformMatrix3x2dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3x2dv(program, location, count, transpose, value); }
void glProgramUniformMatrix3x2fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3x2fv(program, location, count, transpose, value); }
void glProgramUniformMatrix3x4dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3x4dv(program, location, count, transpose, value); }
void glProgramUniformMatrix3x4fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix3x4fv(program, location, count, transpose, value); }
void glProgramUniformMatrix4dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4dv(program, location, count, transpose, value); }
void glProgramUniformMatrix4fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4fv(program, location, count, transpose, value); }
void glProgramUniformMatrix4x2dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4x2dv(program, location, count, transpose, value); }
void glProgramUniformMatrix4x2fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4x2fv(program, location, count, transpose, value); }
void glProgramUniformMatrix4x3dv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4x3dv(program, location, count, transpose, value); }
void glProgramUniformMatrix4x3fv(std::uint32_t program, std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformMatrix4x3fv(program, location, count, transpose, value); }
void glProgramUniformui64NV(std::uint32_t program, std::int32_t location, std::uint64_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformui64NV(program, location, value); }
void glProgramUniformui64vNV(std::uint32_t program, std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().programUniformui64vNV(program, location, count, value); }
void glProvokingVertex(GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().provokingVertex(mode); }
void glPushDebugGroup(GLenum source, std::uint32_t id, std::int32_t length, const char * message) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pushDebugGroup(source, id, length, message); }
void glPushDebugGroupKHR(GLenum source, std::uint32_t id, std::int32_t length, const char * message) MYGL_NOEXCEPT { return mygl::get_static_dispatch().pushDebugGroupKHR(source, id, length, message); }
void glQueryCounter(std::uint32_t id, GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().queryCounter(id, target); }
void glReadBuffer(GLenum src) MYGL_NOEXCEPT { return mygl::get_static_dispatch().readBuffer(src); }
void glReadPixels(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, GLenum format, GLenum type, void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().readPixels(x, y, width, height, format, type, pixels); }
void glReadnPixels(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, GLenum format, GLenum type, std::int32_t bufSize, void * data) MYGL_NOEXCEPT { return mygl::get_static_dispatch().readnPixels(x, y, width, height, format, type, bufSize, data); }
void glReleaseShaderCompiler() MYGL_NOEXCEPT { return mygl::get_static_dispatch().releaseShaderCompiler(); }
void glRenderbufferStorage(GLenum target, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().renderbufferStorage(target, internalformat, width, height); }
void glRenderbufferStorageMultisample(GLenum target, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().renderbufferStorageMultisample(target, samples, internalformat, width, height); }
void glResumeTransformFeedback() MYGL_NOEXCEPT { return mygl::get_static_dispatch().resumeTransformFeedback(); }
void glSampleCoverage(float value, bool invert) MYGL_NOEXCEPT { return mygl::get_static_dispatch().sampleCoverage(value, invert); }
void glSampleMaski(std::uint32_t maskNumber, GLbitfield mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().sampleMaski(maskNumber, mask); }
void glSamplerParameterIiv(std::uint32_t sampler, GLenum pname, const std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameterIiv(sampler, pname, param); }
void glSamplerParameterIuiv(std::uint32_t sampler, GLenum pname, const std::uint32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameterIuiv(sampler, pname, param); }
void glSamplerParameterf(std::uint32_t sampler, GLenum pname, float param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameterf(sampler, pname, param); }
void glSamplerParameterfv(std::uint32_t sampler, GLenum pname, const float * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameterfv(sampler, pname, param); }
void glSamplerParameteri(std::uint32_t sampler, GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameteri(sampler, pname, param); }
void glSamplerParameteriv(std::uint32_t sampler, GLenum pname, const std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().samplerParameteriv(sampler, pname, param); }
void glScissor(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().scissor(x, y, width, height); }
void glScissorArrayv(std::uint32_t first, std::int32_t count, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().scissorArrayv(first, count, v); }
void glScissorIndexed(std::uint32_t index, std::int32_t left, std::int32_t bottom, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().scissorIndexed(index, left, bottom, width, height); }
void glScissorIndexedv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().scissorIndexedv(index, v); }
void glSecondaryColorFormatNV(std::int32_t size, GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().secondaryColorFormatNV(size, type, stride); }
void glSecondaryColorP3ui(GLenum type, std::uint32_t color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().secondaryColorP3ui(type, color); }
void glSecondaryColorP3uiv(GLenum type, const std::uint32_t * color) MYGL_NOEXCEPT { return mygl::get_static_dispatch().secondaryColorP3uiv(type, color); }
void glSemaphoreParameterui64vEXT(std::uint32_t semaphore, GLenum pname, const std::uint64_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().semaphoreParameterui64vEXT(semaphore, pname, params); }
void glShaderBinary(std::int32_t count, const std::uint32_t * shaders, GLenum binaryformat, const void * binary, std::int32_t length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().shaderBinary(count, shaders, binaryformat, binary, length); }
void glShaderSource(std::uint32_t shader, std::int32_t count, const char *const* string, const std::int32_t * length) MYGL_NOEXCEPT { return mygl::get_static_dispatch().shaderSource(shader, count, string, length); }
void glShaderStorageBlockBinding(std::uint32_t program, std::uint32_t storageBlockIndex, std::uint32_t storageBlockBinding) MYGL_NOEXCEPT { return mygl::get_static_dispatch().shaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding); }
void glSignalSemaphoreEXT(std::uint32_t semaphore, std::uint32_t numBufferBarriers, const std::uint32_t * buffers, std::uint32_t numTextureBarriers, const std::uint32_t * textures, const GLenum * dstLayouts) MYGL_NOEXCEPT { return mygl::get_static_dispatch().signalSemaphoreEXT(semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts); }
void glSpecializeShader(std::uint32_t shader, const char * pEntryPoint, std::uint32_t numSpecializationConstants, const std::uint32_t * pConstantIndex, const std::uint32_t * pConstantValue) MYGL_NOEXCEPT { return mygl::get_static_dispatch().specializeShader(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue); }
void glStateCaptureNV(std::uint32_t state, GLenum mode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stateCaptureNV(state, mode); }
void glStencilFillPathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, GLenum fillMode, std::uint32_t mask, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilFillPathInstancedNV(numPaths, pathNameType, paths, pathBase, fillMode, mask, transformType, transformValues); }
void glStencilFillPathNV(std::uint32_t path, GLenum fillMode, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilFillPathNV(path, fillMode, mask); }
void glStencilFunc(GLenum func, std::int32_t ref, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilFunc(func, ref, mask); }
void glStencilFuncSeparate(GLenum face, GLenum func, std::int32_t ref, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilFuncSeparate(face, func, ref, mask); }
void glStencilMask(std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilMask(mask); }
void glStencilMaskSeparate(GLenum face, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilMaskSeparate(face, mask); }
void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilOp(fail, zfail, zpass); }
void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilOpSeparate(face, sfail, dpfail, dppass); }
void glStencilStrokePathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, std::int32_t reference, std::uint32_t mask, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilStrokePathInstancedNV(numPaths, pathNameType, paths, pathBase, reference, mask, transformType, transformValues); }
void glStencilStrokePathNV(std::uint32_t path, std::int32_t reference, std::uint32_t mask) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilStrokePathNV(path, reference, mask); }
void glStencilThenCoverFillPathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, GLenum fillMode, std::uint32_t mask, GLenum coverMode, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilThenCoverFillPathInstancedNV(numPaths, pathNameType, paths, pathBase, fillMode, mask, coverMode, transformType, transformValues); }
void glStencilThenCoverFillPathNV(std::uint32_t path, GLenum fillMode, std::uint32_t mask, GLenum coverMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilThenCoverFillPathNV(path, fillMode, mask, coverMode); }
void glStencilThenCoverStrokePathInstancedNV(std::int32_t numPaths, GLenum pathNameType, const void * paths, std::uint32_t pathBase, std::int32_t reference, std::uint32_t mask, GLenum coverMode, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilThenCoverStrokePathInstancedNV(numPaths, pathNameType, paths, pathBase, reference, mask, coverMode, transformType, transformValues); }
void glStencilThenCoverStrokePathNV(std::uint32_t path, std::int32_t reference, std::uint32_t mask, GLenum coverMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().stencilThenCoverStrokePathNV(path, reference, mask, coverMode); }
void glTexBuffer(GLenum target, GLenum internalformat, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texBuffer(target, internalformat, buffer); }
void glTexBufferRange(GLenum target, GLenum internalformat, std::uint32_t buffer, std::intptr_t offset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texBufferRange(target, internalformat, buffer, offset, size); }
void glTexCoordFormatNV(std::int32_t size, GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordFormatNV(size, type, stride); }
void glTexCoordP1ui(GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP1ui(type, coords); }
void glTexCoordP1uiv(GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP1uiv(type, coords); }
void glTexCoordP2ui(GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP2ui(type, coords); }
void glTexCoordP2uiv(GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP2uiv(type, coords); }
void glTexCoordP3ui(GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP3ui(type, coords); }
void glTexCoordP3uiv(GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP3uiv(type, coords); }
void glTexCoordP4ui(GLenum type, std::uint32_t coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP4ui(type, coords); }
void glTexCoordP4uiv(GLenum type, const std::uint32_t * coords) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texCoordP4uiv(type, coords); }
void glTexImage1D(GLenum target, std::int32_t level, std::int32_t internalformat, std::int32_t width, std::int32_t border, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texImage1D(target, level, internalformat, width, border, format, type, pixels); }
void glTexImage2D(GLenum target, std::int32_t level, std::int32_t internalformat, std::int32_t width, std::int32_t height, std::int32_t border, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texImage2D(target, level, internalformat, width, height, border, format, type, pixels); }
void glTexImage2DMultisample(GLenum target, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations); }
void glTexImage3D(GLenum target, std::int32_t level, std::int32_t internalformat, std::int32_t width, std::int32_t height, std::int32_t depth, std::int32_t border, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels); }
void glTexImage3DMultisample(GLenum target, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations); }
void glTexParameterIiv(GLenum target, GLenum pname, const std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameterIiv(target, pname, params); }
void glTexParameterIuiv(GLenum target, GLenum pname, const std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameterIuiv(target, pname, params); }
void glTexParameterf(GLenum target, GLenum pname, float param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameterf(target, pname, param); }
void glTexParameterfv(GLenum target, GLenum pname, const float * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameterfv(target, pname, params); }
void glTexParameteri(GLenum target, GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameteri(target, pname, param); }
void glTexParameteriv(GLenum target, GLenum pname, const std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texParameteriv(target, pname, params); }
void glTexStorage1D(GLenum target, std::int32_t levels, GLenum internalformat, std::int32_t width) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorage1D(target, levels, internalformat, width); }
void glTexStorage2D(GLenum target, std::int32_t levels, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorage2D(target, levels, internalformat, width, height); }
void glTexStorage2DMultisample(GLenum target, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations); }
void glTexStorage3D(GLenum target, std::int32_t levels, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorage3D(target, levels, internalformat, width, height, depth); }
void glTexStorage3DMultisample(GLenum target, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations); }
void glTexStorageMem1DEXT(GLenum target, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorageMem1DEXT(target, levels, internalFormat, width, memory, offset); }
void glTexStorageMem2DEXT(GLenum target, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::int32_t height, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorageMem2DEXT(target, levels, internalFormat, width, height, memory, offset); }
void glTexStorageMem2DMultisampleEXT(GLenum target, std::int32_t samples, GLenum internalFormat, std::int32_t width, std::int32_t height, bool fixedSampleLocations, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorageMem2DMultisampleEXT(target, samples, internalFormat, width, height, fixedSampleLocations, memory, offset); }
void glTexStorageMem3DEXT(GLenum target, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::int32_t height, std::int32_t depth, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorageMem3DEXT(target, levels, internalFormat, width, height, depth, memory, offset); }
void glTexStorageMem3DMultisampleEXT(GLenum target, std::int32_t samples, GLenum internalFormat, std::int32_t width, std::int32_t height, std::int32_t depth, bool fixedSampleLocations, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texStorageMem3DMultisampleEXT(target, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset); }
void glTexSubImage1D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t width, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texSubImage1D(target, level, xoffset, width, format, type, pixels); }
void glTexSubImage2D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t width, std::int32_t height, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); }
void glTexSubImage3D(GLenum target, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().texSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); }
void glTextureBarrier() MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureBarrier(); }
void glTextureBuffer(std::uint32_t texture, GLenum internalformat, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureBuffer(texture, internalformat, buffer); }
void glTextureBufferRange(std::uint32_t texture, GLenum internalformat, std::uint32_t buffer, std::intptr_t offset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureBufferRange(texture, internalformat, buffer, offset, size); }
void glTextureParameterIiv(std::uint32_t texture, GLenum pname, const std::int32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameterIiv(texture, pname, params); }
void glTextureParameterIuiv(std::uint32_t texture, GLenum pname, const std::uint32_t * params) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameterIuiv(texture, pname, params); }
void glTextureParameterf(std::uint32_t texture, GLenum pname, float param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameterf(texture, pname, param); }
void glTextureParameterfv(std::uint32_t texture, GLenum pname, const float * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameterfv(texture, pname, param); }
void glTextureParameteri(std::uint32_t texture, GLenum pname, std::int32_t param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameteri(texture, pname, param); }
void glTextureParameteriv(std::uint32_t texture, GLenum pname, const std::int32_t * param) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureParameteriv(texture, pname, param); }
void glTextureStorage1D(std::uint32_t texture, std::int32_t levels, GLenum internalformat, std::int32_t width) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorage1D(texture, levels, internalformat, width); }
void glTextureStorage2D(std::uint32_t texture, std::int32_t levels, GLenum internalformat, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorage2D(texture, levels, internalformat, width, height); }
void glTextureStorage2DMultisample(std::uint32_t texture, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorage2DMultisample(texture, samples, internalformat, width, height, fixedsamplelocations); }
void glTextureStorage3D(std::uint32_t texture, std::int32_t levels, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorage3D(texture, levels, internalformat, width, height, depth); }
void glTextureStorage3DMultisample(std::uint32_t texture, std::int32_t samples, GLenum internalformat, std::int32_t width, std::int32_t height, std::int32_t depth, bool fixedsamplelocations) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorage3DMultisample(texture, samples, internalformat, width, height, depth, fixedsamplelocations); }
void glTextureStorageMem1DEXT(std::uint32_t texture, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorageMem1DEXT(texture, levels, internalFormat, width, memory, offset); }
void glTextureStorageMem2DEXT(std::uint32_t texture, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::int32_t height, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorageMem2DEXT(texture, levels, internalFormat, width, height, memory, offset); }
void glTextureStorageMem2DMultisampleEXT(std::uint32_t texture, std::int32_t samples, GLenum internalFormat, std::int32_t width, std::int32_t height, bool fixedSampleLocations, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorageMem2DMultisampleEXT(texture, samples, internalFormat, width, height, fixedSampleLocations, memory, offset); }
void glTextureStorageMem3DEXT(std::uint32_t texture, std::int32_t levels, GLenum internalFormat, std::int32_t width, std::int32_t height, std::int32_t depth, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorageMem3DEXT(texture, levels, internalFormat, width, height, depth, memory, offset); }
void glTextureStorageMem3DMultisampleEXT(std::uint32_t texture, std::int32_t samples, GLenum internalFormat, std::int32_t width, std::int32_t height, std::int32_t depth, bool fixedSampleLocations, std::uint32_t memory, std::uint64_t offset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureStorageMem3DMultisampleEXT(texture, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset); }
void glTextureSubImage1D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t width, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureSubImage1D(texture, level, xoffset, width, format, type, pixels); }
void glTextureSubImage2D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t width, std::int32_t height, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels); }
void glTextureSubImage3D(std::uint32_t texture, std::int32_t level, std::int32_t xoffset, std::int32_t yoffset, std::int32_t zoffset, std::int32_t width, std::int32_t height, std::int32_t depth, GLenum format, GLenum type, const void * pixels) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); }
void glTextureView(std::uint32_t texture, GLenum target, std::uint32_t origtexture, GLenum internalformat, std::uint32_t minlevel, std::uint32_t numlevels, std::uint32_t minlayer, std::uint32_t numlayers) MYGL_NOEXCEPT { return mygl::get_static_dispatch().textureView(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers); }
void glTransformFeedbackBufferBase(std::uint32_t xfb, std::uint32_t index, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().transformFeedbackBufferBase(xfb, index, buffer); }
void glTransformFeedbackBufferRange(std::uint32_t xfb, std::uint32_t index, std::uint32_t buffer, std::intptr_t offset, std::int64_t size) MYGL_NOEXCEPT { return mygl::get_static_dispatch().transformFeedbackBufferRange(xfb, index, buffer, offset, size); }
void glTransformFeedbackVaryings(std::uint32_t program, std::int32_t count, const char *const* varyings, GLenum bufferMode) MYGL_NOEXCEPT { return mygl::get_static_dispatch().transformFeedbackVaryings(program, count, varyings, bufferMode); }
void glTransformPathNV(std::uint32_t resultPath, std::uint32_t srcPath, GLenum transformType, const float * transformValues) MYGL_NOEXCEPT { return mygl::get_static_dispatch().transformPathNV(resultPath, srcPath, transformType, transformValues); }
void glUniform1d(std::int32_t location, double x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1d(location, x); }
void glUniform1dv(std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1dv(location, count, value); }
void glUniform1f(std::int32_t location, float v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1f(location, v0); }
void glUniform1fv(std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1fv(location, count, value); }
void glUniform1i(std::int32_t location, std::int32_t v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1i(location, v0); }
void glUniform1i64ARB(std::int32_t location, std::int64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1i64ARB(location, x); }
void glUniform1i64NV(std::int32_t location, std::int64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1i64NV(location, x); }
void glUniform1i64vARB(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1i64vARB(location, count, value); }
void glUniform1i64vNV(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1i64vNV(location, count, value); }
void glUniform1iv(std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1iv(location, count, value); }
void glUniform1ui(std::int32_t location, std::uint32_t v0) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1ui(location, v0); }
void glUniform1ui64ARB(std::int32_t location, std::uint64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1ui64ARB(location, x); }
void glUniform1ui64NV(std::int32_t location, std::uint64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1ui64NV(location, x); }
void glUniform1ui64vARB(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1ui64vARB(location, count, value); }
void glUniform1ui64vNV(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1ui64vNV(location, count, value); }
void glUniform1uiv(std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform1uiv(location, count, value); }
void glUniform2d(std::int32_t location, double x, double y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2d(location, x, y); }
void glUniform2dv(std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2dv(location, count, value); }
void glUniform2f(std::int32_t location, float v0, float v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2f(location, v0, v1); }
void glUniform2fv(std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2fv(location, count, value); }
void glUniform2i(std::int32_t location, std::int32_t v0, std::int32_t v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2i(location, v0, v1); }
void glUniform2i64ARB(std::int32_t location, std::int64_t x, std::int64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2i64ARB(location, x, y); }
void glUniform2i64NV(std::int32_t location, std::int64_t x, std::int64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2i64NV(location, x, y); }
void glUniform2i64vARB(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2i64vARB(location, count, value); }
void glUniform2i64vNV(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2i64vNV(location, count, value); }
void glUniform2iv(std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2iv(location, count, value); }
void glUniform2ui(std::int32_t location, std::uint32_t v0, std::uint32_t v1) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2ui(location, v0, v1); }
void glUniform2ui64ARB(std::int32_t location, std::uint64_t x, std::uint64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2ui64ARB(location, x, y); }
void glUniform2ui64NV(std::int32_t location, std::uint64_t x, std::uint64_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2ui64NV(location, x, y); }
void glUniform2ui64vARB(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2ui64vARB(location, count, value); }
void glUniform2ui64vNV(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2ui64vNV(location, count, value); }
void glUniform2uiv(std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform2uiv(location, count, value); }
void glUniform3d(std::int32_t location, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3d(location, x, y, z); }
void glUniform3dv(std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3dv(location, count, value); }
void glUniform3f(std::int32_t location, float v0, float v1, float v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3f(location, v0, v1, v2); }
void glUniform3fv(std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3fv(location, count, value); }
void glUniform3i(std::int32_t location, std::int32_t v0, std::int32_t v1, std::int32_t v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3i(location, v0, v1, v2); }
void glUniform3i64ARB(std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3i64ARB(location, x, y, z); }
void glUniform3i64NV(std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3i64NV(location, x, y, z); }
void glUniform3i64vARB(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3i64vARB(location, count, value); }
void glUniform3i64vNV(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3i64vNV(location, count, value); }
void glUniform3iv(std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3iv(location, count, value); }
void glUniform3ui(std::int32_t location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3ui(location, v0, v1, v2); }
void glUniform3ui64ARB(std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3ui64ARB(location, x, y, z); }
void glUniform3ui64NV(std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3ui64NV(location, x, y, z); }
void glUniform3ui64vARB(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3ui64vARB(location, count, value); }
void glUniform3ui64vNV(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3ui64vNV(location, count, value); }
void glUniform3uiv(std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform3uiv(location, count, value); }
void glUniform4d(std::int32_t location, double x, double y, double z, double w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4d(location, x, y, z, w); }
void glUniform4dv(std::int32_t location, std::int32_t count, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4dv(location, count, value); }
void glUniform4f(std::int32_t location, float v0, float v1, float v2, float v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4f(location, v0, v1, v2, v3); }
void glUniform4fv(std::int32_t location, std::int32_t count, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4fv(location, count, value); }
void glUniform4i(std::int32_t location, std::int32_t v0, std::int32_t v1, std::int32_t v2, std::int32_t v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4i(location, v0, v1, v2, v3); }
void glUniform4i64ARB(std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z, std::int64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4i64ARB(location, x, y, z, w); }
void glUniform4i64NV(std::int32_t location, std::int64_t x, std::int64_t y, std::int64_t z, std::int64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4i64NV(location, x, y, z, w); }
void glUniform4i64vARB(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4i64vARB(location, count, value); }
void glUniform4i64vNV(std::int32_t location, std::int32_t count, const std::int64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4i64vNV(location, count, value); }
void glUniform4iv(std::int32_t location, std::int32_t count, const std::int32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4iv(location, count, value); }
void glUniform4ui(std::int32_t location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::uint32_t v3) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4ui(location, v0, v1, v2, v3); }
void glUniform4ui64ARB(std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z, std::uint64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4ui64ARB(location, x, y, z, w); }
void glUniform4ui64NV(std::int32_t location, std::uint64_t x, std::uint64_t y, std::uint64_t z, std::uint64_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4ui64NV(location, x, y, z, w); }
void glUniform4ui64vARB(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4ui64vARB(location, count, value); }
void glUniform4ui64vNV(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4ui64vNV(location, count, value); }
void glUniform4uiv(std::int32_t location, std::int32_t count, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniform4uiv(location, count, value); }
void glUniformBlockBinding(std::uint32_t program, std::uint32_t uniformBlockIndex, std::uint32_t uniformBlockBinding) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding); }
void glUniformHandleui64ARB(std::int32_t location, std::uint64_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformHandleui64ARB(location, value); }
void glUniformHandleui64vARB(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformHandleui64vARB(location, count, value); }
void glUniformMatrix2dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2dv(location, count, transpose, value); }
void glUniformMatrix2fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2fv(location, count, transpose, value); }
void glUniformMatrix2x3dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2x3dv(location, count, transpose, value); }
void glUniformMatrix2x3fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2x3fv(location, count, transpose, value); }
void glUniformMatrix2x4dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2x4dv(location, count, transpose, value); }
void glUniformMatrix2x4fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix2x4fv(location, count, transpose, value); }
void glUniformMatrix3dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3dv(location, count, transpose, value); }
void glUniformMatrix3fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3fv(location, count, transpose, value); }
void glUniformMatrix3x2dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3x2dv(location, count, transpose, value); }
void glUniformMatrix3x2fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3x2fv(location, count, transpose, value); }
void glUniformMatrix3x4dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3x4dv(location, count, transpose, value); }
void glUniformMatrix3x4fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix3x4fv(location, count, transpose, value); }
void glUniformMatrix4dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4dv(location, count, transpose, value); }
void glUniformMatrix4fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4fv(location, count, transpose, value); }
void glUniformMatrix4x2dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4x2dv(location, count, transpose, value); }
void glUniformMatrix4x2fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4x2fv(location, count, transpose, value); }
void glUniformMatrix4x3dv(std::int32_t location, std::int32_t count, bool transpose, const double * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4x3dv(location, count, transpose, value); }
void glUniformMatrix4x3fv(std::int32_t location, std::int32_t count, bool transpose, const float * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformMatrix4x3fv(location, count, transpose, value); }
void glUniformSubroutinesuiv(GLenum shadertype, std::int32_t count, const std::uint32_t * indices) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformSubroutinesuiv(shadertype, count, indices); }
void glUniformui64NV(std::int32_t location, std::uint64_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformui64NV(location, value); }
void glUniformui64vNV(std::int32_t location, std::int32_t count, const std::uint64_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().uniformui64vNV(location, count, value); }
bool glUnmapBuffer(GLenum target) MYGL_NOEXCEPT { return mygl::get_static_dispatch().unmapBuffer(target); }
bool glUnmapNamedBuffer(std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().unmapNamedBuffer(buffer); }
void glUseProgram(std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().useProgram(program); }
void glUseProgramStages(std::uint32_t pipeline, GLbitfield stages, std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().useProgramStages(pipeline, stages, program); }
void glValidateProgram(std::uint32_t program) MYGL_NOEXCEPT { return mygl::get_static_dispatch().validateProgram(program); }
void glValidateProgramPipeline(std::uint32_t pipeline) MYGL_NOEXCEPT { return mygl::get_static_dispatch().validateProgramPipeline(pipeline); }
void glVertexArrayAttribBinding(std::uint32_t vaobj, std::uint32_t attribindex, std::uint32_t bindingindex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayAttribBinding(vaobj, attribindex, bindingindex); }
void glVertexArrayAttribFormat(std::uint32_t vaobj, std::uint32_t attribindex, std::int32_t size, GLenum type, bool normalized, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset); }
void glVertexArrayAttribIFormat(std::uint32_t vaobj, std::uint32_t attribindex, std::int32_t size, GLenum type, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayAttribIFormat(vaobj, attribindex, size, type, relativeoffset); }
void glVertexArrayAttribLFormat(std::uint32_t vaobj, std::uint32_t attribindex, std::int32_t size, GLenum type, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayAttribLFormat(vaobj, attribindex, size, type, relativeoffset); }
void glVertexArrayBindingDivisor(std::uint32_t vaobj, std::uint32_t bindingindex, std::uint32_t divisor) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayBindingDivisor(vaobj, bindingindex, divisor); }
void glVertexArrayElementBuffer(std::uint32_t vaobj, std::uint32_t buffer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayElementBuffer(vaobj, buffer); }
void glVertexArrayVertexBuffer(std::uint32_t vaobj, std::uint32_t bindingindex, std::uint32_t buffer, std::intptr_t offset, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride); }
void glVertexArrayVertexBuffers(std::uint32_t vaobj, std::uint32_t first, std::int32_t count, const std::uint32_t * buffers, const std::intptr_t * offsets, const std::int32_t * strides) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexArrayVertexBuffers(vaobj, first, count, buffers, offsets, strides); }
void glVertexAttrib1d(std::uint32_t index, double x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1d(index, x); }
void glVertexAttrib1dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1dv(index, v); }
void glVertexAttrib1f(std::uint32_t index, float x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1f(index, x); }
void glVertexAttrib1fv(std::uint32_t index, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1fv(index, v); }
void glVertexAttrib1s(std::uint32_t index, std::int16_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1s(index, x); }
void glVertexAttrib1sv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib1sv(index, v); }
void glVertexAttrib2d(std::uint32_t index, double x, double y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2d(index, x, y); }
void glVertexAttrib2dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2dv(index, v); }
void glVertexAttrib2f(std::uint32_t index, float x, float y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2f(index, x, y); }
void glVertexAttrib2fv(std::uint32_t index, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2fv(index, v); }
void glVertexAttrib2s(std::uint32_t index, std::int16_t x, std::int16_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2s(index, x, y); }
void glVertexAttrib2sv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib2sv(index, v); }
void glVertexAttrib3d(std::uint32_t index, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3d(index, x, y, z); }
void glVertexAttrib3dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3dv(index, v); }
void glVertexAttrib3f(std::uint32_t index, float x, float y, float z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3f(index, x, y, z); }
void glVertexAttrib3fv(std::uint32_t index, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3fv(index, v); }
void glVertexAttrib3s(std::uint32_t index, std::int16_t x, std::int16_t y, std::int16_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3s(index, x, y, z); }
void glVertexAttrib3sv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib3sv(index, v); }
void glVertexAttrib4Nbv(std::uint32_t index, const std::int8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nbv(index, v); }
void glVertexAttrib4Niv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Niv(index, v); }
void glVertexAttrib4Nsv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nsv(index, v); }
void glVertexAttrib4Nub(std::uint32_t index, std::uint8_t x, std::uint8_t y, std::uint8_t z, std::uint8_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nub(index, x, y, z, w); }
void glVertexAttrib4Nubv(std::uint32_t index, const std::uint8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nubv(index, v); }
void glVertexAttrib4Nuiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nuiv(index, v); }
void glVertexAttrib4Nusv(std::uint32_t index, const std::uint16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4Nusv(index, v); }
void glVertexAttrib4bv(std::uint32_t index, const std::int8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4bv(index, v); }
void glVertexAttrib4d(std::uint32_t index, double x, double y, double z, double w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4d(index, x, y, z, w); }
void glVertexAttrib4dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4dv(index, v); }
void glVertexAttrib4f(std::uint32_t index, float x, float y, float z, float w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4f(index, x, y, z, w); }
void glVertexAttrib4fv(std::uint32_t index, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4fv(index, v); }
void glVertexAttrib4iv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4iv(index, v); }
void glVertexAttrib4s(std::uint32_t index, std::int16_t x, std::int16_t y, std::int16_t z, std::int16_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4s(index, x, y, z, w); }
void glVertexAttrib4sv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4sv(index, v); }
void glVertexAttrib4ubv(std::uint32_t index, const std::uint8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4ubv(index, v); }
void glVertexAttrib4uiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4uiv(index, v); }
void glVertexAttrib4usv(std::uint32_t index, const std::uint16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttrib4usv(index, v); }
void glVertexAttribBinding(std::uint32_t attribindex, std::uint32_t bindingindex) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribBinding(attribindex, bindingindex); }
void glVertexAttribDivisor(std::uint32_t index, std::uint32_t divisor) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribDivisor(index, divisor); }
void glVertexAttribFormat(std::uint32_t attribindex, std::int32_t size, GLenum type, bool normalized, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribFormat(attribindex, size, type, normalized, relativeoffset); }
void glVertexAttribFormatNV(std::uint32_t index, std::int32_t size, GLenum type, bool normalized, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribFormatNV(index, size, type, normalized, stride); }
void glVertexAttribI1i(std::uint32_t index, std::int32_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI1i(index, x); }
void glVertexAttribI1iv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI1iv(index, v); }
void glVertexAttribI1ui(std::uint32_t index, std::uint32_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI1ui(index, x); }
void glVertexAttribI1uiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI1uiv(index, v); }
void glVertexAttribI2i(std::uint32_t index, std::int32_t x, std::int32_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI2i(index, x, y); }
void glVertexAttribI2iv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI2iv(index, v); }
void glVertexAttribI2ui(std::uint32_t index, std::uint32_t x, std::uint32_t y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI2ui(index, x, y); }
void glVertexAttribI2uiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI2uiv(index, v); }
void glVertexAttribI3i(std::uint32_t index, std::int32_t x, std::int32_t y, std::int32_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI3i(index, x, y, z); }
void glVertexAttribI3iv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI3iv(index, v); }
void glVertexAttribI3ui(std::uint32_t index, std::uint32_t x, std::uint32_t y, std::uint32_t z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI3ui(index, x, y, z); }
void glVertexAttribI3uiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI3uiv(index, v); }
void glVertexAttribI4bv(std::uint32_t index, const std::int8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4bv(index, v); }
void glVertexAttribI4i(std::uint32_t index, std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4i(index, x, y, z, w); }
void glVertexAttribI4iv(std::uint32_t index, const std::int32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4iv(index, v); }
void glVertexAttribI4sv(std::uint32_t index, const std::int16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4sv(index, v); }
void glVertexAttribI4ubv(std::uint32_t index, const std::uint8_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4ubv(index, v); }
void glVertexAttribI4ui(std::uint32_t index, std::uint32_t x, std::uint32_t y, std::uint32_t z, std::uint32_t w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4ui(index, x, y, z, w); }
void glVertexAttribI4uiv(std::uint32_t index, const std::uint32_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4uiv(index, v); }
void glVertexAttribI4usv(std::uint32_t index, const std::uint16_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribI4usv(index, v); }
void glVertexAttribIFormat(std::uint32_t attribindex, std::int32_t size, GLenum type, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribIFormat(attribindex, size, type, relativeoffset); }
void glVertexAttribIFormatNV(std::uint32_t index, std::int32_t size, GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribIFormatNV(index, size, type, stride); }
void glVertexAttribIPointer(std::uint32_t index, std::int32_t size, GLenum type, std::int32_t stride, const void * pointer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribIPointer(index, size, type, stride, pointer); }
void glVertexAttribL1d(std::uint32_t index, double x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL1d(index, x); }
void glVertexAttribL1dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL1dv(index, v); }
void glVertexAttribL1ui64ARB(std::uint32_t index, std::uint64_t x) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL1ui64ARB(index, x); }
void glVertexAttribL1ui64vARB(std::uint32_t index, const std::uint64_t * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL1ui64vARB(index, v); }
void glVertexAttribL2d(std::uint32_t index, double x, double y) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL2d(index, x, y); }
void glVertexAttribL2dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL2dv(index, v); }
void glVertexAttribL3d(std::uint32_t index, double x, double y, double z) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL3d(index, x, y, z); }
void glVertexAttribL3dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL3dv(index, v); }
void glVertexAttribL4d(std::uint32_t index, double x, double y, double z, double w) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL4d(index, x, y, z, w); }
void glVertexAttribL4dv(std::uint32_t index, const double * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribL4dv(index, v); }
void glVertexAttribLFormat(std::uint32_t attribindex, std::int32_t size, GLenum type, std::uint32_t relativeoffset) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribLFormat(attribindex, size, type, relativeoffset); }
void glVertexAttribLPointer(std::uint32_t index, std::int32_t size, GLenum type, std::int32_t stride, const void * pointer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribLPointer(index, size, type, stride, pointer); }
void glVertexAttribP1ui(std::uint32_t index, GLenum type, bool normalized, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP1ui(index, type, normalized, value); }
void glVertexAttribP1uiv(std::uint32_t index, GLenum type, bool normalized, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP1uiv(index, type, normalized, value); }
void glVertexAttribP2ui(std::uint32_t index, GLenum type, bool normalized, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP2ui(index, type, normalized, value); }
void glVertexAttribP2uiv(std::uint32_t index, GLenum type, bool normalized, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP2uiv(index, type, normalized, value); }
void glVertexAttribP3ui(std::uint32_t index, GLenum type, bool normalized, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP3ui(index, type, normalized, value); }
void glVertexAttribP3uiv(std::uint32_t index, GLenum type, bool normalized, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP3uiv(index, type, normalized, value); }
void glVertexAttribP4ui(std::uint32_t index, GLenum type, bool normalized, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP4ui(index, type, normalized, value); }
void glVertexAttribP4uiv(std::uint32_t index, GLenum type, bool normalized, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribP4uiv(index, type, normalized, value); }
void glVertexAttribPointer(std::uint32_t index, std::int32_t size, GLenum type, bool normalized, std::int32_t stride, const void * pointer) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexAttribPointer(index, size, type, normalized, stride, pointer); }
void glVertexBindingDivisor(std::uint32_t bindingindex, std::uint32_t divisor) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexBindingDivisor(bindingindex, divisor); }
void glVertexFormatNV(std::int32_t size, GLenum type, std::int32_t stride) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexFormatNV(size, type, stride); }
void glVertexP2ui(GLenum type, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP2ui(type, value); }
void glVertexP2uiv(GLenum type, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP2uiv(type, value); }
void glVertexP3ui(GLenum type, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP3ui(type, value); }
void glVertexP3uiv(GLenum type, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP3uiv(type, value); }
void glVertexP4ui(GLenum type, std::uint32_t value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP4ui(type, value); }
void glVertexP4uiv(GLenum type, const std::uint32_t * value) MYGL_NOEXCEPT { return mygl::get_static_dispatch().vertexP4uiv(type, value); }
void glViewport(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) MYGL_NOEXCEPT { return mygl::get_static_dispatch().viewport(x, y, width, height); }
void glViewportArrayv(std::uint32_t first, std::int32_t count, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().viewportArrayv(first, count, v); }
void glViewportIndexedf(std::uint32_t index, float x, float y, float w, float h) MYGL_NOEXCEPT { return mygl::get_static_dispatch().viewportIndexedf(index, x, y, w, h); }
void glViewportIndexedfv(std::uint32_t index, const float * v) MYGL_NOEXCEPT { return mygl::get_static_dispatch().viewportIndexedfv(index, v); }
void glWaitSemaphoreEXT(std::uint32_t semaphore, std::uint32_t numBufferBarriers, const std::uint32_t * buffers, std::uint32_t numTextureBarriers, const std::uint32_t * textures, const GLenum * srcLayouts) MYGL_NOEXCEPT { return mygl::get_static_dispatch().waitSemaphoreEXT(semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts); }
void glWaitSync(struct __GLsync * sync, GLbitfield flags, std::uint64_t timeout) MYGL_NOEXCEPT { return mygl::get_static_dispatch().waitSync(sync, flags, timeout); }
void glWeightPathsNV(std::uint32_t resultPath, std::int32_t numPaths, const std::uint32_t * paths, const float * weights) MYGL_NOEXCEPT { return mygl::get_static_dispatch().weightPathsNV(resultPath, numPaths, paths, weights); }
