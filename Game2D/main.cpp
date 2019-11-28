//1st window programming
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dsound.h>
#include <mmsystem.h>
#include "ddutil.h"
#include "dsutil.h"
#include "Winbase.h"
#include "entity.h"

#define _GetKeyState(vkey) HIBYTE(GetAsyncKeyState(vkey))
#define _GetKeyPush(vkey) LOBYTE(GetAsyncKeyState(vkey))

HWND MainHwnd;

LPDIRECTDRAW7          DirectOBJ;
LPDIRECTDRAWSURFACE7   RealScreen;//주면
LPDIRECTDRAWSURFACE7   BackScreen;//후면 

LPDIRECTDRAWSURFACE7   backract; // 배경화면
LPDIRECTDRAWSURFACE7   back_start;
LPDIRECTDRAWSURFACE7   back_end;
LPDIRECTDRAWSURFACE7   back_end_win;
LPDIRECTDRAWSURFACE7   back_way;

LPDIRECTDRAWSURFACE7   planeD;
LPDIRECTDRAWSURFACE7   planeDc;
LPDIRECTDRAWSURFACE7   bleatD;
LPDIRECTDRAWSURFACE7   bleatD2;

LPDIRECTDRAWSURFACE7   meteorD;
LPDIRECTDRAWSURFACE7   meteorD2;
LPDIRECTDRAWSURFACE7   meteorD3;
LPDIRECTDRAWSURFACE7   meteorD_boss;
LPDIRECTDRAWSURFACE7   meteorDc;

LPDIRECTDRAWSURFACE7   planeD_life;

LPDIRECTDRAWSURFACE7   num_1;
LPDIRECTDRAWSURFACE7   num_2;
LPDIRECTDRAWSURFACE7   num_3;
LPDIRECTDRAWSURFACE7   num_4;
LPDIRECTDRAWSURFACE7   num_5;
LPDIRECTDRAWSURFACE7   num_6;
LPDIRECTDRAWSURFACE7   num_7;
LPDIRECTDRAWSURFACE7   num_8;
LPDIRECTDRAWSURFACE7   num_9;
LPDIRECTDRAWSURFACE7   num_0;
LPDIRECTDRAWSURFACE7   score;

LPDIRECTDRAWSURFACE7   num_1e;
LPDIRECTDRAWSURFACE7   num_2e;
LPDIRECTDRAWSURFACE7   num_3e;
LPDIRECTDRAWSURFACE7   num_4e;
LPDIRECTDRAWSURFACE7   num_5e;
LPDIRECTDRAWSURFACE7   num_6e;
LPDIRECTDRAWSURFACE7   num_7e;
LPDIRECTDRAWSURFACE7   num_8e;
LPDIRECTDRAWSURFACE7   num_9e;
LPDIRECTDRAWSURFACE7   num_0e;
LPDIRECTDRAWSURFACE7   score_e;
LPDIRECTDRAWSURFACE7   stage;

LPDIRECTDRAWSURFACE7   game_start;
LPDIRECTDRAWSURFACE7   game_way;
LPDIRECTDRAWSURFACE7   game_end_b;
LPDIRECTDRAWSURFACE7   arrow;

LPDIRECTSOUND			SoundOBJ = NULL;
LPDIRECTSOUNDBUFFER		SoundDSB = NULL;
DSBUFFERDESC			DSB_desc;

HSNDOBJ Sound;
HSNDOBJ boom;
HSNDOBJ back;

short back_fl = 0, back_ft = 0, back_fr = 640, back_fb = 480;
short plane_fl = 0, plane_ft = 0, plane_fr = 88, plane_fb = 109;
short plane_life_fl = 0, plane_life_ft = 0, plane_life_fr = 30, plane_life_fb = 30;
short bleat_fl = 0, bleat_ft = 0, bleat_fr = 30, bleat_fb = 30;
short meteor_fl = 0, meteor_ft = 0, meteor_fr = 70, meteor_fb = 70;
short meteor_boss_fl = 0, meteor_boss_ft = 0, meteor_boss_fr = 100, meteor_boss_fb = 100;
short fx = (back_fr - plane_fr)/2, fy = back_fb - plane_fb - 40;

short bleat_num = -1; 
short bnum = 15;
short bleat_time1 = 5;
short bleat_time2 = 0;
short bleat_time3 = 10;
short bleat_time4 = 0;

short mnum = 50 ;
short mnum2 = 5;
short fnum = 0;

int count = 35;
int start_count = 200;
short start_count_f = 0;

bool start_way = false;
bool start_chose_f = false;

bool start = false;
bool start2 = true;
bool crach_start[10] = {false};
bool crach_start2 = false;
bool stage_start = false;
bool game_start_contrl = false;

short stage_count = 0;
short stage_num = 0;
short count_time = 0;
short crach_timer[10] = { 0 };
short crach_timer2 = 0;

int arri[10];
int bb = 0;

short arrp = 0;
short timerp = 0;
short crachp = 0;
short score_spase_x = 250;
short score_spase_y = 180;
short planec = 4;

plane *fp;
bleat *fb[15];
bleat2 * fb2;

meteor1 *fm[50];
meteor2 *fm2[5];
meteor3 *fm3[5];
meteor_boss *fmb;

