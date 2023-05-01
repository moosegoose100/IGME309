#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
	Init();//Init the default values
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	//create a rigid body that encloses all the objects in this octant, it necessary you will need
	//to subdivide the octant based on how many objects are in it already an how many you IDEALLY
	//want in it, remember each subdivision will create 8 children for this octant but not all children
	//of those children will have children of their own

	// Stores Minimums and Maximums of Rigid Body Around All Blocks
	std::vector<vector3> lMinMax;

	// Loop To Populate Mins and Maxs
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		// Get The Rigid Body For The Entity At This Index
		RigidBody* tempBody = m_pEntityMngr->GetEntity(i)->GetRigidBody();

		// Push Min and Max Values
		lMinMax.push_back(tempBody->GetMinGlobal());
		lMinMax.push_back(tempBody->GetMaxGlobal());
	}

	// Create The Main Rigid Body
	RigidBody* pRigidBody = new RigidBody(lMinMax);

	float fMax = pRigidBody->GetHalfWidth().x;

	//The following will set up the values of the octant, make sure the are right, the rigid body at start
	//is NOT fine, it has made-up values
	m_fSize = pRigidBody->GetHalfWidth().x * 2.0f;
	m_v3Center = pRigidBody->GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody->GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody->GetHalfWidth();

	SafeDelete(pRigidBody);

	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	//If the index given is larger than the number of elements in the bounding object there is no collision
	if (a_uRBIndex >= m_pEntityMngr->GetEntityCount())
	{
		return false;
	}
	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)
	RigidBody* entityBody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 minimums = entityBody->GetMinGlobal();
	vector3 maximums = entityBody->GetMaxGlobal();

	// Check If Can Possibly Overlap In Specific Directions
	if (minimums.x > m_v3Max.x || maximums.x < m_v3Min.x || // X-Direction
		minimums.y > m_v3Max.y || maximums.y < m_v3Min.y || // Y-Direction
		minimums.z > m_v3Max.z || maximums.z < m_v3Min.z) // Z-Direction
	{
		return false;
	}

	return true; // for the sake of startup code
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant
	// Check If The ID Matches The Given Index And Display If It Does
	if (a_nIndex == m_uID)
	{
		// Copied Given From Other Display Method
		m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color);

		// Dont Recursively Call if Already Drawn
		return;
	}

	// Call Recursively Until Index Matches
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex, a_v3Color);
	}
}
void Octant::Display(vector3 a_v3Color)
{
	//Recursively Call For All Children All The Way Until The Leafs
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	//Subdivide the space and allocate 8 children
	//Set Children To 8 So That The Node Won't Subdivide Again AND Can Keep Track Of Children
	m_uChildren = 8;

	// Variable To Change Center For Each Child Without Changing Parent Center
	vector3 octCenter = m_v3Center;

	// Go Through And Create Children Individually

	// Child 0 - Top, Right, Front (+X, +Y, +Z)
	octCenter.x += m_fSize / 4;
	octCenter.y += m_fSize / 4;
	octCenter.z += m_fSize / 4;
	m_pChild[0] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 1 - Bottom, Right, Front (+X, -Y, +Z)
	octCenter.x += m_fSize / 4;
	octCenter.y -= m_fSize / 4;
	octCenter.z += m_fSize / 4;
	m_pChild[1] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 2 - Bottom, Left, Front (-X, -Y, +Z)
	octCenter.x -= m_fSize / 4;
	octCenter.y -= m_fSize / 4;
	octCenter.z += m_fSize / 4;
	m_pChild[2] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 3 - Top, Left, Front (-X, +Y, +Z)
	octCenter.x -= m_fSize / 4;
	octCenter.y += m_fSize / 4;
	octCenter.z += m_fSize / 4;
	m_pChild[3] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 4 - Top, Right, Back (+X, +Y, -Z)
	octCenter.x += m_fSize / 4;
	octCenter.y += m_fSize / 4;
	octCenter.z -= m_fSize / 4;
	m_pChild[4] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 5 - Bottom, Right, Back (+X, -Y, -Z)
	octCenter.x += m_fSize / 4;
	octCenter.y -= m_fSize / 4;
	octCenter.z -= m_fSize / 4;
	m_pChild[5] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 6 - Bottom, Left, Back (-X, -Y, -Z)
	octCenter.x -= m_fSize / 4;
	octCenter.y -= m_fSize / 4;
	octCenter.z -= m_fSize / 4;
	m_pChild[6] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	octCenter = m_v3Center; // Reset Center For Next Calculation

	// Child 7 - Top, Left, Back (-X, +Y, -Z)
	octCenter.x -= m_fSize / 4;
	octCenter.y += m_fSize / 4;
	octCenter.z -= m_fSize / 4;
	m_pChild[7] = new Octant(octCenter, (m_fSize / 2)); // New Child Has Half The Width Of Parent

	// Loop Through And Assign Variables For Each Octant
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		/*
		// Automatically Subdivide Again If Ideal Amount Is Exceeded
		if (m_pChild[i]->ContainsAtLeast(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
		*/
	}

}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	// You need to check how many entity objects live within this octant
	if (m_EntityList.size() >= a_nEntities)
	{
		return true;
	}
	return false;
}
void Octant::AssignIDtoEntity(void)
{
	//Recursive method
	//Have to traverse the tree and make sure to tell the entity manager
	
	// Traverse Tree By Recursively Calling Method Until No Children Are Left
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	// Check If This Child Is A Leaf (Last Node/No Children)
	if (m_uChildren == 0)
	{
		// Any Entities Colliding With This Leaf Should Be Assigned An ID
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i))
			{
				// Assign ID For This Specific Octant
				m_pEntityMngr->AddDimension(0, m_uID);

				// Add The Entity To The List For This Specific Leaf
				m_EntityList.push_back(i);
			}
		}
	}	
}
//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
