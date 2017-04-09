#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H


class SoundSystem
{
public:
	SoundSystem(float scale = 1.0);
	virtual ~SoundSystem();

	void UpdateListener(D3DXVECTOR3 forward, D3DXVECTOR3 position, D3DXVECTOR3 velocity);
	/*DirectMusic does used inside DirectSound.
	  It used Segement->Loader->performance(Has Listener object which acts as microphone in world
	  ->synthesizer->secondary buffer (This is where our loaded 
	  sound actually resides -> primary buffer -> sound card.
	  The above hierarchy is called as audio path.
	  Using the object of DirectMusic or DirectSound we can query any object in 
	  this audio path for example we can query performance object or Listener object.
	  This objects when loaded are cached for better performance after some time
	  this cache needs to freed that task is done by the GarbageCollection method.*/
	void GarbageCollection();

	void SetVolume(long volume);

	IDirectMusicLoader8 *GetLoader();
	IDirectMusicPerformance8 *GetPerformance();

private:

	float m_scale;
	IDirectMusicLoader8 *m_loader;
	IDirectMusicPerformance8 *m_performance;
	IDirectSound3DListener8 *m_listener;

};

class Sound 
{
public: 

	Sound(char * filename);
	virtual ~Sound();

	void Play( bool loop = false, unsigned long flags = DMUS_SEGF_AUTOTRANSITION);

	IDirectMusicSegment8 *GetSegment();

private:

	IDirectMusicSegment8 *m_segment;
};


class AudioPath3D
{

public: 

	AudioPath3D();
	virtual ~AudioPath3D();

	void SetPoistion(D3DXVECTOR3 position);
	void SetVelocity(D3DXVECTOR3 velocity);
	void SetMode(unsigned long mode);

	void Play(IDirectMusicSegment8* segment, bool loop = false, unsigned long flags = DMUS_SEGF_SECONDARY);

private:
	IDirectMusicAudioPath8 *m_audioPath;
	IDirectSound3DBuffer8 *m_soundBuffer;

};

#endif