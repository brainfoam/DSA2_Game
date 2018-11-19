#include "AppClass.h"
using namespace Simplex;

MyMesh* m_block;
float f_cameraSpeed = .1;
int i_gameTick;
std::vector<vector3> v_v3blockPositions;

void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Team \"Notorious Squad\" - Final Project";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCamera->SetPositionTargetAndUpward(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
			AXIS_Y);					//what is up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);

	v_v3blockPositions.push_back(m_pCamera->GetPosition() + vector3(0, 0, -20));
}
void Application::Update(void)
{
	i_gameTick++;

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	//CameraRotation();

	if (i_gameTick % 50 == 0)
	{
		float f_randomX = rand() % 10;
		vector3 position = vector3(f_randomX - 5, 0, 0);
		v_v3blockPositions.push_back(m_pCamera->GetPosition() + vector3(0, 0, -20) + position);

		if (v_v3blockPositions.size() > 4)
		{
			v_v3blockPositions.erase(v_v3blockPositions.begin());
		}
	}

	for (auto i = 0; i < v_v3blockPositions.size(); i++)
	{
		vector3 position = v_v3blockPositions[i];
		m_pMyMeshMngr->AddCubeToRenderList(glm::translate(position) * glm::scale(vector3(10, 1, 1)));
	}
	

	m_pCamera->MoveVertical(f_cameraSpeed);

}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
