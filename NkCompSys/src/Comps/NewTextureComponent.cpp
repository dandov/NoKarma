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

#include "GameComponents/NewTextureComponent.h"

NewTextureComponent::NewTextureComponent()
: mTexs()
, mKa()
, mKd()
, mLightAttenuation(0) { }

NewTextureComponent::NewTextureComponent(const std::vector<Texture2D> texs)
: mTexs(texs)
, mKa()
, mKd()
, mLightAttenuation(0) { }

NewTextureComponent::NewTextureComponent(const ModelData& modelData, const ModelMaterial& material,
	const glm::vec4& Ka, const glm::vec4& Kd, float lightAttenuation) 
	: mTexs()
	, mKa(Ka)
	, mKd(Kd)
	, mLightAttenuation(lightAttenuation) {

	//TODO: que otro nombre de sampler se necesita
	std::string names[2] = { "diffuseTexture", "" };

	// iterar texturas, son 2 es global la bar TOTAL...
	for (int i = 0; i < TOTAL_TEXTURE_TYPES; i++) {
		int textureIndex = material.textureIndices[i];
		if (textureIndex != -1) {
			const Texture& texture = modelData.textures[textureIndex];
			if (texture.bm != NULL) {
				Texture2D tex(texture.bm, names[i], i, texture.channels);
				tex.releaseTexels();
				this->mTexs.push_back(tex);
			}
		}
	}
}
