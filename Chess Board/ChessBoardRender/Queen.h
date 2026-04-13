//
// Queen object
//

#ifndef _QUEEN_H
#define _QUEEN_H

#include "DirectX.h"
#include "TextureType.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include <SimpleMath.h>

using namespace DirectX;

class Queen
{
public:

	Queen() : pShader(nullptr) {};

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
	IndexedPrimitive neckTorusLower;
	IndexedPrimitive neckTorusUpper;
	IndexedPrimitive headTorus;
	IndexedPrimitive headCylinder;
	IndexedPrimitive headSphere;
	IndexedPrimitive tipCylinder;
	IndexedPrimitive tipSphere;

	IndexedPrimitive crownProng1;
	IndexedPrimitive crownProng2;
	IndexedPrimitive crownProng3;
	IndexedPrimitive crownProng4;
	IndexedPrimitive crownProng5;
	IndexedPrimitive crownProng6;
	IndexedPrimitive crownProng7;
	IndexedPrimitive crownProng8;
	IndexedPrimitive crownProng9;

	Matrix baseCylinderMatrix;
	Matrix baseTorusMatrix;
	Matrix baseConeMatrix;
	Matrix centreTorusMatrix;
	Matrix centreConeLowerMatrix1;
	Matrix centreConeLowerMatrix2;
	Matrix centreCylinderMatrix;
	Matrix centreConeUpperMatrix1;
	Matrix centreConeUpperMatrix2;
	Matrix neckTorusLowerMatrix;
	Matrix neckTorusUpperMatrix;
	Matrix headTorusMatrix;
	Matrix headCylinderMatrix;
	Matrix headSphereMatrix;
	Matrix tipCylinderMatrix;
	Matrix tipSphereMatrix;

	Matrix crownProngMatrix1;
	Matrix crownProngMatrix2;
	Matrix crownProngMatrix3;
	Matrix crownProngMatrix4;
	Matrix crownProngMatrix5;
	Matrix crownProngMatrix6;
	Matrix crownProngMatrix7;
	Matrix crownProngMatrix8;
	Matrix crownProngMatrix9;

	float baseOffset;
};

#endif


