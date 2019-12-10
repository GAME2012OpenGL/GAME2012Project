#include "Fence.h"

#include "Mesh.h"

Fence::Fence(GLuint uniformWorld, GLuint uniformShininess, float fShininess)
	:Object(uniformWorld, uniformShininess, fShininess)
{

}

Fence::~Fence()
{
}

void Fence::Render()
{
	glUniformMatrix4fv(m_uniformWorld, 1, GL_FALSE, &m_WorldMatrix[0][0]);
	glUniform1f(m_uniformShininess, m_fShininess);
	glBindTexture(GL_TEXTURE_2D, m_iTexture);


	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	m_pMesh->RenderMesh();
	
	glDepthMask(true);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
