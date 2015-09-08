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

#include "Terrain.h"


Terrain::Terrain(const char* heightmapfile, const char* terraintexturefile, const float& initialH, const unsigned int& width, const unsigned int& lenght, const unsigned short& det)
	: theight(lenght), twidth(width), detail(det), initialheight(initialH)
{
	std::string fname("Terrain\\");
	std::string texfname("Terrain\\");
	heightData = 0;
	texData = 0;
	vao = 0;
	vbo = 0;
	texture = 0;
	texwidth = 0;
	texheight = 0;

	fname +=  heightmapfile;
	texfname += terraintexturefile;
	std::ifstream file(fname, std::ios::ate | std::ios::binary);
	if (file)
	{
	std::ios::pos_type size = file.tellg();
	file.seekg(0);
	vwidth = twidth/detail;
	vheight = theight/detail;

	heightData = new unsigned char[twidth*theight];
	//indexData = new unsigned char[];
	file.read((char*)heightData, twidth*theight);
	this->vertexData = new vertex[vwidth*vheight];

	createTerrain(heightData);
	loadTexture(texfname.c_str());
	computeNormals();
	}
	//charD
	///file

	
}
void Terrain::createTerrain(unsigned char*& heightdata)
{
	/*float flx, flz;
	int index = 0;
	for (int i = 0; i<twidth-1; i+= 2)
	{
		for (int j = 0; j < theight-1; j+= 4)
		{
			for (int k = 0; k< 6; k++)
			{
				flx= (float)i + ((k ==1 || k==2 || k==5))? 1: 0;
				flz= (float)j + ((k ==2 || k==4 || k==5))? 1: 0;

				vertexData[index].x = flx;
				vertexData[index].z = flz;
			vertexData[index].y = (float)heightData[int(flx)*(twidth) + (int)flz] - 90.0f;
			vertexData[index].s = flx/twidth;
			vertexData[index].t = flz/theight;
			index++;
			}

			vertexData[i*twidth + j].x = i+1;
			vertexData[i*twidth + j].z = j/2;
			vertexData[i*twidth + j].y = (float)heightData[(i+1)*(twidth) + j] - 90.0f;
			vertexData[i*twidth + j].s = (i+1.0f)/twidth;
			vertexData[i*twidth + j].t = (float)j/theight;

			vertexData[i*twidth + j+1].x = i;
			vertexData[i*twidth + j+1].z = j/2;
			vertexData[i*twidth + j+1].y = (float)heightData[i*(twidth) + j]- 90.0f;
			vertexData[i*twidth + j+1].s = (float)i/twidth;
			vertexData[i*twidth + j+1].t = (float)j/theight;

			
			vertexData[i*twidth + j+2].x = i + 1;
			vertexData[i*twidth + j+2].z = j/2 + 1;
			vertexData[i*twidth + j+2].y = (float)heightData[(i+1)*(twidth) + j+1]- 90.0f;
			vertexData[i*twidth + j+2].s = (i+1.0f)/twidth;
			vertexData[i*twidth + j+2].t = (j+1.0f)/theight;

			vertexData[i*twidth + j+3].x = i;
			vertexData[i*twidth + j+3].z = j/2 + 1;
			vertexData[i*twidth + j+3].y = (float)heightData[i*(twidth) + j+1]- 90.0f;
			vertexData[i*twidth + j+3].s = (float)i/twidth;
			vertexData[i*twidth + j+3].t = (j+1.0f)/theight;

			
		}
	}*/
	//unsigned int step = d
	for (int i = 0; i < twidth; i+= detail)
	{
		for (int j = 0; j < theight; j+= detail)
		{
			unsigned int indx = (i/detail)*vwidth + j/detail;
			vertexData[indx].x = i;
			vertexData[indx].z = j;
			vertexData[indx].y = (float)heightData[i*(twidth) + j]+ initialheight;
			
			vertexData[indx].s = (float)i/twidth;
			vertexData[indx].t = (float)j/theight;
		}
		//if (i == 1023)
			//std::cout << " vertexData " << vertexData[i*twidth + 1].x << std::endl;
	}


	
	//int i = 0;
	int j = 0;
	int k = 0;
	//detail = 1;

	//indexData.push_back(0);
	//indexData.push_back(twidth);
	indexData.push_back((vwidth-1)*vwidth);
	indexData.push_back((vwidth-2)*vwidth);
	/*indexData.push_back((i+1)*twidth + 1);
	indexData.push_back((i+1)*twidth);
	indexData.push_back(0);
	indexData.push_back(1);*/
	//i = 0;
	/*for (int i = 0; i < twidth-1; i++)
	{
		for (k=1; k < theight; k++, j++)
		{
				unsigned int i1 = (i*twidth)+k*((i+1)%2) + (i%2)*(theight-k-1);
				unsigned int i2 = ((i+1)*twidth)+k*((i+1)%2) + (i%2)*(theight-k-1);
				indexData.push_back(i1);
				indexData.push_back(i2);		
		}
		indexData.push_back(((i+2)*twidth)+(k-1)*((i+1)%2) + ((i+2)%2)*(theight-k));
		k = 1;
	}*/
	for (int i = vwidth-1, col = 0; i > 0; i--, col++)
	{
		for (k=1; k < vheight; k++, j++)			// indices para triangle strips de derecha a izquierda empezando en columna (twidth -1)
		{
				unsigned int i1 = ((i)*vwidth)+k*((col+1)%2) + (col%2)*(vheight-k-1); // cuando es par 
				unsigned int i2 = ((i-1)*vwidth)+k*((col+1)%2) + (col%2)*(vheight-k-1);
				
				indexData.push_back(i1);
				indexData.push_back(i2);		
		}
		indexData.push_back(((i-2)*vwidth)+(k-1)*((col+1)%2) + ((col+2)%2)*(vheight-k));
		k = 1;
	}

	indexData.erase(indexData.end()-1);

			



}