RECT backRect = { back_fl, back_ft , back_fr, back_fb };
RECT planeRect = { plane_fl, plane_ft, plane_fr , plane_fb  };
RECT bleatRect = { bleat_fl, bleat_ft, bleat_fr , bleat_fb };
RECT meteorRect = { meteor_fl, meteor_ft, meteor_fr, meteor_fb };
RECT meteor_boss_Rest = { meteor_boss_fl, meteor_boss_ft, meteor_boss_fr, meteor_boss_fb };

RECT plane_life_Rect = { plane_life_fl, plane_life_ft, plane_life_fr , plane_life_fb };
RECT numRect = { 0, 0, 30, 30};
RECT scoreRect = { 0, 0, 60, 30 };
RECT numRect_e = { 0, 0, 50, 100 };
RECT scoreRect_e = { 0, 0, 100, 100 };
RECT game = { 0, 0, 200, 80 };
RECT arrow_e = { 0, 0, 100, 80 };

BOOL _InitDirectSound(void)
{
	if (DirectSoundCreate(NULL, &SoundOBJ, NULL) == DS_OK) {
		if (SoundOBJ->SetCooperativeLevel(MainHwnd, DSSCL_PRIORITY) != DS_OK) return FALSE;

		memset(&DSB_desc, 0, sizeof(DSBUFFERDESC));
		DSB_desc.dwSize = sizeof(DSBUFFERDESC);
		DSB_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

		if (SoundOBJ->CreateSoundBuffer(&DSB_desc, &SoundDSB, NULL) != DS_OK) return FALSE;
		SoundDSB->SetVolume(0);
		SoundDSB->SetPan(0);
		return TRUE;
	}
	return FALSE;
}

void _Play()
{
	SndObjPlay(Sound, NULL);
}

