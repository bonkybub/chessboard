//
// Knight object
//

#ifndef _KNIGHT_H
#define _KNIGHT_H

#include "DirectX.h"
#include "TextureType.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include <SimpleMath.h>

using namespace DirectX;

class Knight
{
public:

	Knight() : pShader(nullptr) {};

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
	IndexedPrimitive centreConeLower;
	IndexedPrimitive centreSphere;
	IndexedPrimitive centreConeUpper;
	IndexedPrimitive headSphere1;
	IndexedPrimitive headSphere2;
	IndexedPrimitive headCone1;
	IndexedPrimitive headCone2;

	Matrix baseCylinderMatrix;
	Matrix baseTorusMatrix;
	Matrix baseConeMatrix;
	Matrix centreTorusMatrix;
	Matrix centreConeLowerMatrix;
	Matrix centreSphereMatrix;
	Matrix centreConeUpperMatrix;
	Matrix headSphereMatrix1;
	Matrix headSphereMatrix2;
	Matrix headConeMatrix1;
	Matrix headConeMatrix2;
	Matrix tipSphereMatrix;

	float baseOffset;
};

#endif

