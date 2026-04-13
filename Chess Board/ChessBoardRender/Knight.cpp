//
// Knight object
//

#include "Knight.h"

struct MaterialBuffer
{
	Color ambient;
	Color diffuse;
	Color spec; // alpha in w component
};

// called to initialize the object
void Knight::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LitColourShader* pLitShader, bool white, TextureType* mainTex, TextureType* accTex, float bOffset)
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

	centreConeLower.InitializeGeometry(pDevice, Cone);
	centreConeLower.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreSphere.InitializeGeometry(pDevice, Sphere);
	centreSphere.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	centreConeUpper.InitializeGeometry(pDevice, Cone);
	centreConeUpper.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	headSphere1.InitializeGeometry(pDevice, Sphere);
	headSphere1.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	headSphere2.InitializeGeometry(pDevice, Sphere);
	headSphere2.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	headCone1.InitializeGeometry(pDevice, Cone);
	headCone1.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	headCone2.InitializeGeometry(pDevice, Cone);
	headCone2.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	// update the matrices
	baseCylinderMatrix = Matrix::CreateScale(1.58f, 0.4f, 1.58f) *
		Matrix::CreateTranslation(0.0f, -2.0f, 0.0f);

	baseTorusMatrix = Matrix::CreateScale(1.32f, 1.0f, 1.32f) *
		Matrix::CreateTranslation(0.0f, -1.95f, 0.0f);

	baseConeMatrix = Matrix::CreateScale(1.9f, 1.5f, 1.9f) *
		Matrix::CreateTranslation(0.0f, -1.08f, 0.0f);

	centreTorusMatrix = Matrix::CreateScale(1.1f, 0.55f, 1.1f) *
		Matrix::CreateTranslation(0.0f, -1.4, 0.0f);

	centreConeLowerMatrix = Matrix::CreateScale(1.04f, 0.8f, 1.44f) *
		Matrix::CreateRotationX(XM_PI) * 
		Matrix::CreateTranslation(0.0f, -1.0f, -0.1f);

	centreSphereMatrix = Matrix::CreateScale(1.06f, 0.8f, 1.48f) *
		Matrix::CreateTranslation(0.0f, -0.5f, -0.1f);

	centreConeUpperMatrix = Matrix::CreateScale(0.94f, 1.7f, 1.36f) *
		Matrix::CreateRotationX(XM_PI * 0.1f) * 
		Matrix::CreateTranslation(0.0f, 0.3f, 0.17f);

	headSphereMatrix1 = Matrix::CreateScale(0.7f, 1.1f, 0.9f) *
		Matrix::CreateTranslation(0.0f, 0.76f, 0.18f);

	headSphereMatrix2 = Matrix::CreateScale(0.6f, 1.6f, 0.9f) *
		Matrix::CreateRotationX(XM_PI * 0.35) *
		Matrix::CreateTranslation(0.0f, 0.75f, -0.25f);

	headConeMatrix1 = Matrix::CreateScale(0.34f, 0.48f, 0.48f) *
		Matrix::CreateRotationZ(XM_PI * 0.06f) * 
		Matrix::CreateTranslation(-0.18f, 1.28f, 0.1f);

	headConeMatrix2 = Matrix::CreateScale(0.34f, 0.48f, 0.48f) *
		Matrix::CreateRotationZ(XM_PI * -0.06f) *
		Matrix::CreateTranslation(0.18f, 1.28f, 0.1f);

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
void Knight::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
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
	pShader->SetShaders(pDeviceContext, centreConeLowerMatrix * parentMatrix, viewMatrix, projMatrix);
	centreConeLower.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, centreSphereMatrix * parentMatrix, viewMatrix, projMatrix);
	centreSphere.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, centreConeUpperMatrix * parentMatrix, viewMatrix, projMatrix);
	centreConeUpper.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, headSphereMatrix1 * parentMatrix, viewMatrix, projMatrix);
	headSphere1.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, headSphereMatrix2 * parentMatrix, viewMatrix, projMatrix);
	headSphere2.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, headConeMatrix1 * parentMatrix, viewMatrix, projMatrix);
	headCone1.Draw(pDeviceContext);
	
	pShader->SetShaders(pDeviceContext, headConeMatrix2 * parentMatrix, viewMatrix, projMatrix);
	headCone2.Draw(pDeviceContext);
}

// update the object
void Knight::Update(float deltaTime)
{
}

ID3D11Buffer* Knight::MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower)
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