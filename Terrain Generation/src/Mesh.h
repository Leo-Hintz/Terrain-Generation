#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>


static struct Vertex
{
	glm::vec4 pos;
	glm::vec2 tex;
	glm::vec3 norm;
};
class Mesh
{
public:
	static Mesh* CreateMesh(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount)
	{
		Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
		mesh->m_Vertices = vertices;
		mesh->m_Indices = indices;
		mesh->m_IndexCount = indexCount;
		mesh->m_VertexCount = vertexCount;

		glGenVertexArrays(1, &mesh->m_VertexArray);
		glBindVertexArray(mesh->m_VertexArray);

		glGenBuffers(1, &mesh->m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		


		glGenBuffers(1, &mesh->m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return mesh;
	}
	~Mesh()
	{
		free(m_Indices);
		free(m_Vertices);
		free(this);
	}
	void Draw();
	inline const Vertex* GetVertices() const
	{
		return m_Vertices;
	}
	inline const uint32_t& GetVertexCount() const
	{
		return m_VertexCount;
	}
	inline void SetVertices(Vertex* newVertices)
	{
		m_Vertices = newVertices;
	}
	inline const uint32_t* GetIndices() const
	{
		return m_Indices;
	}
	inline const uint32_t& GetIndexCount() const
	{
		return m_IndexCount;
	}

	void Use()
	{
		glBindVertexArray(m_VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	}
protected:
	Vertex* m_Vertices;
	uint32_t* m_Indices;
	
	uint32_t m_IndexCount;
	uint32_t m_VertexCount;

	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
};

class Terrain : public Mesh
{
public:
	static Terrain* CreateTerrain(Vertex* vertices, uint32_t width, uint32_t* indices, uint32_t height) //Creation method enforces heap allocation, needed for large amounts of data
	{
		Terrain* terrain = (Terrain*)malloc(sizeof(Terrain));
		terrain->m_Vertices = vertices;
		terrain->m_Indices = indices;
		terrain->m_IndexCount = width * height * 2 - 1;
		terrain->m_VertexCount = width * height;
		terrain->m_Width = width;
		terrain->m_Height = height;

		glGenVertexArrays(1, &terrain->m_VertexArray);
		glBindVertexArray(terrain->m_VertexArray);

		glGenBuffers(1, &terrain->m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, terrain->m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, terrain->m_VertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
		glEnableVertexAttribArray(1);


		glGenBuffers(1, &terrain->m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain->m_IndexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return terrain;
	}
	void Draw();
	uint32_t& GetWidth() { return m_Width; }
	uint32_t& GetHeight() { return m_Height; }
private:	
	uint32_t m_Width;
	uint32_t m_Height;
};

static uint32_t skyBoxIndices[36] =
{
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};
static Vertex skyBoxVertices[8] =
{
	{{-1,-1,-1, 1},{0,0},{}},
	{{1,-1,-1,1},{1,0},{}},
	{{1,1,-1,1},{1,1},{}},
	{{-1,1,-1,1},{0,1},{}},
	{{-1,-1,1,1},{0,0},{}},
	{{1,-1,1,1},{1,0},{}},
	{{1,1,1,1},{1,1},{}},
	{{-1,1,1,1},{0,0},{}}
};

class Skybox : Mesh
{
public:
	static Skybox* CreateSkybox(const char* directory)
	{
		Skybox* skybox = (Skybox*)malloc(sizeof(Skybox));
		glGenVertexArrays(1, &skybox->m_VertexArray);
		glBindVertexArray(skybox->m_VertexArray);

		glGenBuffers(1, &skybox->m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, skybox->m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, skybox->m_VertexCount * sizeof(Vertex), skyBoxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
		glEnableVertexAttribArray(1);


		glGenBuffers(1, &skybox->m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox->m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, skybox->m_IndexCount * sizeof(uint32_t), skyBoxIndices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};