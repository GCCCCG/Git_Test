// project4.cpp : �������̨Ӧ�ó������ڵ㡣
///***********************************project4��ͼ��ļ��δ���ƽ�ơ����š���ת****************************/
#include "stdafx.h"
#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;
#define PI 3.1415

int _tmain(int argc, _TCHAR* argv[])
{
	char imageName[]="";
	cout<<"������ͼ����"<<endl;
	cin>>imageName;
	Mat srcImg = imread(imageName);         //����һ��Mat���������ͼƬ����
	if (!srcImg.data)
	{
		cout<<"����ͼ��ʧ��"<<endl;
		return -1;
	}
	imshow("ԭͼ��",srcImg);             //��ʾԭͼƬ
	waitKey(6);
	int model = 0;
	cout<<"������Ҫ���е�ͼ�����"<< endl;
	cout << "1��ͼƬƽ��" << endl;
	cout << "2��ͼƬ����" << endl;
	cout << "3��ͼƬ��ת" << endl;
	cin>>model;
	Mat dstImg;   //����һ���µ�Mat������洢Ŀ��ͼ����Ϣ
	int row = srcImg.rows;             //��ԭͼƬ������������row
	int column = srcImg.cols;          //��ԭͼƬ������������column
	int result_rows;   //�������
	int result_columns;  //�������
	switch (model){
	case 1:
		{//ͼ��ƽ��
			dstImg.create(row, column, CV_8UC3);
			cout<<"������ƽ�Ʊ任tx��ty��ֵ"<<endl;
			double tx,ty;
			cin>>tx>>ty;
			cout<<"ͼ�����У����Ժ󡭡�"<<endl;
			Vec3b *p;
			for (int i = 0; i < row; i++)
			{
				p = dstImg.ptr<Vec3b>(i);
				for (int j = 0; j < column; j++)
				{//ƽ�ƺ�����ӳ�䵽ԭͼ��
					int x = j - tx;
					int y = i - ty;
					//��֤ӳ����������ԭͼ��Χ��
					if (x >= 0 && y >= 0 && x < column && y < row)
						p[j] = srcImg.ptr<Vec3b>(y)[x];
					else
					{//���任ǰ�����겻��ԭͼ��Χ���򽫸����긳ֵΪ255
						p[j] [0]= 255;
						p[j] [1]= 255;
						p[j] [2]= 255;
					}
				}
			}
			cout<<"ͼ������ɣ�"<<endl;
			break;
		}
	case 2:
		{//ͼ������
			int zooming = 0;//�Ŵ����С
		    float Ratio = 0;//�Ŵ���С����
			cout << "��ѡ������ģʽ��" << endl;
		    cout << "0����С" << endl;
			cout << "1���Ŵ�" << endl;
			cin >> zooming;
			if (zooming != 0 && zooming != 1)
			{
				cout << "�����������";
				return -1;
			}
			cout << "���������ű�����";
			cin >> Ratio;
			cout<<"ͼ�����У����Ժ󡭡�"<<endl;
			if (Ratio <= 0)
			{
				cout << "�����������";
				return -1;
			}
			if (zooming == 1)
				Ratio = Ratio;
			else if (zooming == 0)
				Ratio = 1.0 / Ratio;
			result_rows = row * Ratio;   //�����µ�������
			result_columns = column * Ratio;
			dstImg.create(result_rows, result_columns, CV_8UC3);
			for (int i = 0; i < result_rows; i++)
			{
				for (int j = 0; j < result_columns; j++)
				{
					if ((int)(i / Ratio) < row - 1 && (int)(j / Ratio) < column - 1 && (int)(i / Ratio) > 0 && (int)(j / Ratio) > 0)
					{//˫���Բ�ֵ
						int u = i / Ratio - (int)(i / Ratio);
						int v = j / Ratio - (int)(j / Ratio);
						dstImg.at<Vec3b>(i, j) = srcImg.at<Vec3b>((int)(i / Ratio), (int)(j / Ratio)) * (1 - u) * (1 - v) + srcImg.at<Vec3b>((int)(i / Ratio) + 1, (int)(j / Ratio)) * u*(1 - v) + srcImg.at<Vec3b>((int)(i / Ratio), (int)(j / Ratio) + 1) * (1 - u)* v + srcImg.at<Vec3b>((int)(i / Ratio) + 1, (int)(j / Ratio) + 1) * u*v;
					}
				}
			}
			cout<<"ͼ������ɣ�"<<endl;
			break;
		}
	case 3:
		{//ͼ����ת
			float degree = 0;//��ת����
			int OriginX;//��ת����X
			int OriginY;//��ת����Y
			cout << "��������ת�Ƕȣ�";  //��ת�Ƕȷ�ΧΪ-180��~+180��
			cin >> degree;
			if (degree > 180 || degree < -180)
			{//�ж�����
				cout << "�����������";
				return -1;
			}
			degree = -degree;
			degree *= PI / 180;
			float f1 = cos(degree);
			float f2 = sin(degree);
			result_rows = row * fabs(f1) + column * fabs(f2);    //�����µ�������
			result_columns = column * fabs(f1) + row * fabs(f2);
			dstImg.create(result_rows, result_columns, CV_8UC3);
			OriginX = result_columns / 2;  
			OriginY = result_rows / 2;
			for (int i = 0; i < result_rows; i++)
			{
				for (int j = 0; j < result_columns; j++)
				{
					int i1 = (row / 2 - ((OriginY - i) * cos(degree) - (j - OriginX) * sin(degree)));
					int j1 = (column / 2 + ((j - OriginX) * cos(degree) + (OriginY - i) * sin(degree)));
					if (i1 > 0 && i1 < row && j1 > 0 && j1 < column)
					dstImg.at<Vec3b>(i, j) = srcImg.at<Vec3b>(i1, j1);
					else
					{
						dstImg.at<Vec3b>(i, j)[0] = 255;
						dstImg.at<Vec3b>(i, j)[1] = 255;
						dstImg.at<Vec3b>(i, j)[2] = 255;
					}
				}
			}
			break;
		}
	}
	imshow("������ͼ��", dstImg);  
	waitKey(); 		
	return 0;	
}

	
		

	
	