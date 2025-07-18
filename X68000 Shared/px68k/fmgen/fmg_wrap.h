#ifndef _win68_opm_fmgen
#define _win68_opm_fmgen

int OPM_Init(int clock, int rate);
void OPM_Cleanup(void);
void OPM_Reset(void);
void OPM_Update(short *buffer, int length, int rate, BYTE *pbsp, BYTE *pbep);
void FASTCALL OPM_Write(DWORD r, BYTE v);
BYTE FASTCALL OPM_Read(WORD a);
void FASTCALL OPM_Timer(DWORD step);
void OPM_SetVolume(BYTE vol);
void OPM_SetRate(int clock, int rate);

#endif //_win68_opm_fmgen
