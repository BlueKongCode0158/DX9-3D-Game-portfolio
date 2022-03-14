#pragma once
#include "Base.h"

BEGIN(Client)
class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)
public:
	enum CHANNELID {BGM, PLAYER, MONSTER, EFFECT, UI, MAXCHANNEL};
private:
	explicit CSound_Manager();
	virtual  ~CSound_Manager() = default;
public:
	void Initialize(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem					= nullptr; 
public:
	virtual void Free()	override;
};
END

