#include "MyRigidBody.h"
using namespace BTX;
//Allocation
uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	//TODO: Calculate the SAT algorithm I STRONGLY suggest you use the
	//Real Time Collision detection algorithm for OBB here but feel free to
	//implement your own solution.
	
	// Declare Variables To Hold Projection Of OBBs Onto Seperating Axes
	float ra, rb;

	// Rotation Matrices
	glm::mat3 rotationMat; // Matrix That Transforms a_pOther Into Same Coordinate Frame
	glm::mat3 absRotationMat; // Same But With Absolute Values To Make Make Calculations Easier

	// Translation Vector From This to Other
	vector3 trans = a_pOther->GetCenterGlobal() - GetCenterGlobal();
	trans = vector3(glm::dot(trans, vector3(m_m4ToWorld[0].x, m_m4ToWorld[0].y, m_m4ToWorld[0].z)),
		glm::dot(trans, vector3(m_m4ToWorld[1].x, m_m4ToWorld[1].y, m_m4ToWorld[1].z)),
		glm::dot(trans, vector3(m_m4ToWorld[2].x, m_m4ToWorld[2].y, m_m4ToWorld[2].z)));

	// Compute Rotation Matrix To Express Other In This Coordinate Frame
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMat[i][j] = glm::dot(vector3(m_m4ToWorld[i].x, m_m4ToWorld[i].y, m_m4ToWorld[i].z), vector3(a_pOther->m_m4ToWorld[j].x, a_pOther->m_m4ToWorld[j].y, a_pOther->m_m4ToWorld[j].z));
		}
	}

	// Absolute Value Matrix
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			absRotationMat[i][j] = abs(rotationMat[i][j]) + DBL_EPSILON;
		}
	}

	// Test The Three Axes Of Each Object
	// This Object A0, A1, A2
	for (int i = 0; i < 3; i++)
	{
		ra = GetCenterGlobal()[i];
		rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[i][0] +
			a_pOther->GetCenterGlobal()[1] * absRotationMat[i][1] +
			a_pOther->GetCenterGlobal()[2] * absRotationMat[i][2];

		if (abs(trans[i]) < ra + rb)
		{
			return i + 1;
		}
	}

	//a_pOther Axes B0, B1, B2
	for (int i = 0; i < 3; i++)
	{
		rb = a_pOther->GetCenterGlobal()[i];
		rb = GetCenterGlobal()[0] * absRotationMat[i][0] +
			GetCenterGlobal()[1] * absRotationMat[i][1] +
			GetCenterGlobal()[2] * absRotationMat[i][2];

		if (abs(trans[0] * rotationMat[0][i] +
			trans[1] * rotationMat[1][i] +
			trans[2] * rotationMat[2][i]) < ra + rb)
		{
			return i + 4;
		}
	}

	// Test Cross Products
	// A0 x B0
	ra = GetCenterGlobal()[1] * absRotationMat[2][0] + GetCenterGlobal()[2] * absRotationMat[1][0];
	rb = a_pOther->GetCenterGlobal()[1] * absRotationMat[0][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[0][1];
	
	if (abs(trans[2] * rotationMat[1][0] - trans[1] * rotationMat[2][0]) < ra + rb)
	{
		return 8;
	}

	// A0 x B1
	ra = GetCenterGlobal()[1] * absRotationMat[2][1] + GetCenterGlobal()[2] * absRotationMat[1][1];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[0][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[0][0];

	if (abs(trans[2] * rotationMat[1][1] - trans[1] * rotationMat[2][1]) < ra + rb)
	{
		return 9;
	}

	// A0 x B2
	ra = GetCenterGlobal()[1] * absRotationMat[2][2] + GetCenterGlobal()[2] * absRotationMat[1][2];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[0][1] + a_pOther->GetCenterGlobal()[1] * absRotationMat[0][0];

	if (abs(trans[2] * rotationMat[1][2] - trans[1] * rotationMat[2][2]) < ra + rb)
	{
		return 10;
	}

	// A1 x B0
	ra = GetCenterGlobal()[0] * absRotationMat[2][0] + GetCenterGlobal()[2] * absRotationMat[0][0];
	rb = a_pOther->GetCenterGlobal()[1] * absRotationMat[1][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[1][1];

	if (abs(trans[0] * rotationMat[2][0] - trans[2] * rotationMat[0][0]) < ra + rb)
	{
		return 11;
	}

	// A1 x B1
	ra = GetCenterGlobal()[0] * absRotationMat[2][1] + GetCenterGlobal()[2] * absRotationMat[0][1];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[1][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[1][0];

	if (abs(trans[0] * rotationMat[2][1] - trans[2] * rotationMat[0][1]) < ra + rb)
	{
		return 12;
	}

	// A1 x B2
	ra = GetCenterGlobal()[0] * absRotationMat[2][2] + GetCenterGlobal()[2] * absRotationMat[0][2];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[1][1] + a_pOther->GetCenterGlobal()[1] * absRotationMat[1][0];

	if (abs(trans[0] * rotationMat[2][2] - trans[2] * rotationMat[0][2]) < ra + rb)
	{
		return 13;
	}

	// A2 x B0
	ra = GetCenterGlobal()[0] * absRotationMat[1][0] + GetCenterGlobal()[1] * absRotationMat[0][0];
	rb = a_pOther->GetCenterGlobal()[1] * absRotationMat[2][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[2][1];

	if (abs(trans[1] * rotationMat[0][1] - trans[0] * rotationMat[1][1]) < ra + rb)
	{
		return 14;
	}

	// A2 x B1
	ra = GetCenterGlobal()[0] * absRotationMat[1][1] + GetCenterGlobal()[1] * absRotationMat[0][1];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[2][2] + a_pOther->GetCenterGlobal()[2] * absRotationMat[2][0];

	if (abs(trans[1] * rotationMat[0][1] - trans[0] * rotationMat[1][1]) < ra + rb)
	{
		return 15;
	}

	// A2 x B2
	ra = GetCenterGlobal()[0] * absRotationMat[1][2] + GetCenterGlobal()[1] * absRotationMat[0][2];
	rb = a_pOther->GetCenterGlobal()[0] * absRotationMat[2][1] + a_pOther->GetCenterGlobal()[1] * absRotationMat[2][0];

	if (abs(trans[1] * rotationMat[0][2] - trans[0] * rotationMat[1][2]) < ra + rb)
	{
		return 16;
	}

	// No Planes Found
	return BTXs::eSATResults::SAT_NONE;
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding
	bool bColliding = true;
	/*
	* We use Bounding Spheres or ARBB as a pre-test to avoid expensive calculations (SAT)
	* we default bColliding to true here to always fall in the need of calculating
	* SAT for the sake of the assignment.
	*/
	if (bColliding) //they are colliding with bounding sphere
	{
		uint nResult = SAT(a_pOther);

		if (nResult != 0) //The SAT shown they are colliding
		{
			this->AddCollisionWith(a_pOther);
			a_pOther->AddCollisionWith(this);
		}
		else //they are not colliding
		{
			this->RemoveCollisionWith(a_pOther);
			a_pOther->RemoveCollisionWith(this);
		}
	}
	else //they are not colliding with bounding sphere
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}
	return bColliding;
}
void MyRigidBody::Init(void)
{
	m_pModelMngr = ModelManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pModelMngr, a_pOther.m_pModelMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pModelMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pModelMngr = a_pOther.m_pModelMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };

//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}