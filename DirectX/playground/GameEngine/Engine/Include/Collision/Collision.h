#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
	static bool CollisionBox2DToBox2D(class CColliderBox2D* src, class CColliderBox2D* dest);

	static bool CollisionBox3DToBox3D(class CColliderBox3D* src, class CColliderBox3D* dest);

	static bool CollisionCircleToCircle(class CColliderCircle* src, class CColliderCircle* dest);

	static bool CollisionBox3DToSphere(class CColliderBox3D* src, class CColliderSphere* dest);

	static bool CollisionBox2DToCircle(class CColliderBox2D* src, class CColliderCircle* dest);

	static bool CollisionBox2DToPixel(class CColliderBox2D* src, class CColliderPixel* dest);

	static bool CollisionCircleToPixel(class CColliderCircle* src, class CColliderPixel* dest);

	static bool CollisionSphereToSphere(class CColliderSphere* src, class CColliderSphere* dest);


public:
	static bool CollisionBox2DToBox2D(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sBox2DInfo& src, const sBox2DInfo& dest);
	static bool CollisionCircleToCircle(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sCircleInfo& src, const sCircleInfo& dest);
	static bool CollisionBox2DToCircle(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sBox2DInfo& src, const sCircleInfo& dest);
	static bool CollisionBox2DToPixel(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sBox2DInfo& src, const sPixelInfo& dest);
	static bool CollisionCircleToPixel(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sCircleInfo& src, const sPixelInfo& dest);

	static bool CollisionBox2DToPoint(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sBox2DInfo& boxInfo, const Vector2& point);
	static bool CollisionCircleToPoint(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sCircleInfo& circleInfo, const Vector2& point);
	static bool CollisionPixelToPoint(sCollisionResult& srcResult, sCollisionResult& destResult,
		const sPixelInfo& pixelInfo, const Vector2& point);

public:
	static bool CollisionBox3DToBox3D(sCollisionResult& srcResult, sCollisionResult& destResult, const sBox3DInfo& src,
		const sBox3DInfo& dest);
	static bool CollisionSphereToSphere(sCollisionResult& srcResult, sCollisionResult& destResult, const sSphereInfo& src,
		const sSphereInfo& dest);
	static bool CollisionBox3DToSphere(sCollisionResult& srcResult, sCollisionResult& destResult, const sBox3DInfo& src,
		const sSphereInfo& dest);

public:
	static bool CollisionRayToSphere(Vector3& hitPoint, const sRay& ray, const sSphereInfo& sphere);
};

