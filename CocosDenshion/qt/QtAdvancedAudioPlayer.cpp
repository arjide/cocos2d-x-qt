/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "QtAdvancedAudioPlayer.h"
#include "pullaudioout.h"
#include "pushaudioout.h"
#include "CCFileUtils.h"
#include "CCApplication.h"
#include <QDebug>

using namespace GE;
USING_NS_CC;

namespace CocosDenshion {

static unsigned int _Hash(const char *key)
{
    unsigned int len = strlen(key);
    const char *end=key+len;
    unsigned int hash;

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (unsigned int) (unsigned char) toupper(*key);
    }
    return (hash);
}

static QString fullPathFromRelativePath(const char *pszRelativePath)
{
    QString strRet="";
    int len = strlen(pszRelativePath);
    if (pszRelativePath == NULL || len <= 0)
    {
        return strRet;
    }

    if (len > 1 && pszRelativePath[0] == '/')
    {
        strRet = pszRelativePath;
    }
    else
    {
        strRet = CCFileUtils::fullPathFromRelativePath(pszRelativePath);
    }
    return strRet;
}

QtAdvancedAudioPlayer* QtAdvancedAudioPlayer::sharedPlayer()
{
    static QtAdvancedAudioPlayer s_SharedPlayer;
    return &s_SharedPlayer;
}

QtAdvancedAudioPlayer::QtAdvancedAudioPlayer() :
    m_mixer(NULL),
    m_audioOut(NULL),
    m_audioEventListener(NULL),
    m_sfxInstanceId(0)
{
    init();
}

void QtAdvancedAudioPlayer::init()
{
    CCApplication::sharedApplication().lock();
    m_mixer = new AudioMixer(this);

#if defined(Q_OS_SYMBIAN)
    m_audioOut = new PullAudioOut(m_mixer, this);
#else
    m_audioOut = new PushAudioOut(m_mixer, this);
#endif
    CCApplication::sharedApplication().unlock();
}

void QtAdvancedAudioPlayer::close()
{
    delete m_mixer;
    m_mixer = NULL;

    delete m_audioOut;
    m_audioOut = NULL;

    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        delete i;
    }
    m_effectInstances.clear();

    foreach (QObject *i, m_effects)
    {
        delete i;
    }
    m_effects.clear();
}

QtAdvancedAudioPlayer::~QtAdvancedAudioPlayer()
{
    close();
}

void QtAdvancedAudioPlayer::checkFinishedEffects()
{
    QHash<unsigned int, QPointer<PlayableAudioSource> >::iterator i =
            m_effectInstances.begin();
    while (i != m_effectInstances.end())
    {
        if (i.value().isNull())
            i = m_effectInstances.erase(i);
        else
            ++i;
    }
}

SfxId QtAdvancedAudioPlayer::loadEffect(const char* pszFilePath)
{
    unsigned int hash = _Hash(pszFilePath);

    QObject *sfx = m_effects.value(hash);
    if (!sfx)
    {
        AudioBuffer *buffer = AudioBuffer::load(
                    fullPathFromRelativePath(pszFilePath));
        if (!buffer)
        {
            CCLOGERROR("could not load audio effect %s", pszFilePath);
            return 0;
        }
        m_effects.insert(hash, buffer);
    }
    return hash;
}

SfxId QtAdvancedAudioPlayer::loadMusic(const char* pszFilePath)
{
    unsigned int hash = _Hash(pszFilePath);

    QObject *sfx = m_effects.value(hash);
    if (!sfx)
    {
        VorbisSource *source = new VorbisSource();
        if (!source->load(fullPathFromRelativePath(pszFilePath)))
        {
            CCLOGERROR("could not load music %s", pszFilePath);
            return 0;
        }

        m_mixer->addAudioSource(source);
        m_effects.insert(hash, source);
    }

    return hash;
}

void QtAdvancedAudioPlayer::unload(SfxId soundId)
{
    checkFinishedEffects();
    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        if (i->getTag() == soundId)
            i->stop();
    }

    QObject* buffer = m_effects.value(soundId);
    delete buffer;
    m_effects.remove(soundId);
}

void QtAdvancedAudioPlayer::pauseAll()
{
    checkFinishedEffects();
    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        i->pause();
    }
}

void QtAdvancedAudioPlayer::resumeAll()
{
    checkFinishedEffects();
    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        i->resume();
    }
}

void QtAdvancedAudioPlayer::stopAll()
{
    checkFinishedEffects();
    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        i->stop();
    }

    m_effectInstances.clear();
}

float QtAdvancedAudioPlayer::getVolume()
{
    return m_mixer->absoluteVolume();
}

void QtAdvancedAudioPlayer::setVolume(float volume)
{
    if (volume > 1.0f)
    {
        volume = 1.0f;
    }
    else if (volume < 0.0f)
    {
        volume = 0.0f;
    }
    m_mixer->setAbsoluteVolume(volume);
}

