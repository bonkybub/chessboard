#include "MyProject.h"
#include <Windowsx.h> // for GET__LPARAM macros
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <sstream>
#include <CommonStates.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

// helper function
//
// returns a random float between 0 & 1
float RandFloat() { return float(rand())/float(RAND_MAX); } 

static const float CAMERA_SPEED = XM_PI * 0.2f;

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	MyProject application(hInstance);    // Create the class variable

	if( application.InitWindowsApp(L"DirectX 3D", nShowCmd) == false )    // Initialize the window, if all is well show and update it so it displays
	{
		return 0;                   // Error creating the window, terminate application
	}

	if( application.InitializeDirect3D() )
	{
		application.SetDepthStencil(true);      // Tell DirectX class to create and maintain a depth stencil buffer
		application.InitializeObjects();
		application.MessageLoop();				// Window has been successfully created, start the application message loop
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------
MyProject::MyProject(HINSTANCE hInstance)
	: DirectXClass(hInstance)
{
	mousePos = Vector2(clientWidth * 0.5f, clientHeight * 0.5f);
	buttonDown = false;
	
	ClearColor = Color(DirectX::Colors::Black.v);

	cameraPos = Vector3(0, 0, 6);

	cameraRadius = 32;
	cameraRadiusSpeed = 0;

	cameraRotation = Vector2(0, XM_PI / 10.0f );
	cameraRotationSpeed = Vector2::Zero;

	spriteBatch = NULL;

	runTime = 0;
}

//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
MyProject::~MyProject()
{
}

//----------------------------------------------------------------------------------------------
// Initialize any fonts we need to use here
//----------------------------------------------------------------------------------------------
void MyProject::InitializeObjects()
{
	spriteBatch = new SpriteBatch(DeviceContext);
	
	whiteCeramic.Load(D3DDevice, DeviceContext, L"..\\Textures\\White_1.jpg");
	beigeCeramic.Load(D3DDevice, DeviceContext, L"..\\Textures\\Beige_1.jpg");
	buttonFont.InitializeFont(D3DDevice, DeviceContext, L"..\\Font\\Verdana24.spriteFont");
	skyBox.Initialize(D3DDevice, DeviceContext, L"..\\Textures\\envMap.dds", 64 );

	// load the shader
	shader.LoadShader(D3DDevice);
	shader.SetAmbientLight(Colors::White.v);
	shader.SetDirectionalLight(Colors::White.v, Vector3(0.0f, -1.0f, 0.0f));

	// load our demo object

	shader.SetAmbientLight(Colors::White.v);
	chessBoard.Initialize(D3DDevice, &shader, Matrix::CreateScale(1.0f, 0.5f, 1.0f), Colors::White.v, Colors::Black.v);

	whiteKing.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.8f);
	whiteQueen.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.8f);
	whiteBishop1.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	whiteBishop2.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	whiteKnight1.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	whiteKnight2.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	whiteRook1.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	whiteRook2.Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.6f);
	
	blackKing.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.8f);
	blackQueen.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.8f);
	blackBishop1.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	blackBishop2.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	blackKnight1.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	blackKnight2.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	blackRook1.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	blackRook2.Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.6f);
	
	for (int i = 0; i < PAWN_NUM; i++)
	{
		whitePawns[i].Initialize(D3DDevice, DeviceContext, &shader, true, &whiteCeramic, &beigeCeramic, 3.4f);
		blackPawns[i].Initialize(D3DDevice, DeviceContext, &shader, false, &whiteCeramic, &beigeCeramic, 3.4f);
	}

	// load the textures
	diffuseTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\marble8.jpg");
	specTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\marbleSpec.jpg");
	buttonTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\Button.png");

	// set on the pawn
	chessBoard.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	
	whiteKing.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteQueen.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteBishop1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteBishop2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteKnight1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteKnight2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteRook1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	whiteRook2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	
	blackKing.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackQueen.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackBishop1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackBishop2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackKnight1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackKnight2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackRook1.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	blackRook2.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	
	for (int i = 0; i < PAWN_NUM; i++)
	{
		whitePawns[i].SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
		blackPawns[i].SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());
	}

	// buttons
	startButton.Initialize(&buttonTex, Vector2(clientWidth * 0.5f, clientHeight * 0.35f), 0, 0.3f, Colors::White.v, 1.0f);
	quitButton.Initialize(&buttonTex, Vector2(clientWidth * 0.5f, clientHeight * 0.65f), 0, 0.3f, Colors::White.v, 1.0f);

	curState = Menu;

	// set the matrices
	startMatrix =Matrix::CreateRotationZ(45.0f *XM_PI / 180.0f) * Matrix::CreateTranslation(-12.0f, 0, 0);
	endMatrix = Matrix::CreateRotationZ(-45.0f * XM_PI / 180.0f) * Matrix::CreateTranslation(12.0f, 0, 0);

}

