
#include "SpriteSample.h"
#include "NNInputSystem.h"

SpriteSample::SpriteSample()
{
	m_Sprite = NNSprite::Create( L"Resources/Texture/character.png" );
	AddChild( m_Sprite );

	// ������ ��������Ʈ ��� ����
}
SpriteSample::~SpriteSample()
{

}

void SpriteSample::Render()
{
	NNScene::Render();
}

void SpriteSample::Update( float dTime )
{
	NNScene::Update(dTime);
}