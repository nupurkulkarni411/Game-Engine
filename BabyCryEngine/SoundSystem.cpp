#include "Engine.h"

/*
We have initialized standard Loader, Performace, Created Standard Audio Path.
From Audio Path we queried Listener
*/
SoundSystem::SoundSystem(float scale)
{
	CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&m_loader);
	CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,(void**)&m_performance);

	m_performance->InitAudio(NULL, NULL, NULL, DMUS_APATH_SHARED_STEREOPLUSREVERB,8, DMUS_AUDIOF_ALL, NULL);

	IDirectMusicAudioPath8 *audioPath3D;
	m_performance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_3D, 1, true, &audioPath3D);
	audioPath3D->GetObjectInPath(0, DMUS_PATH_PRIMARY_BUFFER, 0, GUID_All_Objects, 0, IID_IDirectSound3DListener, (void**)& m_listener);

	m_scale = scale;
	m_listener->SetDistanceFactor(m_scale, DS3D_IMMEDIATE);

}


SoundSystem::~SoundSystem()
{
	m_performance->CloseDown();
	SAFE_RELEASE(m_performance);
	SAFE_RELEASE( m_loader );
}


/*In this method we place our microphone in 3D world*/
void SoundSystem::UpdateListener( D3DXVECTOR3 forward, D3DXVECTOR3 position , D3DXVECTOR3 velocity)
{
	//Set listeners orientation
	m_listener->SetOrientation( forward.x, forward.y, forward.z, 0.0f, 1.0f,0.0f, DS3D_DEFERRED);

	//Scale and set listeners position
	position *= m_scale;
	m_listener->SetPosition( position.x, position.y, position.z, DS3D_DEFERRED);

	//Scale and set listeners velocity
	velocity *= m_scale;
	m_listener->SetVelocity(velocity.x, velocity.y, velocity.z, DS3D_DEFERRED);

	m_listener->CommitDeferredSettings();

}

void SoundSystem::GarbageCollection()
{
	m_loader->CollectGarbage();
}

void SoundSystem::SetVolume( long volume )
{
	m_performance->SetGlobalParam(GUID_PerfMasterVolume, &volume, sizeof(long));
}

IDirectMusicLoader8 *SoundSystem::GetLoader()
{
	return m_loader;
}

IDirectMusicPerformance8 *SoundSystem::GetPerformance()
{
	return m_performance;
}

Sound::Sound(char * filename)
{
	WCHAR* wideFilename = new WCHAR[ strlen( filename ) + 1];
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wideFilename, strlen( filename )+1);
	wideFilename[strlen( filename )] = 0;

	//Load file in segment
	g_engine->GetSoundSystem()->GetLoader()->LoadObjectFromFile(CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, wideFilename, (void**)& m_segment); 
	SAFE_DELETE(wideFilename);

	//Load file from segment loader to perfromance
	m_segment->Download(g_engine->GetSoundSystem()->GetPerformance());
}


Sound::~Sound()
{
	//Loader has segment and segment has performance remove segments data from performance
	m_segment->Unload(g_engine->GetSoundSystem()->GetPerformance());
	//Remove segment from DirectMusic loader, then release it.
	g_engine->GetSoundSystem()->GetLoader()->ReleaseObjectByUnknown( m_segment );
	SAFE_RELEASE(m_segment);
}

void Sound::Play( bool loop, unsigned long flags)
{
	if(loop == true)
		m_segment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	else
		m_segment->SetRepeats(0);

	g_engine->GetSoundSystem()->GetPerformance()->PlaySegment(m_segment, flags, 0, NULL);

}

IDirectMusicSegment8 *Sound::GetSegment()
{
	return m_segment;
}



AudioPath3D::AudioPath3D()
{
	g_engine->GetSoundSystem()->GetPerformance()->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_3D, 1, true, &m_audioPath);
	//Get SoundBuffer from this AudioPath
	m_audioPath->GetObjectInPath(DMUS_PCHANNEL_ALL, DMUS_PATH_BUFFER, 0, GUID_NULL, 0, IID_IDirectSound3DBuffer,(void**)&m_soundBuffer);
}

AudioPath3D::~AudioPath3D()
{
	SAFE_RELEASE(m_soundBuffer);
	SAFE_RELEASE(m_audioPath);
}

void AudioPath3D::SetPoistion(D3DXVECTOR3 position)
{
	position *= g_engine->GetScale();
	m_soundBuffer->SetPosition(position.x, position.y, position.z,DS3D_IMMEDIATE);
}

void AudioPath3D::SetVelocity(D3DXVECTOR3 velocity)
{
	velocity *= g_engine->GetScale();
	m_soundBuffer->SetVelocity(velocity.x, velocity.y, velocity.z, DS3D_IMMEDIATE);
}

void AudioPath3D::SetMode(unsigned long mode)
{
	m_soundBuffer->SetMode(mode, DS3D_IMMEDIATE);
}

/*
Play sound on given audio path.
sound segments can only maintain single looping state. Hence
changing the looping flag will effect all instances of this sound.
*/
void AudioPath3D::Play( IDirectMusicSegment8 *segment, bool loop, unsigned long flags)
{
	if(loop == true)
		segment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	else
		segment->SetRepeats(0);

	g_engine->GetSoundSystem()->GetPerformance()->PlaySegmentEx( segment, NULL, NULL, flags, 0, NULL, NULL,m_audioPath);
}