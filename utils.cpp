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
	ofn.lpstrFilter = "har文件(*.har)\0*.har\0所有文件\0*.*\0";
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
		MessageBox(hwndDlg,"输入文件名不能为空！","警告",MB_ICONWARNING|MB_OK);
		return;
	}
	if(strlen(txtfile)==0){
		MessageBox(hwndDlg,"输出文件名不能为空！","警告",MB_ICONWARNING|MB_OK);
		return;
	}
	har=fopen(harfile,"r");
	if(har==0){
		MessageBox(hwndDlg,"打开输入文件失败！","警告",MB_ICONWARNING|MB_OK);
		return;
	}
	//判断是否勾选不覆盖
	txt=fopen(txtfile,IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX1)?"a+":"w+");
	if(txt==0){
		MessageBox(hwndDlg,"创建输出文件失败！","警告",MB_ICONWARNING|MB_OK);
		return;
	}
	//获取文件大小
	harlen=GetFileSize(har,0)/100;
	//printf("%d\n",harlen);
	delpar=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX2);
	if(filter_inc=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX5))
		GetDlgItemText(hwndDlg,IDC_EDIT3,inc,MAXLEN);
	if(filter_ninc=IsDlgButtonChecked(hwndDlg,IDC_CHECKBOX6))
		GetDlgItemText(hwndDlg,IDC_EDIT4,ninc,MAXLEN);
	//提取链接
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
			//刷新进度条
			SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,ftell(har)/harlen,0);
		}
		#undef cc
	}
	SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,100,0);
	sprintf(msg,"提取到链接%d个\n已存为%s",cnt,txtfile);
	MessageBox(hwndDlg,msg,"完成",MB_ICONINFORMATION|MB_OK);
	SendDlgItemMessage(hwndDlg,IDC_PROGRESS1,PBM_SETPOS,0,0);
	fclose(har);
	fclose(txt);
}
