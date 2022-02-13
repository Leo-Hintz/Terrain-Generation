#include "Mesh.h"
#include "Renderer.hpp"

void Mesh::Draw()
{
	Renderer::Submit(this);
}

void Terrain::Draw()
{
	Renderer::Submit(this);
}