SfxInstanceId QtAdvancedAudioPlayer::play(SfxId soundId)
{
    PlayableAudioSource *inst;

    checkFinishedEffects();

    QObject *source = m_effects.value(soundId);
    if (!source)
    {
        CCLOGERROR("Tried to play non-existing sfx %u", soundId);
        return 0;
    }

    if (qobject_cast<VorbisSource *>(source))
    {
        inst = qobject_cast<PlayableAudioSource *>(source);
        inst->seek(0);
        inst->play();
    }
    else
    {
        AudioBuffer *buffer = qobject_cast<AudioBuffer *>(source);
        inst = buffer->playWithMixer(*m_mixer);
        if (!inst)
        {
            CCLOG("playWithMixer for %s failed", pszFilePath);
            return 0;
        }
    }

    QObject::connect(inst, SIGNAL(finished(unsigned int)),
                     this, SLOT(sfxFinished(unsigned int)));

    m_effectInstances.insertMulti(++m_sfxInstanceId,
        QPointer<PlayableAudioSource>(inst));

    inst->setTag(m_sfxInstanceId);
    inst->setId(soundId);

    return m_sfxInstanceId;
}

void QtAdvancedAudioPlayer::stop(SfxInstanceId soundId)
{
    checkFinishedEffects();
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->stop();
    }
}

void QtAdvancedAudioPlayer::pause(SfxInstanceId soundId)
{
    checkFinishedEffects();
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->pause();
    }
}

void QtAdvancedAudioPlayer::resume(SfxInstanceId soundId)
{
    checkFinishedEffects();
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->resume();
    }
}

bool QtAdvancedAudioPlayer::isPlaying(SfxInstanceId soundId)
{
    checkFinishedEffects();
    return (m_effectInstances.value(soundId) != NULL);
}

void QtAdvancedAudioPlayer::setLoopCount(SfxInstanceId soundId, unsigned int loopCount)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->setLoopCount(loopCount);
    }
}

unsigned int QtAdvancedAudioPlayer::getLoopCount(SfxInstanceId soundId)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        return instance->getLoopCount();
    }
    return 0;
}

void QtAdvancedAudioPlayer::setVolume(SfxInstanceId soundId, float volume)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        if (volume < -1.0f)
            volume = -1.0f;
        if (volume > 1.0f)
            volume = 1.0f;

        // TODO: Doesn't maintain pan
        instance->setLeftVolume(volume);
        instance->setRightVolume(volume);
    }
}

float QtAdvancedAudioPlayer::getVolume(SfxInstanceId soundId)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        return (instance->leftVolume() + instance->rightVolume()) / 2.0f;
    }
    return 0.0f;
}

void QtAdvancedAudioPlayer::setPitch(SfxInstanceId soundId, float pitch)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->setSpeed(pitch);
    }
}

float QtAdvancedAudioPlayer::getPitch(SfxInstanceId soundId)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        return instance->getSpeed();
    }

    return 1.0f;
}

static float panx = 0.0f;

void QtAdvancedAudioPlayer::setPanning(SfxInstanceId soundId, float pan)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        if (pan < -1.0f)
            pan = -1.0f;
        if (pan > 1.0f)
            pan = 1.0f;

        float volume = (instance->leftVolume() + instance->rightVolume()) / 2.0f;
        instance->setLeftVolume(-(pan - 1.0f) * volume);
        instance->setRightVolume((pan + 1.0f) * volume);
        panx = pan;
    }
}

float QtAdvancedAudioPlayer::getPanning(SfxInstanceId soundId)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        // TODO ....
        return panx;
    }
    return 0.0f;
}

void QtAdvancedAudioPlayer::setFadeInDuration(SfxInstanceId soundId,
                                              float fadeInDurationMs)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->setFadeInDuration(fadeInDurationMs);
    }
}

void QtAdvancedAudioPlayer::setFadeOutDuration(SfxInstanceId soundId,
                                               float fadeOutDurationMs)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        instance->setFadeOutDuration(fadeOutDurationMs);
    }
}

void QtAdvancedAudioPlayer::seek(SfxInstanceId soundId, float position)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        // TODO: Calculate position
        instance->seek(position);
    }
}

float QtAdvancedAudioPlayer::position(SfxInstanceId soundId)
{
    PlayableAudioSource *instance = m_effectInstances.value(soundId);
    if (instance)
    {
        // TODO: Calculate position
        return instance->position();
    }
    return 0.0f;
}

void QtAdvancedAudioPlayer::setAudioEventListener(AudioEventListener *listener)
{
    m_audioEventListener = listener;
}

void QtAdvancedAudioPlayer::removeAudioEventListener()
{
    m_audioEventListener = NULL;
}

SfxId QtAdvancedAudioPlayer::sfxIdForFile(const char* pszFilePath)
{
    return _Hash(pszFilePath);
}

SfxInstanceId QtAdvancedAudioPlayer::sfxInstanceIdForSfxId(SfxId soundId)
{
    foreach (QPointer<PlayableAudioSource> i, m_effectInstances)
    {
        if (i->getId() == soundId)
            return i->getTag();
    }

    return 0;
}

SfxInstanceId QtAdvancedAudioPlayer::getActiveSfxInstanceId()
{
    checkFinishedEffects();
    if (m_effectInstances.keys().isEmpty())
    {
        return 0;
    }

    return m_effectInstances.keys().at(m_effectInstances.keys().count() - 1);
}

void QtAdvancedAudioPlayer::sfxFinished(unsigned int tag)
{
    m_effectInstances.remove(tag);
    if (m_audioEventListener)
    {
        m_audioEventListener->effectEnded(tag);
    }
}

}