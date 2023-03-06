#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Mustafa Elfayoumi - mte1124@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the Memory For 46 Meshes
	int meshCount = 46;

	for (int i = 0; i < meshCount; i++)
	{
		// Add To The List Of Meshes Then Create A Black Cube
		MyMesh* current = new MyMesh();
		meshList.push_back(current);
		meshList[i]->GenerateCube(1.0f, C_BLACK);
	}
		
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	// Clock To Track Elapsed Time
	static float elapsed = 0.0f;
	static unsigned int clock = m_pSystem->GenClock();

	// Track Elapsed Time By Getting Change In The Clock From Last Call Of Update
	elapsed += static_cast<float>(m_pSystem->GetDeltaTime(clock));

	// Handle Translation In X Direction
	static vector3 motion = vector3(0, 0, 0);
	motion.x = elapsed;

	// Matrix Position Setting From Center And Apply Translation To X (Z @ -10 Just To Move It Away From Camera
	matrix4 posMatrix = glm::translate(vector3(-5.5f, -4.0f, -10.0f)) * glm::translate(motion);

	// Render Cubes From meshList Individually Column By Column
	// X Will Be 0-10, Y Will Be 0-7, Z Does Not Matter So It Is Kept At 0
	
	// Column 1
	meshList[0]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(0.0f, 1.0f, 0.0f)));
	meshList[1]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(0.0f, 2.0f, 0.0f)));
	meshList[2]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(0.0f, 3.0f, 0.0f)));

	// Column 2
	meshList[3]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(1.0f, 3.0f, 0.0f)));
	meshList[4]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(1.0f, 4.0f, 0.0f)));
	
	// Column 3
	meshList[5]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 1.0f, 0.0f)));
	meshList[6]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 2.0f, 0.0f)));
	meshList[7]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 3.0f, 0.0f)));
	meshList[8]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 4.0f, 0.0f)));
	meshList[9]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 5.0f, 0.0f)));
	meshList[10]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(2.0f, 7.0f, 0.0f)));
	
	// Column 4
	meshList[11]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(3.0f, 0.0f, 0.0f)));
	meshList[12]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(3.0f, 2.0f, 0.0f)));
	meshList[13]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(3.0f, 3.0f, 0.0f)));
	meshList[14]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(3.0f, 5.0f, 0.0f)));
	meshList[15]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(3.0f, 6.0f, 0.0f)));
	
	// Column 5
	meshList[16]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(4.0f, 0.0f, 0.0f)));
	meshList[17]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(4.0f, 2.0f, 0.0f)));
	meshList[18]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(4.0f, 3.0f, 0.0f)));
	meshList[19]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(4.0f, 4.0f, 0.0f)));
	meshList[20]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(4.0f, 5.0f, 0.0f)));
	
	// Column 6
	meshList[21]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(5.0f, 2.0f, 0.0f)));
	meshList[22]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(5.0f, 3.0f, 0.0f)));
	meshList[23]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(5.0f, 4.0f, 0.0f)));
	meshList[24]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(5.0f, 5.0f, 0.0f)));
	
	// Column 7
	meshList[25]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(6.0f, 0.0f, 0.0f)));
	meshList[26]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(6.0f, 2.0f, 0.0f)));
	meshList[27]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(6.0f, 3.0f, 0.0f)));
	meshList[28]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(6.0f, 4.0f, 0.0f)));
	meshList[29]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(6.0f, 5.0f, 0.0f)));
	
	// Column 8
	meshList[30]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(7.0f, 0.0f, 0.0f)));
	meshList[31]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(7.0f, 2.0f, 0.0f)));
	meshList[32]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(7.0f, 3.0f, 0.0f)));
	meshList[33]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(7.0f, 5.0f, 0.0f)));
	meshList[34]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(7.0f, 6.0f, 0.0f)));
	
	// Column 9
	meshList[35]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 1.0f, 0.0f)));
	meshList[36]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 2.0f, 0.0f)));
	meshList[37]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 3.0f, 0.0f)));
	meshList[38]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 4.0f, 0.0f)));
	meshList[39]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 5.0f, 0.0f)));
	meshList[40]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(8.0f, 7.0f, 0.0f)));
	
	// Column 10
	meshList[41]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(9.0f, 3.0f, 0.0f)));
	meshList[42]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(9.0f, 4.0f, 0.0f)));
	
	// Column 11
	meshList[43]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(10.0f, 1.0f, 0.0f)));
	meshList[44]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(10.0f, 2.0f, 0.0f)));
	meshList[45]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(10.0f, 3.0f, 0.0f)));

	// draw a skyboxmeshList[0]->Render(m4Projection, m4View, glm::translate(posMatrix, vector3(0.0f, 1.0f, 0.0f)));
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	std::vector<MyMesh*> meshList;
	uint size = meshList.size();
	for (uint i = 0; i < size; ++i)
	{
		if (meshList[i] != nullptr)
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	};

	//release GUI
	ShutdownGUI();
}