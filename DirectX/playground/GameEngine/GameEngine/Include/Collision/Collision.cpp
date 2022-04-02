#include "Collision.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderPixel.h"

bool CCollision::CollisionBox2DToBox2D(CColliderBox2D* src, CColliderBox2D* dest)
{
	sCollisionResult srcResult, destResult;

	if (CollisionBox2DToBox2D(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
	{
		srcResult.src = src;
		srcResult.dest = dest;

		destResult.src = dest;
		destResult.dest = src;

		src->m_Result = srcResult;
		dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToCircle(CColliderCircle* src, CColliderCircle* dest)
{
	sCollisionResult srcResult, destResult;

	if (CollisionCircleToCircle(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
	{
		srcResult.src = src;
		srcResult.dest = dest;

		destResult.src = dest;
		destResult.dest = src;

		src->m_Result = srcResult;
		dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionBox2DToBox2D(sCollisionResult& srcResult, sCollisionResult& destResult, 
	const sBox2DInfo& src, const sBox2DInfo& dest)
{
	Vector2 centerDir = src.center - dest.center;

	// �� 4�� �� ������ �Ѵ�.

	// ���� src�� x��
	Vector2 axis = src.axis[0];

	float centerProjDist = fabs(centerDir.Dot(axis));

	float r1, r2;

	// src�� r1�� ������ ���� �ʾƵ� �״�� �� ���̰� ������ ����
	r1 = src.length.x;
	// dest�� ��� src�� �࿡ ������ ���Ŀ� ���� x, y�� ���̸� ���ؼ� �����༭ �� ���̸� ���Ѵ�.
	r2 = fabs(dest.axis[0].Dot(axis) * dest.length.x) + fabs(dest.axis[1].Dot(axis) * dest.length.y);

	// ��ģ ���� �Ÿ����� ���ٸ� �浹�� �ƴϴ�.
	if (centerProjDist > r1 + r2)
		return false;

	// src�� y��
	axis = src.axis[1];

	centerProjDist = fabs(centerDir.Dot(axis));

	r1 = src.length.y;
	r2 = fabs(dest.axis[0].Dot(axis) * dest.length.x) + fabs(dest.axis[1].Dot(axis) * dest.length.y);

	if (centerProjDist > r1 + r2)
		return false;

	// dest�� x��
	axis = dest.axis[0];

	centerProjDist = fabs(centerDir.Dot(axis));

	r1 = dest.length.x;
	r2 = fabs(src.axis[0].Dot(axis) * src.length.x) + fabs(src.axis[1].Dot(axis) * src.length.y);

	if (centerProjDist > r1 + r2)
		return false;

	// dest�� y��
	axis = dest.axis[1];

	centerProjDist = fabs(centerDir.Dot(axis));

	r1 = dest.length.y;
	r2 = fabs(src.axis[0].Dot(axis) * src.length.x) + fabs(src.axis[1].Dot(axis) * src.length.y);

	if (centerProjDist > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToCircle(CColliderBox2D* src, CColliderCircle* dest)
{
	sCollisionResult srcResult, destResult;

	if (CollisionBox2DToCircle(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
	{
		srcResult.src = src;
		srcResult.dest = dest;

		destResult.src = dest;
		destResult.dest = src;

		src->m_Result = srcResult;
		dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionBox2DToPixel(CColliderBox2D* src, CColliderPixel* dest)
{
	sCollisionResult srcResult, destResult;

	if (CollisionBox2DToPixel(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
	{
		srcResult.src = src;
		srcResult.dest = dest;

		destResult.src = dest;
		destResult.dest = src;

		src->m_Result = srcResult;
		dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToPixel(CColliderCircle* src, CColliderPixel* dest)
{
	sCollisionResult srcResult, destResult;

	if (CollisionCircleToPixel(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
	{
		srcResult.src = src;
		srcResult.dest = dest;

		destResult.src = dest;
		destResult.dest = src;

		src->m_Result = srcResult;
		dest->m_Result = destResult;

		return true;
	}

	return false;
}

bool CCollision::CollisionBox2DToPoint(sCollisionResult& srcResult, sCollisionResult& destResult, const sBox2DInfo& boxInfo, const Vector2& point)
{
	// ������ x, y �࿡ ���� �����Ͽ� ������ ��ġ���� �Ǵ��Ѵ�.
	Vector2 centerDir = boxInfo.center - point;

	// x��
	Vector2 axis = boxInfo.axis[0];

	float centerProjDist = fabs(centerDir.Dot(axis));

	if (centerProjDist > boxInfo.length.x)
		return false;

	// y��
	axis = boxInfo.axis[1];

	centerProjDist = fabs(centerDir.Dot(axis));

	if (centerProjDist > boxInfo.length.y)
		return false;

	return true;
}

bool CCollision::CollisionCircleToPoint(sCollisionResult& srcResult, sCollisionResult& destResult, const sCircleInfo& circleInfo, const Vector2& point)
{
	float dist = circleInfo.center.Distance(point);

	return dist <= circleInfo.radius;
}


bool CCollision::CollisionCircleToCircle(sCollisionResult& srcResult, sCollisionResult& destResult, const sCircleInfo& src, const sCircleInfo& dest)
{
	float dist = src.center.Distance(dest.center);

	return dist <= src.radius + dest.radius;
}

bool CCollision::CollisionBox2DToCircle(sCollisionResult& srcResult, sCollisionResult& destResult, const sBox2DInfo& src, const sCircleInfo& dest)
{
	Vector2 centerDir = src.center - dest.center;

	Vector2 axis = src.axis[0];

	float centerProjDist = fabs(centerDir.Dot(axis));

	float r1, r2;

	r1 = src.length.x;
	r2 = dest.radius;

	if (centerProjDist > r1 + r2)
		return false;

	axis = src.axis[1];

	centerProjDist = fabs(centerDir.Dot(axis));

	r1 = src.length.y;
	r2 = dest.radius;

	if (centerProjDist > r1 + r2)
		return false;

	axis = centerDir;
	axis.Normalize();

	centerProjDist = centerDir.Length();

	r1 = fabs(src.axis[0].Dot(axis) * src.length.x) + fabs(src.axis[1].Dot(axis) * src.length.y);
	r2 = dest.radius;

	if (centerProjDist > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToPixel(sCollisionResult& srcResult, sCollisionResult& destResult, 
	const sBox2DInfo& src, const sPixelInfo& dest)
{
	// ���� �ڽ� �浹�� �ǽ��Ѵ�.
	if (!CollisionBox2DToBox2D(srcResult, destResult, src, dest.box))
		return false;

	// �������� ���Ѵ�.
	float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
	float right = src.Max.x > dest.Max.x ? dest.Max.x : src.Max.x;

	float bottom = src.Min.y < dest.Min.y ? dest.Min.y : src.Min.y;
	float top = src.Max.y > dest.Max.y ? dest.Max.y : src.Max.y;

	// ���� ���������� �� �ϴ��� ��ǥ�� ���Ѵ�.
	Vector2 LB = dest.box.center - dest.box.length;

	left -= LB.x;
	right -= LB.x;

	bottom -= LB.y;
	top -= LB.y;

	left = left < 0.0f ? 0.0f : left;
	bottom = bottom < 0.0f ? 0.0f : bottom;

	right = right >= (float)dest.width ? (float)dest.width - 1.0f : right;
	top = top >= (float)dest.height ? (float)dest.height - 1.0f : top;


	top = dest.height - top;
	bottom = dest.height - bottom;

	bool collision = false;

	// ������ ������ �ݺ��Ѵ�.
	for (int y = (int)top; y < (int)bottom; y++)
	{
		for (int x = (int)left; x < (int)right; x++)
		{
			// RGBA����ؼ� 4�� ���Ѵ�.
			int index = y * (int)dest.width * 4 + x * 4;

			// ���� �ε����� �ȼ��� ���� �ڽ� �ȿ� �����ϴ����� �Ǵ��Ѵ�.
			// ���� �ȼ��� ������������� ��ġ�� �����ش�.
			Vector2 pixelWorldPos = LB + Vector2((float)x, (float)dest.height - (float)y);

			if (!CollisionBox2DToPoint(srcResult, destResult, src, pixelWorldPos))
				continue;

			switch (dest.type)
			{
			case PixelCollision_Type::Color_Ignore:
				if (dest.pixels[index] == dest.colors[0] &&
					dest.pixels[index + 1] == dest.colors[1] &&
					dest.pixels[index + 2] == dest.colors[2])
					continue;

				collision = true;
				break;
			case PixelCollision_Type::Color_Confirm:
				if (dest.pixels[index] == dest.colors[0] &&
					dest.pixels[index + 1] == dest.colors[1] &&
					dest.pixels[index + 2] == dest.colors[2])
					collision = true;
				break;
			case PixelCollision_Type::Alpha_Ignore:
				if (dest.pixels[index + 3] == dest.colors[3])
					continue;

				collision = true;
				break;
			case PixelCollision_Type::Alpha_Confirm:
				if (dest.pixels[index + 3] == dest.colors[3])
					collision = true;
				break;
			}

			if (collision)
				break;
		}

		if (collision)
			break;
	}

	return collision;
}

bool CCollision::CollisionCircleToPixel(sCollisionResult& srcResult, sCollisionResult& destResult, 
	const sCircleInfo& src, const sPixelInfo& dest)
{
	if (!CollisionBox2DToCircle(srcResult, destResult, dest.box, src))
		return false;

	// �������� ���Ѵ�.
	float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
	float right = src.Max.x > dest.Min.x ? dest.Max.x : src.Max.x;

	float bottom = src.Min.y < dest.Min.y ? dest.Min.y : src.Min.y;
	float top = src.Max.y > dest.Max.y ? dest.Max.y : src.Max.y;

	// ���� ���������� �� �ϴ� ��ǥ�� ���Ѵ�.
	Vector2 LB = dest.box.center - dest.box.length;

	left -= LB.x;
	right -= LB.x;

	bottom -= LB.y;
	top -= LB.y;

	left = left < 0.0f ? 0.0f : left;
	bottom = bottom < 0.0f ? 0.0f : bottom;

	right = right >= (float)dest.width ? (float)dest.width - 1.0f : right;
	top = top >= (float)dest.height ? (float)dest.height - 1.0f : top;

	top = dest.height - top;
	bottom = dest.height - bottom;

	bool collision = false;

	// ������ ������ �ݺ��Ѵ�.
	for (int y = (int)top; y < (int)bottom; y++)
	{
		for (int x = (int)left; x < (int)right; x++)
		{
			int index = y * (int)dest.width * 4 + x * 4;

			// ���� �ε����� �ȼ��� ���� �ڽ��ȿ� �����ϴ����� �Ǻ��Ѵ�.
			// ���� �ȼ��� ������������� ��ġ�� �����ش�.
			Vector2 pixelWorldPos = LB + Vector2((float)x, (float)dest.height - (float)y);

			if (!CollisionCircleToPoint(srcResult, destResult, src, pixelWorldPos))
				continue;

			switch (dest.type)
			{
			case PixelCollision_Type::Color_Ignore:
				if (dest.pixels[index] == dest.colors[0] &&
					dest.pixels[index + 1] == dest.colors[1] &&
					dest.pixels[index + 2] == dest.colors[2])
					continue;

				collision = true;
				break;
			case PixelCollision_Type::Color_Confirm:
				if (dest.pixels[index] == dest.colors[0] &&
					dest.pixels[index + 1] == dest.colors[1] &&
					dest.pixels[index + 2] == dest.colors[2])
					collision = true;
				break;
			case PixelCollision_Type::Alpha_Ignore:
				if (dest.pixels[index + 3] == dest.colors[3])
					continue;

				collision = true;
				break;
			case PixelCollision_Type::Alpha_Confirm:
				if (dest.pixels[index + 3] == dest.colors[3])
					collision = true;
				break;
			}

			if (collision)
				break;
		}
	}

	return collision;
}


bool CCollision::CollisionPixelToPoint(sCollisionResult& srcResult, sCollisionResult& destResult, 
	const sPixelInfo& pixelInfo, const Vector2& point)
{
	if (!CollisionBox2DToPoint(srcResult, destResult, pixelInfo.box, point))
		return false;

	Vector2 LB = pixelInfo.box.center - pixelInfo.box.length;
	
	Vector2 pointToImagePos = point - LB;

	bool collision = false;

	int index = ((int)pixelInfo.height - (int)pointToImagePos.y) * (int)pixelInfo.width * 4 + (int)pointToImagePos.x * 4;

	switch (pixelInfo.type)
	{
	case PixelCollision_Type::Color_Ignore:
		if (pixelInfo.pixels[index] != pixelInfo.colors[0] ||
			pixelInfo.pixels[index + 1] != pixelInfo.colors[1] ||
			pixelInfo.pixels[index + 2] != pixelInfo.colors[2])
			collision = true;
		break;
	case PixelCollision_Type::Color_Confirm:
		if (pixelInfo.pixels[index] == pixelInfo.colors[0] ||
			pixelInfo.pixels[index + 1] == pixelInfo.colors[1] ||
			pixelInfo.pixels[index + 2] == pixelInfo.colors[2])
			collision = true;
		break;
	case PixelCollision_Type::Alpha_Ignore:
		if (pixelInfo.pixels[index + 3] != pixelInfo.colors[3])
			collision = true;
		break;
	case PixelCollision_Type::Alpha_Confirm:
		if (pixelInfo.pixels[index + 3] == pixelInfo.colors[3])
			collision = true;
		break;
	}

	return collision;
}