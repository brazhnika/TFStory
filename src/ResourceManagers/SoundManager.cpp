   
   #include "SoundManager.h"
   

//SoundManager::SoundManager()
//{
//}

void SoundManager::Clear()
{
    m_music.clear();
    m_sounds.clear();
}
   
void SoundManager::PlaySound(std::string sound, bool loop)
{
    if(m_sounds.find(sound) == m_sounds.end())
    {
        m_sounds[sound].buffer = std::make_shared<sf::SoundBuffer> ();
        const auto res = m_sounds[sound].buffer->loadFromFile(sound);
        assert(res);
        m_sounds[sound].sound = std::make_shared<sf::Sound>(*(m_sounds[sound].buffer));
    };
    m_sounds[sound].sound->setLooping(loop);        
    m_sounds[sound].sound->play();
    //m_sounds[sound].sound->getStatus()
    m_lastSound = sound;
}

void SoundManager::StopSound(std::string sound)
{
    if(m_sounds.find(sound) != m_sounds.end())
    {
        m_sounds[sound].sound->stop();
    };
}

void SoundManager::StopLastSound()
{
    if(!m_lastSound.empty())
        m_sounds[m_lastSound].sound->stop();
}


void SoundManager::PlayBackground(std::string back)
{
    if(m_music.find(back) == m_music.end())
    {
        m_music[back].buffer = std::make_shared<sf::SoundBuffer> ();
        const auto res = m_music[back].buffer->loadFromFile(back);
        assert(res);
        m_music[back].sound = std::make_shared<sf::Sound>(*(m_music[back].buffer));
    };
    m_music[back].sound->stop();
    m_music[back].sound->setLooping(true);
    m_music[back].sound->play();
    m_lastBackground = back;
}


void SoundManager::StopLastBackground()
{
    if(!m_lastBackground.empty())
        m_music[m_lastBackground].sound->stop();
}