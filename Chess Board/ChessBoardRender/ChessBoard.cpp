//
// Chess board model
//

#include "ChessBoard.h"

struct MaterialBuffer
{
	Color ambient;
	Color diffuse;
	Color spec; // alpha in w component

};

// called to initialize the object
void ChessBoard::Initialize(ID3D11Device* pDevice, LitColourShader* pLitShader, Matrix wMatrix, Color color1, Color color2)
{
	pShader = pLitShader;

	worldMatrix = wMatrix;

	gridColor1 = color1;
	gridColor2 = color2;

	gridScale = 4;

	float xOffset = X_LENGTH * 0.5f - 0.5f;
	float yOffset = Y_LENGTH * 0.5f - 0.5f;

	for (int x = 0; x < X_LENGTH; x++)
	{
		for (int y = 0; y < Y_LENGTH; y++)
		{
			chessGrid[x][y].InitializeGeometry(pDevice, Cube);
			chessGrid[x][y].InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());
			chessGridMatrix[x][y] = Matrix::CreateScale(gridScale) * Matrix::CreateTranslation((x - xOffset) * gridScale, -1, (y - yOffset) * gridScale);
			pBoardBuffer = MakeMaterialBuffer(pDevice, Colors::White.v, Color(0.2f, 0.2f, 0.2f), Color(0.2f, 0.2f, 0.2f), 2);
		}
	}

}

// called to draw the object
// The parent matrix allows the user to pass in a matrix to transform the entire piece
void ChessBoard::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	// set all 3 to the diffuse
	pDeviceContext->PSSetShaderResources(0, 1, &pDiffuse);
	//pDeviceContext->PSSetShaderResources(2, 1, &pSpec);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBoardBuffer);
	
	for (int x = 0; x < X_LENGTH; x++)
	{
		for (int y = 0; y < Y_LENGTH; y++)
		{
			if ((x + y) % 2 == 0)
			{
				pShader->SetAmbientLight(gridColor1);
			}
			else
			{
				pShader->SetAmbientLight(gridColor2);
			}
			
			pShader->SetShaders(pDeviceContext, chessGridMatrix[x][y] * worldMatrix, viewMatrix, projMatrix);
			chessGrid[x][y].Draw(pDeviceContext);
		}
	}
}

// update the object
void ChessBoard::Update(float deltaTime)
{
}

// Helper to make a buffer from the given materials
//
ID3D11Buffer* ChessBoard::MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower)
{
	MaterialBuffer mat;
	mat.ambient = ambient;
	mat.diffuse = diffuse;
	mat.spec = spec;
	mat.spec.w = specPower;

	// Create the constant buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(MaterialBuffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &mat;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	ID3D11Buffer* pConstantBuffer;
	pDevice->CreateBuffer(&bufferDesc, &data, &pConstantBuffer);
	return pConstantBuffer;
}

Matrix ChessBoard::GetBoardPosition(int x, int y, float pieceBaseOffset)
{
	float xOffset = X_LENGTH * 0.5f - 0.5f;
	float yOffset = Y_LENGTH * 0.5f - 0.5f;

	return Matrix::CreateTranslation((x - xOffset) * gridScale, pieceBaseOffset, (y - yOffset) * gridScale);
}

// constructor
ChessBoard::ChessBoard()
{
	pShader = nullptr;
	pBoardBuffer = nullptr;
	pDiffuse = nullptr;
	pSpec = nullptr;
	gridScale = 0.0f;
}

// destructor
ChessBoard::~ChessBoard()
{
	if (pBoardBuffer) pBoardBuffer->Release();
}