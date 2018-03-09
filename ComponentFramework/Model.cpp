#include "Model.h"
#include "Physics.h"

using namespace GAME;
using namespace MATH;

Model::Model(const std::vector<Mesh*> meshes_, const Shader*  shader_): meshes(meshes_), shader(shader_) {
	useGravity = true;
}

bool Model::OnCreate() {

	return true;
}

void Model::OnDestroy() {}


void Model::Render() const {
	Matrix4 modelMatrix = MMath::translate(pos);
	GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
	for (Mesh* mesh : meshes) {
		mesh->Render();
	}
}

void Model::Update(const float deltaTime) {
	Physics::SimpleNewtonMotion( *this, deltaTime);
	boundingSphere = pos;
}

Model::~Model() {}