void BleatProcn()
{
	for (short bn = 0; bn <= bleat_num; bn++)
	{
		if (fb[bn]->drow() == true)
		{
			BackScreen->BltFast(fb[bn]->show_x(), fb[bn]->show_y(), bleatD, &bleatRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			fb[bn]->bmove_y();
		}
	}
	if (fb2->drow() == true)
	{
		BackScreen->BltFast(fb2->show_x(), fb2->show_y(), bleatD2, &bleatRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		fb2->bmove_y();
	}
	if (bleat_num == bnum-1)
		bleat_num = 0;
}

void meteorProc()
{
	short  b =0 ;
	short c = 0;
	for (short mn = 0; mn < mnum; mn++)
	{
		if (mn % 7 == 0)
			b += meteor_fb + 40;
		fm[mn] = new meteor1(0, b);
	}
	for (short mn = 0; mn < mnum2; mn++)
	{
		if(mn % 2 == 0)
			c += meteor_fb + 50;
		fm2[mn] = new meteor2(-30, c);
	}
	for (short mn = 0; mn < mnum2; mn++)
	{
		if (mn % 2 == 0)
			c += meteor_fb + 50;
		fm3[mn] = new meteor3(0, c);
	}
	fmb = new meteor_boss(640/2-meteor_boss_fr/2, 0);
}

void meteor_procn()
{

	for (short mn = 0; mn < mnum2; mn++)
		{
			if (fm2[mn]->show_y() == 0)
		{
			fm2[mn]->move_x((fm2[mn]->show_x() + 70) % 600);
		}
	}

	for (short mn = 0; mn < mnum2; mn++)
	{
		if (fm3[mn]->show_y() == 0)
		{
			fm3[mn]->move_x(fp->show_x());
		}
	}
	for (short mn1 = 0; mn1 < mnum; mn1++)
	{
		if (fm[mn1]->show_y() == 0)
		{
			fm[mn1]->move_x(rand() % 600);
		}
	}
	
	for (short mn1 = 0; mn1 < mnum; mn1++)
	{
		if (fm[mn1]->drow() == true  && fm[mn1]->show_y() == 0)
		{
			for (short mn2 = mn1 + 1; mn2 < mnum; mn2++)
			{
				if ((fm[mn1]->show_x() - 10 <= fm[mn2]->show_x()) && (fm[mn1]->show_x() + meteor_fr + 50 >= fm[mn2]->show_x()))
				{
					if (fm[mn1]->show_y() == fm[mn2]->show_y())
					{
						fm[mn2]->die();
					}
				}
			}
		}
	}

	for (short mn1 = 0; mn1 < mnum; mn1++)
	{
		if (fm[mn1]->drow() == true)
			BackScreen->BltFast(fm[mn1]->show_x(), fm[mn1]->show_y(), meteorD, &meteorRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		fm[mn1]->move_meteor();
	}

	for (short mn = 0; mn < mnum2; mn++)
	{
		if (fm2[mn]->drow() == true)
			BackScreen->BltFast(fm2[mn]->show_x(), fm2[mn]->show_y(), meteorD2, &meteorRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		if (count >= 15) { fm2[mn]->move_meteor(); }
	}

	for (short mn = 0; mn < mnum2; mn++)
	{
		if (fm3[mn]->drow() == true)
			BackScreen->BltFast(fm3[mn]->show_x(), fm3[mn]->show_y(), meteorD3, &meteorRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		if (count >= 30) {	fm3[mn]->move_meteor();	}
	}
	if (fmb->drow() == true)
	{
		fmb->move_meteor();
		BackScreen->BltFast(fmb->show_x(), fmb->show_y(), meteorD_boss, &meteor_boss_Rest, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
	if (count != 0 &&count >45 && fmb->show_y()==0) { fmb->meteor_true(); }
}

void bleat_proc()
{
	for (int i = 0; i < bnum; i++)
		fb[i] = new bleat(-30,-30);
	fb2 = new bleat2(-30, -30);
}

void crach_show()
{
	for (int i = 0; i < 10; i++)
	{
		if(crach_start[i] == true)
		BackScreen->BltFast(fb[arri[i]]->show_x(), fb[arri[i]]->show_y(), meteorDc, &meteorRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
	if(crach_start2==true)
		BackScreen->BltFast(fb2->show_x(), fb2->show_y(), meteorDc, &meteorRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	if(planec < 4)
		BackScreen->BltFast(fp->show_x(), fp->show_y(), planeDc, &planeRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void Chrch()
{
	for (int i = 0; i < bnum; i++)
	{
		if (fb[i]->drow() == true)
		{
			for (int j = 0; j < mnum; j++)
			{
				if (fm[j]->drow() == true)
				{
					if ((fb[i]->show_x() < fm[j]->show_x() + fm[j]->show_r() / 2 + 20) && (fm[j]->show_x() < fb[i]->show_x() + 10) && (fm[j]->show_y() - 15 <= fb[i]->show_y()) && (fm[j]->show_y() + 15 >= fb[i]->show_y()))
					{
						fb[i]->die(); fm[j]->meteor_crach1();  SndObjPlay(Sound, NULL); 
						if (crachp == 10)
							crachp = 0;
						if (arrp == 10)
							arrp = 0;
						crach_start[crachp] = true; crachp++;
						arri[arrp] = i; arrp++;
						exit;
					}
				}
			}
		}
	}

	for (int i = 0; i < bnum; i++)
	{
		if (fb[i]->drow() == true)
		{
			for (int j = 0; j < mnum2; j++)
			{
				if (fm2[j]->drow() == true)
				{
					if ((fb[i]->show_x() < fm2[j]->show_x() + fm2[j]->show_r() / 2 + 20) && (fm2[j]->show_x() < fb[i]->show_x() + 10) && (fm2[j]->show_y() - 15 <= fb[i]->show_y()) && (fm2[j]->show_y() + 15 >= fb[i]->show_y()))
					{
						fb[i]->die(); fm2[j]->meteor_crach1();  SndObjPlay(Sound, NULL); 
						if (crachp == 10)
							crachp = 0;
						if (arrp == 10)
							arrp = 0;
						crach_start[crachp] = true; crachp++;
						arri[arrp] = i; arrp++;
						exit;
					}
				}
			}
		}
	}

	for (int i = 0; i < bnum; i++)
	{
		if (fb[i]->drow() == true)
		{
			for (int j = 0; j < mnum2; j++)
			{
				if (fm3[j]->drow() == true)
				{
					if ((fb[i]->show_x() < fm3[j]->show_x() + fm3[j]->show_r() / 2 + 20) && (fm3[j]->show_x() < fb[i]->show_x() + 10) && (fm3[j]->show_y() - 15 <= fb[i]->show_y()) && (fm3[j]->show_y() + 15 >= fb[i]->show_y()))
					{
						fb[i]->die(); fm3[j]->meteor_crach1();  SndObjPlay(Sound, NULL); 
						if (crachp == 10)
							crachp = 0;
						if (arrp == 10)
							arrp = 0;
						crach_start[crachp] = true; crachp++;
						arri[arrp] = i; arrp++;
						exit;
					}
				}
			}
		}
	}

	for (int i = 0; i < mnum; i++)
	{
		if (fm[i]->drow() == true)
		{
			if ((fm[i]->show_x() + 20 >= fp->show_x()) && (fm[i]->show_x() - 20 <= fp->show_x()) && (fm[i]->show_y() + meteor_fb/2 >= fp->show_y()) && (fm[i]->show_y() <= fp->show_y()+20) )
			{
				fp->damege(); fm[i]->die(); SndObjPlay(boom, NULL); 
				planec = 0;
			}
		}
	}

	for (int i = 0; i < mnum2; i++)
	{
		if (fm2[i]->drow() == true)
		{
			if ((fm2[i]->show_x() + 20 >= fp->show_x()) && (fm2[i]->show_x() - 20 <= fp->show_x()) && (fm2[i]->show_y() + meteor_fb/2 >= fp->show_y()) && (fm2[i]->show_y()  <= fp->show_y()+ 20))
			{
				fp->damege(); fm2[i]->die(); SndObjPlay(boom, NULL); 
				planec = 0;
			}
		}
	}

	for (int i = 0; i < mnum2; i++)
	{
		if (fm3[i]->drow() == true)
		{
			if ((fm3[i]->show_x() + 20 >= fp->show_x()) && (fm3[i]->show_x() - 20 <= fp->show_x()) && (fm3[i]->show_y() + meteor_fb/2 >= fp->show_y()) && (fm3[i]->show_y() <= fp->show_y()+20) )
			{
				fp->die(); fm3[i]->die(); SndObjPlay(boom, NULL); 
				planec = 0;
			}
		}
	}


	if (fb2->drow() == true)
	{
		for (int i = 0; i < mnum; i++)
		{
			if (fm[i]->drow() == true)
			{
				if ((fb2->show_x() < fm[i]->show_x() + fm[i]->show_r() / 2 + 20) && (fm[i]->show_x() < fb2->show_x() + 10) && (fm[i]->show_y() - 5 <= fb2->show_y()) && (fm[i]->show_y() + 15 >= fb2->show_y()))
				{
					fm[i]->meteor_crach2(); SndObjPlay(Sound, NULL); 
						crach_start2 = true;
				}
			}
		}


		for (int i = 0; i < mnum2; i++)
		{
			if (fm2[i]->drow() == true )
			{
				if ((fb2->show_x() < fm2[i]->show_x() + fm2[i]->show_r() / 2 + 20) && (fm2[i]->show_x() < fb2->show_x() + 10) && (fm2[i]->show_y() - 15 <= fb2->show_y()) && (fm2[i]->show_y() + 15 >= fb2->show_y()))
				{
					fm2[i]->meteor_crach2(); SndObjPlay(Sound, NULL);  crach_start2 = true;
				}
			}
		}

		for (int i = 0; i < mnum2; i++)
		{
			if (fm3[i]->drow() == true )
			{
				if ((fb2->show_x() < fm3[i]->show_x() + fm3[i]->show_r() / 2 + 20) && (fm3[i]->show_x() < fb2->show_x() + 10) && (fm3[i]->show_y() - 15 <= fb2->show_y()) && (fm3[i]->show_y() + 15 >= fb2->show_y()))
				{
					fm3[i]->meteor_crach2(); SndObjPlay(Sound, NULL);  crach_start2 = true;
				}
			}
		}
	}


	if (fmb->drow() == true)
	{
		for (int i = 0; i < bnum; i++)
		{
			if (fb[i]->drow() == true)
			{

				if ((fb[i]->show_x() < fmb->show_x() + meteor_boss_fr / 2 + 20) && (fmb->show_x() < fb[i]->show_x() + 10))
				{
					/*         -                 +      */
					if ((fmb->show_y() <= fb[i]->show_y()) && (fmb->show_y() + 80 >= fb[i]->show_y()))
					{
						fb[i]->die(); fmb->meteor_crach1();  SndObjPlay(Sound, NULL); 
						if (crachp == 10)
							crachp = 0;
						if (arrp == 10)
							arrp = 0;
						crach_start[crachp] = true; crachp++;
						arri[arrp] = i; arrp++;
						exit;
					}
				}
			}
		}

		if (fb2->drow() == true)
		{
			if ((fb2->show_x() < fmb->show_x() + meteor_boss_fr / 2 + 20) && (fmb->show_x() < fb2->show_x() + 10))
			{	
				if ((fmb->show_y() <= fb2->show_y()) && (fmb->show_y() + 80 >= fb2->show_y()))
				{
					fb2->die(); fmb->meteor_crach2();  SndObjPlay(Sound, NULL); crach_start2 = true;
				}
			}
		}

		if (fp->drow() == true)
		{
			if ((fp->show_x() < fmb->show_x() + meteor_boss_fr / 2 + 20) && (fp->show_x() < fb2->show_x() + 10))
			{
				if ((fmb->show_y() <= fp->show_y()) && (fmb->show_y() + 80 >= fp->show_y()))
				{
					fp->die();  SndObjPlay(boom, NULL); planec = 0;
				}
			}
		}
	}
}

void move_plane()
{
	if (fx <= 10)
	{
		fx = 10;
	}
	if (fx >= back_fr - plane_fr)
	{
		fx = back_fr - plane_fr;
	}
	if (fy <= 10)
	{
		fy = 10;
	}
	if (fy >= back_fb - plane_fb)
	{
		fy = back_fb - plane_fb;
	}
}

void planelife()
{
	for (short i = 0; i < fp->show_hp()-1; i++)
	{
		BackScreen->BltFast((i*30)+plane_life_fr, 440, planeD_life, &plane_life_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}

void bleat_fn()
{
	if (fp->drow() == true)
	{
		for (short i = 0; i <= bleat_time1; i++)
		{
			BackScreen->BltFast(650 - (i * 30) - 30, 440, bleatD, &bleatRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}
		if(bleat_time3==10)
			BackScreen->BltFast(400, 440, bleatD2, &bleatRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}
void score_swich(int k,int i)
{
	switch (k)
	{
	case 0:
		BackScreen->BltFast(250 + i, 440, num_0, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 1:
		BackScreen->BltFast(250 + i, 440, num_1, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 2:
		BackScreen->BltFast(250 + i, 440, num_2, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 3:
		BackScreen->BltFast(250 + i, 440, num_3, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 4:
		BackScreen->BltFast(250 + i, 440, num_4, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 5:
		BackScreen->BltFast(250 + i, 440, num_5, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 6:
		BackScreen->BltFast(250 + i, 440, num_6, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 7:
		BackScreen->BltFast(250 + i, 440, num_7, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 8:
		BackScreen->BltFast(250 + i, 440, num_8, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 9:
		BackScreen->BltFast(250 + i, 440, num_9, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	default:
		break;
	}
}
void score_swich_e(int k, int i)
{
	switch (k)
	{
	case 0:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_0e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 1:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_1e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 2:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_2e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 3:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_3e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 4:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_4e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 5:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_5e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 6:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_6e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 7:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_7e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 8:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_8e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	case 9:
		BackScreen->BltFast(score_spase_x + i, score_spase_y, num_9e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;
	default:
		break;
	}
}

void score_f()
{
	int i = 1;
	int j = count;
	int k = 0;
	int a = 0;
	if (start == true)
	{
		BackScreen->BltFast(220, 440, score, &scoreRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		while (i <= count)
		{
			i = i * 10;
		}
		if (count == 0)
		{
			BackScreen->BltFast(250, 440, num_0, &numRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

		}
		while (i > 1)
		{
			i = i / 10;
			k = j / i;
			j = count % i;
			score_swich(k, 15 * a);
			a++;
		}
	}
	else
	{
		BackScreen->BltFast(score_spase_x - 100, score_spase_y, score_e, &scoreRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		while (i <= count)
		{
			i = i * 10;
		}
		if (count == 0)
		{
			BackScreen->BltFast(score_spase_x, score_spase_y, num_0e, &numRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}
		while (i > 1)
		{
			i = i / 10;
			k = j / i;
			j = count % i;
			score_swich_e(k, 50 * a);
			a++;
		}
	}
}

void end_game()
{
	if (fp->drow() == false || (fmb->show_y() >= 350 - meteor_boss_ft && count>=47) || (count >= 50 && fmb->drow() == false))
	{
		start = false;
	}
	if (start==false)
	{
		fp->~plane();
		fb2->~bleat2();
		for (short a = 0; a < mnum; a++)
		{
			fm[a]->~meteor1();
		}
		for (short a = 0; a < bnum; a++)
		{
			fb[a]->~bleat();
		}
		for (short a = 0; a < mnum2; a++)
		{
			fm2[a]->~meteor2();
			fm3[a]->~meteor3();
		}
	}
}

void GameProc()
{
	/*게임 메인 페이지*/
	if(start ==false && start2==true )
	{
		SndObjPlay(back, NULL);
		game_start_contrl = true;
		if (start_way == false)
		{
			BackScreen->BltFast(0, 0, back_start, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
			BackScreen->BltFast(220, 200, game_start, &game, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			BackScreen->BltFast(220, 280, game_way, &game, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			BackScreen->BltFast(220, 360, game_end_b, &game, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			BackScreen->BltFast(120, start_count, arrow, &arrow_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			if (start_count == 200)
			{
				if (_GetKeyState(VK_DOWN)&&start_chose_f==true) { start_count += 80; start_chose_f = false;}
			}
			else if (start_count == 280)
			{
				if (_GetKeyState(VK_UP) && start_chose_f == true)  { start_count -= 80;  start_chose_f = false; }
				else if (_GetKeyState(VK_DOWN) && start_chose_f == true) { start_count += 80; start_chose_f = false; }
			}
			else if (start_count == 360)
			{
				if (_GetKeyState(VK_UP) && start_chose_f == true) { start_count -= 80;  start_chose_f = false;	}
			}
		}
		else if (start_way == true)
		{
			BackScreen->BltFast(0, 0, back_way, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		}
		
		if (_GetKeyPush(VK_SPACE))
		{
			/*게임 시작*/
			if (start_count == 200 )
			{
				game_start_contrl = false;
				start = true; start2 = false;
				meteorProc();
				bleat_proc();
				fp = new plane(fx, fy);
				count = 0;
				bleat_time1 = 5;
				bleat_time3 = 10;
			}
			/*게임방법*/
			else if (start_count == 280 && start_way==false)
			{
				start_way = true;
			}
			else if (start_way == true)
			{
				start_way = false;
			}
			/*게임종료*/
			else if (start_count == 360)
			{
				start = true;
				start2 = true;
			}
		}
	}
	else if (start == false && start2 == false)
	{
		if (fmb->drow() == false)
		{
			if (count <= 48)
			{
				BackScreen->BltFast(0, 0, back_end, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
				score_f();
			}
			else
			{
				BackScreen->BltFast(0, 0, back_end_win, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
				score_f();
			}
		}
		else
		{
			BackScreen->BltFast(0, 0, back_end, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
			score_f();
		}
		if (_GetKeyPush(VK_SPACE)) {
			start2 = true;
		}
	}
	else 
	{
		/* 게임 실행*/
		BackScreen->BltFast(0, 0, backract, &backRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		if (fp->drow() == true) { BackScreen->BltFast(fp->show_x(), fp->show_y(), planeD, &planeRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY); }
		if (_GetKeyState(VK_LEFT)) { fp->move_x(fp->show_x() - 5); }
		if (_GetKeyState(VK_RIGHT)) { fp->move_x(fp->show_x() + 5); }
		if (_GetKeyState(VK_UP)) { fp->move_y(fp->show_y() - 5); }
		if (_GetKeyState(VK_DOWN)) { fp->move_y(fp->show_y() + 5); }
		if (_GetKeyPush(VK_SPACE))
		{
			if (bleat_time1 > 0)
			{
				bleat_num++;
				fb[bleat_num]->bleat_shot(); fb[bleat_num]->move_x(fp->show_x() + bleat_fr + 5);
				fb[bleat_num]->move_y(fp->show_y());
				bleat_time1--;
			}
		}
		if (_GetKeyPush(VK_F2))
		{
			if (bleat_time3 == 10)
			{
				fb2->bleat_shot();
				fb2->move_x(fp->show_x() + bleat_fr + 5);
				fb2->move_y(fp->show_y());
				bleat_time3 = 0;
			}
		}

		move_plane();
		if (stage_start == true)
		{
			planelife();
			bleat_fn();
			meteor_procn();
			BleatProcn();		
			Chrch();
			crach_show();
			score_f();
			end_game();
		}
		else
		{
			bool tt = true;
			BackScreen->BltFast(220, 180, stage, &scoreRect_e, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			score_swich_e((count / 15) + 1, 100);
			if (tt == true)
			{
				for (int i = 0; mnum > i; i++)
				{
					fm[i]->die();
				}
				for (int i = 0; mnum2 > i; i++)
				{
					fm2[i]->die();
					fm3[i]->die();
				}
				tt = false;
			}
		}
		if (count % 15 == 0 && stage_start == true)
			stage_start = false;
	}
	RealScreen->Flip(NULL, DDFLIP_WAIT);//화면에 최종적으로 그려줌
}

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (start == false && start2 == true)
	{
		if (start_chose_f == false) { start_count_f++; }
		if (start_count_f == 3)	{	start_chose_f = true;  start_count_f = 0;	}
	}

	if (start == true)
	{
		if (count_time == 20 && stage_start==true )
		{
			count++;
			count_time = 0;
		}

		if (bleat_time1 != 5)
		{
			bleat_time2++;
			if (bleat_time2 == 30)
			{
				bleat_time1++;
				bleat_time2 = 0;
			}
		}

		if (bleat_time3 < 10)
		{
			bleat_time4++;
			if (bleat_time4 == 12)
			{
				bleat_time3++;
				bleat_time4 = 0;
			}
		}
		count_time++;
	}
	for (int i = 0; i < 10; i++)
	{
		if (crach_start[i] == true)
		{
			crach_timer[i] += 1;
			if (crach_timer[i] == 3)
			{
				crach_start[i] = false;
				crach_timer[i] = 0;
				arri[i] = 0;
			}
		}
	}
	if (crach_start2 == true)
	{
		crach_timer2++;
		if (crach_timer2 == 3)
		{
			crach_start2 = false;
			crach_timer2 = 0;
			bb = 0;
		}
	}
	if (planec < 4)
		planec++;

	if (stage_count == 8)
	{
		stage_count = 0;
		stage_start = true;
		count++; count_time = 0;
	}	
	if (stage_start == false)
		stage_count++;
	GameProc();
}

BOOL Fail(HWND hwnd)
{
	ShowWindow(hwnd, SW_HIDE);
	MessageBox(hwnd, "DIRECT X 초기화에 실패했습니다.", "오류", MB_OK);
	DestroyWindow(hwnd);
	return FALSE;
}

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//;메시지 처리함수 
	switch (message)
	{
	case WM_CREATE://윈도우가 뜰때 O/S  에 의하여 발생되는 메시지
		//초기화할 것들 있으면 여기에 할 수 있다.
		
		SetTimer(hWnd, 1, 50, (TIMERPROC)TimerProc);//1000 이 1초를 나타낸다.
		return 0;
	
	case WM_MOUSEMOVE: //MouseX = LOWORD(lParam);
					   //MouseY = HIWORD(lParam);
		break;
		
		case WM_KEYDOWN:
		{
			switch((int)wParam)
			{
			case VK_DELETE:
			break;

			default:
			break;

			}

			return 0;
			}
			break;
		
		case WM_DESTROY:
		// _ReleaseAll();
		KillTimer(hWnd, 1);
		//MessageBox(NULL, "right", "aaab", MB_OK);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	//	HRESULT result;

	//(1) 윈도우를 만든다.
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;//메시지처리함수(Callback 함수)
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_HAND);// IDC_ARROW);// IDC_NO);
	wc.hbrBackground = GetStockBrush(BLACK_BRUSH); //(COLOR_BTNFACE); //kfrf(BLACK_BRUSH);//(COLOR_BTNFACE);//(GRAY_BRUSH);//(COLOR_BTNFACE);//(BLACK_BRUSH);//(COLOR_BTNFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Game2D";
	RegisterClass(&wc);

	MainHwnd = CreateWindowEx(WS_EX_ACCEPTFILES,"Game2D", NULL,WS_POPUP,50, 50, 100, 100,NULL, NULL, hInstance, NULL);
	//MainHwnd = CreateWindow(wc.lpszClassName, wc.lpszMenuName, WS_OVERLAPPED,50, 50, 500, 300,NULL,NULL, hInstance,NULL);

	if (!MainHwnd) return FALSE;

	SetFocus(MainHwnd);
	ShowWindow(MainHwnd, nCmdShow);
	UpdateWindow(MainHwnd);
	ShowCursor(FALSE);
	//(2) DirectX 시작하기
	HRESULT result;//DX 리턴값을 받기 위한 데이터 
	DDSURFACEDESC2 ddsd;//DierctDraw Surface Description
	DDSCAPS2 ddscaps;
	LPDIRECTDRAW pdd;//그래픽카드 메모리 제어 데이터 
	int x, y, bpp;

	x = 640; y = 480; bpp = 16;//bits per pixel

	result = DirectDrawCreate(NULL, &pdd, NULL);//pdd : 표면을 사용하기 위한 첫 표면 데이터 
	if (result != DD_OK) return Fail(MainHwnd);

	//DirectDraw version 7을 DirectOBJ으로 사용한다.
	//이제 표면 사용을 주도하는 데이터는 DirectOBJ이 된다.
	result = pdd->QueryInterface(IID_IDirectDraw7, (LPVOID *)&DirectOBJ);
	if (result != DD_OK) return Fail(MainHwnd);

	//윈도우 화면을 [게임할 때 사용하는] 풀스크린으로 사용하겠다.
	result = DirectOBJ->SetCooperativeLevel(MainHwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if (result != DD_OK) return Fail(MainHwnd);

	//화면 해상도를 640 x 480으로 하고 각 픽셀의 색깔을 나타낼 때 16 비트를 사용하겠다. 
	result = DirectOBJ->SetDisplayMode(x, y, bpp, 0, 0);
	if (result != DD_OK) return Fail(MainHwnd);

	//표면 정보를 담을 데이터 ddsd를 모두 0으로 초기화 
	memset(&ddsd, 0, sizeof(ddsd));
	//그리고 다음 멤버들만 값을 아래와 같이 넣는다.
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;//back surface를 1개 사용하겠다.

							   //상기와 같은 표면 정보(ddsd)를 갖는 주표면(Real Screen)
	result = DirectOBJ->CreateSurface(&ddsd, &RealScreen, NULL);
	if (result != DD_OK) { MessageBox(NULL, "...", "OK", MB_OK); return Fail(MainHwnd); }//return Fail(MainHwnd);//Back Surface 생성 

	memset(&ddscaps, 0, sizeof(ddscaps));
	//이놈은 후면이고 
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	//주면의 붙은 후면이다.
	result = RealScreen->GetAttachedSurface(&ddscaps, &BackScreen);
	if (result != DD_OK) return Fail(MainHwnd);

	back_start = DDLoadBitmap(DirectOBJ, "image/back_start.bmp", 0, 0);
	back_end = DDLoadBitmap(DirectOBJ, "image/end_game.bmp", 0, 0);
	back_end_win = DDLoadBitmap(DirectOBJ, "image/end_game2.bmp", 0, 0);
	backract = DDLoadBitmap(DirectOBJ, "image/backract.bmp", 0, 0);
	back_way = DDLoadBitmap(DirectOBJ, "image/way.bmp", 0, 0);
	stage= DDLoadBitmap(DirectOBJ, "image/stage.bmp", 0, 0);
	
	planeD = DDLoadBitmap(DirectOBJ, "image/plane1.bmp", 0, 0);
	planeDc = DDLoadBitmap(DirectOBJ, "image/planec.bmp", 0, 0);
	planeD_life = DDLoadBitmap(DirectOBJ, "image/plane_life.bmp", 0, 0);

	bleatD = DDLoadBitmap(DirectOBJ, "image/bleat.bmp", 0, 0);
	bleatD2 = DDLoadBitmap(DirectOBJ, "image/bleat2.bmp", 0, 0);

	meteorD = DDLoadBitmap(DirectOBJ, "image/meteor.bmp", 0, 0);
	meteorD2 = DDLoadBitmap(DirectOBJ, "image/meteor2.bmp", 0, 0);
	meteorD3 = DDLoadBitmap(DirectOBJ, "image/meteor3.bmp", 0, 0);
	meteorDc = DDLoadBitmap(DirectOBJ, "image/meteorc.bmp", 0, 0);
	meteorD_boss= DDLoadBitmap(DirectOBJ, "image/meteor_boss.bmp", 0, 0);

	game_start = DDLoadBitmap(DirectOBJ, "image/game_start.bmp", 0, 0);
	game_way = DDLoadBitmap(DirectOBJ, "image/game_way.bmp", 0, 0);
	game_end_b = DDLoadBitmap(DirectOBJ, "image/game_end.bmp", 0, 0);
	arrow = DDLoadBitmap(DirectOBJ, "image/화살표.bmp", 0, 0);

	num_1= DDLoadBitmap(DirectOBJ, "image/1.bmp", 0, 0);
	num_2 = DDLoadBitmap(DirectOBJ, "image/2.bmp", 0, 0);
	num_3 = DDLoadBitmap(DirectOBJ, "image/3.bmp", 0, 0);
	num_4 = DDLoadBitmap(DirectOBJ, "image/4.bmp", 0, 0);
	num_5 = DDLoadBitmap(DirectOBJ, "image/5.bmp", 0, 0);
	num_6 = DDLoadBitmap(DirectOBJ, "image/6.bmp", 0, 0);
	num_7 = DDLoadBitmap(DirectOBJ, "image/7.bmp", 0, 0);
	num_8 = DDLoadBitmap(DirectOBJ, "image/8.bmp", 0, 0);
	num_9 = DDLoadBitmap(DirectOBJ, "image/9.bmp", 0, 0);
	num_0 = DDLoadBitmap(DirectOBJ, "image/0.bmp", 0, 0);
	score = DDLoadBitmap(DirectOBJ, "image/점수.bmp", 0, 0);

	num_1e = DDLoadBitmap(DirectOBJ, "image/1-1.bmp", 0, 0);
	num_2e = DDLoadBitmap(DirectOBJ, "image/2-1.bmp", 0, 0);
	num_3e = DDLoadBitmap(DirectOBJ, "image/3-1.bmp", 0, 0);
	num_4e = DDLoadBitmap(DirectOBJ, "image/4-1.bmp", 0, 0);
	num_5e = DDLoadBitmap(DirectOBJ, "image/5-1.bmp", 0, 0);
	num_6e = DDLoadBitmap(DirectOBJ, "image/6-1.bmp", 0, 0);
	num_7e = DDLoadBitmap(DirectOBJ, "image/7-1.bmp", 0, 0);
	num_8e = DDLoadBitmap(DirectOBJ, "image/8-1.bmp", 0, 0);
	num_9e = DDLoadBitmap(DirectOBJ, "image/9-1.bmp", 0, 0);
	num_0e = DDLoadBitmap(DirectOBJ, "image/0-1.bmp", 0, 0);
	score_e = DDLoadBitmap(DirectOBJ, "image/점수1.bmp", 0, 0);

	DDSetColorKey(planeD, RGB(0, 0, 0)); 
	DDSetColorKey(planeDc, RGB(0, 0, 0));
	DDSetColorKey(bleatD, RGB(0, 0, 0));
	DDSetColorKey(bleatD2, RGB(0, 0, 0));
	DDSetColorKey(meteorD, RGB(0, 0, 0));
	DDSetColorKey(meteorD2, RGB(0, 0, 0));
	DDSetColorKey(meteorD3, RGB(0, 0, 0));
	DDSetColorKey(meteorD_boss, RGB(255, 255, 255));
	DDSetColorKey(meteorDc, RGB(0, 0, 0));
	DDSetColorKey(planeD_life, RGB(0, 0, 0));

	DDSetColorKey(num_1, RGB(0, 0, 0));
	DDSetColorKey(num_2, RGB(0, 0, 0));
	DDSetColorKey(num_3, RGB(0, 0, 0));
	DDSetColorKey(num_4, RGB(0, 0, 0));
	DDSetColorKey(num_5, RGB(0, 0, 0));
	DDSetColorKey(num_6, RGB(0, 0, 0));
	DDSetColorKey(num_7, RGB(0, 0, 0));
	DDSetColorKey(num_8, RGB(0, 0, 0));
	DDSetColorKey(num_9, RGB(0, 0, 0));
	DDSetColorKey(num_0, RGB(0, 0, 0));
	DDSetColorKey(score, RGB(0, 0, 0));

	DDSetColorKey(game_start, RGB(0, 0, 0));
	DDSetColorKey(game_way, RGB(0, 0, 0));
	DDSetColorKey(game_end_b, RGB(0, 0, 0));
	DDSetColorKey(stage, RGB(255, 255, 255));

	DDSetColorKey(num_1e, RGB(255, 255, 255));
	DDSetColorKey(num_2e, RGB(255, 255, 255));
	DDSetColorKey(num_3e, RGB(255, 255, 255));
	DDSetColorKey(num_4e, RGB(255, 255, 255));
	DDSetColorKey(num_5e, RGB(255, 255, 255));
	DDSetColorKey(num_6e, RGB(255, 255, 255));
	DDSetColorKey(num_7e, RGB(255, 255, 255));
	DDSetColorKey(num_8e, RGB(255, 255, 255));
	DDSetColorKey(num_9e, RGB(255, 255, 255));
	DDSetColorKey(num_0e, RGB(255, 255, 255));
	DDSetColorKey(score_e, RGB(255, 255, 255));
	
	DDSetColorKey(arrow, RGB(255, 255, 255));

	//direct sound
	if (_InitDirectSound()) {
			Sound = SndObjCreate(SoundOBJ, "image/Shot.wav", 4);
			boom = SndObjCreate(SoundOBJ, "image/boom.wav", 4);
			back= SndObjCreate(SoundOBJ, "image/Immediate.wav",0 );
		SndObjPlay(Sound, DSBPLAY_LOOPING);
		SndObjPlay(boom, DSBPLAY_LOOPING);
		SndObjPlay(back, DSBPLAY_LOOPING);// Loop
												//		SndObjPlay(Sound[0], NULL);				// 1회 재생
	}

	//(4) 메시지 처리 부분

	while(!(start==true && start2==true))
	{   
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{  //있으면 가져와서
			if (!GetMessage(&msg, NULL, 0, 0)) return msg.wParam;

			TranslateMessage(&msg);//해석하고 
			DispatchMessage(&msg);//메시지 처리함수(WindowProc)
								  //로 보낸다 
		}
		//else GameProc();
	}
	DestroyWindow(MainHwnd);
	return TRUE;
}