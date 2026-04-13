//
// Chess board model
//

#ifndef _CHESS_BOARD_H
#define _CHESS_BOARD_H

#include "DirectX.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include <d3d11_1.h>
#include <SimpleMath.h>

using namespace DirectX;

class ChessBoard
{
public:

	ChessBoard();
	~ChessBoard();

	// called to initialize the object
	void Initialize(ID3D11Device* pDevice, LitColourShader* pLitShader, Matrix wMatrix, Color color1, Color color2);

	// called to draw the object
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix);

	// update the object
	void Update(float deltaTime);

	void SetTextures(ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* spec) {
		pDiffuse = diffuse;
		pSpec = spec;
	}

	Matrix GetBoardPosition(int x, int y, float pieceBaseOffset);

private:

	ID3D11Buffer* MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower);

	ID3D11ShaderResourceView* pDiffuse;
	ID3D11ShaderResourceView* pSpec;

	// store a copy of the shader
	LitColourShader* pShader;

	const static int BOARDHEIGHT_MODIFIER = 1;
	const static int X_LENGTH = 8;
	const static int Y_LENGTH = 8;
	float gridScale;

	IndexedPrimitive chessGrid[X_LENGTH][Y_LENGTH];
	Matrix chessGridMatrix[X_LENGTH][Y_LENGTH];

	Color gridColor1;
	Color gridColor2;

	Matrix worldMatrix;

	ID3D11Buffer* pBoardBuffer;

};

#endif

