// #include "AllHeadphone.h"
// //Adding audio files and library
// #include "Audio_Agate_Thakun.h"
// #include "Audio_Bame_Badha.h"
// #include "Audio_Bame_Jan.h"
// #include "Audio_Battery_20.h"
// #include "Audio_Battery_50.h"
// #include "Audio_Battery_Full.h"
// #include "Audio_Battery_Low.h"
// #include "Audio_Dane_Badha.h"
// #include "Audio_Dane_Jan.h"
// #include "Audio_Halka_Dane_Jan.h"
// #include "Audio_Power_Off.h"
// #include "Audio_Power_On.h"
// #include "Audio_Thamun.h"
// //#include "SoundData.h"
// //#include "XT_DAC_Audio.h"


// AllHeadphone::AllHeadphone(){
//     //initializing and assigning class to the audio array
//     XT_Wav_Class Agate_Thakun_Audio(Agate_Thakun);
//     XT_Wav_Class Bame_Badha_Audio(Bame_Badha);
//     XT_Wav_Class Bame_Jan_Audio(Bame_Jan);
//     XT_Wav_Class Battery_20_Audio(Battery_20);
//     XT_Wav_Class Battery_50_Audio(Battery_50);
//     XT_Wav_Class Battery_Full_Audio(Battery_Full);
//     XT_Wav_Class Battery_Low_Audio(Battery_Low);
//     XT_Wav_Class Dane_Badha_Audio(Dane_Badha);
//     XT_Wav_Class Dane_Jan_Audio(Dane_Jan);
//     XT_Wav_Class Halka_Dane_Audio(Halka_Dane_Jan);
//     XT_Wav_Class Power_Off_Audio(Power_Off);
//     XT_Wav_Class Power_On_Audio(Power_On);
//     XT_Wav_Class Thamu_Audio(Thamun);
//     //XT_Wav_Class Sample_audio(sample);   //initializing the sample audio data
//     XT_DAC_Audio_Class DacAudio(25, 0); //connected to pin10 or GPIO25 or DAC1 (create the main player class object)
// }

// void AllHeadphone::Play_Agate_Thakun()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Agate_Thakun_Audio);
//   while (Agate_Thakun_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Bame_Badha()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Bame_Badha_Audio);
//   while (Bame_Badha_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Bame_Jan()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Bame_Jan_Audio);
//   while (Bame_Jan_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Battery_20()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Battery_20_Audio);
//   while (Battery_20_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Battery_50()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Battery_50_Audio);
//   while (Battery_50_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Battery_Full()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Battery_Full_Audio);
//   while (Battery_Full_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Battery_Low()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Battery_Low_Audio);
//   while (Battery_Low_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Dane_Badha()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Dane_Badha_Audio);
//   while (Dane_Badha_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Dane_Jan()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Dane_Jan_Audio);
//   while (Dane_Jan_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Halka_Dane_Jan()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Halka_Dane_Audio);
//   while (Halka_Dane_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Power_Off()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Power_Off_Audio);
//   while (Power_Off_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Power_On()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Power_On_Audio);
//   while (Power_On_Audio.Playing) DacAudio.FillBuffer();
// }
// void AllHeadphone::Play_Thamun()
// {
//   DacAudio.FillBuffer();
//   DacAudio.Play(&Thamu_Audio);
//   while (Thamu_Audio.Playing) DacAudio.FillBuffer();
// }