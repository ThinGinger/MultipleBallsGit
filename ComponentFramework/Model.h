#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include "Shader.h"

namespace GAME {
using namespace MATH;

class PhysicsModel {
	friend class Physics;		/// This a test of the idea I had about using
protected:						/// a static Physics class that is a friend 
	Vec3 pos;					/// of just the data in the PhysicsModel class
	Vec3 vel;
	Vec3 accel;
	Vec3 orientation;
	float mass;
	bool useGravity;
	Sphere boundingSphere;
};


class Model: public PhysicsModel {
protected:
	Matrix4 modelMatrix;
	const std::vector<Mesh*> meshes;
	const Shader*  shader;
public:

	inline void setPos(const Vec3& pos_){ pos = pos_; }
	inline void setVel(const Vec3& vel_){ vel = vel_; }
	inline void setAccel(const Vec3& accel_) { accel = accel_; }
	inline void setMass(const float& mass_) { mass = mass_; }

	inline void setBoundingSphere(const float r) { boundingSphere = pos;  boundingSphere.r = r; }
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	Model(const std::vector<Mesh*> meshes_, const Shader* shader_);
	bool OnCreate();
	void OnDestroy();
	void Render() const;
	void Update(const float deltaTime);
	virtual ~Model();
	
};


} /// end of namespace

#endif