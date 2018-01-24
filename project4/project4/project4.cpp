// project4.cpp : 定义控制台应用程序的入口点。
///***********************************project4：图像的几何处理：平移、缩放、旋转****************************/
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
	cout<<"请输入图像名"<<endl;
	cin>>imageName;
	Mat srcImg = imread(imageName);         //定义一个Mat类变量并将图片读入
	if (!srcImg.data)
	{
		cout<<"读入图像失败"<<endl;
		return -1;
	}
	imshow("原图像",srcImg);             //显示原图片
	waitKey(6);
	int model = 0;
	cout<<"请输入要进行的图像操作"<< endl;
	cout << "1：图片平移" << endl;
	cout << "2：图片缩放" << endl;
	cout << "3：图片旋转" << endl;
	cin>>model;
	Mat dstImg;   //定义一个新的Mat类变量存储目标图像信息
	int row = srcImg.rows;             //将原图片行数赋给变量row
	int column = srcImg.cols;          //将原图片列数赋给变量column
	int result_rows;   //结果行数
	int result_columns;  //结果列数
	switch (model){
	case 1:
		{//图像平移
			dstImg.create(row, column, CV_8UC3);
			cout<<"请输入平移变换tx，ty的值"<<endl;
			double tx,ty;
			cin>>tx>>ty;
			cout<<"图像处理中，请稍后……"<<endl;
			Vec3b *p;
			for (int i = 0; i < row; i++)
			{
				p = dstImg.ptr<Vec3b>(i);
				for (int j = 0; j < column; j++)
				{//平移后坐标映射到原图像
					int x = j - tx;
					int y = i - ty;
					//保证映射后的坐标在原图像范围内
					if (x >= 0 && y >= 0 && x < column && y < row)
						p[j] = srcImg.ptr<Vec3b>(y)[x];
					else
					{//若变换前的坐标不在原图范围内则将该坐标赋值为255
						p[j] [0]= 255;
						p[j] [1]= 255;
						p[j] [2]= 255;
					}
				}
			}
			cout<<"图像处理完成！"<<endl;
			break;
		}
	case 2:
		{//图像缩放
			int zooming = 0;//放大或缩小
		    float Ratio = 0;//放大缩小倍数
			cout << "请选择缩放模式：" << endl;
		    cout << "0：缩小" << endl;
			cout << "1：放大" << endl;
			cin >> zooming;
			if (zooming != 0 && zooming != 1)
			{
				cout << "输入参数有误！";
				return -1;
			}
			cout << "请输入缩放倍数：";
			cin >> Ratio;
			cout<<"图像处理中，请稍后……"<<endl;
			if (Ratio <= 0)
			{
				cout << "输入参数有误！";
				return -1;
			}
			if (zooming == 1)
				Ratio = Ratio;
			else if (zooming == 0)
				Ratio = 1.0 / Ratio;
			result_rows = row * Ratio;   //计算新的行列数
			result_columns = column * Ratio;
			dstImg.create(result_rows, result_columns, CV_8UC3);
			for (int i = 0; i < result_rows; i++)
			{
				for (int j = 0; j < result_columns; j++)
				{
					if ((int)(i / Ratio) < row - 1 && (int)(j / Ratio) < column - 1 && (int)(i / Ratio) > 0 && (int)(j / Ratio) > 0)
					{//双线性插值
						int u = i / Ratio - (int)(i / Ratio);
						int v = j / Ratio - (int)(j / Ratio);
						dstImg.at<Vec3b>(i, j) = srcImg.at<Vec3b>((int)(i / Ratio), (int)(j / Ratio)) * (1 - u) * (1 - v) + srcImg.at<Vec3b>((int)(i / Ratio) + 1, (int)(j / Ratio)) * u*(1 - v) + srcImg.at<Vec3b>((int)(i / Ratio), (int)(j / Ratio) + 1) * (1 - u)* v + srcImg.at<Vec3b>((int)(i / Ratio) + 1, (int)(j / Ratio) + 1) * u*v;
					}
				}
			}
			cout<<"图像处理完成！"<<endl;
			break;
		}
	case 3:
		{//图像旋转
			float degree = 0;//旋转度数
			int OriginX;//旋转中心X
			int OriginY;//旋转中心Y
			cout << "请输入旋转角度：";  //旋转角度范围为-180°~+180°
			cin >> degree;
			if (degree > 180 || degree < -180)
			{//判断输入
				cout << "输入参数有误！";
				return -1;
			}
			degree = -degree;
			degree *= PI / 180;
			float f1 = cos(degree);
			float f2 = sin(degree);
			result_rows = row * fabs(f1) + column * fabs(f2);    //计算新的行列数
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
	imshow("处理后的图像", dstImg);  
	waitKey(); 		
	return 0;	
}

	
		

	
	