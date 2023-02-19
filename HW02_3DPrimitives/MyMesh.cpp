#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Tip Point Of Cone
	vector3 coneTip = ZERO_V3;
	coneTip.z += a_fHeight/2;

	// Center of Base
	vector3 baseCenter = ZERO_V3;
	baseCenter.z -= a_fHeight / 2;

	// Vector To Hold All Vertices in "Circle"
	std::vector<vector3> vertices;

	// Variables For Unit Circle
	GLfloat theta = 0; // Storing Additive Angle Change Each Iteration

	// Divide Full Circle (2pi) Equally Into Specific Number of Subdivisions
	GLfloat delta = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisions));

	// For Each Subdivision, Push A New Vertex, Using Unit Circle For Each New Position
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// (x,y) = (cos*rad, sin*rad)
		vector3 current = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, -(a_fHeight/2));

		// Push To Vector List Of Verticies
		vertices.push_back(current);

		// Increment Theta To Next Angle Around Circle
		theta += delta;
	}

	// Loop Through Newly Calculated Vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// Make Quads Around The Axis
		AddQuad(baseCenter, vertices[i], vertices[(i + 1) % a_nSubdivisions], coneTip);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Locations For Center Of Each Base
	vector3 bottomCenter = ZERO_V3;
	bottomCenter.z = -a_fHeight/2;

	vector3 topCenter = ZERO_V3;
	topCenter.z = a_fHeight/2;

	// Vector To Hold All Vertices in Each "Circle"
	std::vector<vector3> bottomVertices;
	std::vector<vector3> topVertices;

	// Variables For Unit Circle
	GLfloat theta = 0; // Storing Additive Angle Change Each Iteration

	// Divide Full Circle (2pi) Equally Into Specific Number of Subdivisions
	GLfloat delta = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisions));

	// For Each Subdivision, Push A New Vertex, Using Unit Circle For Each New Position
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// (x,y) = (cos*rad, sin*rad)
		vector3 currentBottom = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, bottomCenter.z);
		vector3 currentTop = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, topCenter.z);

		// Push To Vector List Of Vertices
		bottomVertices.push_back(currentBottom);
		topVertices.push_back(currentTop);

		// Increment Theta To Next Angle Around Circle
		theta += delta;
	}

	// Loop Through Newly Calculated Vertices for Bottom Circle
	for (int i = 0; i < bottomVertices.size(); i++)
	{
		// Form Triangles For Each From Base Center and Next Index (Looping Back To 0 At End)
		AddTri(bottomCenter, bottomVertices[(i + 1) % a_nSubdivisions], bottomVertices[i]);
	}

	// Loop Through Newly Calculated Vertices for Top Circle
	for (int i = 0; i < topVertices.size(); i++)
	{
		// Form Triangles For Each From Top Center and Next Index (Looping Back To 0 At End)
		AddTri(topCenter, topVertices[i], topVertices[(i + 1) % a_nSubdivisions]);
	}

	// Loop Through Each Subdivision To Connect Top and Bottom Vertices with Quads
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(bottomVertices[i], bottomVertices[(i + 1) % a_nSubdivisions], topVertices[i], topVertices[(i + 1) % a_nSubdivisions]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Locations For Center Of Each Base
	vector3 bottomCenter = ZERO_V3;
	bottomCenter.z = -a_fHeight / 2;

	vector3 topCenter = ZERO_V3;
	topCenter.z = a_fHeight / 2;

	// Vector To Hold All Vertices in Each "Circle"
	std::vector<vector3> bottomVerticesOuter;
	std::vector<vector3> topVerticesOuter;
	std::vector<vector3> bottomVerticesInner;
	std::vector<vector3> topVerticesInner;

	// Variables For Unit Circle
	GLfloat theta = 0; // Storing Additive Angle Change Each Iteration

	// Divide Full Circle (2pi) Equally Into Specific Number of Subdivisions
	GLfloat delta = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisions));

	// For Each Subdivision, Push A New Vertex, Using Unit Circle For Each New Position
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// (x,y) = (cos*rad, sin*rad)
		vector3 currentBottomOuter = vector3(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, bottomCenter.z);
		vector3 currentTopOuter = vector3(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, topCenter.z);
		vector3 currentBottomInner = vector3(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, bottomCenter.z);
		vector3 currentTopInner = vector3(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, topCenter.z);

		// Push To Vector List Of Vertices
		bottomVerticesOuter.push_back(currentBottomOuter);
		topVerticesOuter.push_back(currentTopOuter);
		bottomVerticesInner.push_back(currentBottomInner);
		topVerticesInner.push_back(currentTopInner);

		// Increment Theta To Next Angle Around Circle
		theta += delta;
	}

	// Loop Through Newly Calculated Vertices for Bottom Circle Base
	for (int i = 0; i < bottomVerticesOuter.size(); i++)
	{
		// Form Triangles For Each From Base Center and Next Index (Looping Back To 0 At End)
		AddQuad(bottomVerticesOuter[(i + 1) % a_nSubdivisions], bottomVerticesOuter[i], bottomVerticesInner[(i + 1) % a_nSubdivisions], bottomVerticesInner[i]);
	}

	// Loop Through Newly Calculated Vertices for Top Outer Circle
	for (int i = 0; i < topVerticesOuter.size(); i++)
	{
		// Form Triangles For Each From Base Center and Next Index (Looping Back To 0 At End)
		AddQuad(topVerticesInner[(i + 1) % a_nSubdivisions], topVerticesInner[i], topVerticesOuter[(i + 1) % a_nSubdivisions], topVerticesOuter[i]);
	}

	// Loop Through Each Subdivision To Connect Top and Bottom Vertices with Quads (Both Inner And Outer)
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Inner
		AddQuad(bottomVerticesInner[(i + 1) % a_nSubdivisions], bottomVerticesInner[i], topVerticesInner[(i + 1) % a_nSubdivisions], topVerticesInner[i]);

		// Outer
		AddQuad(topVerticesOuter[(i + 1) % a_nSubdivisions], topVerticesOuter[i], bottomVerticesOuter[(i + 1) % a_nSubdivisions], bottomVerticesOuter[i]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Variables For Unit Circle
	GLfloat thetaA = 0;
	GLfloat thetaB = 0;

	// Divide Full Circle (2pi) Equally Into Specific Number of Subdivisions
	GLfloat deltaA = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisionsA));
	GLfloat deltaB = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisionsB));

	// Get Radius of Tube
	GLfloat tubeRadius = a_fOuterRadius - a_fInnerRadius;

	// Loop Through All Subdivisions, Chunking by Each Of The Opposite Subdivisions
	// All B Subdivisions Between 2 A Subdivisions, Then Move To Same With Next 2 A Subdivisions
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		GLfloat nextThetaA = thetaA + deltaA; // Next Theta A Value Will Help Find One Of Four Points in the Quad

		for (int j = 0; j < a_nSubdivisionsB; j++)
		{
			GLfloat nextThetaB = thetaB + deltaB; // Next Theta B Value Will Help Find One Of Four Points in the Quad
		
			// Generate Points For The Current Quad To Be Added To Mesh
			// X Position - (tubeRadius * cos(thetaB)) + Outer Raidus is Distance, Multiply By cos(thetaA) to get X Coord
			float x = ((tubeRadius * std::cos(thetaB)) + a_fOuterRadius) * std::cos(thetaA);
			// Y Position - Same As X but with sin(thetaA) To Get Vertical Value for Y Pos
			float y = ((tubeRadius * std::cos(thetaB)) + a_fOuterRadius) * std::sin(thetaA);
			// Z Position - Simply use Inside Of Ring And Sin of the thetaB To Get Vertical Component
			float z = tubeRadius * std::sin(thetaB);

			// Put It All Together Into A Vertex
			vector3 vertex1 = vector3(x, y, z);

			// New X Y Z Values Using Alternating Future Theta Values For 3 More Points In Quad
			x = ((tubeRadius * std::cos(thetaB)) + a_fOuterRadius) * std::cos(nextThetaA);
			y = ((tubeRadius * std::cos(thetaB)) + a_fOuterRadius) * std::sin(nextThetaA);
			z = tubeRadius * std::sin(thetaB);

			// Put It All Together Into A Vertex
			vector3 vertex2 = vector3(x, y, z);

			// New X Y Z Values Using Alternating Future Theta Values For 2 More Points In Quad
			x = ((tubeRadius * std::cos(nextThetaB)) + a_fOuterRadius) * std::cos(thetaA);
			y = ((tubeRadius * std::cos(nextThetaB)) + a_fOuterRadius) * std::sin(thetaA);
			z = tubeRadius * std::sin(nextThetaB);

			// Put It All Together Into A Vertex
			vector3 vertex3 = vector3(x, y, z);

			// New X Y Z Values Using Alternating Future Theta Values For Last Point
			x = ((tubeRadius * std::cos(nextThetaB)) + a_fOuterRadius) * std::cos(nextThetaA);
			y = ((tubeRadius * std::cos(nextThetaB)) + a_fOuterRadius) * std::sin(nextThetaA);
			z = tubeRadius * std::sin(nextThetaB);

			// Put It All Together Into A Vertex
			vector3 vertex4 = vector3(x, y, z);

			// Add The Quad
			AddQuad(vertex1, vertex2, vertex3, vertex4);

			thetaB = nextThetaB;
		}
		thetaA = nextThetaA;
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();	

	// Vector To Hold All Vertices in "Circle"
	std::vector<vector3> vertices;

	// Variables For Unit Circle
	GLfloat theta = 0; // Storing Additive Angle Change Each Iteration

	// Divide Full Circle (2pi) Equally Into Specific Number of Subdivisions
	GLfloat delta = static_cast<GLfloat>((2.0f * PI) / static_cast<GLfloat>(a_nSubdivisions));

	// For Each Subdivision, Push A New Vertex, Using Unit Circle For Each New Position
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Theta Detrmines Horizontal Position While Delta (Only Cos In Z) Determines Vertical Position So:
		// X-pos = cos(theta) * sin(delta)
		// Y-pos = sin(theta) * sin(delta)
		vector3 current = vector3(cos(theta) * sin(delta), sin(theta) * sin(delta), cos(delta));

		// Push To Vector List Of Verticies
		vertices.push_back(current);

		// Increment Theta To Next Angle Around Circle
		theta += delta;
	}

	// Loop Through Newly Calculated Vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// Form Triangles For Each From World's 0,0,0 and Next Index (Looping Back To 0 At End)
		AddTri(ZERO_V3, vertices[i], vertices[(i + 1) % a_nSubdivisions]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}