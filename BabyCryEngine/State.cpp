#include "Engine.h"

State::State( unsigned long id )
{
	m_id = id;
}

void State::Load()
{

}

void State::Close()
{

}

void State::RequestViewer(ViewerSetup *viewer)
{

}

void State::Update(float elapsed)
{

}

void State::Render()
{

}


unsigned long State::GetID()
{
	return m_id;
}