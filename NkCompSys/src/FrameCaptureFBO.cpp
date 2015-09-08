/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "FrameCaptureFBO.h"


FrameCaptureFBO::FrameCaptureFBO(int resWidth, int resHeight) : resHeight(resHeight), resWidth(resWidth), frameCount(0)
{


 glGenFramebuffers(1, &this->fbo);
 glBindFramebuffer(GL_FRAMEBUFFER, fbo);

 glGenTextures(1, &this->textureId);
 glBindTexture(GL_TEXTURE_2D, textureId);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resWidth, resHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
 
 //glDrawBuffer(GL_NONE);

 pixels = std::shared_ptr<BYTE>(new BYTE[3*resWidth*resHeight]);

 glGenRenderbuffers(1, &this->renderBufferId);
 glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resWidth, resHeight);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);
 
 if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
  std::cout << "ERROR FRAMEBUFFER STATUS " << std::endl;

 }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  logGLError("FrameCaptureFBO: FBO creation");

}

void FrameCaptureFBO::saveFrame(const char* fileName) {
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.get());
	glBindTexture(GL_TEXTURE_2D, 0);
	char buff[sizeof(long)*8+1];
	_ltoa_s(frameCount, buff, 10);
	std::string st("frames\\");
	st.append(fileName);
	st.append("_");
	st.append(buff);
	st.append(".bmp");
	FIBITMAP* bm = FreeImage_ConvertFromRawBits(pixels.get(), resWidth, resHeight, 3*resWidth, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_BMP, bm, st.c_str(), 0);
	//GameStateMachine::setCaptureFrames(false);
	frameCount++;
}

FrameCaptureFBO::~FrameCaptureFBO(void)
{
}
