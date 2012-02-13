require'fonixtalk'

-- FONIXTALK_VOICE_PAUL
-- FONIXTALK_VOICE_BETTY
-- FONIXTALK_VOICE_HARRY
-- FONIXTALK_VOICE_FRANK
-- FONIXTALK_VOICE_DENNIS
-- FONIXTALK_VOICE_KIT
-- FONIXTALK_VOICE_URSULA
-- FONIXTALK_VOICE_RITA
-- FONIXTALK_VOICE_WENDY
FonixTalk.SetVoice(FONIXTALK_VOICE_FRANK)

-- Pitch range is 50 to 350
FonixTalk.SetPitch(100)

-- Rate is between 50 and 600 in WPM
FonixTalk.SetRate(180)

-- Speak some text.
FonixTalk.Speak("john madden")

-- Resets all parameters and stops playing
-- FonixTalk.Reset()