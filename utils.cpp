#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include "resource.h"
#include "utils.h"

int GetFile(HWND hwndDlg,HINSTANCE hInst,char *filename){
	OPENFILENAME ofn;
	int result;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	ofn.lpstrFilter = "har�ļ�(*.har)\0*.har\0�����ļ�\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAXLEN;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	return GetOpenFileName(&ofn);
}
void GetTxtFileName(char *har,char *txt){
	char *p=har,*q=txt;
	while(*q++=*p++);
	strcpy(q-4,"txt");
}
void GetURL(HWND hwndDlg,char *harfile,char *txtfile){
	FILE *har,*txt;
	char c,url[MAXLEN],msg[MAXLEN],delpar,*p;
	char inc[MAXLEN],ninc[MAXLEN],filter_inc,filter_ninc;
	int harlen,cnt=0;
	if(strlen(harfile)==0){
		MessageBox(hwndDlg,"�����ļ�������Ϊ�գ�","����",MB_ICONWARNING|MB_OK);
		return;
	}
	if(strlen(txtfile)==0){
		MessageBox(hwndDlg,"����ļ�������Ϊ�գ�","����",MB_ICONWARNING|MB_OK);
		return;
	}
	har=fopen(harfile,"r");
	if(har==0){
		MessageBox(hwndDlg,"�������ļ�ʧ�ܣ�","����",MB_ICONWARNING|MB_OK);
		return;
	}
	//�ж��Ƿ�ѡ������
	txt=fopen(txtfile,IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX1)?"a+":"w+");
	if(txt==0){
		MessageBox(hwndDlg,"��������ļ�ʧ�ܣ�","����",MB_ICONWARNING|MB_OK);
		return;
	}
	//��ȡ�ļ���С
	harlen=GetFileSize(har,0)/100;
	//printf("%d\n",harlen);
	delpar=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX2);
	if(filter_inc=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX5))
		GetDlgItemText(hwndDlg,IDC_EDIT3,inc,MAXLEN);
	if(filter_ninc=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX6))
		GetDlgItemText(hwndDlg,IDC_EDIT4,ninc,MAXLEN);
	//��ȡ����
	while(!feof(har)){
		c=fgetc(har);
		#define cc fgetc(har)
		if(c=='u'&&cc=='r'&&cc=='l'&&cc=='"'&&cc==':'&&cc==' '&&cc=='"'){
			fgets(url,MAXLEN,har);
			p=url;
			while(*p){
				if(delpar&&*p=='?'||*p=='"'){
					*p=0;
					break;
				}
				p++;
			}
			if(!(filter_inc&&strstr(url,inc)==0)&&!(filter_ninc&&strstr(url,ninc))){
				fprintf(txt,"%s\n",url);
				cnt++;
			}
			//ˢ�½�����
			SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,ftell(har)/harlen,0);
		}
		#undef cc
	}
	SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,100,0);
	sprintf(msg,"��ȡ������%d��\n�Ѵ�Ϊ%s",cnt,txtfile);
	MessageBox(hwndDlg,msg,"���",MB_ICONINFORMATION|MB_OK);
	SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,0,0);
	fclose(har);
	fclose(txt);
}
