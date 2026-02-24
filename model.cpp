#include "model.h"

Model::Model(std::vector<float> vertices, std::vector<int> indices, std::vector<int> textures, bool load_triangles) {
    vertex_count = vertices.size();
    index_count = indices.size();
    this->textures = textures;

    //VBO
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    size_t vboBufferSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, vboBufferSize, vertices.data(),
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //INDEX
    glGenBuffers(1, &index_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
    size_t indexBufferSize = indices.size() * sizeof(int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //VAO
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), (void*)0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    //uv
    glVertexAttribPointer(2, 2, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    if (load_triangles) {
        //Loads all the triangles into a triangle array
        int current_texture = 0;
        int next = textures[0];
        for (int i = 0; i < indices.size(); i += 3) {
            next--;
            if (next <= 0) {
                current_texture++;
                next = textures[current_texture]+1;
            }
            int v = 8 * indices[i];
            //Getting positions and normals of all 3 vertices
            glm::vec3 a = glm::vec3(vertices[v], vertices[v + 1], vertices[v + 2]);
            glm::vec3 nA = glm::vec3(vertices[v+3], vertices[v + 4], vertices[v + 5]);
            v = 8 * indices[i+1];
            glm::vec3 b = glm::vec3(vertices[v], vertices[v + 1], vertices[v + 2]);
            glm::vec3 nB = glm::vec3(vertices[v+3], vertices[v + 4], vertices[v + 5]);
            v = 8 * indices[i + 2];
            glm::vec3 c = glm::vec3(vertices[v], vertices[v + 1], vertices[v + 2]);
            glm::vec3 nC = glm::vec3(vertices[v+3], vertices[v + 4], vertices[v + 5]);
            triangles.push_back({ a,b,c,nA,nB,nC,glm::normalize(nA+nB+nC),current_texture});
            
        }
    }
}
Model::~Model() {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &index_id);
}
void Model::bind() {
	glBindVertexArray(vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
}
void Model::unbind() {
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Model::render() {
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
void Model::render_section(int start, int length) {
    
    glDrawElements(GL_TRIANGLES, length * 3, GL_UNSIGNED_INT, (const void*)(start * 3 * sizeof(int)));
}