//----------------------------------------------------------------------------------------------
// Window message handler
//----------------------------------------------------------------------------------------------
LRESULT MyProject::ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// for mouse positions
	Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));

	switch (msg )
	{
	case WM_MOUSEMOVE:
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseMove();
		return 0;
	case WM_LBUTTONUP:
		buttonDown = false;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseDown();
		break;
	case WM_KEYUP:
		if (wParam >= '0' && wParam <= '4')
		{
			PresentInterval = wParam - '0';
		}
		else if (wParam == VK_UP)	{	cameraRotationSpeed.y = 0;	}
		else if (wParam == VK_DOWN) {	cameraRotationSpeed.y = 0; }
		else if (wParam == VK_LEFT) {	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_RIGHT){	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_ADD)  {	cameraRadiusSpeed = 0; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 0; }
		else if (wParam == VK_SPACE)
		{
			cameraRotation = Vector2::Zero;
			cameraRadius   = 6;
		}

		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)	{ cameraRotationSpeed.y = CAMERA_SPEED; }
		else if (wParam == VK_DOWN) { cameraRotationSpeed.y = -CAMERA_SPEED; }
		else if (wParam == VK_LEFT) { cameraRotationSpeed.x = -CAMERA_SPEED; }
		else if (wParam == VK_RIGHT){ cameraRotationSpeed.x = CAMERA_SPEED; }
		else if (wParam == VK_ADD)  { cameraRadiusSpeed = -1.0f; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 1.0f; }
		break;

	}

	// let the base class handle remaining messages
	return DirectXClass::ProcessWindowMessages(msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
// Called by the render loop to render a single frame
//----------------------------------------------------------------------------------------------
void MyProject::Render(void)
{
	// calculate camera matrices
	ComputeViewProjection();

	// draw the skybox FIRST
	skyBox.Draw(DeviceContext, viewMatrix, projectionMatrix);

	// draw our object
	switch (curState)
	{
	case Menu:
		buttonFont.PrintMessage(startButton.GetPosition().x, startButton.GetPosition().y, L"Start Game", Colors::Black.v);
		buttonFont.PrintMessage(quitButton.GetPosition().x, quitButton.GetPosition().y, L"Quit Game", Colors::Black.v);
		break;
	case Game:
		chessBoard.Draw(DeviceContext, Matrix::Identity, viewMatrix, projectionMatrix);

		whiteKing.Draw(DeviceContext, Matrix::CreateScale(1.5f) * chessBoard.GetBoardPosition(3, 7, whiteKing.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteQueen.Draw(DeviceContext, Matrix::CreateScale(1.5f) * chessBoard.GetBoardPosition(4, 7, whiteQueen.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteBishop1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(2, 7, whiteBishop1.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteBishop2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(5, 7, whiteBishop2.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteKnight1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(1, 7, whiteKnight1.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteKnight2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(6, 7, whiteKnight2.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteRook1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(0, 7, whiteRook1.GetBaseOffset()), viewMatrix, projectionMatrix);
		whiteRook2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * chessBoard.GetBoardPosition(7, 7, whiteRook2.GetBaseOffset()), viewMatrix, projectionMatrix);

		blackKing.Draw(DeviceContext, Matrix::CreateScale(1.5f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(3, 0, blackKing.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackQueen.Draw(DeviceContext, Matrix::CreateScale(1.5f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(4, 0, blackQueen.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackBishop1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(2, 0, blackBishop1.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackBishop2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(5, 0, blackBishop2.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackKnight1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(1, 0, blackKnight1.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackKnight2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(6, 0, blackKnight2.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackRook1.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(0, 0, blackRook1.GetBaseOffset()), viewMatrix, projectionMatrix);
		blackRook2.Draw(DeviceContext, Matrix::CreateScale(1.4f) * Matrix::CreateRotationY(XM_PI) * chessBoard.GetBoardPosition(7, 0, blackRook2.GetBaseOffset()), viewMatrix, projectionMatrix);

		for (int i = 0; i < PAWN_NUM; i++)
		{
			whitePawns[i].Draw(DeviceContext, Matrix::CreateScale(1.3f) * chessBoard.GetBoardPosition(i, 6, whitePawns[i].GetBaseOffset()), viewMatrix, projectionMatrix);
			blackPawns[i].Draw(DeviceContext, Matrix::CreateScale(1.3f) * chessBoard.GetBoardPosition(PAWN_NUM - i - 1, 1, blackPawns[i].GetBaseOffset()), viewMatrix, projectionMatrix);
		}
		break;
	}

	// buttons
	spriteBatch->Begin();
	
	switch (curState)
	{
	case Menu:
		startButton.Draw(spriteBatch);
	
		if (startButton.ContainsPoint(mousePos))
		{
			startButton.SetScale(0.32f);
			startButton.SetColor(Colors::LightGray.v);
		}
		else
		{
			startButton.SetScale(0.3f);
			startButton.SetColor(Colors::White.v);
		}
	
		if (quitButton.ContainsPoint(mousePos))
		{
			quitButton.SetScale(0.32f);
			quitButton.SetColor(Colors::LightGray.v);
		}
		else
		{
			quitButton.SetScale(0.3f);
			quitButton.SetColor(Colors::White.v);
		}
		break;
	case Game:
		if (quitButton.ContainsPoint(mousePos))
		{
			quitButton.SetScale(0.16f);
			quitButton.SetColor(Colors::LightGray.v);
		}
		else
		{
			quitButton.SetScale(0.15f);
			quitButton.SetColor(Colors::White.v);
		}
		break;
	}
	
	quitButton.Draw(spriteBatch);
	
	spriteBatch->End();

	switch (curState)
	{
	case Menu:
		buttonFont.PrintMessage(startButton.GetPosition().x - startButton.GetExtents().x * 0.4f, startButton.GetPosition().y - startButton.GetExtents().y * 0.3f, L"Start Game", Colors::Black.v);
		buttonFont.PrintMessage(quitButton.GetPosition().x - quitButton.GetExtents().x * 0.4f, quitButton.GetPosition().y - quitButton.GetExtents().y * 0.3f, L"Quit Game", Colors::Black.v);
		break;
	case Game:
		buttonFont.PrintMessage(quitButton.GetPosition().x - quitButton.GetExtents().x * 0.8f, quitButton.GetPosition().y - quitButton.GetExtents().y * 0.3f, L"Quit Game", Colors::Black.v);
		buttonFont.PrintMessage(clientWidth * 0.1f, clientHeight * 0.1f, L"Move: 0", Colors::White.v);
		break;
	}

	// render the base class
	DirectXClass::Render();
}

//----------------------------------------------------------------------------------------------
// Given a running value, returns a value between 0 and 1 which 'ping pongs' back and forth
//----------------------------------------------------------------------------------------------
float PingPong(float value)
{	
	value = fabs(value);
	int wholeNumber = (int)value;	// integer component
	float decimal = value - (float)wholeNumber; // decimal parts

	// if it's odd, flip it 
	if (wholeNumber % 2 == 1)
		decimal = 1.0f - decimal;

	return decimal;
}

//----------------------------------------------------------------------------------------------
// Linear Interpolate Matrices, decomposing if possible to give nondistorting results
//----------------------------------------------------------------------------------------------
Matrix LerpMatrices(Matrix& start, Matrix& end, float t)
{
	Vector3 scaleA, scaleB;
	Quaternion rotA, rotB;
	Vector3 transA, transB;

	Matrix result;
	if (start.Decompose(scaleA, rotA, transA) && end.Decompose(scaleB, rotB, transB))
	{
		Vector3 scale = Vector3::Lerp(scaleA, scaleB, t);
		Quaternion rot = Quaternion::Lerp(rotA, rotB, t);
		Vector3 trans = Vector3::Lerp(transA, transB, t);
		result = Matrix::CreateScale(scale)*Matrix::CreateFromQuaternion(rot)*Matrix::CreateTranslation(trans);
	}
	else
	{
		result = Matrix::Lerp(start, end, t);
	}
	return result;
}

//----------------------------------------------------------------------------------------------
// Called every frame to update objects.
//	deltaTime: how much time in seconds has elapsed since the last frame
//----------------------------------------------------------------------------------------------
void MyProject::Update(float deltaTime)
{
	// how long one movement takes
	const float MOVEMENT_TIME_IN_SECONDS = 3.0f; 
	runTime += deltaTime;

	// get a 0 - 1 value
	float blendValue = PingPong(runTime / MOVEMENT_TIME_IN_SECONDS);

	// update the camera movement
	UpdateCamera(deltaTime);

	// update the sample object
	chessBoard.Update(deltaTime);

	whiteKing.Update(deltaTime);
	whiteQueen.Update(deltaTime);
	whiteBishop1.Update(deltaTime);
	whiteBishop2.Update(deltaTime);
	whiteKnight1.Update(deltaTime);
	whiteKnight2.Update(deltaTime);
	whiteRook1.Update(deltaTime);
	whiteRook2.Update(deltaTime);

	blackKing.Update(deltaTime);
	blackQueen.Update(deltaTime);
	blackBishop1.Update(deltaTime);
	blackBishop2.Update(deltaTime);
	blackKnight1.Update(deltaTime);
	blackKnight2.Update(deltaTime);
	blackRook1.Update(deltaTime);
	blackRook2.Update(deltaTime);

	for (int i = 0; i < PAWN_NUM; i++)
	{
		whitePawns[i].Update(deltaTime);
		blackPawns[i].Update(deltaTime);
	}
}


//----------------------------------------------------------------------------------------------
// Called when the mouse is released
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseDown()
{

	// this is called when the left mouse button is clicked
	// mouse position is stored in mousePos variable

	switch (curState)
	{
	case Menu:
		if (startButton.ContainsPoint(mousePos))
		{
			curState = Game;
			quitButton.SetPosition(Vector2(clientWidth * 0.15f, clientHeight * 0.9f));
			quitButton.SetScale(0.15f);
		}
		if (quitButton.ContainsPoint(mousePos))
		{
			exit(EXIT_SUCCESS);
		}
		break;
	case Game:
		if (quitButton.ContainsPoint(mousePos))
		{
			curState = Menu;
			quitButton.SetPosition(Vector2(clientWidth * 0.5f, clientHeight * 0.65f));
			quitButton.SetScale(0.3f);
		}
		break;
	}
}

//----------------------------------------------------------------------------------------------
// Called when the mouse is moved
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseMove()
{
}

//----------------------------------------------------------------------------------------------
// Computes the view and camera matrix
//----------------------------------------------------------------------------------------------
void MyProject::ComputeViewProjection()
{
	viewMatrix = Matrix::CreateLookAt(cameraPos, Vector3::Zero, Vector3::UnitY);
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(60.0f * XM_PI / 180.0f, (float)clientWidth / (float)clientHeight, 1, 128);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void MyProject::UpdateCamera(float deltaTime)
{
	const float VERT_LIMIT = XM_PI * 0.35f;

	// update the radius
	cameraRadius += cameraRadiusSpeed * deltaTime;
	if (cameraRadius < 1) cameraRadius = 1;


	// update the rotation amounts
	cameraRotation += cameraRotationSpeed * deltaTime;

	// clamp the vertical rotation
	if (cameraRotation.y < -VERT_LIMIT) cameraRotation.y = -VERT_LIMIT;
	else if (cameraRotation.y > VERT_LIMIT) cameraRotation.y = VERT_LIMIT;

	// calculate the height
	cameraPos.y = cameraRadius * sinf(cameraRotation.y);
	float r = cameraRadius * cosf(cameraRotation.y);

	// calculate the orbit
	cameraPos.x = sinf(cameraRotation.x) * r;
	cameraPos.z = cosf(cameraRotation.x) * r;
}