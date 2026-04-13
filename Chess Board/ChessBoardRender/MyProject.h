#ifndef _MyProject_h
#define _MyProject_h

#include "DirectX.h"
#include "Font.h"
#include "TextureType.h"
#include "Sprite.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include "ChessBoard.h"
#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "SkyBox.h"

// forward declare the sprite batch

using namespace DirectX;
using namespace DirectX::SimpleMath;


//----------------------------------------------------------------------------------------------
// Main project class
//	Inherits the directx class to help us initalize directX
//----------------------------------------------------------------------------------------------

class MyProject : public DirectXClass
{
public:

	enum ScreenState
	{
		Menu,
		Game
	};

	// constructor
	MyProject(HINSTANCE hInstance);
	~MyProject();

	// Initialize any directX objects
	void InitializeObjects();

	// window message handler
	LRESULT ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);

	// Called by the render loop to render a single frame
	void Render(void);

	// Called by directX framework to allow you to update any scene objects
	void Update(float deltaTime);

	// Compute the view and projection matrices
	void ComputeViewProjection();

private:

	ScreenState curState;

	LitColourShader shader;
	SkyBox skyBox;

	float runTime;

	static const int PAWN_NUM = 8;

	ChessBoard chessBoard;

	King whiteKing;
	Queen whiteQueen;
	Bishop whiteBishop1;
	Bishop whiteBishop2;
	Knight whiteKnight1;
	Knight whiteKnight2;
	Rook whiteRook1;
	Rook whiteRook2;
	Pawn whitePawns[PAWN_NUM];

	King blackKing;
	Queen blackQueen;
	Bishop blackBishop1;
	Bishop blackBishop2;
	Knight blackKnight1;
	Knight blackKnight2;
	Rook blackRook1;
	Rook blackRook2;
	Pawn blackPawns[PAWN_NUM];

	Sprite startButton;
	Sprite quitButton;
	SpriteBatch* spriteBatch;

	Matrix startMatrix;
	Matrix endMatrix;

	TextureType diffuseTex;
	TextureType specTex;
	TextureType whiteCeramic;
	TextureType beigeCeramic;
	TextureType buttonTex;

	FontType buttonFont;

	Matrix viewMatrix;
	Matrix projectionMatrix;

	// mouse variables
	Vector2 mousePos;
	Vector2 mouseDelta;
	bool buttonDown;

	// for camera controls
	Vector3 cameraPos;
	Vector2 cameraRotationSpeed;
	Vector2 cameraRotation;
	float cameraRadius;
	float cameraRadiusSpeed;

	void UpdateCamera(float deltaTime);

	// call when the mouse is released
	void OnMouseDown();
	void OnMouseMove();

};

#endif