void Terrain::computeNormals()
{
	//unsigned int size = twidth*theight;
	unsigned int col = 0;
	int offset = 0;
	int prev =  indexData[2];
	int next =0, target = 0;
	for (int i = 0; i < indexData.size(); i++)
	{
		if (i+1 == indexData.size())		/// solo para el ultimo caso
			next = target;
		else
			next = indexData[i+1];

		target = indexData[i];
		

		vertex v1 = vertexData[target];
		vertex v2 = vertexData[prev];
		vertex v3 = vertexData[next];

		glm::vec3 dir1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		dir1 = glm::normalize(dir1);
		glm::vec3 dir2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
		dir2 = glm::normalize(dir2);

		glm::vec3 normal = glm::cross(dir1, dir2);

		normal = (normal.y < 0)? glm::vec3(0) - normal: normal; 

		vertexData[target].nx = normal.x;
		vertexData[target].ny = normal.y;
		vertexData[target].nz = normal.z;

		col++;
		
		int cond = col/(2*vheight-1)%2;
		int cond2 = ((i+1)/(2*vheight-1))%2;
		col = (col == 2*vheight-1)? 0: col;

		prev = target;
		if (cond && cond2)
			prev -= (vheight+1);
		else if (cond && !cond2)
			prev -= (vheight-1);
		
		//prev = target - (col/(2*twidth - 1))%2*(twidth+1) - (i/(2*twidth -1))%2*(twidth-1);	
	}



	
	/*for (int i = 0; i<size; i++)
	{
		vertex v1 = vertexData[i];
		vertex v2 = vertexData[(i+1)%size];
		vertex v3 = vertexData[(i+2)%size];
		glm::vec3 dir1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		dir1 = glm::normalize(dir1);
		glm::vec3 dir2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		dir2 = glm::normalize(dir2);
		glm::vec3 normal = glm::cross(dir1, dir2);
		vertexData[i].nx = normal.x;
		vertexData[i].ny = normal.y;
		vertexData[i].nz = normal.z;
	}*/
}

bool Terrain::createGLBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenTextures(1, &texture);
	GLuint indx = 0;
	glGenBuffers(1, &indx);

	unsigned int size = vwidth*vheight;

	glBindVertexArray(vao);
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 32*size, vertexData, GL_STATIC_DRAW);
			std::cout  << " 0 " << vertexData[0].x << " z " << vertexData[0].z << " 1 " << vertexData[1].x << " z" << vertexData[1].z 
				<< " 2 " << vertexData[2].x << " z" << vertexData[2].z
				<< " 3 " << vertexData[3].x << " z" << vertexData[3].z << std::endl;

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*)12);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (GLvoid*)20);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indx);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size()*sizeof(unsigned int), &(indexData[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_BGR, GL_UNSIGNED_BYTE, texData);

		float maxa = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxa);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// bilinear Filtering
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxa);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] heightData;
	delete[] vertexData;
	heightData = 0;
	vertexData = 0;
	
	return true;
}

GLubyte * Terrain::loadImage(const char* heightmapfile)
{
	GLubyte* heightdata = 0;
	FREE_IMAGE_FORMAT f = FreeImage_GetFileType(heightmapfile, 0);
	FIBITMAP* bm = FreeImage_Load(f, heightmapfile);
		if (bm != NULL)
		{

		bm = FreeImage_ConvertTo8Bits(bm);
		heightdata = ((GLubyte*)FreeImage_GetBits(bm));
		std::cout << " Texture " << heightmapfile <<  " loaded! " << std::endl;
		unsigned int mapwidth = FreeImage_GetWidth(bm);
		unsigned int mapheight = FreeImage_GetHeight(bm);
		}
		else
		std::cout << " could not find texture "<< std::endl;
		return heightdata;
}

bool Terrain::loadTexture(const char* texturefile)
{
	texData = 0;
	FREE_IMAGE_FORMAT f = FreeImage_GetFileType(texturefile, 0);
	FIBITMAP* bm = FreeImage_Load(f, texturefile);
		if (bm != NULL)
		{

			bm = FreeImage_ConvertTo24Bits(bm);
			texData = ((GLubyte*)FreeImage_GetBits(bm));
			std::cout << " Texture " << texturefile <<  " loaded! " << std::endl;
			texwidth = FreeImage_GetWidth(bm);
			texheight = FreeImage_GetHeight(bm);
			return 1;
		}
		else
		{
			std::cout << " could not find texture "<< texturefile << std::endl;
			return 0;
		}
}

void Terrain::draw(const GLuint& drawProgram, const glm::mat4& modelViewMat)
{

	glUseProgram(drawProgram);
	GLint matloc = glGetUniformLocation(drawProgram, "modelViewMatrix");
	GLuint sizeid = glGetUniformLocation(drawProgram, "modelSize");
	glUniform1f(sizeid, 1);
	//printError("Gl terr2");
		glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
	glm::mat4 mvm = glm::translate(modelViewMat, glm::vec3(-512, -80, -512));
	glUniformMatrix4fv(matloc, 1, GL_FALSE, &mvm[0][0]);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
	//printError("Gl terr2");
	glPointSize(5);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, vwidth*vheight);
	glDrawElements(GL_TRIANGLE_STRIP, indexData.size(), GL_UNSIGNED_INT, 0);
	//printError("Gl terr2");
	glBindVertexArray(0);

	//glUseProgram(0);
}


Terrain::~Terrain(void)
{
	if (heightData)
		delete[] heightData;
	if (vertexData)
		delete[] vertexData;

}
