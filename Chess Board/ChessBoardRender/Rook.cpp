//
// Rook object
//

#include "Rook.h"

struct MaterialBuffer
{
	Color ambient;
	Color diffuse;
	Color spec; // alpha in w component
};

// called to initialize the object
void Rook::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LitColourShader* pLitShader, bool white, TextureType* mainTex, TextureType* accTex, float bOffset)
{
	pShader = pLitShader;

	SetBaseOffset(bOffset);

	// textures
	whiteCeramic = mainTex;
	beigeCeramic = accTex;

	// geometry
	baseCylinder.InitializeGeometry(pDevice, Cylinder);
	baseCylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	baseTorus.InitializeGeometry(pDevice, Torus);
	baseTorus.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	baseCone.InitializeGeometry(pDevice, Cone);
	baseCone.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreTorus.InitializeGeometry(pDevice, Torus);
	centreTorus.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreConeLower1.InitializeGeometry(pDevice, Cone);
	centreConeLower1.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreConeLower2.InitializeGeometry(pDevice, Cone);
	centreConeLower2.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreCylinder.InitializeGeometry(pDevice, Cylinder);
	centreCylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreConeUpper1.InitializeGeometry(pDevice, Cone);
	centreConeUpper1.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreConeUpper2.InitializeGeometry(pDevice, Cone);
	centreConeUpper2.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	neckTorus.InitializeGeometry(pDevice, Torus);
	neckTorus.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	headCylinder.InitializeGeometry(pDevice, Cylinder);
	headCylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng1.InitializeGeometry(pDevice, Cube);
	tipProng1.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng2.InitializeGeometry(pDevice, Cube);
	tipProng2.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng3.InitializeGeometry(pDevice, Cube);
	tipProng3.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng4.InitializeGeometry(pDevice, Cube);
	tipProng4.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng5.InitializeGeometry(pDevice, Cube);
	tipProng5.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	tipProng6.InitializeGeometry(pDevice, Cube);
	tipProng6.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	// update the matrices
	baseCylinderMatrix = Matrix::CreateScale(1.58f, 0.4f, 1.58f) *
		Matrix::CreateTranslation(0.0f, -2.0f, 0.0f);

	baseTorusMatrix = Matrix::CreateScale(1.32f, 1.0f, 1.32f) *
		Matrix::CreateTranslation(0.0f, -1.95f, 0.0f);

	baseConeMatrix = Matrix::CreateScale(1.9f, 1.5f, 1.9f) *
		Matrix::CreateTranslation(0.0f, -1.08f, 0.0f);

	centreTorusMatrix = Matrix::CreateScale(1.1f, 0.55f, 1.1f) *
		Matrix::CreateTranslation(0.0f, -1.4f, 0.0f);

	centreConeLowerMatrix1 = Matrix::CreateScale(1.44f, 1.5f, 1.44f) *
		Matrix::CreateTranslation(0.0f, -0.6f, 0.0f);

	centreConeLowerMatrix2 = Matrix::CreateScale(1.0f, 1.9f, 1.0f) *
		Matrix::CreateTranslation(0.0f, -0.1f, 0.0f);

	centreCylinderMatrix = Matrix::CreateScale(0.57f, 1.0f, 0.57f) *
		Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	centreConeUpperMatrix1 = Matrix::CreateScale(0.8f, 1.25f, 0.8f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.0f, -0.5f, 0.0f);

	centreConeUpperMatrix2 = Matrix::CreateScale(0.8f, 0.9f, 0.8f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.0f, 0.05f, 0.0f);

	neckTorusMatrix = Matrix::CreateScale(0.78f, 0.43f, 0.78f) *
		Matrix::CreateTranslation(0.0f, 0.2f, 0.0f);

	headCylinderMatrix = Matrix::CreateScale(1.25f, 0.5f, 1.25f) *
		Matrix::CreateTranslation(0.0f, 0.55f, 0.0f);

	tipProngMatrix1 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) * 
		Matrix::CreateRotationY(XM_PI / 3.0f - XM_PI / 6.0f);

	tipProngMatrix2 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) * 
		Matrix::CreateRotationY(XM_PI / 3.0f * 2.0f - XM_PI / 6.0f);

	tipProngMatrix3 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) *
		Matrix::CreateRotationY(XM_PI / 3.0f * 3.0f - XM_PI / 6.0f);

	tipProngMatrix4 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) *
		Matrix::CreateRotationY(XM_PI / 3.0f * 4.0f - XM_PI / 6.0f);

	tipProngMatrix5 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) *
		Matrix::CreateRotationY(XM_PI / 3.0f * 5.0f - XM_PI / 6.0f);

	tipProngMatrix6 = Matrix::CreateScale(0.3f, 0.5f, 0.15f) *
		Matrix::CreateTranslation(0.0f, 0.75f, 0.55f) *
		Matrix::CreateRotationY(XM_PI / 3.0f * 6.0f - XM_PI / 6.0f);

	if (white)
	{
		pMainBuffer = MakeMaterialBuffer(pDevice, Color(0.9f, 0.9f, 0.9f), Colors::White.v, Colors::White.v, 2);
		pAccentBuffer = MakeMaterialBuffer(pDevice, Color(0.91f, 0.86f, 0.79f), Colors::Beige.v, Colors::Beige.v, 2);
	}
	else
	{
		pMainBuffer = MakeMaterialBuffer(pDevice, Color(0.05f, 0.05f, 0.05f), Colors::White.v, Colors::White.v, 2);
		pAccentBuffer = MakeMaterialBuffer(pDevice, Color(0.11f, 0.01f, 0.0f), Colors::White.v, Colors::White.v, 2);
	}
}

