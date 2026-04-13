//
// Rook object
//

#ifndef _ROOK_H
#define _ROOK_H

#include "DirectX.h"
#include "TextureType.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include <SimpleMath.h>

using namespace DirectX;

class Rook
{
public:

	Rook() : pShader(nullptr) {};

	// called to initialize the object
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LitColourShader* pLitShader, bool white, TextureType* mainTex, TextureType* accTex, float bOffset);

	// called to draw the object
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix);

	// update the object
	void Update(float deltaTime);

	void SetTextures(ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* spec) {
		pDiffuse = diffuse;
		pSpec = spec;
	}

	void SetBaseOffset(const float newOffset) { baseOffset = newOffset; }
	float GetBaseOffset() { return baseOffset; }

private:

	ID3D11Buffer* MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower);

	// store a copy of the shader
	LitColourShader* pShader;

	ID3D11ShaderResourceView* pDiffuse;
	ID3D11ShaderResourceView* pSpec;

	// textures
	TextureType* whiteCeramic;
	TextureType* beigeCeramic;

	ID3D11Buffer* pMainBuffer;
	ID3D11Buffer* pAccentBuffer;

	// store each object
	IndexedPrimitive baseCylinder;
	IndexedPrimitive baseTorus;
	IndexedPrimitive baseCone;
	IndexedPrimitive centreTorus;
	IndexedPrimitive centreConeLower1;
	IndexedPrimitive centreConeLower2;
	IndexedPrimitive centreCylinder;
	IndexedPrimitive centreConeUpper1;
	IndexedPrimitive centreConeUpper2;
	IndexedPrimitive neckTorus;
	IndexedPrimitive headCylinder;

	IndexedPrimitive tipProng1;
	IndexedPrimitive tipProng2;
	IndexedPrimitive tipProng3;
	IndexedPrimitive tipProng4;
	IndexedPrimitive tipProng5;
	IndexedPrimitive tipProng6;

	Matrix baseCylinderMatrix;
	Matrix baseTorusMatrix;
	Matrix baseConeMatrix;
	Matrix centreTorusMatrix;
	Matrix centreConeLowerMatrix1;
	Matrix centreConeLowerMatrix2;
	Matrix centreCylinderMatrix;
	Matrix centreConeUpperMatrix1;
	Matrix centreConeUpperMatrix2;
	Matrix neckTorusMatrix;
	Matrix headCylinderMatrix;

	Matrix tipProngMatrix1;
	Matrix tipProngMatrix2;
	Matrix tipProngMatrix3;
	Matrix tipProngMatrix4;
	Matrix tipProngMatrix5;
	Matrix tipProngMatrix6;

	float baseOffset;
};

#endif
