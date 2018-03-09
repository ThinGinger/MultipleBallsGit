#include "Physics.h"
#include <stdio.h>
using namespace GAME;

void Physics::SimpleNewtonMotion(PhysicsModel &model, const float deltaTime) {
	if (model.useGravity) {
		Vec3 g(0.0f, -0.8f, 0.0f);
		model.pos += model.vel * deltaTime + 0.5f * (model.accel + g) * deltaTime * deltaTime;
		model.vel += (model.accel + g) * deltaTime;
		
	}
	else {
		model.pos += model.vel * deltaTime + 0.5f * model.accel * deltaTime * deltaTime;
		model.vel += model.accel * deltaTime;
	}
}

bool Physics::PlaneSphereCollision(const Model &model, const Plane &p) {
	float sphereToPlane = VMath::distance(model.boundingSphere, p);
	if (sphereToPlane < 0.0f) {
		return true;
	}
	return false;
}

void Physics::PlaneSphereCollisionResponse(PhysicsModel &model, const Plane &p) {
	float e = 1.0f; ///Coefficiaent of restistution
	/// Check that the model is moving toward the wall otherwise return
	/// This prevents the model from getting stuck at the wall
	if (VMath::dot(model.vel, p) >= 0.0f) return;
	float magnitude = VMath::mag(model.vel);
	if (fabs(magnitude) < VERY_SMALL) {
		printf("Warning: CollisionResponse with no velocity\n");
		return;
	}
	Vec3 nVelocity = VMath::normalize(model.vel);
	Vec3 result = VMath::reflect(nVelocity,p);
	model.vel  = result * 0.99 * magnitude;
}



bool Physics::SphereSphereCollision(const PhysicsModel &model1, const PhysicsModel &model2) {
	Vec3 center1 = model1.boundingSphere;
	Vec3 center2 = model2.boundingSphere;
	float dist = VMath::distance(center1, center2);
	if ( dist < (model1.boundingSphere.r + model2.boundingSphere.r) ) {
		return true; 
	}
	return false;
}

void Physics::SphereSphereCollisionResponse(PhysicsModel &model1, PhysicsModel &model2) {
	float e = 0.05f; /// Coefficient of restitution
	Vec3 lineOfAction = model1.boundingSphere - model2.boundingSphere;
	Vec3 normalizedLineOfAction = VMath::normalize(lineOfAction);
	float v1p = VMath::dot(normalizedLineOfAction, model1.vel);
	float v2p = VMath::dot(normalizedLineOfAction, model2.vel);
	
	float v1pnew = (((model1.mass - e * model2.mass) * v1p) + ((1.0f + e) * model2.mass * v2p)) / (model1.mass + model2.mass);
	float v2pnew = (((model2.mass - e * model1.mass) * v2p) + ((1.0f + e) * model1.mass * v1p)) / (model1.mass + model2.mass);

	if (v1p - v2p > 0.0f) return; ///  This prevents them from sticking together
	model1.vel += (v1pnew - v1p) * normalizedLineOfAction;
	model2.vel += (v2pnew - v2p) * normalizedLineOfAction;
}