// called to draw the object
// the parent matrix allows the user to pass in a matrix to transform the entire piece
void Rook::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	ID3D11ShaderResourceView* pTex1 = whiteCeramic->GetResourceView();
	ID3D11ShaderResourceView* pTex2 = beigeCeramic->GetResourceView();

	pDeviceContext->PSSetConstantBuffers(2, 1, &pMainBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &pTex1);
	pShader->SetShaders(pDeviceContext, baseCylinderMatrix * parentMatrix, viewMatrix, projMatrix);
	baseCylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, baseTorusMatrix * parentMatrix, viewMatrix, projMatrix);
	baseTorus.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, baseConeMatrix * parentMatrix, viewMatrix, projMatrix);
	baseCone.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pAccentBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &pTex2);
	pShader->SetShaders(pDeviceContext, centreTorusMatrix * parentMatrix, viewMatrix, projMatrix);
	centreTorus.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pMainBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &pTex1);
	pShader->SetShaders(pDeviceContext, centreConeLowerMatrix1 * parentMatrix, viewMatrix, projMatrix);
	centreConeLower1.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, centreConeLowerMatrix2 * parentMatrix, viewMatrix, projMatrix);
	centreConeLower2.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, centreCylinderMatrix * parentMatrix, viewMatrix, projMatrix);
	centreCylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, centreConeUpperMatrix1 * parentMatrix, viewMatrix, projMatrix);
	centreConeUpper1.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, centreConeUpperMatrix2 * parentMatrix, viewMatrix, projMatrix);
	centreConeUpper2.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pAccentBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &pTex2);
	pShader->SetShaders(pDeviceContext, neckTorusMatrix * parentMatrix, viewMatrix, projMatrix);
	neckTorus.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pMainBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &pTex1);
	pShader->SetShaders(pDeviceContext, headCylinderMatrix * parentMatrix, viewMatrix, projMatrix);
	headCylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix1 * parentMatrix, viewMatrix, projMatrix);
	tipProng1.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix2 * parentMatrix, viewMatrix, projMatrix);
	tipProng2.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix3 * parentMatrix, viewMatrix, projMatrix);
	tipProng3.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix4 * parentMatrix, viewMatrix, projMatrix);
	tipProng4.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix5 * parentMatrix, viewMatrix, projMatrix);
	tipProng5.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, tipProngMatrix6 * parentMatrix, viewMatrix, projMatrix);
	tipProng6.Draw(pDeviceContext);
}

// update the object
void Rook::Update(float deltaTime)
{
}

ID3D11Buffer* Rook::MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower)
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
